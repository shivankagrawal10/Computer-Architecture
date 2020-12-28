#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"first.h"
int check (int argc, char** argv){
  int fin=1;
  if(argc-1!=5){ //checking that 5 inputs have been fed
    return 0;
  }
  int cachesize=atoi(argv[1]);
  if(floor(log2(cachesize))!=log2(cachesize)){ //checking that 1st input (cache size) is pow of 2
    return 0;
  }
  int blocksize=atoi(argv[4]);
  if(floor(log2(blocksize))!=log2(blocksize)){ //checking that 4th input (block size) is pow of 2
    return 0;
  }
  FILE *curr=fopen(argv[5],"r");
  if(curr==0){
    return 0;
  }
  fclose(curr);
  return fin;
}

int cachehit(unsigned long long address,struct set *cache,int pol,int A,int s,int b){
  unsigned long long one=1;
  unsigned long long set = ((((one<<s)-1)<<b)&address)>>b;
  //unsigned long block= ((1<<b)-1)&address;
  unsigned long long tag= ((((one<<(48-(s+b)))-1)<<(s+b))&address)>>(s+b);
  //printf("address %lld\n",address);
  // printf("tag %llu\n",tag);
  //printf("b %d\n",b);
  for (int i=0;i<A;i++){
    if(cache[set].lines[i].valid==1){
      if(cache[set].lines[i].tag==tag){
	if(pol==1 && A!=1){//moving most recently used to the end of list
	  struct LLNode* temp=cache[set].lines[i].ptr;
	  //printf("%p\n",temp->next);
	  if(temp->next!=0){
	    if(temp->prev!=0){
	      temp->prev->next=temp->next;
	      temp->next->prev=temp->prev;
	    }
	    else if(temp->prev==0){
	      cache[set].front=temp->next;
	      cache[set].front->prev=0;
	    }
	    cache[set].back->next=temp;
	    temp->prev=cache[set].back;
	    cache[set].back=cache[set].back->next;
	    cache[set].back->next=0;
	  }
	}
	return 1;
      }
    }
  }
  //printf("%ld %llx",set,address);
  return 0;
}

void replace(unsigned long long address, struct set* cache,int s, int b, int index){
  unsigned long long one=1;
  unsigned long long set = ((((one<<s)-1)<<b)&address)>>b;
  //unsigned long block= ((1<<b)-1)&address;
  unsigned long long tag= ((((one<<(48-(s+b)))-1)<<(s+b))&address)>>(s+b);
  cache[set].lines[index].valid=1;
  cache[set].lines[index].tag=tag;
}

void freecache(struct set* cache, int S, int A){
  for (int i=0;i<S;i++){
    for(int j=0;j<A;j++){
      free(cache[i].lines[j].blocks);
    }
    struct LLNode *temp=cache[i].front;
    while(temp!=0){
      cache[i].front=temp->next;
      free(temp);
      temp=cache[i].front;
    }
    free(cache[i].lines);
  }
  free(cache);
}

struct LLNode *enqueue(int val, struct LLNode *back){
  /*
   * in java everything is passed as references
   * in C things will be passed by memory so important to manually
   * pass as references 
   */
  struct LLNode *temp= malloc(sizeof(struct LLNode));
  temp -> val = val;	//since temp is a struct pointer have to use
  //-> to refer to its field 
  temp -> next = 0; //0 is null pointer in C for now
  temp->prev=back;
  if(back==0){
    return temp;
  }
  back->next=temp;
  return back;
}

struct LLNode *pop(struct LLNode *front){
  if(front == 0){
    return front;
  }
  struct LLNode *next = front->next;
  free(front);
  return next;
}

int update(unsigned long long address, struct set *cache, int s, int b){//updates the queue to move 
  unsigned long long one =1;
  unsigned long long set = ((((one<<s)-1)<<b)&address)>>b;
  //unsigned long block= ((1<<b)-1)&address;
  //unsigned long tag= ((1<<(48-(s+b)))-1)<<(s+b);
   
  struct LLNode* temp =cache[set].front;
  if(temp->next!=0){
    //printf("%d\n",cache[set].back->val);
    cache[set].back->next=temp;
    cache[set].front=temp->next;
    temp->next=0;
    temp->prev=cache[set].back;
    cache[set].back=temp;
    cache[set].front->prev=0;
  }
  return temp->val;
}

void printLL(struct set* cache, int S, int A){
  for (int i=0;i<S;i++){
   
      struct LLNode* temp=cache[i].front;
      while(temp!=0){
	printf("%d, ",temp->val);
	temp=temp->next;
      }
      printf("\n");
    
   
  }
   printf("end\n");
}

int main(int argc, char* argv[1+argc]){

  int ch=check(argc,argv);
  if(ch==0){
    printf("error");
    return EXIT_SUCCESS;
  }
  FILE* file=fopen(argv[5],"r");
  
  int C,S,A,B; //C -cache capacity, S-# of sets, A-Assoc. of Cache, B-blocks in Assoc
  int s,b; //bits for Set and Block in address
  char replacepol=0; //0 is FIFO, 1 is Least Recently Used
  if(strcmp(argv[3],"fifo")==0){
    replacepol=0;
  }
  else if(strcmp(argv[3],"lru")==0){
    replacepol=1;
  }
  else{
    printf("error");
    return EXIT_SUCCESS;
  }
  A=0;
  C=atoi(argv[1]);
  B=atoi(argv[4]);
  S=0;
  s=0;
  b=0;
  char command[6];
  // char* colon;
  for (int i=0;i<5;i++){
    command[i]=argv[2][i];
  }
  command[5]='\0';
  //printf("%s",command);
  if(strcmp(argv[2],"direct")==0){
    A=1;
  }
  else if(strcmp(command,"assoc")==0){
    if(strlen(argv[2])==5){
      A=C/B;
    }
    else{
      //printf("%s",argv[2]);
      sscanf(argv[2],"%5s:%d",command,&A);
    }
  }
  else{
    printf("error");
    return EXIT_SUCCESS;
  }
  S=C/(A*B);
  s=log2(S);
  b=log2(B);
  //printf("%d %d\n",s,b);
  //Creating cache according to specifications
  struct set* cache=malloc(S*sizeof(struct set));
  for(int i=0;i<S;i++){
    cache[i].lines=malloc(A*sizeof(struct assoc));
    cache[i].front=0;
    cache[i].back=0;
    for(int j=0;j<A;j++){
      cache[i].lines[j].valid=0;
      cache[i].lines[j].tag=0;
      cache[i].lines[j].blocks=malloc(B*sizeof(int));
      cache[i].back=enqueue(j,cache[i].back);
      if(cache[i].front==0){
	cache[i].front=cache[i].back;
      }
      else{
	cache[i].back=cache[i].back->next;
      }
      //printf("%d %p\n",j, cache[i].front->next);
      cache[i].lines[j].ptr=cache[i].back;
    }
    /*struct LLNode* temp=cache[i].lines[0].ptr;
    while(temp!=0){
      printf("%d --> ",temp->val);
      temp=temp->next;
      }*/
  }
  //printf("Starting with a Cold Cache");
  //printf("
  //printLL(cache,S,A);
  //Reading trace files
  int ans[4];
  for(int i=0;i<4;i++){
    ans[i]=0; //0-# memread, 1-# memwrite, 2-# cachehit, 3-# cachemiss
  }
  char action[5];
  unsigned long long address=0;
  int hit=0;
  while(fscanf(file,"%s 0x%llx\n",action,&address)!=EOF){
    hit=cachehit(address,cache,replacepol,A,s,b);//incomplete
    if(hit==1){
      ans[2]+=1;
    }
    else if(hit==0){
      ans[0]+=1;
      ans[3]+=1;
      int index=update(address,cache,s,b);
      replace(address,cache,s,b,index);
    }
    if(strcmp(action,"R")==0){
     
      /*if(hit==0){
	ans[0]+=1;
	ans[3]+=1;
	int index=update(address,cache,s,b);
	replace(address,cache,s,b,index);
	}*/
    }
    else if(strcmp(action,"W")==0){
      ans[1]+=1;
      /*if(hit==0){
	ans[0]+=1;
	ans[3]+=1;
	int index=update(address,cache,s,b);
	replace(address,cache,s,b,index);
      }*/
    }
    //printLL(cache,S,A);
    //printf("%s %llx\n",action,address);
  }
  //printLL(cache,S,A);
  freecache(cache,S,A);
  printf("memread:%d\nmemwrite:%d\ncachehit:%d\ncachemiss:%d\n",ans[0],ans[1],ans[2],ans[3]);
  //printf("C %d, A %d, B %d\n",C,A,B);
  return 0;

}

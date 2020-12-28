#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"second.h"
int check (int argc, char** argv){
  int fin=1;
  if(argc-1!=8){ //checking that 5 inputs have been fed
    return 0;
  }
  int cachesize=atoi(argv[1]);
  int cache2size=atoi(argv[5]);
  if(floor(log2(cachesize))!=log2(cachesize)){ //checking that 1st input (cache size) is pow of 2
    return 0;
  }
  int blocksize=atoi(argv[4]);
  if(floor(log2(blocksize))!=log2(blocksize)){ //checking that 4th input (block size) is pow of 2
    return 0;
  }
  if(floor(log2(cache2size))!=log2(cache2size)){ //checking that 1st input (cache size) is pow of 2
    return 0;
  }
  FILE *curr=fopen(argv[8],"r");
  if(curr==0){
    return 0;
  }
  fclose(curr);
  return fin;
}

int cachehit(unsigned long long address,struct set *cache,int pol,int A,int s,int b,int L2){
  unsigned long long one=1;
  unsigned long long set = ((((one<<s)-1)<<b)&address)>>b;

  //unsigned long block= ((1<<b)-1)&address;
  unsigned long long tag= ((((one<<(48-(s+b)))-1)<<(s+b))&address)>>(s+b);
  //if(L2==0){
  //  printf("L1 %llx\n",tag);
  //}
  //else{
  //   printf("L2 %llx\n",tag);
  //}
    // printf("tag %llu\n",tag);
  //printf("b %d\n",b);
  for (int i=0;i<A;i++){
    if(cache[set].lines[i].valid==1){
      if(cache[set].lines[i].tag==tag){
	if((pol==1 && A!=1)&&L2==0){//moving most recently used to the end of list, note: looks like LRU does not apply to L2
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
	if(L2==1){
	  cache[set].lines[i].valid=0;
	  struct LLNode* temp=cache[set].lines[i].ptr;
	  if(temp->prev!=0){
	    //printf("%d\n",cache[set].back->val);
	    if(temp->next!=0){
	      temp->prev->next=temp->next;
	      temp->next->prev=temp->prev;
	    }
	    else{
	      temp->prev->next=0;
	      cache[set].back=temp->prev;
	    }
	    temp->next=cache[set].front;
	    temp->prev=0;
	    cache[set].front=temp;
	    //temp->prev=0;
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
  unsigned long block= ((one<<b)-1)&address;
  unsigned long long tag= ((((one<<(48-(s+b)))-1)<<(s+b))&address)>>(s+b);
  cache[set].lines[index].valid=1;
  cache[set].lines[index].tag=tag;
  cache[set].lines[index].blocks=block;
}

void replace2layer(unsigned long long address, struct set* cacheL1,struct set* cacheL2,int sizevar[2][7]){
  unsigned long long one=1;
  int s1 = sizevar[0][4];
  int b1 = sizevar[0][5];
  int s2 = sizevar[1][4];
  int b2 = sizevar[1][5];
  unsigned long long setL1 = ((((one<<s1)-1)<<b1)&address)>>b1;
  //unsigned long block= ((1<<b)-1)&address;
  int ind1 = update(address,cacheL1,s1,b1);
  unsigned long long tag= cacheL1[setL1].lines[ind1].tag<<(s1+b1);//((((one<<(48-(s1+b1)))-1)<<(s1+b1))&address);
  unsigned long long addressorig=(setL1<<b1)+tag+cacheL1[setL1].lines[ind1].blocks;
  //printf("%llx\n",tag);
  //printf("orig: %llx\n",addressorig);
  if(cacheL1[setL1].lines[ind1].valid==1){
    int ind2= update(addressorig,cacheL2,s2,b2);
    replace(addressorig,cacheL2,s2,b2,ind2);
    //printf("%llx\n",addressorig);
  }
  replace(address,cacheL1,s1,b1,ind1);
}

void freecache(struct set* cache, int S, int A){
  for (int i=0;i<S;i++){
    //for(int j=0;j<A;j++){
    //  free(cache[i].lines[j].blocks);
    //}
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

int update(unsigned long long address, struct set *cache, int s, int b){//ret index of line to replace, updates the queue to 
  unsigned long long one=1;
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
	printf("%lld, ",cache[i].lines[temp->val].tag);
	temp=temp->next;
      }
      printf("\n");
    
   
  }
   printf("end\n");
}

struct set* populate(char** argv,struct set* cache, int sizevar[2][7],int capindex, int associndex, int bckindex, int polindex, int bcksize){
  int C,S,A,B; //C -cache capacity, S-# of sets, A-Assoc. of Cache, B-blocks in Assoc
  int s,b; //bits for Set and Block in address
  char replacepol=0; //0 is FIFO, 1 is Least Recently Used
  if(strcmp(argv[polindex],"fifo")==0){
    replacepol=0;
  }
  else if(strcmp(argv[polindex],"lru")==0){
    replacepol=1;
  }
  else{
    // printf("error");
    return 0;
  }
  A=0;
  if(capindex!=-1){
    C=atoi(argv[capindex]);
  }
  if(bckindex!=-1){
    B=atoi(argv[bckindex]);
  }
  else{
    B=bcksize;
  }
  S=0;
  s=0;
  b=0;
  char command[6];
  // char* colon;
  for (int i=0;i<5;i++){
    command[i]=argv[associndex][i];
  }
  command[5]='\0';
  //printf("%s",command);
  if(strcmp(argv[associndex],"direct")==0){
    A=1;
  }
  else if(strcmp(command,"assoc")==0){
    if(strlen(argv[associndex])==5){
      A=C/B;
    }
    else{
      //printf("%s",argv[2]);
      sscanf(argv[associndex],"%5s:%d",command,&A);
    }
  }
  else{
    //printf("error");
    return 0;
  }
  S=C/(A*B);
  s=log2(S);
  b=log2(B);
  //printf("%d %d\n",s,b);
  //Creating cache according to specifications
  cache=malloc(S*sizeof(struct set));
  for(int i=0;i<S;i++){
    cache[i].lines=malloc(A*sizeof(struct assoc));
    cache[i].front=0;
    cache[i].back=0;
    for(int j=0;j<A;j++){
      cache[i].lines[j].valid=0;
      cache[i].lines[j].tag=0;
      cache[i].lines[j].blocks=0;//malloc(B*sizeof(int));
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
  
  }
  int i=0;
  if(bckindex==-1){
    i=1;
  }
  sizevar[i][0]=C;
  sizevar[i][1]=S;
  sizevar[i][2]=A;
  sizevar[i][3]=B;
  sizevar[i][4]=s;
  sizevar[i][5]=b;
  sizevar[i][6]=replacepol;
  //for(int j=0;j<7;j++){
  //  printf("Cache Facts: %d, %d\n",j,sizevar[i][j]);
  //}
    return cache;
}

int main(int argc, char* argv[1+argc]){

  int ch=check(argc,argv);
  if(ch==0){
    printf("error");
    return EXIT_SUCCESS;
  }
  FILE* file=fopen(argv[8],"r");
  int sizevar[2][7];
  struct set* cacheL1 = 0;
  cacheL1=populate(argv,cacheL1,sizevar,1,2,4,3,-1);
  if(cacheL1==0){
    printf("error");
    return EXIT_SUCCESS;
  }
  struct set* cacheL2 =0;
  cacheL2=populate(argv,cacheL2,sizevar,5,6,-1,7,sizevar[0][3]);
  if(cacheL2==0){
    printf("error");
    return EXIT_SUCCESS;
  }
  //printf("prepared cold cache");
  //printf("Starting with a Cold Cache");
  //printf("
  //printLL(cache,S,A);

  //Reading trace files
  int ans[2][4];
  
  for(int i=0;i<2;i++){
    for(int j=0;j<4;j++){
      ans[i][j]=0; //0-# memread, 1-# memwrite, 2-# cachehit, 3-# cachemiss
    }
  }
  char action[5];
  unsigned long long address=0;
  int hit=0;
  while(fscanf(file,"%s 0x%llx\n",action,&address)!=EOF){
    hit=cachehit(address,cacheL1,sizevar[0][6],sizevar[0][2],sizevar[0][4],sizevar[0][5],0);//policy,Assoc,s,b, L2 or not
    //printf("%llx L1 %d\n",address,hit);
    if(hit==1){
      ans[0][2]+=1;
    }
    else if(hit==0){
      hit=cachehit(address,cacheL2,sizevar[1][6],sizevar[1][2],sizevar[1][4],sizevar[1][5],1);//handles the L2 delete
      //printf("%llx L2 %d\n",address,hit);
      ans[0][3]+=1;

      if(hit==1){
	ans[1][2]+=1;
	
      }
      else{
	ans[0][0]+=1;
	ans[1][3]+=1;
	
      }
      //int index=update(address,cacheL1,sizevar[0][4],sizevar[0][5]);
      replace2layer(address,cacheL1,cacheL2,sizevar);
    }
    if(strcmp(action,"R")==0){     
      /* if(hit==0){
	ans[0]+=1;
	ans[3]+=1;
	int index=update(address,cache,s,b);
	replace(address,cache,s,b,index);
	}*/
    }
    else if(strcmp(action,"W")==0){
      ans[0][1]+=1;
      /*if(hit==0){
	ans[0]+=1;
	ans[3]+=1;
	int index=update(address,cache,s,b);
	replace(address,cache,s,b,index);
	}*/
    }
    //printLL(cacheL1,sizevar[0][1],sizevar[0][2]);
    //printLL(cacheL2,sizevar[1][1],sizevar[1][2]);
    //printf("%s %llx\n",action,address);
  }
  //printLL(cache,S,A);
  freecache(cacheL1,sizevar[0][1],sizevar[0][2]);
  freecache(cacheL2,sizevar[1][1],sizevar[1][2]);
  printf("memread:%d\nmemwrite:%d\nl1cachehit:%d\nl1cachemiss:%d\n",ans[0][0],ans[0][1],ans[0][2],ans[0][3]);
  printf("l2cachehit:%d\nl2cachemiss:%d\n",ans[1][2],ans[1][3]);
  //printf("C %d, A %d, B %d\n",C,A,B);
  return 0;

}

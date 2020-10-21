#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"third.h"

typedef struct graphvert * gptr;
typedef struct hashnode * hptr;

gptr* initALL(char *name,int pos,gptr* ALL){
  ALL[pos]=malloc(sizeof(struct graphvert));
  char *temp=malloc(strlen(name)+1);
  //printf("%ld",sizeof(*temp));
  strcpy(temp,name);
  ALL[pos]->name=temp;
  ALL[pos]->index=pos;
  ALL[pos]->next=0;
  ALL[pos]->deg=0;
  ALL[pos]->visited=0;
  ALL[pos]->onQ=0;
  //printf("%s",ALL[pos]->name);
  return ALL;
}

gptr* insert(char* conn,int connindex,int index,gptr *ALL){
  gptr temp = ALL[index]->next;
  gptr prev =ALL[index];
  gptr ins = malloc(sizeof(struct graphvert));
  //gptr gcopy= malloc(sizeof(struct graphvert));
  //gcp(gcopy,ALL[index]);
  char *nametemp=malloc(strlen(conn)+1);
  strcpy(nametemp,conn);
  ins->name=nametemp;
  ins->index=connindex;
  ins->deg=0;
  ins->visited=0;
  ins->onQ=0;
  prev->deg+=1;
  while(temp!=0){
    int check=strcmp(conn,temp->name);
    if(check<0){
      prev->next=ins;
      ins->next=temp;
      return ALL;
    }
    /*if(check==0){
      ALL[index]->deg-=1;
      free(nametemp);
      free(ins);
      return ALL;
      }*/
    temp=temp->next;
    prev=prev->next;
  }
  prev->next=ins;
  ins->next=temp;  
  return ALL;
}

void gcp(gptr copy,gptr orig){
  copy->name=orig->name;
  copy->index=orig->index;
  copy->deg=0;
  copy->next=0;
}

void getdegree (int num, gptr* ALL){
  printf("%d\n",ALL[num]->deg);
}

void getchain (int num, gptr* ALL){
  gptr temp=ALL[num]->next;
  while(temp!=0){
    if(temp->next==0){
      printf("%s",temp->name);
    }
    else{
      printf("%s ",temp->name);
    }
    temp=temp->next;
  }
  printf("\n");
}

hptr* initHASH(hptr* HASH, int n){
  for(int i=0;i<n;i++){
    HASH[i]=0;
  }
  return HASH;
}

hptr* insertHASH(char *name, int pos, hptr*HASH, int n){
  int hc=0;
  for(int i=0;i<strlen(name);i++){
    hc+=name[i];
  }
  //printf("%s -> %d\n",name,hc%n);
  int hi=hc%n;
  hptr temp= malloc(sizeof(struct hashnode));
  char *nametemp=malloc(strlen(name)+1);
  strcpy(nametemp,name);
  temp->name=nametemp;
  temp->hashcode=hc;
  temp->index=pos;
  if(HASH[hi]==0){
    temp->next=0;
  }
  else{
    temp->next=HASH[hi];
  }
  HASH[hi]=temp;
  return HASH;
}

int getIndex(char *name, int n,hptr *HASH){
  int hc=0;
  for(int i=0;i<strlen(name);i++){
    hc+=name[i];
  }
  int hi = hc%n;
  hptr temp=HASH[hi];
  while(temp!=0){
    if(strcmp(temp->name,name)==0){
      return temp->index;
    }
    temp=temp->next;
  }
  return -1;
}
  
void printALL(gptr* ALL,int n){
  for (int i=0;i<n;i++){
    gptr temp=ALL[i];
    while(temp!=0){
      printf("%s (%d)",temp->name, temp->deg);
      temp=temp->next;
    }
    printf("\n");
  }
}
void printHASH(hptr *HASH,int n){
  for(int i=0;i<n;i++){
    hptr temp=HASH[i];
    while(temp!=0){
      printf("%s -> %d", temp->name,temp->index);
      temp=temp->next;
    }
    printf("\n");
  }
}

void freenodes (hptr *HASH,gptr* ALL, int n){
  for(int i=0;i<n;i++){
    hptr prev = 0;
    hptr temp = HASH[i];
    while(temp!=0){
      prev=temp;
      temp=temp->next;
      free(prev->name);
      free(prev);
    }
    /*if(temp==0 && prev!=0){
      free(prev->name);
      free(prev);
      }*/
    gptr prev1 = 0;
    gptr temp1 = ALL[i];
    while(temp1!=0){
      prev1=temp1;
      temp1=temp1->next;
      free(prev1->name);
      free(prev1);
    }
    /*if(prev1!=0 && temp1==0){
      free(prev1->name);
      free(prev1);
      }*/
  }
  //free(HASH);
  //free(ALL);
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
	if(back==0){
		return temp;
	}
	back->next=temp;
	return back;
}

struct LLNode *dequeue(struct LLNode *front){
	if(front == 0){
		return front;
	}
	struct LLNode *next = front->next;
	free(front);
	return next;
}
void freespace(struct LLNode *front){
	if(front==0){
		return;
	}
	freespace(front->next);
	free(front);
}
void printLL(struct LLNode *front){
	if(front==0){
		printf("EMPTY");
	}
	while(front!= 0){
		struct LLNode *temp=front->next;
		if(temp==0){
			printf("%d",front->val);
		}
		else{
			printf("%d ",front->val);
		}
		front = front -> next;
	}
	printf("\n");
}

void bfs(char* name, gptr* ALL, hptr* HASH, int hashsize){
  char *temp=malloc(strlen(name)+1);
  strcpy(temp,name);
  struct LLNode*front=0;
  struct LLNode *back=0;
  //int origind=getIndex(name,hashsize,HASH);
  while(front!=0 || name!=0){
    gptr LL=ALL[getIndex(name,hashsize,HASH)];
    // if(LL->visited==0){
    LL->visited=1;
    while(LL!=0){
      if(ALL[LL->index]->visited==0 && ALL[LL->index]->onQ==0){
	back=enqueue(LL->index,back);
	ALL[LL->index]->onQ=1;
	if(front==0){
	  front=back;
	}
	else{
	  back=back->next;
	}
      }
      LL=LL->next;
    }
    if(front==0){
      printf("%s\n",name);
    }
    else{
      printf("%s ",name);
    }
    if(front!=0){
      name=ALL[front->val]->name;
      front =dequeue(front);
      if(front==0){
	back=0;
      }
    }
    else{
      name=0;
    }
  }
  reset(ALL,hashsize);
  free(temp); 
}

void reset (gptr* ALL,int size){
  for(int i=0;i<size;i++){
    ALL[i]->visited=0;
    ALL[i]->onQ=0;
  }
}
    
int main(int argc, char* argv[argc+1]){
  int n=0;
  gptr* ALL=0;
  hptr* HASH=0;
  char*name=malloc(20);
  char* conn=malloc(20);
  for(int i=1;i<argc;i++){
    FILE *curr=fopen(argv[i],"r");
    if(curr==0){
      printf("error\n");
      return EXIT_SUCCESS;
    }
    if(i==1){
      fscanf(curr,"%d",&n);
      ALL=malloc(n*sizeof(struct graphvert*));
      HASH=malloc(n*sizeof(struct hashnode*));
      initHASH(HASH,n);
      for(int j=0;j<n;j++){
	fscanf(curr,"%s",name);
	ALL=initALL(name,j,ALL);
	HASH=insertHASH(name, j, HASH,n);
      }
      
      while(fscanf(curr,"%s %s\n", name, conn)!=EOF){
	//printf("%s -> %s\n",name,conn);
	ALL=insert(conn,getIndex(conn,n,HASH),getIndex(name,n,HASH),ALL);
	//gptr gcopy= malloc(sizeof(struct graphvert));
	//gcp(gcopy,ALL[index]);
	ALL=insert(name,getIndex(name,n,HASH),getIndex(conn,n,HASH),ALL);
      }
    }
   
    else if(i==2){  
      while(fscanf(curr,"%s\n", name)!=EOF){
	bfs(name,ALL,HASH,n);
      }
    }
    //printALL(ALL,n);
    //printHASH(HASH,n);
    fclose(curr);
  }
  //printALL(ALL,n);
  //printHASH(HASH,n);
  freenodes(HASH,ALL,n);
  free(HASH);
  free(ALL);
  free(name);
  free(conn);
  return EXIT_SUCCESS;
}

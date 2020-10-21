#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include"sixth.h"

typedef struct graphvert * gptr;
typedef struct hashnode * hptr;
typedef struct gptrStack * gsptr;
gptr* initALL(char *name,int pos,gptr* ALL){
  ALL[pos]=malloc(sizeof(struct graphvert));
  char *temp=malloc(strlen(name)+1);
  strcpy(temp,name);
  ALL[pos]->name=temp;
  ALL[pos]->index=pos;
  ALL[pos]->next=0;
  ALL[pos]->outdeg=0;
  ALL[pos]->indeg=0;
  ALL[pos]->weight=0;
  ALL[pos]->visited=0;
  ALL[pos]->inStack=0;
  //printf("%s",ALL[pos]->name);
  return ALL;
}

void mergesort(int l,int h, gptr* ALL){
  if(h-l==0){
    return;
  }
  mergesort(l,(h+l)/2,ALL);
  //printf("L: %d ",l);
  mergesort((l+h)/2+1,h,ALL);
  //printf("R: %d ",l);
  merge(l,h,ALL);
  return;
}

void merge(int l, int h, gptr* ALL){
  int rightsi=(h-l+1)/2;
  int leftsi=(h-l+1)/2;
  if((h-l)%2==0){
    leftsi=rightsi+1;
  }
  gptr* tl=malloc(sizeof(gptr)*(leftsi));
  gptr* tr=malloc(sizeof(gptr)*rightsi);
  for(int i=0;i<leftsi;i++){
    tl[i]=ALL[l+i];
  }
  for(int i=0;i<rightsi;i++){
    tr[i]=ALL[(l+h)/2+1+i];
  }

  int indmain=l;
  int indl=0;
  int indr=0;
  while(indl!=leftsi || indr!=rightsi){
    if(indl==leftsi){
      ALL[indmain]=tr[indr];
      indr++;
      indmain++;
      continue;
    }
    if(indr==rightsi){
      ALL[indmain]=tl[indl];
      indl++;
      indmain++;
      continue;
    }
    int cmp=strcmp(tl[indl]->name,tr[indr]->name);
    if(cmp<=0){
      ALL[indmain]=tl[indl];
      indl++;
      indmain++;
      continue;
    }
    else{
      ALL[indmain]=tr[indr];
      indr++;
      indmain++;
    }
  }
  free(tl);
  free(tr);
}

void resetind(gptr* ALL,int n){
  for (int i=0;i<n;i++){
    gptr temp=ALL[i];
    temp->index=i;
  }
}

gptr* insert(char* conn,int connindex,int weight, int index,gptr *ALL){
  gptr temp = ALL[index]->next;
  gptr prev =ALL[index];
  gptr ins = malloc(sizeof(struct graphvert));
  char *nametemp=malloc(strlen(conn)+1);
  strcpy(nametemp,conn);
  ins->name=nametemp;
  ins->index=connindex;
  ins->outdeg=0;
  ins->indeg=0;
  ins->weight=weight;
  ins->visited=0;
  ins->inStack=0;
  prev->outdeg+=1;
  ALL[connindex]->indeg+=1;
  while(temp!=0){
    int check=strcmp(conn,temp->name);
    if(check<0){
      prev->next=ins;
      ins->next=temp;
      return ALL;
    }
    if(check==0){
      ALL[index]->outdeg-=1;
      free(nametemp);
      free(ins);
      return ALL;
    }
    temp=temp->next;
    prev=prev->next;
  }
  prev->next=ins;
  ins->next=temp;  
  return ALL;
}

void getindegree (int num, gptr* ALL){
  printf("%d\n",ALL[num]->indeg);
}

void getoutdegree (int num, gptr* ALL){
  printf("%d\n",ALL[num]->outdeg);
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
      printf("%s in(%d) out(%d)",temp->name, temp->indeg,temp->outdeg);
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
void printMat(gptr* ALL,int* dist,int n){
 
  for(int i=0;i<n;i++){
     if(dist[i]==INT_MAX){
       printf("%s INF\n",ALL[i]->name);
     }
     else{
       printf("%s %d\n",ALL[i]->name, dist[i]);
     }
   }
  printf("\n");    
}

void printGS(gsptr top, int n){
  while(top!=0){
    printf("%s ->",top->node->name);
    top=top->next;
  }
  printf("\n");
}
void freenodes (gsptr top,gptr* ALL,hptr*HASH, int n){
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
  }
  gsptr prev=0;
  gsptr curr=top;
  while(curr!=0){
    prev=curr;
    curr=curr->next;
    free(prev);
  }
  //free(HASH);
  //free(ALL);
}
/*
void freeLL(gptr* ALL,int size){
   gptr prev1 = 0;
   gptr temp1 = ALL[i];
   while(temp1!=0){
      prev1=temp1;
      temp1=temp1->next;
      free(prev1->name);
      free(prev1);
    }
}
*/
gsptr dfs(gsptr top,gptr*ALL,hptr*HASH,int n){
  //int curr=0;
  for(int i=0;i<n;i++){
    if(ALL[i]->visited==1){continue;}
    else{
      //top[curr]=ALL[i];
      top=dfshelp(top,ALL[i],ALL,HASH,n);
      //resetVisIn(ALL,n,1);
    }
    if(top==0){
      printf("CYCLE\n");
      resetVisIn(ALL,n,0);
      break;
    }
  }
  return top;
}
void resetVisIn(gptr*ALL, int n, int choice){
  for(int i=0;i<n;i++){
    if(choice==0){
      ALL[i]->visited=0;
    }
    else{
      ALL[i]->inStack=0;
    }
  }
}
//earlier void
gsptr dfshelp(gsptr top, gptr L, gptr*ALL,hptr*HASH,int n){
  /*if(L->inStack==1){
    gsptr prev=0;
    while(top!=0){
      prev=top;
      top=top->next;
      free(prev);
    }
    return top;
    }*/
  if(L==0 || L->visited==1){
    return top;
  }
  L->visited=1;
  L->inStack=1;
  gptr temp=L->next;
  while(temp!=0){
    top=dfshelp(top,ALL[temp->index],ALL,HASH,n);
    if(top==0){
      return top;
    }
    temp=temp->next;
  }
  L->inStack=0;
  if(top->node==0){
    top->node=ALL[L->index];
  }
  else{
    gsptr temp=malloc(sizeof(struct gptrStack));
    temp->node=ALL[L->index];
    temp->next=top;
    top=temp;
    //printGS(top,n);
  }
  //printf("%d\n",curr);
  
  return top;
}

void initDist(int* dist,int n){
  for(int i=0;i<n;i++){
    // for(int j=0;j<n;j++){
      dist[i]=INT_MAX;
      // }
  }
  // printMat(dist,n);
}

void sspa (gptr L, int* dist,gsptr top, gptr* ALL,hptr* HASH,int n){
  initDist(dist,n);
  int currind=L->index;
  dist[currind]=0;
  //printMat(dist,n);
  //printALL(top,n);
  gsptr topptr=top;
  while(topptr->node!=L){
    topptr=topptr->next;
  }
  //printGS(topptr,n);
  while(topptr!=0){
    gptr temp=topptr->node;
    currind=temp->index;
    temp=temp->next;
    while(temp!=0){
      //printf("%s: %d > %d\n",temp->name, dist[temp->index],dist[currind]+temp->weight);
      //if(!((dist[currind]>0 && temp->weight>0)&&(dist[currind]<0)&&!((dist[currind]<0 && temp->weight<0)&&(dist[currind]>0))
      if(dist[temp->index]>dist[currind]+temp->weight && dist[currind]!=INT_MAX){
	dist[temp->index]=dist[currind]+temp->weight;
      }
      //printMat(dist,n);
      temp=temp->next;
    }
    topptr=topptr->next;
  }
  printMat(ALL,dist,n);
}

void dijkstra (gptr L, int* dist, gptr* ALL,hptr* HASH,int n){
  resetVisIn(ALL,n,0);
  initDist(dist,n);
  int currind=L->index;
  dist[currind]=0;
  //printMat(dist,n);
  //printALL(top,n);
  gptr* priorque=malloc(n*sizeof(gptr));
  for(int i=0;i<n;i++){
    priorque[i]=0;
  }
  priorque[0]=L;
  //printGS(topptr,n);
  while(priorque[0]!=0){
    gptr temp=priorque[0];
    delHeap(dist,priorque,n);
    temp->visited=1;
    temp->inStack=0;
    currind=temp->index;
    temp=temp->next;
    while(temp!=0){
      if(ALL[temp->index]->visited==0){
	if(dist[temp->index]>dist[currind]+temp->weight && dist[currind]!=INT_MAX){
	  dist[temp->index]=dist[currind]+temp->weight;
	}
	if(ALL[temp->index]->inStack==0){
	  insHeap(ALL[temp->index],dist,priorque,n);
	  ALL[temp->index]->inStack=1;
	}
	else{
	  siftup(ALL[temp->index],dist,priorque,n);
	}
      }
      //printMat(dist,n);
      temp=temp->next;
    }
  }
  free(priorque);
  printMat(ALL,dist,n);
}

void insHeap(gptr curr, int* dist, gptr* priorque,int n){
  int index=0;
  for(int i=0;i<n;i++){
    if(priorque[i]==0){
      priorque[i]=curr;
      index=i;
      break;
    }
  }
  while(index!=0 && dist[priorque[(index-1)/2]->index]>dist[priorque[index]->index]){
    gptr temp=priorque[(index-1)/2];
    priorque[(index-1)/2]=curr;
    priorque[index]=temp;
    index=(index-1)/2;
  }
}

void siftup(gptr curr, int* dist,gptr* priorque,int n){
  int index=0;
  while(priorque[index]!=curr){
    index++;
  }
  while(index!=0 && dist[priorque[(index-1)/2]->index]>dist[priorque[index]->index]){
    gptr temp=priorque[(index-1)/2];
    priorque[(index-1)/2]=curr;
    priorque[index]=temp;
    index=(index-1)/2;
  }
}
void delHeap(int* dist,gptr* priorque, int n){
  priorque[0]=0;
  for(int i=1;i<n;i++){
    if(priorque[i]==0){
      if(i==1){
	return;
      }
      priorque[0]=priorque[i-1];
      priorque[i-1]=0;
      break;
    }
  }
  int index=0;
  gptr temp=0;
  while(2*index+1<n){
    if(priorque[2*index+1]!=0 && dist[priorque[index]->index]>dist[priorque[2*index+1]->index]){
      if(2*index+2<n && (priorque[2*index+2]!=0 &&  dist[priorque[2*index+1]->index]>dist[priorque[2*index+2]->index])){
	temp=priorque[2*index+2];
	priorque[2*index+2]=priorque[index];
	priorque[index]=temp;
	index=2*index+2;
      }
      else{
	temp=priorque[2*index+1];
	priorque[2*index+1]=priorque[index];
	priorque[index]=temp;
	index=2*index+1;
      }
      continue;
    }
    else if(2*index+2<n && ( priorque[2*index+2]!=0 && dist[priorque[index]->index]>dist[priorque[2*index+2]->index])){
      	temp=priorque[2*index+2];
	priorque[2*index+2]=priorque[index];
	priorque[index]=temp;
	index=2*index+2;
	continue;
    }
    else{
      return;
    }
  }
}

int main(int argc, char* argv[argc+1]){
  int n=0;
  gptr* ALL=0;
  hptr* HASH=0;
  gsptr top=0;
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
      top=malloc(sizeof(struct gptrStack));
      top->node=0;
      top->next=0;
      initHASH(HASH,n);
      for(int j=0;j<n;j++){
	fscanf(curr,"%s",name);
	ALL=initALL(name,j,ALL);
      }
      mergesort(0,n-1,ALL);
      resetind(ALL,n);

      for(int j=0;j<n;j++){
	HASH=insertHASH(ALL[j]->name, j, HASH,n);
      }
      //printALL(ALL,n);
      int weight=0;
      while(fscanf(curr,"%s %s %d\n", name, conn,&weight)!=EOF){
	//printf("%s -> %s\n",name,conn);
	ALL=insert(conn,getIndex(conn,n,HASH),weight,getIndex(name,n,HASH),ALL);
      }
      top=dfs(top,ALL,HASH,n);
      //printGS(top,n);
      fclose(curr);
      if(top==0){
	break;
      }
    }
    if(i==2){
      int* dist=malloc(n*sizeof(int));
      while(fscanf(curr,"%s",name)!=EOF){
	dijkstra(ALL[getIndex(name,n,HASH)],dist,ALL,HASH,n);
      }     
      free(dist);
      fclose(curr);
    }
    
  }
  //printALL(ALL,n);
  //printHASH(HASH,n);
  // printALL(top,n);
  freenodes(top,ALL,HASH,n);
  free(HASH);
  free(ALL);
  //free(top);
  free(name);
  free(conn);
  return EXIT_SUCCESS;
}

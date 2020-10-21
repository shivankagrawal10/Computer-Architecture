#ifndef __first_H
#define __first_H
struct graphvert{
  char* name;
  struct graphvert * next;
  int indeg; //only applicable for first node of each row in LL
  int outdeg;//only applicable for first node of each row in LL
  int weight; //weight between first node in LL and current node
  int index; //stores index of node's primary identity (first node in Adj LL)
  int visited; //to make sure graph does not repeat visits
  int inStack;//for checking whether graph is a cycle - will be used in dfs
};

struct hashnode{
  char* name;
  int hashcode;
  int index;
  struct hashnode * next;
};

typedef struct graphvert * gptr;
typedef struct hashnode * hptr;

struct gptrStack{
  gptr node;
  struct gptrStack *next;
};

typedef struct gptrStack * gsptr;
//Core methods for creating graph structure
gptr* initALL(char *,int,gptr*);
gptr* insert(char*,int,int,int,gptr *);
void resetVisIn(gptr*,int, int);
void gcp(gptr,gptr);
void getdegree (int, gptr*);
void getchain (int, gptr*);
hptr* initHASH(hptr*, int);
hptr* insertHASH(char *, int, hptr*, int);
int getIndex(char *, int,hptr *);
void printALL(gptr*,int);
void printHASH(hptr *,int);
void printMat(gptr*, int*,int);
void freenodes (gsptr,gptr*,hptr*, int);

//dfs functionality
gsptr dfs(gsptr,gptr*,hptr*,int);
gsptr dfshelp(gsptr,gptr,gptr*,hptr*,int);
void mergesort(int,int,gptr*);
void merge(int,int,gptr*);
void resetind(gptr*,int);
void sspa (gptr,int*,gsptr,gptr*,hptr*,int);
void dijkstra (gptr,int*,gptr*,hptr*,int);
void insHeap(gptr,int*,gptr*,int);
void siftup(gptr,int*,gptr*,int);
void delHeap(int*,gptr*,int);
#endif

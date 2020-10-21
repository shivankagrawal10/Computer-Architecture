#ifndef __first_H
#define __first_H
struct graphvert{
  char* name;
  struct graphvert * next;
  int indeg;
  int outdeg;
  int weight;
  int index;
  int visited;
};

struct hashnode{
  char* name;
  int hashcode;
  int index;
  struct hashnode * next;
};

typedef struct graphvert * gptr;
typedef struct hashnode * hptr;

//Core methods for creating graph structure
gptr* initALL(char *,int,gptr*);
gptr* insert(char*,int,int,int,gptr *);
void gcp(gptr,gptr);
void getdegree (int, gptr*);
void getchain (int, gptr*);
hptr* initHASH(hptr*, int);
hptr* insertHASH(char *, int, hptr*, int);
int getIndex(char *, int,hptr *);
void printALL(gptr*,int);
void printHASH(hptr *,int);
void freenodes (hptr *,gptr*, int);

//dfs functionality
void dfs(gptr*,hptr*,int);
void dfshelp(gptr,gptr*,hptr*,int);
void mergesort(int,int,gptr*);
void merge(int,int,gptr*);
void resetind(gptr*,int);
#endif

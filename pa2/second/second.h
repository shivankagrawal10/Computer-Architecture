#ifndef __first_H
#define __first_H
struct graphvert{
  char* name;
  struct graphvert * next;
  int indeg;
  int outdeg;
  int weight;
  int index;
};

struct hashnode{
  char* name;
  int hashcode;
  int index;
  struct hashnode * next;
};

typedef struct graphvert * gptr;
typedef struct hashnode * hptr;

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
  
#endif

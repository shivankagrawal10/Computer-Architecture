#ifndef __THIRD_H
#define __THIRD_H
struct graphvert{
  char* name;
  struct graphvert * next;
  int deg;
  int index;
  int visited;
  int onQ;
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
gptr* insert(char*,int,int,gptr *);
void gcp(gptr,gptr);
void getdegree (int, gptr*);
void getchain (int, gptr*);
hptr* initHASH(hptr*, int);
hptr* insertHASH(char *, int, hptr*, int);
int getIndex(char *, int,hptr *);
void printALL(gptr*,int);
void printHASH(hptr *,int);
void freenodes (hptr *,gptr*, int);


struct LLNode{
	int val;
	struct LLNode *next;
};
struct LLNode *enqueue(int,struct LLNode *);
struct LLNode *dequeue(struct LLNode *);
void freespace(struct LLNode *);
void printLL(struct LLNode *);//THIS IS PROTOTYPE (DEFINES FUNC AND RETURN TYPE)

void bfs(char*,gptr*,hptr*,int);
void reset(gptr*,int);
#endif

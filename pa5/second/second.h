#ifndef __FIRST_H
#define __FIRST_H

struct set{
  struct assoc* lines;
  struct LLNode* front;
  struct LLNode* back;
  struct LLNode* block;
};
struct assoc{
  char valid;
  unsigned long long tag;
  unsigned long long blocks;
  struct LLNode *ptr;
};
struct LLNode{
  int val;
  struct LLNode *next;
  struct LLNode *prev;
};
struct LLNode *enqueue(int,struct LLNode *);
struct LLNode *push(int, struct LLNode *);


int check(int,char**);
struct set* populate(char**,struct set*,int[2][7],int,int,int,int,int);
int cachehit(unsigned long long,struct set *,int,int,int,int,int);
void replace(unsigned long long,struct set*,int,int,int);
void replace2layer(unsigned long long, struct set*,struct set*,int[2][7]);
int update(unsigned long long,struct set *, int, int);
void printLL(struct set*,int,int);
#endif

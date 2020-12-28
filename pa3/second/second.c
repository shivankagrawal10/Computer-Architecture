/*
Key Lesson:
unsigned 4 bytes
unsigned long 4 bytes
unsigned long long 8 bytes
*/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int* assignzero(int*,int);
int* int2unsign(int*,unsigned long long,int);
int* int2signed(int*,long long, int);
void print(int*,int);

int* assignzero(int* bin,int end){
  for(int i=0;i<end;i++){
    bin[i]=0;
  }
  return bin;
}

void print(int* bin,int size){
  for(int i=size-1;i>=0;i--){
    printf("%d",bin[i]);
  }
  printf("\n");
}

int* int2unsign(int* bin,unsigned long long num, int size){
  if(size<=0){
    printf("Error, size <= 0\n");
    return bin;
  }
  if(num==0){
    print(bin,size);
    return bin;
  }
  int significant=0;
  unsigned long long sigval=1;//need to make this unsigned so that power of 2 is positive at end
  while(sigval < num && significant<63){
    significant++;
    sigval= sigval<<1;
  }
  if(sigval!=num ){
    significant--;
    sigval=sigval-sigval/2;
  }
  int counter=significant;
  for(;counter>=0;counter--){
    if(num==0){
      break;
    }
    if(sigval<=num){
      num=num-sigval;
      bin[counter]=1;
    }
    sigval=sigval-sigval/2;
  }
  //print(bin,size);
  //bin=assignzero(bin,significant+1);
  return bin;
}

int* int2sign(int* bin, long long num, int size){
  if(num==0){
    return bin;
  }
  long long sigval=0;
  long long one=1;
  if(num<0){   
     bin[size-1]=1;
     if(size==1 || (abs(num)>= one<<size || num == one<<size)){
       return bin;
     }
     sigval=-(one<<(size-1));
     num=num-sigval;
  }
  else{
    if(num>=(one<<(size-1))){
      for(int i=size-2;i>=0;i--){
	bin[i]=1;
      }
      return bin;
    }
  }
  sigval=one<<(size-2);
  //bin=int2unsign(bin,num-sum,size-1);
  for(int counter=size-2;counter>=0;counter--){
    if(num==0){
      break;
    }
    if(sigval<=num){
      num=num-sigval;
      bin[counter]=1;
    }
    sigval=sigval-sigval/2;
  }
  
  //print(bin,size);
  //bin=assignzero(bin,64);
  return bin;
}

int main(int argc, char * argv[argc+1]){
  long long num=0;
  int size=0;
  FILE* fp=fopen(argv[1],"r");
  if(fp==0){
    printf("Cannot open file\n");
    return EXIT_SUCCESS;
  }
  //long long one=1;
  //printf("%lld",~(one<<63));
  int* bin=malloc(64*sizeof(int));
  bin=assignzero(bin,64);
  while(fscanf(fp,"%lld %d",&num,&size)!=EOF){
    //printf("start");
    //printf("%llu %d\n",num,size);
    // printf("%llu %d\n",~num,size);
    bin=int2sign(bin,num,size);
    print(bin,size);
    bin=assignzero(bin,64);
  }
  //unsigned long long  x=0;
  //printf("%llu, %llu",x,~x);
  free(bin);
}

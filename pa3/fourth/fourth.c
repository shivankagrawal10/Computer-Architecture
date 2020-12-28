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
int cannonical(int*,double,int);
void print(int*,int);
void printcan(int*,int,int);
void printval(int*,int,int);

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

void printval(int* bin,int size, int uors){
  if(uors==0){//prints signed
    long long sum=0;
    long long one=1;
    if(bin[size-1]==1){
      if(size!=64){
	sum=-(one<<(size-1));
      }
      else{
	sum=(one<<(size-1));
      }
    }
    for(int i=size-2;i>=0;i--){
      // printf("%lld %d",sum,size);
      if(bin[i]==1){
	sum+=one<<i;
      }
    }
    printf("%lld\n",sum);
  }
  else{//prints unsigned
    unsigned long long sum=0;
    unsigned long long one =1;
    for(int i=0;i<size;i++){
       if(bin[i]==1){
	sum+=one<<i;
      }
    }
    printf("%llu\n",sum);
  }
}
    

int* int2unsign(int* bin,unsigned long long num, int size){
  if(size<=0){
    printf("Error, size <= 0\n");
    return bin;
  }
  if(num==0){
    //print(bin,size);
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

int cannonical(int* bin,double num,int size){
  //double sum=0;
  //double one=1;
  double temp=1;
  int significant=0;
  while(temp < num){
    significant++;
    temp*=2 ;
  }
  if(temp>num && significant==0){
    while(temp>num){
      //printf("temp: %le, num: %le,sig: %d\n",temp,num,significant);
      temp/=2;
      significant--;
    }
    //printf("temp: %le, num: %le,sig: %d\n",temp,num,significant);
    if(temp!=num){
      //significant++;
      //temp*=2 ;
    }
    //printf("temp: %le, num: %le,sig: %d\n",temp,num,significant);
  }
  else if(temp!=num ){
    significant--;
    temp=temp/2;
  }
  for(int counter=size;counter>=0;counter--){
    //printf("%le\n",num);
    if(counter==size){
      num=num-temp;
    }
    else if(num==0){
      bin[counter]=0;
      continue;
    }
    else if(temp<=num){
      num=num-temp;
      bin[counter]=1;
    }
    temp=temp/2;
  }
  return significant;
}

void printcan(int* bin,int size, int dec){
  printf("1.");
  for(int i=size-1;i>=0;i--){
    printf("%d",bin[i]);
  }
  printf(" %d\n",dec);
}

int main(int argc, char * argv[argc+1]){
  double num=0;
  int size=0;
  int dec=0;
  FILE* fp=fopen(argv[1],"r");
  if(fp==0){
    printf("Cannot open file\n");
    return EXIT_SUCCESS;
  }
  int* bin=malloc(64*sizeof(int));
  bin=assignzero(bin,64);
  while(fscanf(fp,"%le %d",&num,&size)!=EOF){
    dec=cannonical(bin,num,size);
    printcan(bin,size,dec);
    bin=assignzero(bin,64);
  }
  free(bin);
}

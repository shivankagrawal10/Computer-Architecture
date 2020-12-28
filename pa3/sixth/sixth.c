/*
Key Lesson:
unsigned 4 bytes
unsigned long 4 bytes
unsigned long long 8 bytes
*/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<float.h>
int* assignzero(int*,int);
int* int2unsign(int*,unsigned long long,int);
int* int2signed(int*,long long, int);
int cannonicalfrac(int*,double,int,int);
int* cannonicalexp(int*,int, int,int);
float bin2float(int*,int,int,int);
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

int cannonicalfrac(int* bin,double num,int size,int bias){
  //double sum=0;
  //double one=1;
  double temp=1;
  int significant=0;
  while(temp < num){
    significant++;
    temp*=2 ;
  }
  //printf("%le %le %d\n",num,temp,significant);
  if(temp>num && significant==0){
    while(temp>num){
      temp/=2;
      significant--;
    }
    if(temp!=num){//no condition needed because we want it to over step
     
    }
  }
  else if(temp!=num){ //Adjusting power and place value because went over by 1 
    significant--;
    temp=temp/2;
  }
  if(significant < 1-bias){
    significant=1-bias;
    temp=pow(2,(1-bias));
  }
  //printf("%le %le %d\n",num,temp,significant);
  //printcan(bin,num,significant);
  for(int counter=size;counter>=0;counter--){
    //printf("%le\n",num);
    if(counter==size && significant!=1-bias){
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
  //print(bin,size);
  if(num!=0){
      int mask=1;
      if(num>=temp){
	if(num==temp && bin[0]==0){ //if binary is stuck between and the lower number is even, round down
	  return significant;
	}
	for(int i=0;i<size;i++){//rounding up
	  if(mask==0){
	    break;
	  }
	  bin[i]=bin[i]^mask;
	  if(bin[i]==1){
	    mask=0;
	  }
	  if(i==size-1 && bin[i]==0){
	    for(int j=0;j<size;j++){//if rounds up such that cannot rep by frac, then increase exp
	      if(bin[j]==1){break;}
	      else if(j==size-1){
		significant++;
	      }
	    }
	  }
	}
      }
  }
      

  return significant;
}

int* cannonicalexp(int* bin,int dec, int expsize,int fracsize){
  if(expsize<=0 || fracsize<=0){
    printf("Error, size <= 0\n");
    return bin;
  }
  if(dec==0){
    //print(bin,size);
    return bin;
  }
  int significant=0;
  unsigned long long sigval=1;//need to make this unsigned so that power of 2 is positive at end
  while(sigval < dec){
    significant++;
    sigval= sigval<<1;
  }
  if(sigval!=dec){
    significant--;
    sigval=sigval-sigval/2;
  }
  int counter=significant;
  for(;counter>=0;counter--){
    if(dec==0){
      break;
    }
    if(sigval<=dec){
      dec=dec-sigval;
      bin[counter+fracsize]=1;
    }
    sigval=sigval-sigval/2;
  }
  return bin;  
}
  
void printcan(int* bin,int size, int dec){
  printf("1.");
  for(int i=size-1;i>=0;i--){
    printf("%d",bin[i]);
  }
  printf(" %d\n",dec);
}

float bin2float(int* bin,int size, int expsize, int fracsize){
  float frac=0;
  int sign=1, exp=0;
  int bias=(1<<(expsize-1))-1;
  if(bin[size-1]==1){
    sign=-1;
  }
  int iszero=1;
  for(int i=fracsize+expsize-1;i>=fracsize;i--){
    if(bin[i]==1){
      iszero=0;
    }
    exp+=bin[i]<<(i-fracsize);
  }
  for(int i=fracsize-1;i>=0;i--){
    frac+=bin[i]<<i;
  }
  if(!iszero){
    frac=1+frac*powf(2,-fracsize);
    exp=exp-bias;
  }
  else{
    frac=frac*powf(2,-fracsize);
    exp=1-bias;
  }
  frac=sign*frac*pow(2,exp);
  return frac;
}
int main(int argc, char * argv[argc+1]){
  unsigned long long num=0;
  int size=0, expsize=0, fracsize=0;
  float fl=0;
  //int dec=0, bias=0, one=1,
  int precision=0;
  FILE* fp=fopen(argv[1],"r");
  if(fp==0){
    printf("Cannot open file\n");
    return EXIT_SUCCESS;
  }
  int* bin=malloc(64*sizeof(int));
  bin=assignzero(bin,64);
  while(fscanf(fp,"%d %d %d 0x%llx %d",&size,&expsize,&fracsize,&num,&precision)!=EOF){
    bin=int2unsign(bin,num,size);//extracting binary representation of hexadecimal
    fl = bin2float(bin,size,expsize,fracsize);
    printf("%.*f\n",precision,fl);

    /*
    if(num<0){
      bin[size-1]=1;
    }
    bias=(one<<(expsize-1))-1;
    num=fabs(num);
    dec=cannonicalfrac(bin,num,fracsize,bias);
    if(dec==1-bias){
      dec=0;
    }
    else{
      dec+=bias;
    }
    cannonicalexp(bin,dec,expsize,fracsize);
    //printcan(bin,size,dec);
    */
    //print(bin,size);
    bin=assignzero(bin,64);
  }
  free(bin);
}

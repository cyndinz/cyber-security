// TEAcipher.cpp:defines the entry point for the console application.

#include"stdafx.h"
#include"string.h"

/*take 64 bits of data in v[0] and v[1] and 128 bits of key[0] - key[3]*/

#define num_rounds 32
void encipher(unsigned int v[2], unsigned int key[4]){
  unsigned int i;
  unsigned int v0=v[0], v1=v[1], sum=0, delta=0x9E3779B9;
  for(i=0; i<num_rounds; i++){
    v0 += (((v1<<4) ^ (v1>>5)) + v1) ^ (sum + key[sum & 3]);
    sum += delta;
    v1 += (((v0<<4) ^ (v0>>5)) + v0) ^ (sum + key[(sum>>11) & 3]);
  }
  v[0]=v0; v[1]=v1;
}


#define num_rounds 32
void decipher(unsigned int v[2], unsigned int key[4]){
  unsigned int i;
  unsigned int v0=v[0], v1=v[1], delta=0x9E3779B9, sum=delta*num_rounds;
  for (i=0; i<num_rounds; i++){
    v1 -= (((v0<<4) ^ (v0>>5)) + v0) ^ (sum + key[(sum>>11) & 3]);
    sum -= delta;
    v0 += (((v1<<4) ^ (v1>>5)) + v1) ^ (sum + key[sum & 3]);
  }
  v[0]=v0; v[1]=v1;
}


int main(int argc, char* argv[]){
  if(argc !=5){
    printf("\nUsage:%s e(encipher) password  sourceFileName outFileName");
    printf("\nUsage:%s d(decipher) password  sourceFileName outFileName");
    return 0;
  }
  char password[100]={0}; //={0} is important
  strcpy(password, argv[2]);
  FILE *fpin=fopen(argv[3], "rb");
  FILE *fpout=fopen(argv[4], "wb");
  if(fpin!=NULL && fpout!=NULL){
    while(!feof(fpin)){
      unsigned int msg[2];
      msg[0]=msg[1]=0;
      if(fread(msg, 1, 8, fpin)==0) break;
      if(argv[1][0]=='e'){
        encipher(msg, (unsigned int*) password);
      }else{
        decipher(msg, (unsigned int*) password);
      }
      fwrite(msg, 1, 8, fpout);
    }
  }
  fclose(fpin);
  fclose(fpout);
  return 0;
}

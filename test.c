#include<stdlib.h>
#include <stdio.h>
int main(){
  char * test = malloc(70);
  for(int i = 0; i < 75; i++){
    test[i] = 'A';
  }
  printf("test1: %s\n", test);
}
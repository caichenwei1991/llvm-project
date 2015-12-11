#include <iostream>

int main(){
  int a = -1;
  int b = 2;
  int c = a-b;
  int d = 8;
  while(a < 100){
    a = a*(-1);
    b = b + 1;
    c = a*b;
  }

  if(1){
    d = 2;
  }
  c = d+a;


  return b+c;
}

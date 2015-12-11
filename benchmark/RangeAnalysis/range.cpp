#include <iostream>

int main(){
  int a = -1;
  int b = 2;
  int c = a-b;
  while(b < 100){
    a = a*(-1);
    b = b + 1;
    c = a*b;
  }


  return b+c;
}

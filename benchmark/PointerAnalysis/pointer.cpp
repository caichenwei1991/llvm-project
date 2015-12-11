#include <iostream>

int main(){
  int  a = 1, b=2, c=3;
  int *p1, *p2, *p4, *p5, *p7, *p8;
  int **p3, **p6;
  //test A = &B
  if(2){
    p1 = &a;
  }
  p1 = &b;
  //test A = B
  p2 = p1;
  //test A = *B
  p5 = &c;
  p3 = &p5;
  p4 = *p3;
  //test *A = B
  p6 = &p7;
  *p6 = p1;




  return *p1;
}

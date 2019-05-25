
#include <iostream>
#include "antSys.h"
// #include "elitistAntSys.h"
using namespace std;

int main(){
      srand(time(NULL));
      string fileName= "prueba1.txt";
      cout<<"AntColonySystema \n";
      AntSystem r1(100,50,0,0.1,true);

      // ------------------------------------------------
      // string fileName= "ElitistAntSysOut.txt";
      // cout<<"ElitistAntSystema \n";
      // AntSystem r1(50,10,0,10,false);
      r1.inicio(fileName);
      
      
}
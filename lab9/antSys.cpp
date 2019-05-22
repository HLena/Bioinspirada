
#include <iostream>
#include "antSys.h"
// #include "elitistAntSys.h"
using namespace std;

int main(){
      srand(time(NULL));
      // string fileName= "AntSysOut_.txt";
      // cout<<"AntSystema \n";
      // AntSystem r1(100,5,0,0.1,true);

      // ------------------------------------------------
      string fileName= "ElitistAntSysOut.txt";
      cout<<"ElitistAntSystema \n";
      AntSystem r1(50,5,0,10.0,false);
      r1.inicio(fileName);
      
      
}
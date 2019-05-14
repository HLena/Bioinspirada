
#ifndef TYPES_H
#define TYPES_H

#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <tgmath.h>
#include <algorithm>    // std::sort
using namespace std;

typedef struct
{
      string cadena;
      float fitness = 0;
      vector<int> values;
}indv;
typedef vector<int> vi;
vector<indv> pop; //almacenamiento de poblacion
ofstream file;


int np;     //numero de poblacion
float pc;   //probabilidad de cruce
float pr;   //probabilidad de reproduccion
float pm;   //probabilidad de mutacion
int iterations;
int tam_cadena = 7;
char operators[] = {'+','-','*','%','/'};
float inputs[]={0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9};
float outputs[]={0,0.005,0.02,0.045,0.08,0.125,0.18,0.245,0.32,0.405};



#endif
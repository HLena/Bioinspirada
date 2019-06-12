#include<bits/stdc++.h>
using namespace std;

typedef int num;
typedef float numf;

num iteraciones = 3;
num CS = 6;
num NS = 3;
num D = 2;
num L = (CS*D)/2;
num MCN = 20;
numf lim_inf = -5;
numf lim_sup = 5;
num k, j;
numf phi;
numf Sumfit=0;

typedef struct
{
     num id;
     numf x1, x2;
     numf fx=0;
     numf fit=0;
     num cont=0;
     numf p=0;

}solucion;

vector<solucion> vs,vst;
solucion mj;

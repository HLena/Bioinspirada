#ifndef EVOLUCIONDIF_H
#define EVOLUCIONDIF_H


#include <iostream>
#include <vector>
#include <ctime>
#include <tuple>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <math.h>
#include <iomanip>
#include <stdlib.h>
using namespace std;

#define M_PI 3.14159265358979323846

typedef double num;

typedef struct{
      num x; 
      num y;
      num fitness;
   
}ind;

//funciones externas
num funcion(num,num);

bool odenar(ind, ind);


class evolucionDif
{
      private:
            num F;
            num CR;
            num lim_inf;
            num lim_sup;
            int n_ind;
            int iteraciones;
            int target;
      public:
            ofstream file;
            vector<ind> individuos;
            vector<ind> hijos;
            evolucionDif(int);
            int torneo();
            void crearPoblacion();
            void hallar_xy(ind &);
            void mutacion(int, int, ind&);
            void seleccionarIndividuos();
            void cruzamiento(ind&);
            void mostrarPoblacion();
            void seleccionMejorIndividuo(ind, ind);
            void inicio();



};

int evolucionDif::torneo()
{
	// srand( time( NULL ) );
	int p1,p2;
	p1 = rand()%individuos.size();
	do p2 = rand()%individuos.size();while(p1 == p2);
	return (individuos[p1].fitness > individuos[p1].fitness)? p1: p2;
}


evolucionDif::evolucionDif(int it)
{
      iteraciones = it;
      n_ind = 100 + rand()%120;
      F = 0.5;
      CR = 0.9;
      lim_inf = -1;
      lim_sup = 2;
      target = 0;
}

void evolucionDif::hallar_xy(ind& i)
{
      // ((double) rand()*(101)/(double)RAND_MAX-0);
      do 
      i.x =((num)rand() * (lim_sup-lim_inf)) /(num)RAND_MAX + lim_inf;
      while(i.x < lim_inf && i.x > lim_sup);
      do 
      i.y =((num)rand() * (lim_sup-lim_inf)) /(num)RAND_MAX + lim_inf; 
      while(i.y < lim_inf && i.y > lim_sup);
}

void evolucionDif::crearPoblacion()
{
      while(individuos.size() < n_ind){
            ind tmp;
            hallar_xy(tmp);
            tmp.fitness = funcion(tmp.x,tmp.y);
            individuos.push_back(tmp);
      }
      file<<"***INDIVIDUOS CREADOS***"<<endl;
      
}



void evolucionDif::mutacion(int i1,int i2, ind &vd)
{
      int itm;
      do itm = torneo(); while(itm == i1 or itm == i2);
      // file<<"vm:"<<itm<<"]"<<endl;
      file<<"VTarget("<<target<<"):("<<individuos[target].x<<","<<individuos[target].y<<")"<<"["<<individuos[target].fitness<<"]"<<endl;
      file<<"Vid("<<i1<<"):("<<individuos[i1].x<<","<<individuos[i1].y<<")"<<"["<<individuos[i1].fitness<<"]"<<endl;
      file<<"Vid("<<i2<<"):("<<individuos[i2].x<<","<<individuos[i2].y<<")"<<"["<<individuos[i2].fitness<<"]"<<endl;
      file<<"Vm("<<itm<<"):("<<individuos[itm].x<<","<<individuos[itm].y<<")"<<"["<<individuos[itm].fitness<<"]"<<endl;
      vd.x = (individuos[i1].x - individuos[i2].x)* F +individuos[itm].x;
      vd.y = (individuos[i1].y - individuos[i2].y)* F +individuos[itm].y;
      file<<"***INDIVIDUO MUTADO***\n";
      file<<"("<<vd.x<<","<<vd.y<<")"<<endl;
      
}


void evolucionDif(ind &a , ind &b){
      (a.fitness > b.fitness)? hijos.push_back(a):hijos.push_back(b);

}
void evolucionDif::cruzamiento(ind &mt)
{     
      file<<"***CRUZAMIENTO***\n";
      // ((num)rand() * (lim_sup-lim_inf)) /(num)RAND_MAX + lim_inf;
      num nj = ((num) rand()*(1)/(num)RAND_MAX-0);
      file<<"nj: "<<nj<<endl;
      if (nj > CR) mt.x = individuos[target].x;

      nj = ((num) rand()*(1)/(num)RAND_MAX-0);
      file<<"nj: "<<nj<<endl;
      if (nj > CR) mt.y = individuos[target].y;

      mt.fitness = funcion(mt.x, mt.y);
      file<<"("<<mt.x<<","<<mt.y<<")"<<endl;
      seleccionMejorIndividuo(mt,individuos[target]);
      
}
void evolucionDif::seleccionarIndividuos()
{
      int id1, id2;
      ind vm;
      id1 = torneo();
      do id2 = torneo(); while(id1 == id2);
      // file<<"vetores: [target:"<<target<<" ind:"<<id1<<" ind:"<<id2;
      file<<"***INDIVIDUOS SELECCIONADOS***\n";
      mutacion(id1, id2, vm);
      cruzamiento(vm);

}

void evolucionDif::mostrarPoblacion()
{
      for (size_t i = 0; i < individuos.size(); i++)
      {
            file<<"("<<individuos[i].x<<" "<<individuos[i].y<<")["<<individuos[i].fitness<<"] ";
            if(i%10==0) file<<endl;
      }
      file<<endl;
      file<<"--------------------------\n";
      
}

void evolucionDif::inicio()
{
      file.open("evodif.txt");
      for (size_t i = 0; i <= iteraciones; i++)
      {
            if(individuos.size() > 0)
            {
                  for (size_t i = 0; i < individuos.size(); i++)
                  {
                        mostrarPoblacion();
                        seleccionarIndividuos();
                        break;
                  }
            }else{
                  crearPoblacion();
            }
      }
      file.close();
      
      
}

num funcion(num x, num y)
{
      return x * sin(1*M_PI*x) - y*sin(4*M_PI*y + M_PI) + 1;
}


#endif
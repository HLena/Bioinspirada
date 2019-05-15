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
      vector<num> valores;
      num fitness;
   
}ind;

//funciones externas
num funcion(num,num);

bool odenar(ind, ind);


class evolucionDif
{
      // private:
      public:
            int variables;
            num F;
            num CR;
            num lim_inf;
            num lim_sup;
            int n_ind;
            int iteraciones;
            // int target;
            ofstream file;
            vector<ind> individuos;
            vector<ind> hijos;
            evolucionDif(int);
            int torneo();
            void crearPoblacion();
            void hallar_xy(ind &);
            bool mutacion(int, int, ind&);
            // void mutacion(int, int, ind&);
            void seleccionarIndividuos(int&);
            void cruzamiento(ind&,int&);
            void mostrarPoblacion(/*char*/);
            void seleccionMejorIndividuo(ind&, ind&);
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
      // n_ind = 100 + rand()%110;
      n_ind = 100;
      F = 0.5;
      CR = 0.9;
      lim_inf = -1;
      lim_sup = 2;
      // target = 0;
      variables = 2;
}

void evolucionDif::hallar_xy(ind& aux)
{
      num valor;

      for(size_t i=0; i < variables; i++)
      {     
            do{
                  valor = ((num)rand() * (lim_sup-lim_inf)) /(num)RAND_MAX + lim_inf;
            } while(valor < lim_inf && valor > lim_sup);
            aux.valores.push_back(valor);
            // file<<"poblacion creada..."<<endl;

      }
}

void evolucionDif::crearPoblacion()
{
      file<<"NUMERO DE POBLACION ES: "<<n_ind<<endl;
      while(individuos.size() < n_ind){
            ind tmp;
            hallar_xy(tmp);
            tmp.fitness = funcion(tmp.valores[0],tmp.valores[1]);
            individuos.push_back(tmp);
      }
      file<<"***INDIVIDUOS CREADOS***"<<endl;
      
}



bool evolucionDif::mutacion(int i1,int i2, ind &vd)
// void evolucionDif::mutacion(int i1,int i2, ind &vd)
{     
      
      int itm;
      do itm = torneo(); while(itm == i1 or itm == i2);
      vd.valores.clear();
      vd.valores.push_back((individuos[i1].valores[0] - individuos[i2].valores[0])* F +individuos[itm].valores[0]);
      vd.valores.push_back((individuos[i1].valores[1] - individuos[i2].valores[1])* F +individuos[itm].valores[1]);
            // file<<"**VMutado("<<vd.valores[0]<<","<<vd.valores[1]<<")"<<endl;
      if((vd.valores[0] < -1 or vd.valores[0] > 2) or (vd.valores[1] < -1 or vd.valores[1] > 2))
            return false;
      else{
            file<<"Vid("<<i1<<"):("<<individuos[i1].valores[0]<<","<<individuos[i1].valores[1]<<")"<<"["<<individuos[i1].fitness<<"]"<<endl;
            file<<"Vid("<<i2<<"):("<<individuos[i2].valores[0]<<","<<individuos[i2].valores[1]<<")"<<"["<<individuos[i2].fitness<<"]"<<endl;
            file<<"Vm("<<itm<<"):("<<individuos[itm].valores[0]<<","<<individuos[itm].valores[1]<<")"<<"["<<individuos[itm].fitness<<"]"<<endl;
            file<<"***INDIVIDUO MUTADO***\n";
            file<<"VMutado("<<vd.valores[0]<<","<<vd.valores[1]<<")"<<endl;
            return true;
      }
      
}


void evolucionDif::seleccionMejorIndividuo(ind &a , ind &targ){
      (a.fitness > targ.fitness)? hijos.push_back(a):hijos.push_back(targ);

}
void evolucionDif::cruzamiento(ind &mt, int&target)
{     
      file<<"***CRUZAMIENTO***\n";
      num nj;
      for(size_t i=0; i<mt.valores.size();i++){
            nj =  ((num) rand()*(1)/(num)RAND_MAX-0);
            file<<"nj: "<<nj<<endl;
            if (nj > CR) mt.valores[i] = individuos[target].valores[i];
      }
      
      mt.fitness = funcion(mt.valores[0], mt.valores[1]);
      file<<"VTarget("<<target<<"):("<<individuos[target].valores[0]<<","<<individuos[target].valores[1]<<") ["<<individuos[target].fitness<<"]"<<endl;
      file<<"VTrial("<<mt.valores[0]<<","<<mt.valores[1]<<") ["<<mt.fitness<<"]"<<endl;
      seleccionMejorIndividuo(mt,individuos[target]);
      // file<<":::::::::::::::::\n";
      
}

void evolucionDif::seleccionarIndividuos(int &i)
{
      int id1, id2;
      ind vm;
      id1 = torneo();
      do id2 = torneo(); while(id1 == id2);
      while(!mutacion(id1, id2, vm)){
            id1 = torneo();
            do id2 = torneo(); while(id1 == id2);
      }
      // mutacion(id1, id2, vm);
      file<<"***INDIVIDUOS SELECCIONADOS***\n";
      cruzamiento(vm,i);

}

void evolucionDif::mostrarPoblacion(/*char c*/)
{
      file<<"--------INDIVIDUOS------------\n";
      for (size_t i = 0; i < individuos.size(); i++)
            file<<"("<<i<<")("<<individuos[i].valores[0]<<" "<<individuos[i].valores[1]<<")["<<individuos[i].fitness<<"] "<<endl;
    
      file<<endl;
      
      
}

bool ordenar(ind &a, ind &b){
      return a.fitness > b.fitness;
}

void evolucionDif::inicio()
{
      file.open("2000it.txt");
      for (size_t i = 0; i <= iteraciones; i++)
      {
            file<<"===============ITERACION "<<i<<" ==================\n";
            if(individuos.size() > 0)
            {
                  // target = 0;
                  for(int i=0; i < individuos.size();i++){
                        file<<"--------------TG: ["<<i<<"]-------------"<<endl;
                        seleccionarIndividuos(i);

                  }
                  file<<"*****NUEVA POBLACION******\n";
                  mostrarPoblacion();
                  individuos = hijos;
                  hijos.clear();
            }else{
                  crearPoblacion();
                  mostrarPoblacion();
                  file<<"poblacion creada"<<endl;
            }
      }
      file<<"--------------------MAXIMOS---------------------------\n";
      sort(individuos.begin(),individuos.end(),ordenar);
      mostrarPoblacion();
      file.close();
      
      
}

num funcion(num x, num y)
{
      return x * sin(1*M_PI*x) - y*sin(4*M_PI*y + M_PI) + 1;
}


#endif
#ifndef P_EVOLUTIVA_H
#define P_EVOLUTIVA_H


#include <iostream>
#include <vector>
#include <ctime>
#include <tuple>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <math.h>
#include <iomanip>
using namespace std;

typedef num num;
typedef struct{
      num x1;
      num x2;
      num d1;
      num d2;
      num fitness;
   
}ind;

//funciones externas
num funcion(num,num);
num normal(num, num);
num valor_x(num,num, num, num, num);
bool odenar(ind, ind);
int ruleta();
ind torneo();

class PE
{

      private: 
            ofstream file;
      public:
            vector<ind> individuos;
            vector<ind> hijos;
            num n_ind;
            num desvio;
            int iteraciones;
            num alfa;
            num lim_sup;
            num lim_inf;
            num delta;
            PE(int);
            void crearPoblacion();
            void evaluarPoblacion(int);
            ind seleccionarPadres();
            void procrearOffspring();
            ind mutacion(ind&);
            void inicio();

            ~PE();
};

PE::PE( int it)
{
      n_ind = 10;
      desvio = 0.3;
      lim_inf = -10;
      lim_sup = 10;
      iteraciones = it;
      delta = 0.1;
      // alfa = (num)rand()%1;

}

void PE :: crearPoblacion()
{
      ind ind1;
      num aleatorio;
      int i;
      while(i < individuos.size())
      {
            aleatorio=(num)(rand()%100)/100.0;
            ind1.x1 = valor_x(lim_inf,lim_sup,desvio,delta,aleatorio);
            ind1.x2 = valor_x(lim_inf,lim_sup,desvio,delta,aleatorio);
            ind1.d1 = 0.3;
            ind1.d2 = 0.3;
            if((ind1.x1 >= lim_inf && ind1.x1 <= lim_sup) && (ind1.x1 >= lim_inf && ind1.x1 <= lim_sup))
            {
                  ind1.fitness = funcion(ind1.x1, ind1.x2);
                  individuos.push_back(ind1);

            }

      }
      
      
}

void PE::evaluarPoblacion(int a){
     if (a == 1)
            sort(individuos.begin(), individuos.end(), ordenar);
      else
            sort(hijos.begin(), hijos.end(), ordenar);
}

ind PE::seleccionarPadres(){
      return torneo();      
}

ind PE::mutacion(ind &h1){
      // h1.d1 = h1.d1*(1 + alfa*valor_x(lim_inf,lim_sup,1,delta,aleatorio));
}

void PE::procrearOffspring(){
      ind p1, h1;
      while(hijos.size() <= individuos.size()){
            p1 = seleccionarPadres();
            h1 = p1;
            mutacion(h1);
            
            

      }
}

void PE::inicio()
{
      for(int i = 0; i <= iteraciones; i++)
      {
            if(individuos.size() > 0)
            {
                  evaluarPoblacion(1);
                  
            }else
            {
                  crearPoblacion();
            }
            
      }
}


//===========================================================0
num valor_x(num lim_inf,num lim_sup, num desvio, num delta, num aleatorio)
{
	num area = 0;
	num aux_suma, aux= normal(lim_inf, desvio);
	for(num i = lim_inf + delta; i < lim_sup; i+=delta){
		aux_suma = normal(i, desvio);
		area += (aux + aux_suma);
		if(area * (delta / 2) > aleatorio){
			return i;
		}
		aux = aux_suma;
	}
	return -1 ;
}

num normal(num x, num desvio)
{
	num retorno = -0.5 * pow(x/desvio,2);
	retorno = exp(retorno);
	retorno = retorno / (desvio * sqrt(2 * M_PI));
	return retorno;
}

num  funcion(num  x1, num  x2)
{
	return -cos(x1)*cos(x2)*exp(-pow(x1-pi,2)-pow(x2-pi,2));
}

bool ordenar(ind a, ind b){
      return a.fitness > b.fitness;
}






PE::~PE(){}

#endif

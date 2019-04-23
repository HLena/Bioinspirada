#include <iostream>
#include <vector>
#include <math.h> 
#include <cmath>
#include <stdlib.h>  
#include <algorithm> 
#include <bits/stdc++.h>
#include <bitset>
#include <map>
#include <fstream>
#include <iomanip>
//#include "gnuplot-iostream.h"

using namespace std;


typedef struct {
      int id;
      double x=0,y=0;
      double f1,f2;
      int front=0;
      double peri = 0;

}indv; 

typedef map<int, indv> mp;
typedef pair<indv, double> p;
#define  inf 1000;

//-------------variables globales-------------------
mp  population;
vector< int > paretos;


//--------------funciones externas-------------------
// bool order(indv i1, indv i2) 
// { 
//     return (i1.f1 < i2.f1); 
// } 

bool orderX(indv i1, indv  i2) 
{ 
    return (i1.f1 < i2.f1); 
} 

bool orderP(indv i1, indv i2) 
{ 
    return (i1.peri> i2.peri); 
} 
bool orderId(indv i1, indv i2) 
{ 
    return (i1.id < i2.id); 
} 


double f1(double x_, double y_){
      return 4*pow(x_,2) + 4*pow(y_,2);
}

double f2(double x_, double y_){
      return pow(x_-5,2) + pow(y_-5,2);
}

//--------------------------------------------------
class multiobj{
      public:
            ofstream file,file1;
            int np;
            int ng;
            double pc;
            double pm;
            multiobj(int, int, double,double);
            void run();
            int torneo();
            void createPopulation();
            void dominanceFronts();
            void selectionP();
            void uniformMutation(indv&);
            void showP();
            void showPP(mp);
            void showA(vector<indv>);
            void evaluatePopulation();
            void crossOver();
            void stackingDistances();
            indv BLX(indv, indv,double);
            double findBeta(int);

};


//   estados - n_generaciones - n_population - p_mutacion - p_cruce
multiobj::multiobj(int p, int g,double c, double m){
     
      np = p;
      ng = g;
      pc = c;
      pm = m;   
}
void multiobj::run(){
      file.open("salida2.txt");
      for (size_t i = 0; i <= ng ; i++)
      {      
            if(population.size()>0)
            {     file<<"==================== generacion "<<i<<" =================="  <<endl; 
                  dominanceFronts();
                  showP();
                  stackingDistances();
                  crossOver();
            }else
            {
                  createPopulation();
                  file<<"----------hallando poblacion Q -----------"<<endl;
                  crossOver();
                  file<<"----------poblacion P + Q -----------"<<endl;
                  showP();
                  
            }

      }
      file.close();

}


void multiobj::createPopulation(){
      indv aux;
      file1.open("puntos.dat");
      file<<"------Creando population P------"<<endl;
      for(int it=0; it<np; it++)
      {
            aux.x = ((double) rand()*(5-0)/(double)RAND_MAX-0);
            aux.y = ((double) rand()*(3-0)/(double)RAND_MAX-0);
            // newnum = round(num * 10) / 10;
            aux.id = it;
            aux.f1  = f1(aux.x,aux.y);
            aux.f2  = f2(aux.x,aux.y);
            aux.front = 0;
            aux.peri = 0;
            population[it]=aux;
            file1<<aux.f1<<" "<<aux.f2<<endl;
      }
      
      showP();
    
}

void multiobj::stackingDistances(){
      int f=0;
      int cups = np;
      mp aux;
      p pr; //pair <indv, double>
      vector<indv> px; // vector de pairs
      for(auto i = 0; i < paretos.size(); i++){
            if(paretos[i] <= cups) 
                  cups -= paretos[i];
            else{
                  f = i+1;
                  break;
            }
      }
      if(f != 0){
            file<<"pareto de apilacion: "<<f<<endl;
            for(mp::iterator it = population.begin(); it != population.end(); it++)
            {
                  if(it->second.front == f)
                        px.push_back(it->second);
            }
            sort(px.begin(),px.end(),orderX);
            px[0].peri = inf;
            px[px.size()-1].peri = inf;
            // file<<"pareto: "<<f<<" ordenado por x(f1)"<<endl;
            // showA(px);
            int j = 0;
            double pmt=0;
            for(auto i=0; i< px.size()-2; i++)
            {
                  j=i+2;
                  pmt =2*(px[j].f1-px[i].f1)+2*(abs(px[j].f2-px[i].f2));
                  px[i+1].peri = pmt;
                  
            }
            sort(px.begin(),px.end(),orderP);
            // file<<"perimetros de pareto: "<<f<<endl;
            // showA(px);
            px.erase(px.begin()+cups,px.end());
            sort(px.begin(),px.end(),orderId);
            // file<<"elegidos del pareton en px"<<f<<endl;
            // showA(px);
      
            for (auto i = population.begin(); i != population.end(); i++)
            {
                  if(i->second.front >= f)
                        population.erase(i);
                       
            }
            for (size_t i = 0; i < px.size(); i++)
                  population[px[i].id] = px[i];
            
      }
      
      file<<"----------------- Elegidos--------------------"<<endl;
      showP();
}
void multiobj::dominanceFronts(){
      file <<"--------hallando fronteras de pareto---------"<<endl;
      mp aux;
      int f = 0;
      file<<" f es: "<<f<<endl;
      indv x1, y1, z1;
      mp::iterator it;
      bool pase;
      while(population.size())
      {
            it=population.begin();
            f++;
            pase = true;
            paretos.push_back(0);
            for (mp::iterator it1=population.begin(); it1!=population.end(); it1++)
            {     
                  if(it != it1)
                  {
                        // file<<"tmp :"<<it->first<<":  ("<<it->second.f1<<","<<it->second.f2<<")"<<endl;
                        if(it1->second.f1 <= it->second.f1 && it1->second.f2 <= it->second.f2)
                        {     
                              // file<<it1->first<<" ("<<it1->second.f1<<","<<it1->second.f2<<" domina a "<<it->first<<": "" ("<<it->second.f1<<","<<it->second.f2<<")"<<endl;
                              it = it1;
                        }else if(it1->second.f1 <= it->second.f1 or it1->second.f2 <= it->second.f2)
                        {
                              // file<<it1->first<<" ("<<it1->second.f1<<","<<it1->second.f2<<") no domina"<<endl;
                              for(mp::iterator i=aux.begin();i!=aux.end();i++)
                              {
                                    if((it1->second.f1 <= i->second.f1 && it1->second.f2 <= i->second.f2)&& i->second.front == f)
                                    {
                                          // file<<i->first<<" ("<<i->second.f1<<","<<i->second.f2<<") deleted"<<endl;
                                          i->second.front=0;
                                          population[i->first]=i->second;
                                          aux.erase(i);
                                          paretos[f-1]--;
                                    }else if((it1->second.f1 >= i->second.f1 && it1->second.f2 >= i->second.f2)&& i->second.front == f){
                                          pase = false;
                                          break;
                                    }
                              }
                              if(pase){
                                    it1->second.front = f;
                                    aux[it1->first] = it1->second;
                                    // file<<it1->first<<" ("<<it1->second.f1<<","<<it1->second.f2<<") capa:"<<f <<endl;
                                    paretos[f-1]++;
                                    population.erase(it1);
                              }
                        }
                  }     
            }
            it->second.front = f;
            aux[it->first] = it->second;
            population.erase(it);
            paretos[f-1]++;
            
      }
      population = aux;
      
}

void multiobj::showP(){
      for(auto i=population.begin();i != population.end();i++){
            file<<"id:"<<i->second.id<<" ("<<i->second.x<<","<<i->second.y<<") f1:"<<i->second.f1<<" f2:"<<i->second.f2<<" c:"<<i->second.front<<" pr:"<<i->second.peri<<endl;
      }
}

void multiobj::showPP(mp p){
      file<<" mapa de elegidos"<<endl;
      for(auto i=p.begin();i != p.end();i++){
            file<<"key: "<<i->first<<" id:"<<i->second.id<<"  ("<<i->second.x<<" , "<<i->second.y<<") "<<i->second.f1<<" "<<i->second.f2<<" capa: "<<i->second.front<<endl;
      }
}

void multiobj::showA(vector<indv> v){
      for(auto i=0;i < v.size();i++){
            file<<"id:"<<v[i].id<<" ("<<v[i].f1<<" , "<<v[i].f2<<") "<<v[i].peri<<" capa: "<<v[i].front<<endl;
      }
}


void multiobj::uniformMutation(indv &x){
      double pro = rand()%100+1;
      if(pro/100 < pm)
      {     
            file<<"mutacion: Si ";
            int pos = rand()%2;
            (pos==0)? x.x = rand()%5+1:x.y =rand()%3+1;
            file <<"("<<x.x<<","<<x.y<<") "<<endl;

      }else
      {
            file<<"mutacion: No"<<endl;
      }
}

double multiobj::findBeta(int a){
      int aa = (2*a)+1;
      double beta = (rand()% aa)-a; //FALTA 
      if(beta < -5) beta = beta + 10;
      return beta /10;
}
int multiobj::torneo(){
      int p1,p2;
      p1 = rand()%np+1;
      do{ p2=rand()%np+1; }while(p1 == p2);
      if(population[p1].f1 <= population[p2].f1 && population[p1].f2 <= population[p2].f2 ) 
            return p1;
      // else if(population[p1].f1 == population[p2].f1 && population[p1].f2 <population[p2].f2 ) return p1;
      // else if(population[p1].f1 < population[p2].f1 && population[p1].f2== population[p2].f2 ) return p1;
      else if(population[p2].f1 <= population[p1].f1 && population[p2].f2<= population[p1].f2 ) 
            return p2;
      // else if(population[p2].f1 == population[p1].f1 && population[p2].f2 <population[p1].f2 ) return p2;
      // else if(population[p2].f1 < population[p1].f1 && population[p2].f2== population[p1].f2 ) return p2;
      else{
            file<<"no hay dominacion"<<endl;
            if(population[p1].peri > population[p2].peri)
                  return p1;
            if(population[p1].peri == population[p2].peri)
            {
                  int t = rand()%1+1;
                  return (t == 0) ? p1: p2;
            }
            else
                  return p2;
      }
      

}
void multiobj::crossOver(){
      indv h;
      int r = population.rbegin()->first;
      for(auto i=0; i < np; i++){
           
            int p1, p2;
            indv h;
            p1= torneo();
            do p2=torneo(); while(p1 == p2);
            file<<"padre "<<p1<<" madre "<<p2<<endl;
            h = BLX(population[p1],population[p2],0.5);
            file<<"hijo: "<<h.x<<","<<h.y<<endl;
            uniformMutation(h);
            h.f1 = f1(h.x,h.y);  h.f2 = f2(h.x,h.y);
            h.front =0;  h.peri=0;
            file1 << h.f1<<" "<<h.f2<<endl;
            r++;
            h.id = r;
            population[r]=h;
            
      }
}

indv multiobj::BLX(indv p1, indv p2, double alfa){
      indv h;
      int a = (alfa+1)*10;
      double beta;
      do
      {
            beta=findBeta(a);
            h.x = p1.x + beta*(p2.x-p1.x);
      }while(h.x < 0 or h.x >5 );
            
      do
      {
            h.y = p1.y + beta*(p2.y-p1.y);
            beta=findBeta(a);
      } while (h.y < 0 or h.y >3);
      return h;
      
}





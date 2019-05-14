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

typedef vector<indv> v;

#define  inf 1000;

//-------------variables globales-------------------
v pp;//vector poblacion
vector< int > paretos;


//--------------funciones externas-------------------
bool order(indv i1, indv  i2) 
{
//     if(i1.f1 == i2.f1) return (i1.f2 < i2.f2); 
    return (i1.f1 < i2.f1);
} 

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
            void selectP();
            void uniformMutation(indv&);
            void showP();
            void showPP(v);
            void showA(v);
            void evaluatePopulation();
            void crossOver();
            void stackingDistances();
            indv BLX(indv, indv,double);
            double findBeta(int);
            bool verifRepite(indv);

};


//   estados - n_generaciones - n_pp - p_mutacion - p_cruce
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
            if(pp.size()>0)
            {     file<<"========== generacion "<<i<<" ==========="  <<endl; 
                  file<<"----------PARETOS-----------"<<endl;
                  dominanceFronts();
                  file<<"----------APILAMIENTO-----------"<<endl;
                  stackingDistances();
                  file<<"----------CRUZE-----------"<<endl;
                  selectP();
            }else
            {
                  createPopulation();
                  file<<"----------hallando poblacion Q -----------"<<endl;
                  selectP();
                  file<<"----------poblacion P + Q -----------"<<endl;
                  showP();
                  
            }

      }
      file.close();

}


void multiobj::createPopulation(){
      indv aux;
      file1.open("puntos.dat");
      file<<"------Creando pp P------"<<endl;
      for(int it=0; it<np; it++)
      {
            aux.x = ((double) rand()*(5-0)/(double)RAND_MAX-0);
            aux.y = ((double) rand()*(3-0)/(double)RAND_MAX-0);
            aux.id = it;
            aux.f1  = f1(aux.x,aux.y);
            aux.f2  = f2(aux.x,aux.y);
            aux.front = 0;
            aux.peri = 0;
            pp.push_back(aux);
            file1<<aux.f1<<" "<<aux.f2<<endl;
      }
      showP();
}

void multiobj::stackingDistances(){
      int f=0;
      int cups = np;
      v px; // vector de pairs
      for(auto i = 0; i < paretos.size(); i++){
            if(paretos[i] <= cups) 
                  cups -= paretos[i];
            else{
                  f = i+1;
                  break;
            }
      }
      if(f != 0){
            file<<"------pareto N°: "<<f<<"------"<<endl;
            for(auto it = 0; it < pp.size(); it++)
            {
                  if(pp[it].front == f)
                        px.push_back(pp[it]);
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
            file<<"perimetros de pareto: "<<f<<endl;
            showA(px);
            px.erase(px.begin()+cups,px.end());
            //sort(px.begin(),px.end(),orderId);
            // file<<"elegidos del pareton en px"<<f<<endl;
            // showA(px);
            for(auto i =0; i < pp.size(); i++)
            {
                  if(pp[i].front < f)
                        px.push_back(pp[i]);                      
            }
            
      }
      pp.clear();
      pp = px;
      file<<"---------- Elegidos de apilacion ::orden id-------------"<<endl;
      sort(pp.begin(),pp.end(),orderId);
      showP();
      file<<"-----------------------------------------------"<<endl;
      
}
void multiobj::dominanceFronts(){
     // file <<"--------hallando fronteras de pareto---------"<<endl;
      showP();
      v aux;
      int f = 0;
      int pos;
      file<<" f es: "<<f<<endl;
      indv x1, y1, z1;
      indv it;
      bool pase;
      while(pp.size())
      {     file<<"*"<<endl;
            it = pp[0];
            f++;
            // pase = true;
            paretos.push_back(0);
            file<<"tmp :"<<it.id<<": ("<<it.f1<<","<<it.f2<<")"<<endl;
            pos = 0;
            for (auto it1=1; it1 < pp.size(); it1++)
            {     
                  file<<"tmp :"<<it.id<<": ("<<it.f1<<","<<it.f2<<")"<<endl;
                  // file<<" ** "<< it1 <<" < "<<pp.size()<<endl;
                  if(it.id != pp[it1].id){
                        if(pp[it1].f1 <= it.f1 && pp[it1].f2 <= it.f2)
                        {    
                              file<<pp[it1].id<<" domina a tmp: "<<it.id<<endl;
                              it = pp[it1];
                              pos = it1;
                        } 
                        else if(pp[it1].f1 <= it.f1 or pp[it1].f2 <= it.f2)
                        {
                              pase = true;
                              file<<pp[it1].id<<" ("<<pp[it1].f1<<","<<pp[it1].f2<<") no domina a tmp"<<endl;
                              for(auto i = 0;i < aux.size();i++)
                              {
                                    // file<<"***"<<endl;
                                    if((pp[it1].f1 <= aux[i].f1 && pp[it1].f2 <= aux[i].f2)&& aux[i].front == f)
                                    {
                                          
                                          // // if(pp[it1].f1 == aux[i].f1 && pp[it1].f2 == aux[i].f2){
// 
                                          // }else{
                                                file<<aux[i].id<<" ("<<aux[i].f1<<","<<aux[i].f2<<") deleted"<<endl;
                                                aux[i].front = 0;
                                                pp.push_back(aux[i]);
                                                //sort(pp.begin(),pp.end(),order);
                                                aux.erase(aux.begin()+i);
                                                i--;
                                                paretos[f-1]--;
                                          // }
                              
                                    }else if((pp[it1].f1 >= aux[i].f1 && pp[it1].f2 >= aux[i].f2) && aux[i].front == f)
                                    {     
                                          file<<"es dominado por aux: "<<aux[i].id<<endl;
                                          pase = false;
                                          break;
                              
                                    }else file<<"no domina a "<<aux[i].id<<endl;
                              
                              }
                              if(pase && it.id != pp[it1].id){
                                    pp[it1].front = f;
                                    aux.push_back(pp[it1]);
                                    file<<pp[it1].id<<" ("<<pp[it1].f1<<","<<pp[it1].f2<<") capa:"<<f<<endl;
                                    paretos[f-1]++;
                                    pp.erase(pp.begin()+it1);
                                    it1--;
                              }
                              showA(aux);
                        }
                        else file<<pp[it1].id<<" ("<<pp[it1].f1<<","<<pp[it1].f2<<" es dominado"<<endl;
                  }
            }
            it.front = f;
            aux.push_back(it);
            pp.erase(pp.begin()+pos);
            paretos[f-1]++;
            file<<" tmp es includo"<<endl;
            showA(aux);
            
      }
      pp = aux;
      showP();
      
}

void multiobj::showP(){
      for(auto i=0;i < pp.size();i++){
            file<<"id:"<<pp[i].id<<" ("<<pp[i].x<<","<<pp[i].y<<") f("<<pp[i].f1<<","<<pp[i].f2<<") c:"<<pp[i].front<<" pr:"<<pp[i].peri<<endl;
      }
}



void multiobj::showA(vector<indv> v1){
      file<<"\n-----------------fronteras----------"<<endl;
      for(auto i=0;i < v1.size();i++){
            file<<"\nid:"<<v1[i].id<<" ("<<v1[i].f1<<" , "<<v1[i].f2<<") "<<v1[i].peri<<" capa: "<<v1[i].front<<endl;
      }
      file<<"\n-----------------------------------"<<endl;
}


void multiobj::uniformMutation(indv &x){
      double pro = rand()%100+1;
      if(pro/100 < pm)
      {     
            file<<"\tmutacion: Si ";
            int pos = rand()%2;
            (pos==0)? x.x = ((double) rand()*(5-0)/(double)RAND_MAX-0):
                      x.y =((double) rand()*(3-0)/(double)RAND_MAX-0);
            file<<"("<<x.x<<","<<x.y<<") "<<endl;

      }else
      {
            file<<"\tmutacion: No"<<endl;
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
      if(pp[p1].f1 <= pp[p2].f1 && pp[p1].f2 <= pp[p2].f2 ) 
      {
            return p1;    file<<p1<<" domina a "<<p2<<endl;
      }
      else if(pp[p2].f1 <= pp[p1].f1 && pp[p2].f2<= pp[p1].f2 ) 
      {
            return p2;    file<<p2<<" domina a "<<p1<<endl;
      }
      else{
            file<<"no domina"<<endl;
            if(pp[p1].peri > pp[p2].peri)
                  return p1;
            if(pp[p1].peri == pp[p2].peri)
            {
                  int t = rand()%1+1;
                  return (t == 0) ? p1: p2;
            }
            else
                  return p2;
      }
      

}

void multiobj::selectP(){
      for(auto i=0; i < np; i++){
            file<<"\ncruce: "<<i<<endl;
            crossOver();
      } 
      file1.close();
}
void multiobj::crossOver(){
      indv h;
      int lastId = pp[pp.size()-1].id;
      // for(auto i=0; i < np; i++){
      int p1, p2;
      p1= torneo();   do p2=torneo(); while(p1 == p2);
      file<<"padre "<<pp[p1].id<<" madre "<<pp[p2].id<<endl;
      h = BLX(pp[p1],pp[p2],0.5);
      file<<"\thijo: "<<h.x<<","<<h.y<<endl;
      uniformMutation(h);
      if(!verifRepite(h)){
            file<<"---no repite---"<<endl;
            h.f1 = f1(h.x,h.y);  h.f2 = f2(h.x,h.y);
            file1 << h.f1<<" "<<h.f2<<endl;
            h.id = ++lastId;
            pp.push_back(h);    
      }else{
            file<<"---repite---"<<endl;
            crossOver();
      }
      // }
      // file1.close();
      //sort(pp.begin(),pp.end(),order);
}

bool multiobj::verifRepite(indv o){
      for(auto i=0; i<pp.size();i++){
            if(o.x==pp[i].x && o.y==pp[i].y)
                  return true;
      }
      return false;
}

indv multiobj::BLX(indv p1, indv p2, double alfa){
      indv h;
      int a = (alfa+1)*10;
      double beta;
      do
      {
            beta=findBeta(a);
            if(beta==1) beta++;
            file <<"hx: "<<beta<<" ";
            h.x = p1.x + beta*(p2.x-p1.x);
      }while(h.x < 0 or h.x >5 );
            
      do
      {
            file<<"hy: "<<beta<<endl;
            h.y = p1.y + beta*(p2.y-p1.y);
            beta=findBeta(a);
            if(beta==1) beta++;
      } while (h.y < 0 or h.y >3);
      return h;
      
}





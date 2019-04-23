#include <iostream>
#include <vector>
#include <math.h> 
#include <stdlib.h>  
#include <algorithm> 
#include <bits/stdc++.h>
#include <bitset>
#include <map>
#include <fstream>

using namespace std;


map<char,int> nodos;


int graph[]={0,12,3,23,1,5,23,56,12,11,
            12,0,9,18,3,41,45,5,41,27,
            3,9,0,89,56,21,12,48,14,29,
            23,18,89,0,87,46,75,17,50,42,
            1,3,56,87,0,55,22,86,14,33,
            5,41,21,46,55,0,21,76,54,81,
            23,45,12,75,22,21,0,11,57,48,
            56,5,48,17,86,76,11,0,63,24,
            12,41,14,50,14,54,57,63,0,9,
            11,27,29,42,33,81,48,24,9,0};

typedef struct {
    string cadena;
    double fitness;
    double inv = 0;
    double pct =0;
}indv; 
//-------------variables globales-------------------
vector<indv> population;



//--------------funciones externas-------------------
bool order(indv i1, indv i2) 
{ 
    return (i1.fitness < i2.fitness); 
} 

//--------------------------------------------------
class tsp{
      public:
      ofstream file;
      double total;
      double pc;
      string states;
      int np;
      int ng;
      tsp(string, int,int,double);
      void readStates(string);
      void createPopulation();
      double fitness(string);
      void evaluatePopulation();
      void selectParents();
      void run();
      void showP();
      int mruleta();
      void crossOverOBX(indv, indv);
      void mutation(indv&);

};

tsp::tsp(string s,int n,int g,double c)
{    
      states = s;
      np = n;
      ng = g;
      pc = c;
}

void tsp::createPopulation(){
      readStates(states);
      indv tmp;
      file<<"         population inicial        \n";
      for (size_t i = 0; i < 3*np; i++)
      {
            next_permutation( states.begin( ), states.end( ) ) ;
            tmp.cadena = states;
            tmp.fitness = fitness(states);
            tmp.inv = 1 / tmp.fitness;
            population.push_back(tmp);
      }
      showP();
      file<<"----los 10 mejores----"<<endl;

}

void tsp::run(){
      file.open("salida1.txt");
      do{
            if(population.size()>0)
            {
                  evaluatePopulation();
                  selectParents();
            }else
            {
                  createPopulation();
            }
            file <<"generacion:" <<ng<<endl;
      }while(ng--);
      file.close();
}
int tsp::mruleta(){
      double limit = 0;
      double num = rand()%100;  
      // cout<<"numero aleatorio: "<<num<<endl;
      for(int i=0;i < population.size();i++){
            //cout<<limit<<" "<<population[i].porcent+limit<<endl;
            if(num <= population[i].pct+limit){
                  return i;
            }
            else    
                  limit+=population[i].pct;
      }
}

void tsp::evaluatePopulation()
{
      file<<"------evaluando poblacion--------"<<endl;
      sort(population.begin(),population.end(),order);
      population.erase(population.begin()+10,population.end());
      for(int i=0;i < population.size();i++)
            total += population[i].inv;

      for(int i=0;i < population.size();i++)
            population[i].pct = (population[i].inv/total)*100;

      showP();
}


void tsp::crossOverOBX(indv p1, indv p2)
{
      file <<"---------Cuzamiento OBX---------------\n";
      indv h1,h2;
      map<char,int> cd1,cd2;
      double pro;
      vector<int> pos1,pos2;
      file <<"padre: "<<p1.cadena<<"\n"<<"madre  "<<p2.cadena<<"\n";
      file<<"---posiciones seleccionadas---\n";
      for (size_t i = 0; i < states.size(); i++)
      {
            pro = (rand()%10+1);
            if(pro/10<= 0.5){
                  cd1[p1.cadena[i]]=0;
                  file <<"pos"<<i<<" p1:"<<p1.cadena[i]<<" ";
                  cd2[p2.cadena[i]]=0;
                  file <<"p2:"<<p2.cadena[i]<<"\n";
                  pos1.push_back(i);
            } 
      }
      file<<"\n";

      h1=p1; h2=p2;
      pos2 = pos1;
      for (size_t i = 0; i < states.size(); i++)
      {
            int i1, i2;
            auto x1 = cd1.find(p2.cadena[i]);
            auto x2 = cd2.find(p1.cadena[i]);
            if(x1 != cd1.end()){
                  h1.cadena[pos1.front()]=p2.cadena[i]; 
                  pos1.erase(pos1.begin());
            }
            if(x2 != cd2.end()){
                  h2.cadena[pos2.front()]=p1.cadena[i];
                  pos2.erase(pos2.begin());
            }
            
      }
      file <<"hijo1  "<<h1.cadena<<"\n"<<"hijo2  "<<h2.cadena<<"\n";
      h1.fitness = fitness(h1.cadena);
      file<<"mutacion hijo 1: ";
      mutation(h1);
      h2.fitness = fitness(h2.cadena);
      file<<"mutacion hijo 2: ";
      mutation(h2);
      population.push_back(h1);
      population.push_back(h2);
}

void tsp::selectParents(){
      file<<"--------seleccion de padres--------"<<endl;
      int p1,p2;
      p1 = rand()%10+1;
      p2 = rand()%10+1;
      while(p1 == p2) p2 = rand()%10+1;
      file <<"padre "<<p1<<"   "<<"madre "<<p2<<endl;
      crossOverOBX(population[p1],population[p2]);
}

void tsp::showP()
{
      for(int i=0;i < population.size();i++){
            file<<population[i].cadena<<" -> "<<population[i].fitness<<" "<<population[i].pct<<endl;
      }
}
void tsp::readStates(string s)
{
      for(int it = 0; it < s.length(); it++){
            nodos.insert(pair<char,int>(s[it],it));
      }
}

double tsp::fitness(string s)
{
      char s0,s1;
      double sum = 0;
      int s0v, s1v;
      int l = s.length();
      s0 = s[0];
      auto x0  = nodos.find(s0);
      auto x1 = x0;
      s0v = x0->second;
      for(int it = 1; it < l; it++){
            s1 = s[it];
            x1 = nodos.find(s1);
            s1v = x1->second; 
            sum += graph[s0v*l+s1v];
            s0 = s1; x0 = x1; s0v = s1v;
      }
      return sum;
            
}

void tsp::mutation(indv &x){
      int i1, i2;
      string tmp;
      int v = 10;
      double f;
      while(v){
            tmp = x.cadena;
            i1 = rand()%x.cadena.length();
            do i2 = rand()%x.cadena.length();while(i1==i2);
            swap(tmp[i1], tmp[i2]);
            f = fitness(tmp);
            if(f < x.fitness){
                  x.fitness = f;
                  x.cadena = tmp;
            }
            v--;
      }
      file<<x.cadena<<" "<<x.fitness<<endl;

}


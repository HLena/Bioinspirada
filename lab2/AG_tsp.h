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


int graph[]={0,2,2,1,4,
            2,0,3,2,3,
            2,3,0,2,2,
            1,2,2,0,4,
            4,3,2,4,0};

typedef struct {
    string cadena;
    double fitness;
    double comp = 0;
    double pct =0;
}indv; 
//-------------variables globales-------------------
vector<indv> poblacion;
vector<double> ruleta;


//--------------funciones externas-------------------
bool ordenar(indv i1, indv i2) 
{ 
    return (i1.fitness < i2.fitness); 
} 

//--------------------------------------------------
class tsp{
      public:
      ofstream file;
      double total;
      string states;
      // int nst;
      int np;
      int ng;
      double pm;
      double pc;
      tsp(string, int, int,double,double);
      void read_states(string);
      void crear_poblacion();
      double fitness(string);
      void seleccionar_poblacion();
      void cruce_pmx();
      void mutacion(indv&);
      void mostrar_poblacion();
      int mruleta();
      void evaluar();
      void pmx(string,string,indv&,map<char,char> , int , int );

};


//   estados - n_generaciones - n_poblacion - p_mutacion - p_cruce
tsp::tsp(string s, int g, int p,double m,double c ){
      states = s;
      ng = g; 
      np = p;
      pm = m;
      pc = c;

      read_states(states);
      int x=0;
      file.open("salida1.txt");
       while(x <= ng){
            if(poblacion.size() > 0){
                  seleccionar_poblacion();
                  mostrar_poblacion();
            }else
                  crear_poblacion();
            x++;
      }
      file.close();
      
}

void tsp::read_states(string s){
      for(int it = 0; it < s.length(); it++){
            nodos.insert(pair<char,int>(s[it],it));
      }
}
double tsp::fitness(string s){
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
void tsp::crear_poblacion(){
      indv aux;
      total = 0;
      for(int it=0; it<np; it++){
            next_permutation( states.begin( ), states.end( ) ) ;
            aux.cadena = states;
            aux.fitness = fitness(states);
            aux.comp = 1/aux.fitness;
            total +=aux.comp;
            poblacion.push_back(aux);
      }
      file<<"-----------POBLACION INICIAL------------"<<endl;
      mostrar_poblacion();
    
}

int tsp::mruleta(){
      double limit = 0;
      double num = rand()%100;  
      // cout<<"numero aleatorio: "<<num<<endl;
      for(int i=0;i < poblacion.size();i++){
            //cout<<limit<<" "<<poblacion[i].porcent+limit<<endl;
            if(num <= poblacion[i].pct+limit){
                  return i;
            }
            else    
                  limit+=poblacion[i].pct;
      }
}
void tsp::evaluar(){
      total = 0;
      file<<"---------------------------"<<endl;
      sort(poblacion.begin(),poblacion.end(),ordenar);
      while(poblacion.size() > np)
            poblacion.pop_back();
     
      for(int i=0;i < poblacion.size();i++)
            total += poblacion[i].comp;

      for(int i=0;i < poblacion.size();i++)
            poblacion[i].pct = (poblacion[i].comp/total)*100;
      mostrar_poblacion();
      
}


void tsp::mostrar_poblacion(){
      for(int i=0;i < poblacion.size();i++){
            // cout<<poblacion[i].cadena<<" -> "<<poblacion[i].fitness<<" "<<poblacion[i].comp<<" "<<poblacion[i].pct<<" : "<<ruleta[i]<<endl;
            file<<poblacion[i].cadena<<" -> "<<poblacion[i].fitness<<" "<<poblacion[i].comp<<" "<<poblacion[i].pct<<endl;
      }
}
void tsp::mutacion(indv &x){
      double r = rand()%10;
      if(r/10 < pm){
            int i1, i2;
            i1 = rand()%x.cadena.length();
            i2 = rand()%x.cadena.length();
            while(i1==i2)     i2 = rand()%x.cadena.length();
            swap(x.cadena[i1], x.cadena[i2]);
            file<<"mutacion: "<<x.cadena<<endl;

      }else
            file<< " NO HAY MUTACION "<<endl;

}

//void tsp::pmx(string p1,string p2, indv &h1, indv &h2){
void tsp::pmx(string p1,string s, indv &h1,map<char,char> ss, int p, int l){
            string s1, s2;
            map<char,char>::iterator it;
            char aux;
            h1.cadena = p1;  
            //cout<<"s: " <<s <<endl;
            h1.cadena.replace(p,l,s);  
            //cout<<"padre: "<<p1<<"  hijo:"<<h1.cadena<<endl;
            
            for(int i=0; i< p1.length(); i++){
                  if(i<p or i > p+l-1){
                        aux = h1.cadena[i];
                        //cout<<"leo: "<<h1.cadena[i]<<endl;
                        it = ss.find(aux);
                        if(it == ss.end()){
                                    //cout<<"no encontrado = "<<h1.cadena[i]<<endl;
                                    ss.insert(pair<char,int>(aux,0));
                                    h1.cadena[i]= aux;
                              
                                    
                        }else
                        {       
                             // cout<<"encontrado = "<<aux<<endl;
                              do{
                                    aux =it->second;
                                    //cout<<"reemplazo = "<<aux<<endl;
                                    it = ss.find(aux); 
                              }while(it != ss.end());
                                                
                              ss.insert(pair<char,int>(aux,0));
                              h1.cadena[i]= aux;
                        }  
                  }     
       }
      // cout<<p1<<"---------"<<h1.cadena<<endl;
      
            

}
void tsp::cruce_pmx(){
      double r = rand()%1;
      map<char,char> ss;
      string s1, s2;
      indv h1, h2;
      if( r < pc){
            int p1,p2;
            p1 = mruleta();
            p2 = mruleta();
            while(p1 == p2) p2 = mruleta();
            int p ,l ;
            // l = 2 + rand() % (3 +1 - 2) ;
            // p = 1 + rand() % (2 +1 - 1) ;
            // cout<<"p:"<<p<<" l:"<<l<<endl;
            // cout<<" :"<<p+l<<" :"<<endl;
            // cout<<":"<<states.length()-1<<" :"<<endl;
            // while( p >=1 && ((p + l) < states.length()-1) )
                  // p = 1 + rand() % (2+1 - 1) ;

            l = 2; p= 2;
            file<<"padre "<<p1<<" "<<poblacion[p1].cadena<<"\tpadre "<<p2<<" "<<poblacion[p2].cadena<<endl;
            //cout<<"p:"<<p<<" l:"<<l<<endl;
            // p1 = "ABCDE"; p2 = "BCAED";

            s1 = poblacion[p1].cadena.substr(p,l);     s2 = poblacion[p2].cadena.substr(p,l);
            for(int i=0; i<s1.length();i++)    ss.insert(pair<char,char>(s2[i],s1[i]));
            pmx(poblacion[p1].cadena, s2 ,h1,ss,p,l);
            file<<"hijo 1\t"<<h1.cadena<<endl;
            mutacion(h1);
            h1.fitness = fitness(h1.cadena);
            h1.comp = 1/h1.fitness;
            poblacion.push_back(h1);
            ss.clear();
            //return;
            for(int i=0; i<s1.length();i++)    ss.insert(pair<char,char>(s1[i],s2[i]));
            pmx(poblacion[p2].cadena, s1,h2,ss,p,l);
            file<<"hijo 2\t"<<h2.cadena<<endl;
            mutacion(h2);
            h2.fitness = fitness(h2.cadena);
            h2.comp = 1/h2.fitness;
            poblacion.push_back(h2);
            //cout<<"salio"<<endl;

            
      }else
            file << " NO HAY CRUCE "<<endl;
      sort(poblacion.begin(),poblacion.end(),ordenar);
      total = 0;
      for(int i=0; i<np; i++)
            total += poblacion[i].comp;

}
void tsp::seleccionar_poblacion(){
      evaluar();
      for(int i =0; i < np/2; i++){
            file<<"---------------------"<<endl;
            cruce_pmx();
      }

      

}


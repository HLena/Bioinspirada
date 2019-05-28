#ifndef ANTSYS_H
#define ANTSYS_H

#include  <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include "Max_min.h"
using namespace std;

#define inf 1000;
typedef int city;
typedef float num;
typedef vector<num> vec_num ;


enum CITIES{A,B,C,D,E,F,G,H,I,J};
typedef struct
{
      float l = 0;
      vector<city> cities;
      vector<num> costos;
      string ruta="";
}ant;


class AntSystem{
      private:
            num Tmax,Tmin;
            vector<ant> pop;
            int n_ciudades;
            int ciudadInicial;
            string s_ciudades;
            float phi,alfa,beta;
            int mejor_local;
            num feromona;
            num Q,e;
            int n_hormigas;
            size_t iteraciones;
            bool tipo;
      public :
            ofstream  file;
            vec_num  Mv;
            vec_num  Md;
            vec_num  Mf;
            AntSystem();
            void iniMatrices();
            void iniMatrizFeromona();
            void Recorrido(ant &, int , int&);
            void actualizarArco(int i, int j);
            void iniMatrizVisibilidad();
            void printCities(vec_num &, int);
            void obtenerMatrizVisibilidad();
            void crearHormigas();
            void elegirSgtCiudad(ant&);
            void mostrarPop();
            void actualizarMatrizFeronoma();
            float nivelFeromona(int, int);
            int IndVector(vector<city> &,int);
            float buscar_ij(ant& , int, int);
            void inicio(string);
            void mostrarRuta(ant&);
};

AntSystem::AntSystem(){
      s_ciudades = "ABCDEFGHIJ";
      ciudadInicial = A;
      n_ciudades = s_ciudades.length();
      phi = 0.7;
      Q = 1;
      alfa = 1;
      beta = 1;
      Tmin = 0.00000009;
      Tmax = 10;
      feromona = Tmax;
      n_hormigas = 5;
      iteraciones = 100;
      mejor_local = 0;
}

void AntSystem::iniMatrizFeromona(){
      int x;
      for (size_t i = 0; i < n_ciudades; i++)
      {
            x = i*n_ciudades+i;
            Mf[x] = 0;
            // file << x <<endl;
      }
      
}

void AntSystem::iniMatrices(){
      Md = ReadTSP("TSP.txt",n_ciudades);
      vector<float>mf(n_ciudades*n_ciudades,feromona);
      vector<float>mv(n_ciudades*n_ciudades,0);
      Mf = mf;
      Mv = mv;
      iniMatrizFeromona();
      obtenerMatrizVisibilidad();
      file<<"---------Matriz de distacias-----------\n";
      printCities(Md,n_ciudades);
      file<<"---------Matriz de visibilidad-----------\n";
      printCities(Mv,n_ciudades);
      // file<<"---------Matriz de feromona-----------\n";
      // printCities(Mf,n_ciudades);

}

void AntSystem::printCities(vec_num  &vec, int n){
      int city;
      for (size_t i = 0; i < n; i++)
      {
            for (size_t j = 0; j < n; j++)
                  file <<vec[i*n+j]<< " ";
            file<<endl;      
      }
      
}

void AntSystem::obtenerMatrizVisibilidad(){
      float num,dem;
      for (size_t i = 0; i < n_ciudades; i++)
      {
            for (size_t j = 0; j < n_ciudades; j++)
            {
                  if(i != j)
                        Mv[i*n_ciudades+j] = 1/Md[i*n_ciudades+j];
            }
      }
}

void AntSystem::crearHormigas(){
      for (size_t i = 0; i < n_hormigas; i++)
      {
            ant a1;
            a1.cities.push_back(ciudadInicial);
            a1.ruta += s_ciudades[ciudadInicial];
            pop.push_back(a1);
      }
      
}

void AntSystem::mostrarPop(){
      file<<"------------------------------------------------\n";
      for (size_t i = 0; i < pop.size(); i++){
            file<<"Hormiga "<<i<<": "<<pop[i].ruta<<" - Costo: "<<pop[i].l<<" dis: ";
            mostrarRuta(pop[i]);
      }
      file<<"------------------------------------------------\n";
}

void AntSystem::mostrarRuta(ant &tmp){
      for (size_t i = 0; i < tmp.costos.size(); i++)
      {
            file<<tmp.costos[i]<<" ";
      }
      file<<endl;
      
}


int AntSystem::IndVector(vector<city> &vec,int x){   
      auto it = find(vec.begin(), vec.end(), x);
      if(it != vec.end())
            return  distance(vec.begin(), it);
}


float AntSystem::buscar_ij(ant &tmp, int pos, int j){
      if(pos > 0 && pos <n_ciudades){
            return (tmp.cities[pos+1]== j || tmp.cities[pos+1]== j)? 1/tmp.l : 0;
      }else if(pos == 0 ){
            return (tmp.cities[pos+1]== j)? 1/tmp.l : 0;
      }else{
            return (tmp.cities[pos-1]== j) ? 1/tmp.l : 0;
      }
}
float AntSystem::nivelFeromona(int i, int j){
      float t_ij = 0;
      city pos;
      vector<city> tmp;
      pos = IndVector(pop[mejor_local].cities,i);
      return buscar_ij(pop[mejor_local],pos,j);
      
}

void AntSystem::actualizarMatrizFeronoma(){
      for (size_t i = 0; i < n_ciudades; i++)
      {
            for (size_t j = 0; j < n_ciudades; j++)
            {
                  if(j != i){

                        Mf[i*n_ciudades+j] = (1-phi)*Mf[i*n_ciudades+j]+nivelFeromona(i,j);
                        if(Mf[i*n_ciudades+j] < Tmin) Mf[i*n_ciudades+j] = Tmin;
                        else if(Mf[i*n_ciudades+j] > Tmax) Mf[i*n_ciudades+j] = Tmax;
                        
                  }
            } 
      }
      
}




void AntSystem::elegirSgtCiudad(ant& h1){
      city currentCity = h1.cities.back(); 
      // int nextCity = ct * n_ciudades;
      num tn=0;
      num sum=0;
      map<int,num> pb; //PROBABILIDADES
      // file<<"ct: "<<ct<<endl;
      for (size_t i =0; i < n_ciudades; i++)
      {
            auto it =find(h1.cities.begin(), h1.cities.end(), i);
            if(it == h1.cities.end()){
                  tn = pow(Mf[currentCity*n_ciudades+i],alfa) * pow(Mv[currentCity*n_ciudades+i],beta);
                  file << s_ciudades[currentCity]<<"-"<<s_ciudades[i]<<": t = "<<Mf[currentCity*n_ciudades+i]<<" n = "<<Mv[currentCity*n_ciudades+i]<<" t*n = "<<tn<<endl;
                  pb[i] = tn;
                  sum+=tn;
            }
      }
      file<<"suma: "<<sum<<endl;
      num aleatorio = ((num)rand() * (1-0)) /(num)RAND_MAX + 0;
      
      city id;
      num limite = 0;
      for(auto it = pb.begin();it != pb.end(); it++){
            it->second = it->second/sum;
            file<<s_ciudades[currentCity]<<"-"<<s_ciudades[it->first]<<": Prob =  "<<it->second<<endl;
            if(limite < aleatorio && aleatorio<limite +it->second)
                  id = it->first;
            limite += it->second; 
      }
      h1.cities.push_back(id);
      h1.costos.push_back(Md[currentCity*n_ciudades+ id]);
      h1.l += Md[currentCity*n_ciudades+ id];
      h1.ruta += s_ciudades[id];
      file<<"Numero aleatorio para la probabilidad: "<<aleatorio<<endl;
      file<<"Ciudad siguiente: "<<s_ciudades[id]<<" ("<<Md[currentCity*n_ciudades+ id]<<")"<<endl;
      file<<"ruta: "<<h1.l<<endl;


}


void AntSystem::inicio(string filename){
      file.open(filename);
      //INICIALIZACION DE PARAMETROS E INDIVIDUOS
      iniMatrices();
      for (int it = 0; it < iteraciones; it++)
      {
            mejor_local=0;
            file<<"---------Matriz de feromona-----------\n";
            printCities(Mf,n_ciudades);
            file<<"================ ITERACION "<<it<<"==================="<<endl;
            crearHormigas();  
            for (int k = 0; k < n_hormigas; k++)
            {
                  file<<"--------------Hormiga "<<k<<"--------------"<<endl;
                  while(pop[k].cities.size()!=n_ciudades){
                        file<<"---------Elegir siguiente ciudad--------"<<endl;
                        elegirSgtCiudad(pop[k]);
                        
                  }
                  if(pop[mejor_local].l > pop[k].l) mejor_local=k;
            }            
            mostrarPop();
            file<<"Mejor Hormiga local: "<<pop[mejor_local].ruta<<" - Costo: "<<pop[mejor_local].l<<endl;
            actualizarMatrizFeronoma();
            pop.clear();
            
      }
      cout<<"Listo!!!"<<endl;
      

}


#endif
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
      string ruta="";
}ant;


class AntSystem{
      private:
            vector<ant> pop;
            int n_ciudades;
            int ciudadInicial;
            string s_ciudades;
            float phi,alfa,beta,e,Q, q0, qp,q;
            ant mejor_global;
            int mejor_local;
            float feromona;
            int n_hormigas;
            size_t iteraciones;
            bool tipo;
      public :
            ofstream  file;
            vec_num  Mv;
            vec_num  Md;
            vec_num  Mf;
            AntSystem(int, int,int, float,bool);
            void iniMatrices();
            void iniMatrizFeromona();
            num recorridoDiversificacion(int, int);
            num recorridoIntensificacion(int, int);
            void recorridoDiversificacion_(ant &, int , int&);
            void recorridoIntensificacion_(ant &, int , int&);
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

AntSystem::AntSystem(int it, int n, int inicity, float f,bool p){
      s_ciudades = "ABCDEFGHIJ";
      ciudadInicial = inicity;
      n_ciudades = s_ciudades.length();


      //------------------------------------------
      phi = 0.7;
      alfa = 1;
      beta = 1;
      q0 = 0.7;
      qp = 0.05;
      e = n_ciudades;
      Q = 1;
      feromona = f;
      n_hormigas = n;
      iteraciones = it;
      tipo = p;
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
      float a[] = {0,12,3,23,1,5,23,56,12,11,
                  12,0,9,18,3,41,45,5,41,27,
                  3,9,0,89,56,21,12,48,14,29,
                  23,18,89,0,87,46,75,17,50,42,
                  1,3,56,87,0,55,22,86,14,33,
                  5,41,21,46,55,0,21,76,54,81,
                  23,45,12,75,22,21,0,11,57,48,
                  56,5,48,17,86,76,11,0,63,24,
                  12,41,14,50,14,54,57,63,0,9,
                  11,27,29,42,33,81,48,24,9,0};

      unsigned tam = sizeof(a) / sizeof(num);

      vector<float>md(a,a+tam );
      vector<float>mf(n_ciudades*n_ciudades,feromona);
      vector<float>mv(n_ciudades*n_ciudades,0);
      Md = md;
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
      for (size_t i = 0; i < tmp.cities.size(); i++)
      {
            file<<tmp.cities[i]<<" ";
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
            return (tmp.cities[pos+1]== j || tmp.cities[pos+1]== j)? Q/tmp.l : 0;
      }else if(pos == 0 ){
            return (tmp.cities[pos+1]== j)? Q/tmp.l : 0;
      }else{
            return (tmp.cities[pos-1]== j) ? Q/tmp.l : 0;
      }
}
float AntSystem::nivelFeromona(int i, int j){
      float t_ij = 0;
      city pos;
      vector<city> tmp;
      pos = IndVector(mejor_global.cities,i);
      return buscar_ij(mejor_global,pos,j);
      
}

void AntSystem::actualizarMatrizFeronoma(){
      for (size_t i = 0; i < n_ciudades; i++)
      {
            for (size_t j = 0; j < n_ciudades; j++)
            {
                  if(j != i){

                        Mf[i*n_ciudades+j] = phi*Mf[i*n_ciudades+j]+(1-phi)*nivelFeromona(i,j);
                        
                  }
            } 
      }
      
}

void AntSystem::actualizarArco(int i, int j){
      Mf[i*n_ciudades+j] = (1-qp)*Mf[i*n_ciudades+j]+(qp*feromona);

}

// num AntSystem::recorridoDiversificacion(int pos_ct, int i){
//       return pow(Mf[pos_ct+i],alfa)*pow(Mv[pos_ct+i],beta);
// }

// num AntSystem::recorridoIntensificacion(int pos_ct, int i){
//       return Mf[pos_ct+i]*pow(Mv[pos_ct+i],beta);
// }

void AntSystem::recorridoIntensificacion_(ant &h1, int c_city,int &next_city){
      file<<"Recorrido por Intensificacion\n";
      num arg_max = 0;
      int pos_ct = c_city * n_ciudades;
      map<int,num> pb; //PROBABILIDADES
      city id_city;
      for (size_t i =0; i < n_ciudades; i++)
      {
            auto it =find(h1.cities.begin(), h1.cities.end(), i);
            if(it == h1.cities.end()){
                  pb[i] = pow(Mf[pos_ct+i],alfa)*pow(Mv[pos_ct+i],beta);
                  file << s_ciudades[c_city]<<"-"<<s_ciudades[i]<<": t = "<<Mf[pos_ct+i]<<" n = "<<Mv[pos_ct+i]<<" t*n = "<<pb[i]<<endl;
                  if(arg_max < pb[i]){
                              arg_max = pb[i];
                              next_city = i;     
                        }
            }
      }
     
     

      
}

void AntSystem::recorridoDiversificacion_(ant &h1, int c_city,int &next_city){
      file<<"Recorrido por Diversificacion\n";
      int pos_ct = c_city * n_ciudades;
      num sum=0;
      map<int,num> pb; //PROBABILIDADES
      city id_city;
      for (size_t i =0; i < n_ciudades; i++)
      {
            auto it =find(h1.cities.begin(), h1.cities.end(), i);
            if(it == h1.cities.end()){
                  pb[i] = pow(Mf[pos_ct+i],alfa)*pow(Mv[pos_ct+i],beta);
                  file << s_ciudades[c_city]<<"-"<<s_ciudades[i]<<": t = "<<Mf[pos_ct+i]<<" n = "<<Mv[pos_ct+i]<<" t*n = "<<pb[i]<<endl;
                  sum+=pb[i];
            }
      }
      file<<"suma: "<<sum<<endl;
      num aleatorio = ((num)rand() * (1-0)) /(num)RAND_MAX + 0;
      num limite = 0;
      for(auto it = pb.begin();it != pb.end(); it++){
            it->second = it->second/sum;
            file<<s_ciudades[c_city]<<"-"<<s_ciudades[it->first]<<": Prob =  "<<it->second<<endl;
            if(limite < aleatorio && aleatorio<limite +it->second)
            {
                  next_city = it->first;   
                  // break;
            }
            limite += it->second; 
      }
      file<<"Numero aleatorio para la probabilidad: "<<aleatorio<<endl;

      
}

void AntSystem::elegirSgtCiudad(ant& h1){
      city c_city = h1.cities.back(); 
      city next_city;
      q = ((num)rand() * (1-0)) /(num)RAND_MAX + 0;
      file<<"valor de q: "<<q<<endl;
      if( q <= q0)
            recorridoIntensificacion_(h1,c_city,next_city);
      else
            recorridoDiversificacion_(h1,c_city,next_city);
      h1.cities.push_back(next_city);
      h1.l += Md[c_city*n_ciudades+ next_city];
      h1.ruta += s_ciudades[next_city];
      file<<"Ciudad siguiente: "<<s_ciudades[next_city]<<" ("<<Md[c_city*n_ciudades+ next_city]<<")"<<endl;
      file<<"ruta: "<<h1.l<<endl;
      actualizarArco(c_city,next_city);

}

// void AntSystem::elegirSgtCiudad(ant& h1){
//       city ct = h1.cities.back(); 
//       int pos_ct = ct * n_ciudades;
//       num t_n=0;
//       num sum=0;
//       map<int,num> pb; //PROBABILIDADES
//       num arg_max=0;
//       city id_city;
//       q = ((num)rand() * (1-0)) /(num)RAND_MAX + 0;
//       file<<"valor de q: "<<q<<endl;
//       ( q > q0)? file<<"Recorrido por Diversificacion\n":file<<"Recorrido por Intensificacion\n";;
//       for (size_t i =0; i < n_ciudades; i++)
//       {
//             auto it =find(h1.cities.begin(), h1.cities.end(), i);
//             if(it == h1.cities.end()){
//                   pb[i] = (q <= q0)? recorridoIntensificacion(pos_ct, i):recorridoDiversificacion(pos_ct,i); 
//                   file << s_ciudades[ct]<<"-"<<s_ciudades[i]<<": t = "<<Mf[pos_ct+i]<<" n = "<<Mv[pos_ct+i]<<" t*n = "<<pb[i]<<endl;
//                   sum+=pb[i];
//                   if(arg_max < pb[i]){
//                         arg_max = pb[i];
//                         id_city = i;     
//                   }
//             }
//       }
//       if(q > q0)
//       {
//             file<<"suma: "<<sum<<endl;
//             num aleatorio = ((num)rand() * (1-0)) /(num)RAND_MAX + 0;
//             num limite = 0;
//             for(auto it = pb.begin();it != pb.end(); it++){
//                   it->second = it->second/sum;
//                   file<<s_ciudades[ct]<<"-"<<s_ciudades[it->first]<<": Prob =  "<<it->second<<endl;
//                   if(limite < aleatorio && aleatorio<limite +it->second)
//                   {
//                         id_city = it->first;   
//                         break;
//                   }
//                   limite += it->second; 
//             }
//             file<<"Numero aleatorio para la probabilidad: "<<aleatorio<<endl;
//       }
//       h1.cities.push_back(id_city);
//       h1.l += Md[pos_ct+ id_city];
//       h1.ruta += s_ciudades[id_city];
//       file<<"Ciudad siguiente: "<<s_ciudades[id_city]<<" ("<<Md[pos_ct+ id_city]<<")"<<endl;
//       file<<"ruta: "<<h1.l<<endl;
//       actualizarArco(h1.cities.back(),id_city);

// }

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
                  if(mejor_global.l == 0) 
                        mejor_global = pop[mejor_local];
                  else 
                        if(mejor_global.l > pop[mejor_local].l) mejor_global = pop[mejor_local];
                  file<<"camino: "<<pop[k].ruta<<endl;
            }            
            mostrarPop();
            file<<"Mejor Hormiga local: "<<pop[mejor_local].ruta<<" - Costo: "<<pop[mejor_local].l<<endl;
            file<<"Mejor global: "<< mejor_global.ruta <<" - Costo:"<<mejor_global.l<<endl;
            actualizarMatrizFeronoma();
            pop.clear();
            
      }
      cout<<"Listo!!!"<<endl;
      

}


#endif
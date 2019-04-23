#include <iostream>
#include <vector>
#include <math.h> 
#include <stdlib.h>  
#include <algorithm> 
#include <bits/stdc++.h>

using namespace std;


//VARIABLES 
typedef double num;
//vector<string> cromosomas;
//vector<double> porcent;



typedef struct {
    string fenotipo;
	int genotipo;
    num fitness;
    
}indv;

//priority_queue<indv> poblacion;
vector<indv> poblacion;//population
//vector<indv> hijos;
//vector<double> evaluacion;
vector<double> porcentaje;
//vector<int>pos;



double funcion_objetivo(int fenotipo){    return pow(fenotipo,2)-6*fenotipo+1;}

/*bool orden(const indv& a, const indv& b) {
        return a.genotipo >b.genotipo;
    
}*/

int fun_top(int n){
    int t = 0;
    for( int i = n; i > 0; i-- )
        t += pow( 2, i-1 );
    return t;
}
 

int ruleta(){
    double limit = 0;
    double num = rand()%100;  
   // cout<<"numero aleatorio: "<<num<<endl;
    for(int i=0;i < porcentaje.size();i++){
        //cout<<limit<<" "<<poblacion[i].porcent+limit<<endl;
        if(num <= porcentaje[i]+limit){
            return i;
        }
        else    
            limit+=porcentaje[i];
    }
}

string decimal_to_binary(double fenotipo,int n_genes){
    string cadena = "";
    int genotipo = fenotipo;
    while(genotipo != 0){
        if(genotipo % 2 == 1)
            cadena = "1" + cadena ;
        else
            cadena = "0" + cadena;
        genotipo = genotipo / 2;
    }
    if(cadena.length()< n_genes){
        int cl = cadena.length();
        for(int i = 0; i < n_genes-cl; i++)  cadena = "0" + cadena;
    }
    return cadena;
}

double binary_to_decimal(string str){
    //cout<<str<<endl;
    double n=0;
    int j = str.length();
    for(int l = 0; l < str.length(); l ++){
        if(str[j-1] == '1')   n += pow(2,l);
        j--;
    }
    return n;
}

bool ordenar (indv i1, indv i2) 
{ 
    return (i1.genotipo > i2.genotipo); 
} 
//==========================================================================================================

//string decimal_to_binary(double);
//double binary_to_decimal(string);

class genetico{
    public:
        int top;
        int n_individuos;
        int n_genes;
        int punto;
        num p_cruce;
        num p_mutacion;
        int num_generaciones;
        double total;
        genetico(int, int, int, num, num, int);
        void evaluando();
        void creacion_poblacion();
        void seleccion_poblacion();
        void cruzamiento(int, int);
        void nuevos_hijos(int, int);
        void mutacion(indv&);
        void mostrar_poblacion();
        void mostrar_mas();
        void calcular_fitness();
        void normalizar(double);
        void hallar_porcentajes();

};

genetico::genetico(int n_i, int n_g, int p, num p_c, num p_m, int g){
    n_individuos = n_i;
    n_genes = n_g;
    punto = p;
    p_cruce = p_c;
    p_mutacion = p_m;
    num_generaciones=g;
    top = fun_top(n_genes);
    evaluando();
    for(int i=0; i<num_generaciones; i++){
        cout<<" iteracion "<<i<<endl;
        evaluando();
    }
}
void genetico:: evaluando(){
    if(poblacion.size()> 0){
        seleccion_poblacion();
    }else{
        creacion_poblacion();
    }
}

void genetico::creacion_poblacion(){
    cout<<"-----Creando poblacion-----"<<endl;
    indv i1;
    int min = 0;
    for (int n = 0; n < n_individuos; n++){
        i1.genotipo = rand()%top;
        i1.fenotipo = decimal_to_binary(i1.genotipo,n_genes);
        poblacion.push_back(i1);
    }
    mostrar_poblacion();
    sort(poblacion.begin(),poblacion.end(),ordenar);
    //mostrar_poblacion();   
}
void genetico::mostrar_poblacion(){
    for (int l = 0; l < poblacion.size(); l++)
        cout <<l<<") "<<poblacion[l].fenotipo<<" - "<<poblacion[l].genotipo<<endl;
}

void genetico::mostrar_mas(){
    for (int l = 0; l < poblacion.size(); l++)
        cout <<l<<") "<<poblacion[l].fitness<<" - "<<poblacion[l].genotipo<<" - "<<porcentaje[l]<<endl;
}



void genetico::normalizar(double m){
    //cout<<"....m: "<<m<<endl;
    for( int t = 0; t < n_individuos; t++) { 
        poblacion[t].fitness = poblacion[t].fitness-(m);
        total += poblacion[t].fitness ;
    }

}
void genetico::calcular_fitness(){
    total = 0;
    double min = 0;
    for(int i = 0; i < n_individuos;i++){
        poblacion[i].fitness = funcion_objetivo(poblacion[i].genotipo);
        if(min > poblacion[i].fitness) min = poblacion[i].fitness;
        total += poblacion[i].fitness;
    }
    if(min < 0){
        total = 0;
        normalizar(min);   
    }
}

void genetico::seleccion_poblacion(){
    cout<<"-----evaluando individios------"<<endl;
    mostrar_poblacion();
    while(poblacion.size()>n_individuos)   poblacion.pop_back();
    calcular_fitness();
    porcentaje.clear();   
    for(int n = 0; n < n_individuos; n++)
        porcentaje.push_back(( poblacion[n].fitness * 100)/total); 
    int padre1,padre2;
    cout<<"----seleccion individuos - Metodo de la ruleta----"<<endl;
    mostrar_mas();
    for(int p = 0; p < n_individuos/2; p++){
        padre1 = ruleta();     padre2 = ruleta(); 
        while(padre1 == padre2)  padre2 = ruleta();
        cout <<"padre "<<padre1<<": "<<poblacion[padre1].fenotipo<<" madre "<<padre2<<": "<<poblacion[padre2].fenotipo<<endl;   
        cruzamiento(padre1,padre2);
        cout<<"----------------()-----------------"<<endl;
    } 
    cout<<" Seleccion de sgt poblacion"<<endl;
    sort(poblacion.begin(),poblacion.end(),ordenar);
}
void genetico::cruzamiento(int p1,int p2){
     double random_cruce= rand()%100/100.0;  
     cout<<"Hay cruce? ";
     if(p_cruce > random_cruce ){
        cout<<"Si"<<endl;
        nuevos_hijos(p1,p2);
        mostrar_poblacion();
     }else
        cout<<"No"<<endl; 
}
void genetico::nuevos_hijos(int p, int m){
    indv id1, id2;
        id1.fenotipo = poblacion[p].fenotipo.substr(0,punto) + poblacion[m].fenotipo.substr(punto,n_genes);
        id1.genotipo= binary_to_decimal(id1.fenotipo);
        cout<<"hijo 1: "<<id1.fenotipo<<" = "<<id1.genotipo<<endl;
        mutacion(id1);
        poblacion.push_back(id1);
        id2.fenotipo = poblacion[m].fenotipo.substr(0,punto) + poblacion[p].fenotipo.substr(punto,n_genes);
        id2.genotipo= binary_to_decimal(id2.fenotipo);
        cout<<"hijo 2: "<<id2.fenotipo<<" = "<<id2.genotipo<<endl;
        mutacion(id2);
        poblacion.push_back(id2);
    
}

void genetico::mutacion(indv &x){
    double random_mutacion= rand()%100/100.0;  
    cout<<"Hay mutacion? ";
    if(random_mutacion<p_mutacion){
        cout<<"Si"<<endl;
        int bit = rand()%n_genes;
        (x.fenotipo[bit]=='0') ? x.fenotipo[bit]='1':  x.fenotipo[bit]='0';
        x.genotipo = binary_to_decimal(x.fenotipo);
        cout<<x.fenotipo<<" - "<<x.genotipo<<endl;
    }
    else
        cout<<"No"<<endl;
}



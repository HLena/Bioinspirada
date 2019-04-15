
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
#define M 12
int limit = 100;


double alfa,beta;

typedef struct {
    //vector<double> var;
    double x,y,z,v,w;
    double fitness;
    
}indv;

vector<indv> poblacion;
class g_reales{
    public:
        //int n_bits;
        ofstream file;
        int n_variables;
        int n_p;
        double p_mutacion;
        double p_cruce;
        int n_generaciones;
        double total;
        g_reales(int, int, double, double, int );
        void evaluando();
        void crear_poblacion();
        void seleccionar_poblacion();
        void evaluar();
        void cruce_blx(double);
        void nuevos_hijos(int, int);
        void mutacion(indv&);
        void mostrar_poblacion();
        void mostrar_hijo(indv);
        void calcular_fitness();
        void normalizar(double);
        void hallar_porcentajes();
        int torneo();
        double hallar_beta(int);

};


//================================================================
double funcion_f(int x, int y, int z,int v, int w){
        return v - w + x -y + z;
}

bool ordenar(indv i1, indv i2) 
{ 
    return (i1.fitness > i2.fitness); 
} 
//===============================================================
g_reales::g_reales(int nv, int np, double px, double pm, int ng){
        n_variables = nv;
        n_p = np;
        p_mutacion = pm;
        p_cruce = px;
        n_generaciones = ng;
        file.open("salida.txt");
        for(int g=0; g <= n_generaciones; g++){
                if(poblacion.size() > 0){
                        file<<"---------GENERACION "<<g<<" ---------\n";
                        seleccionar_poblacion();
                        //break;

                }
                else    
                        crear_poblacion();
        }
        //crear_poblacion();
        file.close();
}

void g_reales::mostrar_poblacion(){
        indv tmp;
         //file<<"size p: "<< poblacion.size()<<endl;
        for(int n=0; n < poblacion.size(); n++)
        {
                tmp = poblacion[n];
                file<<n<<" ( "<<tmp.x<<" , "<<tmp.y<<" , "<<tmp.z<<" , "<<tmp.v<<" , "<<tmp.w<<") f: "<<tmp.fitness<<'\n';
        } 
        //file<<"salir"<<endl;
}

void g_reales::mostrar_hijo(indv tmp){
        file<<"( "<<tmp.x<<" , "<<tmp.y<<" , "<<tmp.z<<" , "<<tmp.v<<" , "<<tmp.w<<")"<<endl;
}
void g_reales::crear_poblacion()
{
        indv tmp;
        double rando;
        for(int n=0; n < n_p; n++)
        {
                tmp.x = (rand()% 201)-limit;
                tmp.y = (rand()% 201)-limit;
                tmp.z = (rand()% 201)-limit;
                tmp.v = (rand()% 201)-limit;
                tmp.w = (rand()% 201)-limit;
                tmp.fitness = funcion_f(tmp.x,tmp.x,tmp.z,tmp.v,tmp.w);
                poblacion.push_back(tmp);
        }
        file<<"-----Poblacion inicial-------"<<endl;
        mostrar_poblacion();
        file<<"-----------------------------"<<endl;
}

int g_reales::torneo(){
        map<int,double> t;
        //pair<int,double> id;
        int rando, x, max=0;
        double f;
        for (int it = 0; it < 3; it++){
                rando = rand()%n_p;
                auto id = t.find(rando);
                while(id != t.end()){
                        rando = rand()%n_p;
                        id = t.find(rando);
                }
                //id->first = rando;
                f = poblacion[rando].fitness;
                if(f > max){
                        max = f;
                        x = rando ;
                }
                t.insert(pair<int,double>(rando,f));
        }
        return x;
}

void g_reales::seleccionar_poblacion(){
        evaluar();
        //return;
        for(int n = 0; n < n_p  ; n++){
                file<<"cruce: "<<n<<endl;
                cruce_blx(0.5);
        }
        file<<"-----------nueva poblacion--------"<<endl;
        mostrar_poblacion();

}

double g_reales::hallar_beta(int a){
        int aa = (2*a)+1;
        double beta = (rand()% aa)-a; //FALTA 
        if(beta < -5) beta = beta + 10;
        return beta /10;

}

void g_reales::cruce_blx(double alfa){
        int p= (alfa+1)*10;
        double beta = hallar_beta(p);
        double r = rand()%10 ;
        file<<"pc: "<<r/10;
        if(r/10 <= p_cruce){
                file<<" Si hay Cruce: ";
                int id1,id2;
                id1 = torneo();
                id2 = torneo();
                while(id1 == id2){
                        id2 = torneo();
                }
                file<<"padre "<< id1<<" --- madre "<<id2<<endl;
                indv p1,p2,h1;
                p1 = poblacion[id1]; p2 = poblacion[id2];
                
                        h1.x= p1.x + beta*(p2.x - p1.x);
                        h1.y= p1.y + beta*(p2.y - p1.y); 
                        h1.z= p1.z + beta*(p2.z - p1.z); 
                        h1.v= p1.v + beta*(p2.v - p1.v); 
                        h1.w= p1.w + beta*(p2.w - p1.w); 
                        while( h1.x < -100 or h1.x > 100)
                                h1.x= p1.x + hallar_beta(p) *(p2.x - p1.x); 
                        while( h1.y < -100 or h1.y > 100)
                                h1.y= p1.y + hallar_beta(p) *(p2.y - p1.y); 
                        while( h1.z < -100 or h1.z > 100)
                                h1.z= p1.z + hallar_beta(p) *(p2.z - p1.z); 
                        while( h1.v < -100 or h1.v > 100)
                                h1.v= p1.v + hallar_beta(p) *(p2.v - p1.v); 
                        while( h1.w < -100 or h1.w > 100)
                                h1.w= p1.w + hallar_beta(p) *(p2.w - p1.w); 

        
                file<<endl;
                mutacion(h1);
                mostrar_hijo(h1);
                h1.fitness = funcion_f(h1.x,h1.y,h1.z,h1.v,h1.w);
                poblacion.push_back(h1);
        }else
                file<<" No hay Cruce"<<endl;
}

void g_reales::mutacion(indv &p){
        double r = rand()%10;
        file<<"pm: " <<r/10;
        if(r/10 <= p_mutacion){
                int n = rand()%n_variables;
                file<<" Si hay mutacion"<<endl; 
                if(n== 0)       p.x = (rand()% 201)-limit; 
                else if(n== 1)  p.y = (rand()% 201)-limit; 
                else if(n== 2)  p.z = (rand()% 201)-limit; 
                else if(n== 3)  p.v = (rand()% 201)-limit; 
                else if(n== 4)  p.w = (rand()% 201)-limit; 

                // p.x = (rand()% 201)-limit;   
        }else
             file<<" No hay mutacion"<<endl;   
}

void g_reales::evaluar(){
        file<<"--------evaluando--------"<<endl;
        sort(poblacion.begin(),poblacion.end(),ordenar);
        //file<<"--------ordenado--------"<<endl;
        while(poblacion.size() > n_p)
                poblacion.pop_back();
        mostrar_poblacion();
        
}




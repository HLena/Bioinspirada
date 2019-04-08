#include <iostream>
#include <vector>
#include <math.h> 
#include <stdlib.h>  
#include <algorithm> 
using namespace std;


//VARIABLES 
typedef double num;
//vector<string> cromosomas;
//vector<double> porcent;



typedef struct {
    string X;
	int valor;
    num porcent;
    num fitness;
}indv;

vector<indv> poblacion;//population
vector<indv> hijos;
vector<double> evaluacion;
vector<int>pos;


//FUNCIONES
double funcion(int x){    return pow(x,2)-6*x+1;}

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
    for(int i=0;i < poblacion.size();i++){
        //cout<<limit<<" "<<poblacion[i].porcent+limit<<endl;
        if(num <= poblacion[i].porcent+limit){
            return i;
        }
        else    
            limit+=poblacion[i].porcent;
    }
}

//string decimal_to_binary(double);
//double binary_to_decimal(string);

class genetico{
    public:
        int top;
        //vector<num> poblacion;
        int n_individuos;
        int n_genes;
        int punto;
        num p_cruce;
        num p_mutacion;
        int num_generaciones;
        genetico(int, int, int, num, num, int);
        void creacion_poblacion();
        void seleccion_poblacion();
        void cruzamiento(int, int);
        void nuevos_hijos(int, int);
        void mutacion(string);
        void metodo_ruleta();
        void mostrar_poblacion();
        void mostrar_hijos();
        void act_porcent(double);
        void evaluacion_ind(double);
        //void mostrar_cromo(vector<string> v);

};

genetico::genetico(int n_i, int n_g, int p, num p_c, num p_m, int g){
    n_individuos = n_i;
    n_genes = n_g;
    punto = p;
    p_cruce = p_c;
    p_mutacion = p_m;
    num_generaciones=g;
    top = fun_top(n_genes);
    for(int i=0; i<num_generaciones; i++){
        cout<<"generacion "<<i<<endl;
        if(poblacion.size()==0)
            creacion_poblacion();
        else
            seleccion_poblacion();
    }
   

}
string decimal_to_binary(double x,int n_genes){
    string cadena = "";
    int valor = x;
    while(valor != 0){
        if(valor % 2 == 1)
            cadena = "1" + cadena ;
        else
            cadena = "0" + cadena;
        valor = valor / 2;
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

void genetico::creacion_poblacion(){
    indv i1;
    int t_p = 0;
    int min = 0;
    for (int n = 0; n < n_individuos; n++){
        i1.valor = rand()%top;
        i1.X = decimal_to_binary(i1.valor,n_genes);
        i1.fitness = funcion(i1.valor);
        if( i1.fitness < min ) min = i1.fitness;
        poblacion.push_back(i1);
    
    }
    act_porcent(min - 1);
    cout<<"-----Creando poblacion-----"<<endl;
    mostrar_poblacion();
    seleccion_poblacion();
    
}
void genetico::mostrar_poblacion(){
    for (int l = 0; l < poblacion.size(); l++)
        cout <<l<<")   "<<poblacion[l].X<<" - "<<poblacion[l].valor<<" f("<<poblacion[l].valor<<") = "<<poblacion[l].fitness<<" P:"<<poblacion[l].porcent<<"%"<<endl;
}

void genetico::mostrar_hijos(){
    for (int l = 0; l < hijos.size(); l++)
        cout <<l<<")   "<<hijos[l].X<<" - "<<hijos[l].valor<<" f("<<hijos[l].valor<<") = "<<hijos[l].fitness<<" P:"<<hijos[l].porcent<<"%"<<endl;
    
}


void genetico::act_porcent(double m){
    double t_p = 0;
    for (int n = 0; n < poblacion.size(); n++){
        if(m<0) poblacion[n].fitness = poblacion[n].fitness - m;
        t_p += poblacion[n].fitness;
        evaluacion.push_back(poblacion[n].fitness);
        //break;
    }
    for(int n = 0; n < poblacion.size(); n++){
        poblacion[n].porcent = (poblacion[n].fitness * 100)/t_p;
        //cout<<"%: "<< poblacion[n].porcent<<endl;
    }

    
}

void genetico::seleccion_poblacion(){
    int x1,x2;
    cout<<"-------seleccion de los mejores---------"<<endl;
    if(hijos.size() > 0){
        //poblacion.clear();
        sort(evaluacion.begin(), evaluacion.begin()+evaluacion.size()); 
            /*for(int i=evaluacion.size()-1; i>=evaluacion.size()-n_individuos;i--){
                indv id;
                id.fitness = 
                poblacion.push_back();
            }*/
               
    }
    for(int p = 0; p < poblacion.size()/2; p++){
            x1 = ruleta();     x2 = ruleta(); 
            while(x1 == x2)
                x2 = ruleta();
            cout <<"padre "<<x1<<": "<<poblacion[x1].X<<"     madre "<<x2<<": "<<poblacion[x2].X<<endl;   
            cruzamiento(x1,x2);
    }  
    mostrar_poblacion();
    cout<<"------hijos------"<<endl;
    mostrar_hijos();
}
void genetico::cruzamiento(int x1,int x2){
     double random_cruce= rand()%100/100.0;  
     cout<<"Hay cruce? ";
     if(p_cruce > random_cruce ){
        cout<<"Si"<<endl;
        nuevos_hijos(x1,x2);
     }else{
        cout<<"No"<<endl;
        hijos.push_back(poblacion[x1]);
        hijos.push_back(poblacion[x2]);
     }
    
}
void genetico::nuevos_hijos(int p, int m){
    indv id1, id2;
        id1.X = poblacion[p].X.substr (0,punto) + poblacion[m].X.substr (punto,n_genes);
        id1.valor= binary_to_decimal(id1.X);
        id1.fitness = funcion(id1.valor);
        evaluacion.push_back( id1.fitness);
        cout<<"hijo 1: "<<id1.X<<" = "<<binary_to_decimal(id1.X)<<endl;
        mutacion(id1.X);
        hijos.push_back(id1);
        id2.X = poblacion[m].X.substr (0,punto) + poblacion[p].X.substr (punto,n_genes);
        id2.valor= binary_to_decimal(id2.X);
        id2.fitness = funcion(id2.valor);
        evaluacion.push_back( id2.fitness);
        cout<<"hijo 2: "<<id2.X<<" = "<<binary_to_decimal(id2.X)<<endl;
        mutacion(id2.X);
        hijos.push_back(id2);
}

void genetico::mutacion(string x){
    double random_mutacion= rand()%100/100.0;  
    cout<<"Hay mutacion? ";

    if(random_mutacion<p_mutacion){
        cout<<"Si"<<endl;
        int bit = rand()%n_genes;
        //cout<< "bit: "<<bit<<" cadena: "<<x;
        (x[bit]=='0') ? x[bit]='1':  x[bit]='0';
        //cout<<" mutacion: "<<x;
    }
    else
        cout<<"No"<<endl;
    //cout<<endl;
}


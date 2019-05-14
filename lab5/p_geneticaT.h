

#ifndef P_GENETICA_H
#define P_GENETICA_H
#include <vector>
#include <string>
#include <iostream>
#include <tgmath.h>
#include <algorithm>
#include <fstream>
using namespace std;

float entradas[]={0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9};
float salidas[]={0,0.005,0.02,0.045,0.08,0.125,0.18,0.245,0.32,0.405};
char operadores[] = {'+','-','*','%','/'};



typedef struct{
	string cadena= " ";
	float fitness = 0;
	float inv =0;
	vector<int> valores;
	float porcent;
}indv;


class pgenetica{
	public:
		ofstream file;
		vector<indv> pob;
		vector<indv> npob;
		int num_poblacion;
		float pro_cruce;
		float pro_mutacion;
		float pro_reproduccion;
		float total_fit = 0;
		int tam_cadena;
		int iteraciones;
		int limit_op;
		int num_op;
		pgenetica(int ,float, float, float,int, int);
		void inicio();
		void crearPoblacion();
		indv crearIndividuo();
		float validarIndividuo(vector<int> &, int, float);
		void mostrarPoblacion();
		void mostrarValoresVec(vector<int> &v);
		string leerValoresVec(vector<int> &v, int pos);
		float hallandoFitness(vector<int> &);
		int metodoRuleta();
		int metodoTorneo();
		void generarPorcentajes();
        void selecionandoOperacion();
        void mutacion();
        void cruzamiento();
        void Reproduccion();
        void selecionandoIndividuos();

};

bool ordenar(indv &a, indv &b)
{
    return a.fitness < b.fitness;
}

pgenetica::pgenetica(int a, float b, float c, float d, int e, int f){
	num_poblacion = a;
	pro_cruce = b;
	pro_reproduccion = c;
	pro_mutacion = d;
	tam_cadena = e;
	iteraciones = f;
	limit_op = tam_cadena - pow(2,2);
	num_op = 5;
}
void pgenetica::mostrarValoresVec(vector<int> &v){
	file<<"[ ";
	for (int i = 0; i < v.size(); ++i)
		file<<v[i]<<" ";
	file<<"] ";
}
void pgenetica::mostrarPoblacion(){
	for (int i = 0; i < pob.size(); ++i)
	{	
		file<<"("<<i<<") ";
		mostrarValoresVec(pob[i].valores);
		file<<"c:"<<pob[i].cadena<<" f:"<<pob[i].fitness<<endl;
	}
}

string pgenetica::leerValoresVec(vector<int> &i, int pos){
	if(pos < tam_cadena)
	{
		if(pos < limit_op)
		{
			switch(i[pos]){
				case 0:
					return "(+ "+leerValoresVec(i,pos*2+1)+leerValoresVec(i,pos*2+2)+")";
				case 1:
					return "(- "+leerValoresVec(i,pos*2+1)+leerValoresVec(i,pos*2+2)+")";
				case 2:
					return "(* "+leerValoresVec(i,pos*2+1)+leerValoresVec(i,pos*2+2)+")";
				case 3:
					return "(% "+leerValoresVec(i,pos*2+1)+leerValoresVec(i,pos*2+2)+")";
				case 4:
					return "(/ "+leerValoresVec(i,pos*2+1)+leerValoresVec(i,pos*2+2)+")";
			}
		}else{
			int x = 20;
			if(i[pos] == x)
				return "x ";
			return to_string(i[pos])+" ";
		}
	}
}

indv pgenetica::crearIndividuo(){
	indv tmp;
	int x = 20; // 20 representara la variable x
	float ale;
	for (int i = 0; i < tam_cadena; ++i)
	{
		if( i < limit_op)
		{
			ale = rand()% num_op;
			tmp.valores.push_back(ale);
		}else{
			ale = (float)(rand() % 101)/100;
			if(ale <= 0.5)
				tmp.valores.push_back(x);
			else{
				ale = (rand()% 11)-5;
				tmp.valores.push_back(ale);
			}
		}
	}
	return tmp;
}

float pgenetica::validarIndividuo(vector<int> &i, int pos, float v)
{

	if(pos < tam_cadena)
	{
		if(pos < limit_op)
		{
			switch(i[pos]){
				case 0:
					return validarIndividuo(i,pos*2+1,v) +  validarIndividuo(i,pos*2+2,v);
				case 1:
					return validarIndividuo(i,pos*2+1,v) -  validarIndividuo(i,pos*2+2,v);
				case 2:
					return validarIndividuo(i,pos*2+1,v) *  validarIndividuo(i,pos*2+2,v);
				case 3:
					return fmod(validarIndividuo(i,pos*2+1,v),validarIndividuo(i,pos*2+2,v));
				case 4:
					return validarIndividuo(i,pos*2+1,v) /  validarIndividuo(i,pos*2+2,v);
			}
		}else{
			int x = 20;
			if(i[pos] == x)
				return v;
			return i[pos];
		}
	}
}

float pgenetica::hallandoFitness(vector<int> &v)
{
	float suma=0,dif,s,n = 10;
	for (int j = 0; j < n; ++j)
	{
		s= validarIndividuo(v,0,entradas[j]);
		dif = salidas[j]-s;
		suma += pow(dif,2);
	}
	return suma / n;
}

void pgenetica::generarPorcentajes()
{
	// file <<"hallando porcentajes: ("<<total_fit<<")"<<endl;

	for (int i = 0; i < pob.size(); ++i){
		// file<<pob[i].fitness<<" ";
		pob[i].porcent = (pob[i].fitness*100)/total_fit;
		// file<<pob[i].porcent<<endl;
	}
	total_fit = 0;
}

int pgenetica::metodoRuleta()
{
	int ale = rand()%101;
	float limite = 0;
	// file<<"ruleta: "<< ale<<endl;
	for (int i = 0; i < pob.size(); ++i)
	{
		limite += pob[i].porcent;
		// file <<"-> "<<limite<<endl;
		if(ale <= limite){
			// file <<"pos: "<<i<<endl;
			return i;
		}
	}

}

int pgenetica::metodoTorneo()
{
	// srand( time( NULL ) );
	int p1,p2;
	p1 = rand()%pob.size();
	do p2 = rand()%pob.size();while(p1 == p2);
	return (pob[p1].fitness < pob[p1].fitness)? p1: p2;
}
void pgenetica::crearPoblacion()
{
	file <<"<-----------------Probalcion inicial------------------->"<<endl;
	int ale;
	indv tmp;
	for (int i = 0; i < num_poblacion; ++i)
	{
		do tmp = crearIndividuo();
		while(isnan(validarIndividuo(tmp.valores,0,0)) or isinf(validarIndividuo(tmp.valores,0,0)));
		tmp.cadena = leerValoresVec(tmp.valores,0);
		file<<"individuo: "<<tmp.cadena<<endl;
		tmp.fitness = hallandoFitness(tmp.valores);
		// total_fit += tmp.fitness;
		pob.push_back(tmp);
	}
	// file<<"Total fit:"<<total_fit<<endl;

}
void pgenetica::mutacion()
{
	file <<"...MUTACION..."<<endl;
	// srand( time( NULL ) );
    int id = metodoTorneo();
    indv aux;
    int pos;
    int val;
    float c;
    int n = -1;
  	// mostrarValoresVec( pob[id].valores);file<<endl;
    do{
        aux = pob[id];
        pos=rand() % tam_cadena;
        if(pos < limit_op){
            val = rand()%num_op;            
            aux.valores[pos] = val;
        }else{        	
            c = (float)(rand() % 101)/100;
            if(c <= 0.5){
                aux.valores[pos]=20;              
            }
            else{
                val = (rand()% 11)-5;
                aux.valores[pos]=val;                
            }
        }
        n++;
        c = validarIndividuo(aux.valores,0,0);
    }while(isnan(c) or isinf(c));    
    mostrarValoresVec(aux.valores);file<<endl;
    // file <<"Repeticiones: "<<n<<endl;
    file<<"id: "<<id<<endl;
    aux.cadena = leerValoresVec(aux.valores,0);
    aux.fitness = hallandoFitness(aux.valores);
    file<<"p1: "<<pob[id].cadena<<endl;
    file<<"m1: "<<aux.cadena<<endl;
    npob.push_back(aux);
}

void pgenetica::cruzamiento()
{
	file <<"...CRUZAMIENTO..."<<endl;
	file<<"capacidad: "<<pob.size()-npob.size()<<endl;
    int p1, p2;
    float v1,v2;
    indv h1, h2;
    int n = -1;
    int r = 2;
    do{
        p1 = metodoTorneo();
        do{p2 = metodoTorneo();}while (p1 == p2);
        h1 = pob[p1]; h2 = pob[p2];
        h1.valores[r] = pob[p2].valores[r];
        h1.valores[r*2+1] = pob[p2].valores[r*2+1] ;
        h1.valores[r*2+2] = pob[p2].valores[r*2+2] ;
        h2.valores[r] = pob[p1].valores[r];
        h2.valores[r*2+1] = pob[p1].valores[r*2+1] ;
        h2.valores[r*2+2] = pob[p1].valores[r*2+2] ;
        v1 = validarIndividuo(h1.valores,0,0);
        v2 = validarIndividuo(h2.valores,0,0);
        n++;
    }while(isnan(v1) or isnan(v2) or isinf(v1) or isinf(v2));
    file<<"id1: "<<p1<<"  id2: "<<p2<<endl;
    h1.cadena = leerValoresVec(h1.valores,0);
    h1.fitness = hallandoFitness(h1.valores);
    h2.cadena = leerValoresVec(h2.valores,0);
    h2.fitness =  hallandoFitness(h2.valores);
    npob.push_back(h1);
    npob.push_back(h2); 
    file<<"p1: "<<pob[p1].cadena<<" p2: "<<pob[p2].cadena<<endl;
    file<<"h1: "<<h1.cadena<<" h2: "<<h2.cadena<<endl;

}

void pgenetica::selecionandoOperacion()
{	
	int op;
	srand( time( NULL ) );
	file <<"<------------------------ Operaciones ----------------->"<<endl;
    while(npob.size() < pob.size()) {
    	op = rand()%101;
    	file<<"tam: "<<npob.size()<<endl;
        // file<<"pobabilidad: "<<op<<endl;
        if(op <= pro_mutacion)
            mutacion();
        else if((op > pro_mutacion && op <=  pro_mutacion + pro_cruce) && (pob.size()-npob.size())> 1 )
            cruzamiento();
        else
            Reproduccion();
    }
    // file <<"<------------------------------------------------------>"<<endl;
    pob = npob;
    npob.clear();
}

void pgenetica::Reproduccion()
{
	file <<"...REPRODUCCION..."<<endl;
	int alt = rand()% pob.size();
	file<<"id: "<<alt<<endl;
	file << pob[alt].cadena<<endl;
	npob.push_back(pob[alt]);
}

void pgenetica:: inicio()
{
	file.open("p_geneticaT.txt");
	for(int i=0; i < iteraciones; ++i)
	{
		file<<"====================ITERACION "<<i<<" ==================="<<endl;
		if(pob.size() > 0)
		{
			// generarPorcentajes();
            // mostrarPoblacion();
			// break;
            file <<"<----------------Poblacion ordenada ------------->"<<endl;
            sort(pob.begin(), pob.end(),ordenar);
            mostrarPoblacion();
            // while(pob.size() > num_poblacion) pob.pop_back();
            selecionandoOperacion();
		}else
			crearPoblacion();
		// mostrarPoblacion();
	}
	file.close();

}

#endif

#include<bits/stdc++.h>
using namespace std;

typedef int num;
typedef float numf;

num np = 5;
num iteraciones = 100;
numf phi1 = 2, phi2 = 2;
numf rand1, rand2;
numf w;
// num g1, g2;
pair<numf, numf> g;
numf pBest_fitness;
numf lim_inf=-1, lim_sup=2;


numf funcion(numf x, numf y){
    return x*sin(4*M_PI*x)-y*sin(4*M_PI*y + M_PI) + 1;
}

typedef struct{
    numf x1, x2;
    numf v1, v2;
    // numf p1, p2;
    numf fitness_x;
    numf pBest1, pBest2;
}particula;

vector<particula> vp;

bool ordenar(particula &p1, particula &p2){
    return p1.fitness_x > p2.fitness_x;
}

void generar_X(particula &p){
    p.x1=((numf)rand() * (lim_sup-lim_inf)) /(numf)RAND_MAX + lim_inf;
    do p.x2=((numf)rand() * (lim_sup-lim_inf)) /(numf)RAND_MAX + lim_inf; while(p.x1 == p.x2);
    p.v1=((numf)rand() * (1-0)) /(numf)RAND_MAX + 0;
    do p.v2=((numf)rand() * (1-0)) /(numf)RAND_MAX + 0; while(p.v1 == p.v2);
    p.pBest1 = p.x1;
    p.pBest2 = p.x2;
}
// void hallarAleatorios(){
//     w = ((numf)rand() * (1-0)) /(numf)RAND_MAX + 0;
//     rand1 = ((numf)rand() * (1-0)) /(numf)RAND_MAX + 0;
//     rand2 = ((numf)rand() * (1-0)) /(numf)RAND_MAX + 0;
// }
void actualizarValores(particula &p){
    do{
        rand1 = ((numf)rand() * (1-0)) /(numf)RAND_MAX + 0;
        rand2 = ((numf)rand() * (1-0)) /(numf)RAND_MAX + 0;
        p.v1= w*p.v1 + phi1*rand1*(p.pBest1-p.x1) +phi2*rand2*(g.first-p.x1);
        p.v2= w*p.v2 + phi1*rand1*(p.pBest2-p.x2) +phi2*rand2*(g.second-p.x2);
        p.x1 = p.x1 + p.v1;
        p.x2 = p.x2 + p.v2;
    }while ((p.x1<lim_inf or p.x1>lim_sup) or (p.x2<lim_inf or p.x2>lim_sup));
    p.fitness_x = funcion(p.x1,p.x1);
    if(funcion(p.pBest1,p.pBest2) > p.fitness_x)
    {
        p.pBest1 = p.x1;
        p.pBest2 = p.x2;
    }
    
}

void crearParticulas(){
    for(size_t i=0; i<np; i++){
        particula tmp;
        generar_X(tmp);
        tmp.fitness_x = funcion(tmp.x1, tmp.x2);
        vp.push_back(tmp);   
    }

}

void mostrarPoblacion(){
    for(size_t i=0; i<vp.size();i++)
        cout<<"#"<<i+1<<" x("<<vp[i].x1<<","<<vp[i].x2<<") v("<<vp[i].v1<<" , "<<vp[i].v2<<") fit= "<<vp[i].fitness_x<<endl;
}

void actualizarParticulas(){
    for (size_t i = 0; i < vp.size(); i++)
        actualizarValores(vp[i]);
    
}


void inicio(){

    for(size_t i=0; i <= iteraciones;i++){
        if(vp.size() > 0){
            w = ((numf)rand() * (1-0)) /(numf)RAND_MAX + 0;
            sort(vp.begin(), vp.end(),ordenar);
            mostrarPoblacion();
            cout<<"--------Iteracion "<<i<<" -----------"<<endl;
            g.first = vp[0].x1;
            g.second = vp[0].x2;
            pBest_fitness = vp[0].fitness_x;
            cout<<"Mejor: x("<<g.first<<" , "<<g.second<<") fit= "<<pBest_fitness<<endl;
            actualizarParticulas();
        }else{
            cout<<"------Poblacion inicial---------\n";
            crearParticulas(); 
        }
    }
    cout<<"Listo!!"<<endl;
}

int main(){
    inicio();
}
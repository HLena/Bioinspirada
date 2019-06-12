#include "ColonyBees.h"

numf fitness(numf x, numf y){
    return  x*sin(4*M_PI*x) - y*sin((4*M_PI*y)+M_PI) + 1;
}

numf fit(numf fx){
    return (fx >= 0)? 1/(1+fx) : 1+abs(fx);
}


void crear_poblacion(){
    solucion ss;
    for(size_t i=0; i<NS ; i++){
        ss.id = i;
        ss.x1=((numf)rand() * (lim_sup-lim_inf)) /(numf)RAND_MAX + lim_inf;
        ss.x2=((numf)rand() * (lim_sup-lim_inf)) /(numf)RAND_MAX + lim_inf;
        ss.fx =fitness(ss.x1,ss.x2);
        ss.fit = fit(ss.fx);
        if(ss.fx > mj.fx) mj = ss;
        vs.push_back(ss);
        
    }
    cout<<"---------------------Creando poblacion---------------------\n";
}

void mostrar_soluciones(){
    for(size_t i=0; i<vs.size(); i++){
        cout<<vs[i].id<<" ("<<vs[i].x1<<", "<<vs[i].x2<<") f(x):"<<vs[i].fx<<" fit:"<<vs[i].fit<<" p:"<<vs[i].p<<" c:"<<vs[i].cont<<endl;
    }
}

void valores_aleatorios(num i){
    do k=rand()%(NS+1); while(k==i);
    j=1+rand()%(D);
    // phi=((numf)rand() * (1-(-1))) /(numf)RAND_MAX + (-1);
}
void calcular_probabilidad(){
    Sumfit=0;
    for (size_t i = 0; i < vs.size(); i++){
        Sumfit += vs[i].fit ;
    }
    // cout<<"SumFit: "<<Sumfit<<endl;
    for (size_t i = 0; i < vs.size(); i++){
        vs[i].p = vs[i].fit / Sumfit;
    }
}
void fuente_abandonada(solucion &m){
    numf r;
    if(m.cont > L){
        cout<<"**fuente abandonada***\n";
        cout<<m.id<<" ("<<m.x1<<", "<<m.x2<<") f(x):"<<m.fx<<" fit:"<<m.fit<<" p:"<<m.p<<" c:"<<m.cont<<endl;
        r = ((numf)rand() * (1-0)) /(numf)RAND_MAX + 0;
        m.x1 = lim_inf + r*(lim_sup-lim_inf);
        r = ((numf)rand() * (1-0)) /(numf)RAND_MAX + 0;
        m.x2 = lim_inf + r*(lim_sup-lim_inf);
        m.fx = fitness(m.x1,m.x2);
        m.fit= fit(m.fx);
        m.cont = 0;
        calcular_probabilidad();
        cout<<m.id<<" ("<<m.x1<<", "<<m.x2<<") f(x):"<<m.fx<<" fit:"<<m.fit<<" p:"<<m.p<<" c:"<<m.cont<<endl;
    }
}
void generando_solucion(solucion& st,num i){
    bool t = false;
    valores_aleatorios(i);
    do{
        phi=((numf)rand() * (1-(-1))) /(numf)RAND_MAX + (-1);
        if(j == 1)
            st.x1 = vs[i].x1 + phi*(vs[i].x1 - vs[k].x1);
        else
            st.x2 = vs[i].x2 + phi*(vs[i].x2 - vs[k].x2);
    }while((st.x1 <lim_inf or st.x1>lim_sup) or (st.x2 <lim_inf or st.x2>lim_sup));
    st.fx = fitness(st.x1,st.x2);
    st.fit = fit(st.fx);
    if(st.fx > vs[i].fx){
        t=1;
        st.cont = 0;
        vs[i] = st;
        if(vs[i].fx > mj.fx) mj = vs[i];
        // mj = (st.fx > vs[it].fx)? st: vs[it];
    }else{
        t=0;
        st.cont++;
        vs[i].cont++;
        fuente_abandonada(vs[i]);
    }
    cout<<"k: "<<k<<" j:"<<j<<" phi:"<<phi<<" ("<<st.x1<<","<<st.x2<<") f(x):"<<st.fx<<" fit:"<<st.fit<<" Mejora?:"<<t<<" cont:"<<st.cont<<endl;
}
void soluciones_candidatas(){
    cout<<"---------------------Soluciones Candidatas---------------------"<<endl;
    bool t = false;
    Sumfit = 0;
    for (size_t i = 0; i < vs.size(); i++)
    {
        solucion st = vs[i]; st.p = 0;
        generando_solucion(st,i);
        // if(vs[i].fx > mj.fx) mj = vs[i];
    }
    cout<<"---------------------Mejores soluciones---------------------"<<endl;
    // calcular_probabilidad();
    mostrar_soluciones();
    cout<<"---------------------Probabilidad de Seleccion---------------------"<<endl;
    calcular_probabilidad();
    
}

num seleccionar_fuente(numf ale){
    numf l=0;
    for (num i = 0; i <vs.size(); i++){
        if(l >= ale or  ale<= l + vs[i].p)
            return i;
        l += vs[i].p;
    }

}

void abejas_observadoras(){
    numf ale;
    num it;
    Sumfit = 0;
    cout<<"==================================================\n";
    for (size_t i = 0; i <NS; i++){
        cout<<"abeja observadora "<<i<<endl;
        ale = ((numf)rand() * (1-0)) /(numf)RAND_MAX + 0;
        it = seleccionar_fuente(ale);
        cout<<"N.alt: "<<ale<<" fuente:"<<it<<endl;
        solucion st=vs[it]; st.p=0; st.cont=0;
        generando_solucion(st,it);
        // cout<<"*-------mostrando:------------\n";
        calcular_probabilidad();
        cout<<"-----------Solucion Mejorada---------------\n";
        mostrar_soluciones();
        mj = (vs[it].fx > mj.fx)? vs[it]: mj;
    }
    cout<<"==================================================\n";
}


int main(){
    srand(time(NULL));
    for(size_t it = 0; it <= MCN; it++){
        cout<<"***************************************iteracion "<<it<<"***********************************************************\n";
        if(vs.size()>0){
            mostrar_soluciones();
            // cout<<" Nuevas soluciones para abejas empleadas"<<endl;
            soluciones_candidatas();
            abejas_observadoras();
            cout<<"mejor: "<<mj.x1<<" "<<mj.x2<<" f(x):"<<mj.fx<<" fit:"<<mj.fit<<endl;

        }else{
            crear_poblacion();
            // mostrar_soluciones();
        }    
    }
}
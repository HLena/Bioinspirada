#ifndef LAB5_H
#define LAB5_H

#include "types.h"

bool order(indv &id1, indv &id2){
      return id1.fitness < id2.fitness;
}
void fitness(indv &);
void defineParameter(int tam, float c, float r, float m, int it){
      np = tam;
      pc = c;
      pr = r;
      pm = m;
      iterations = it;
}

void generateIndividuo(vector<int> &vec, int posi, int posf, int op){

      int pv = (posf+posi)/2;
      int x;
      if(posf-posi == 0 )
      {
            if(rand()% 11 >3 && op !=3 && op!=4){
                  x=20;
            }else{
                  x = (rand()% 11)-5;
                  // if(op == 4 && x == 0) // operador divisionx(/)
                        // do x = (rand()% 11)-5; while (x==0);
                  // if(op == 3 && x < 0) // operador divisionx(/)
                        // do x = (rand()% 11)-5; while (x<0);
            }
            vec[posi] = x;
            return;            
      }
      op = rand()% 5;
      vec[posi] = op;
      generateIndividuo(vec,posi+1,pv,op);
      generateIndividuo(vec,pv+1,posf,op);
}

string buildCadena(vector<int> &vec, int posi, int posf){
      int pv = (posf+posi)/2;
      if(posf-posi == 0)
            return (vec[posi]==20)? "x" :to_string(vec[posi]);
      return operators[vec[posi]]+buildCadena(vec,posi+1,pv)+buildCadena(vec,pv+1,posf);
      
}
float calculateValue(vector<int> &v, int i, int f,float &x)
{     
      int m = (i+f)/2;
      if(f-i == 0)
      {
            return (v[f]==20)? x : v[f];
      }
      switch (v[i])
      {
            case 0:
                  return calculateValue(v,i+1,m,x) + calculateValue(v,m+1,f,x);
            case 1:
                  return calculateValue(v,i+1,m,x) - calculateValue(v,m+1,f,x);
            case 2:
                  return calculateValue(v,i+1,m,x) * calculateValue(v,m+1,f,x);
            case 3:
                  return fmod(calculateValue(v,i+1,m,x),calculateValue(v,m+1,f,x));
            case 4:
                  return calculateValue(v,i+1,m,x) / calculateValue(v,m+1,f,x);
      }
            

}
void createPopulation(){
      indv s1;
      vector<int> v(7,0);
      int m=0;
      float x = 0;
      float val;
      while(m<np){

            generateIndividuo(v,0,6,-1);
            val = calculateValue(v,0,6,x);
            while(isnan(val) or isinf(val)){
                  generateIndividuo(v,0,6,-1);
                  val = calculateValue(v,0,6,x);
            }
            
            s1.values = v;
            s1.cadena = buildCadena(v,0,6);
            fitness(s1);
            pop.push_back(s1);
            m++;
      }
}

void printPop(){
      for(int i=0; i<pop.size();i++)
      {     
            file<<"[";
            for(size_t j=0; j<pop[i].values.size();j++){
                  file<<pop[i].values[j]<<" ";
            }
            file<<"] ("<<pop[i].cadena<<") f: "<<pop[i].fitness<<endl;
            // file<<"("+buildCadena(pop[i].values,0,6)+")"<<endl;
      }
}

void fitness(indv &v){
      float calc=0;
      float dif=0;
      float sum=0;
      float n = np;
      // file<<"operacion: "<<v.cadena<<endl;
      for(size_t j=0; j<n;j++){
            // file<<"entrada: "<<inputs[j]<<" salida: "<<outputs[j]<<endl;
            calc = calculateValue(v.values,0,6,inputs[j]);
            // file<<"calculo: "<<calc<<endl;
            dif =  outputs[j]-calc;
            // file<<"dif: "<<dif<<endl;
            sum += pow(dif,2);
            // file<<"======================"<<endl;
      }
      v.fitness = sum / n;
      // file<<"fitness: "<<v.fitness<<endl;

}

// void calculateFitness()
// {
//       int n = 10;
//       float sum=0;
//       float calc=0;
//       float dif=0;
//       for(size_t i=0; i<pop.size();i++)
//       {
//             file<<"*******************" <<i<<"*******************"<<endl;
//             file<<"ecuacion: "<<pop[i].cadena<<endl;
//             for(size_t j=0; j<n;j++){
//                   file<<"entrada: "<<inputs[j]<<" salida: "<<outputs[j]<<endl;
//                   calc = calculateValue(pop[i].values,0,6,inputs[j]);
//                   file<<"calculo: "<<calc<<endl;
//                   dif =  outputs[j]-calc;
//                   file<<"dif: "<<dif<<endl;
//                   sum += pow(dif,2);
//                   file<<"======================"<<endl;
//             }
//             sum = sum / n;
//             file<<"fitness "<<sum<<endl;
//             pop[i].fitness = sum;
//             sum =0; calc =0; dif = 0;
//       }
// }
indv ruleta(){
      int i = rand()%10;
      return pop[i];
}

int torneo(){
      int p1,p2;
      p1 = rand()%10;  
      do p2 = rand()%10; while(p1 == p2);
      return (pop[p1].fitness < pop[p2].fitness)? p1: p2;
      
}

void reproduction(){
      file<<" <El mismo individuo> "<<endl;
}

void mutation(indv &i){
      int which_pos = rand()%3;
      int which_sig;
      float val;
      float x = 0;
      if (which_pos==2) which_pos = 4;
      do{
            which_sig= rand()%5;
            i.values[which_pos]=which_sig;
            val = calculateValue(i.values,0,6,x);
      }while(isnan(val) or isinf(val));
      i.cadena = buildCadena(i.values,0,6);
      fitness(i);

}
void crossOver(){
      indv h1,h2;
      int p1,p2;
      float valh1, valh2, x=0;
      do{
            file<<"*"<<endl;
            p1 = torneo();
            do p2=torneo(); while (pop[p1].cadena== pop[p2].cadena);
            h1 = pop[p1];
            h2 = pop[p2];
            h1.values[4]=pop[p2].values[4];
            h1.values[5]=pop[p2].values[5];
            h1.values[6]=pop[p2].values[6];

            h2.values[4]=pop[p1].values[4];
            h2.values[5]=pop[p1].values[5];
            h2.values[6]=pop[p1].values[6];
            valh1 = calculateValue(h1.values,0,6,x);
            valh2 = calculateValue(h2.values,0,6,x);
      }while(isnan(valh1) or isnan(valh2));
      h1.cadena =  buildCadena(h1.values,0,6);
      h2.cadena =  buildCadena(h2.values,0,6);
      fitness(h1);
      fitness(h2);
      file<<"p1: ("<<pop[p1].cadena<<" ) p2: ("<<pop[p2].cadena<<")"<<endl;
      file<<"h1: ("<<h1.cadena<<" ) h2: ("<<h2.cadena<<")"<<endl;
      pop.push_back(h1);
      pop.push_back(h2);


}

void selecionToOperation(){
      for(int i = 0; i < np/2; i++){
            file <<"================ "<< i<<" =============="<<endl;
            int pro = rand()%101;
            int p1,p2;
            file<<"prob: "<<pro<<endl;
            if( pro< pr) //hay repoduccion
            {
                  file<<"Reproduccion: ["<<0<<"-"<<pr<<"] "<<pro<<endl;
                  reproduction();
                  p1 = torneo();
                  file<<pop[p1].cadena<<endl;
            }else if(pro >pr && pro<pc){
                  file<<"Cruce: ["<<pr+1<<"-"<<pc+pr<<"] "<<pro<<endl;
                  crossOver();
            }else{
                  file<<"mutacion: ["<<pc+pr+1<<"-"<<pc+pr+pm<<"] "<<pro<<endl;
                  p1 = torneo();
                  file<<pop[p1].cadena<<" "<<pop[p1].fitness<<endl;
                  mutation(pop[p1]);
                  file<<pop[p1].cadena<<endl;
            }
      }
      file<<"=========== Nueva Pop ============"<<endl;;
      printPop();
      file<<"=========== Pop seleccionada ============"<<endl;
      sort(pop.begin(),pop.end(),order);
      while(pop.size() > np)
                pop.pop_back();
      printPop();
}

void run(){
      defineParameter(10,70,20,10,1000);
      file <<"=========poblacion inicial========="<<endl;
      createPopulation();
      printPop();
      for(size_t i=0; i<iterations;i++){
      file <<"============== iteracion "<<i<<" ================"<<endl;
            selecionToOperation();
      }
      

}



#endif
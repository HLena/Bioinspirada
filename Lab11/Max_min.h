#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

// int numCities;
// int city;

void PrintMatriz(vector<float>& m, int numCities){
    for(size_t i=0; i < numCities; i++){
        for (size_t j = 0; j < numCities; j++)
            cout << m[i*numCities+j]<<" ";
        cout<<endl;
    }
}

void BuiltTSP(int n, int &i, int &j,string v, vector<float> &cities){
    // cout<<v<<" i: "<<i<<" j: "<<j<<endl;
    cities[i*n+j]= atof(v.c_str());
    cities[j*n+i]= atof(v.c_str());
    if( j == n-1){
        i++;
        j = i+1;
    }else
        j++;
}

vector<float> ReadTSP(string filename, int numCities){
    int i,j;
    ifstream readfile;
    readfile.open(filename);
    string line;
    getline(readfile, line);
    numCities = atoi(line.c_str());
    vector<float> cities(numCities*numCities,0);
    if(readfile.is_open()){
        i = 0;
        j = 1;
        while(getline(readfile, line)){
            BuiltTSP(numCities,i,j,line,cities);

        }
    }else{
        cout<<"No se pudo abrir archivo\n";
    }
    readfile.close();
    // PrintMatriz(cities,numCities);
    return cities;
}


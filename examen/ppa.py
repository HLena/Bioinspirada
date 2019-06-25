import math
import random
import numpy as np
import copy

dimensions = 2
l_inf = -3
l_sup = 3
pop = []
phi = []
n_max = 5

class plant:
    def __init__(self):
        self.x = []
        self.f = 0
        self.N = 0



def ShowIndv(i):
    print("V{} fit:{} N:{}".format(i.x,i.f,i.N))

def ShowPopulation(l):
    for i in l:
        ShowIndv(i)


def distance(p):
    r = random.uniform(0,1)
    return 2*(1-p.N)*(r-0.5)

def function(x1,x2):
    # return pow(x1,2) + pow(x2,2)
    return (4-2.1*pow(x1,2)+(pow(x1,4)/3))*pow(x1,2)+x1*x2+(-4+4*pow(x2,2))*pow(x2,2)
    # return -math.cos(x1)*math.cos(x2)*math.exp(-math.pow(x1-math.pi,2)-pow(x2-math.pi,2))
    # return x1+x2


def XVector(dim):
    x = []
    for d in range(dim):
        x.append(random.uniform(l_inf,l_sup))
    return x

def GetN(fx):
    return (0.5)*(np.tanh(4*fx-2)+1)

def GeneratePopulation(m,pop,dims):
    for g in range(m):
        plt = plant()
        plt.x = XVector(dims)
        plt.f= function(plt.x[0],plt.x[1])
        plt.N = GetN(plt.f)
        pop.append(plt)

def union(l,ind):
    for i in l:
        if(i.x != ind.x):
            l.append(ind)
            break




def PlantPropagationAlg(g_max,m,pop):
    print("-------Poblacion inicial-----")
    GeneratePopulation(m,pop,dimensions)
    # ShowPopulation(pop)
    for gen in range(g_max):
        pop.sort(key=lambda x: x.N)
        ShowPopulation(pop)
        print("--------ITERACION {}-----------".format(gen))
        phi = copy.deepcopy(pop)
        nid = 0
        for p in pop:
            print("individuo {}".format(nid))
            nid += 1
            ShowIndv(p)
            r = random.uniform(0,1)
            nr = math.ceil((n_max*p.N*r)+0.1)
            print("nr:{} (numero de corredores)".format(nr))
            for t in range(nr):
                print("\tCorredor ",t)
                ind = plant()
                for j in range(dimensions):                    
                    d = distance(p)
                    print("\t\tdistancia: {}".format(d))
                    x_j =p.x[j] + (l_sup - l_inf)*d                    
                    if( x_j <l_inf or x_j >l_sup):
                        r = random.uniform(0, 1)
                        x_j = l_inf + r*(l_sup - l_inf)                    
                    ind.x.append(x_j)
                ind.f = function(ind.x[0],ind.x[1])
                ind.N = GetN(ind.f)                
                print("\t\t",end="")
                ShowIndv(ind)
                union(phi,ind)
        phi.sort(key=lambda x: x.N)        
        print("------FINAL DE ITERACION--------")
        del phi[m:]
        # ShowPopulation(phi)
        pop =copy.deepcopy(phi)
    print("Mejor de la poblacion")
    ShowIndv(pop[0])

PlantPropagationAlg(100,5,pop)
        
                    

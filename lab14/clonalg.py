import copy 
import random 
import math
import json
import numpy as np
from operator import attrgetter

# global variables
limInf = -10
limSup = 10
stopCond = 100
n_population = 4
n_selection = 4
mutationF = -2.5
maxf = 0
minf = 0
n_binary = 16


class indv:
    costo = 0
    x1 = 0
    x2 = 0
    affinity = 0
    Sbinary = []

def GenerateX(sum):
    return limInf + ((limSup-limInf)/(pow(2.0,n_binary/2)-1.0))*sum

def Funtion(x1,x2):
    return -math.cos(x1)*math.cos(x2)*math.exp(-math.pow(x1-math.pi,2)-pow(x2-math.pi,2))


def ShowIndv(i):
    m = {'x1':i.x1,'x2':i.x2,'str':i.Sbinary,'f':i.costo,'aff':i.affinity}
    print(json.dumps(m, sort_keys=True))

def ShowPopulation(pop):
    for i in pop:
        ShowIndv(i)

def GenerateCell():
    ind = indv()
    b_list = list(np.random.choice([0, 1], size=(n_binary,)))
    ind.Sbinary = ''.join(str(s) for s in b_list)
    ind.x1 = GenerateX(int(ind.Sbinary[:8],2))
    ind.x2 = GenerateX(int(ind.Sbinary[8:],2))
    ind.costo = Funtion(ind.x1,ind.x2)
    return ind

def Affinity(f):
    return 1.0 -(f/(maxf.costo-minf.costo))


def CreateRandomCells():
    vpop = []    
    print("-------Creando poblacion-----------")
    for t in range(0,n_population):
        vpop.append(GenerateCell())
    global minf, maxf
    minf = min(vpop,key=attrgetter('costo'))
    maxf = max(vpop,key=attrgetter('costo'))
    return vpop


def SelectPopulation(pop, n):
        if(len(pop) != n):
                pop.sort(key=lambda x: x.costo)
                m = len(pop)-n
                del pop[-m:]
                return pop
        return pop

def InsertAlet(pop,randomCells):
        print("Insercion Aleatorio")
        for i in range(randomCells):
                tmp = indv()
                tmp = GenerateCell()
                ShowIndv(tmp)
                pop.append(tmp)

def Hypermutate(clone,muta_rate):
        l = list(clone.Sbinary)
        for i in range(len(l)):
                rand = random.uniform(0,1)
                if(rand < muta_rate):
                        # print(rand,muta_rate)
                        if(l[i]=='1'):
                                l[i]='0'
                        else:
                                l[i]='1'
        clone.Sbinary = ''.join(l)
        clone.x1 = GenerateX(int(clone.Sbinary[:8],2))
        clone.x2 = GenerateX(int(clone.Sbinary[8:],2))
        clone.costo = Funtion(clone.x1,clone.x2)
        return clone

def clone(tmp, n, pop_clones):
        muta_rate = math.exp(mutationF*tmp.affinity)
        for i in range(n):
                clone = indv()
                clone = copy.deepcopy(tmp)
                clone.affinity = 0.0
                # l = list(clone.Sbinary)
                clone = Hypermutate(clone,muta_rate)
                pop_clones.append(clone)
        pop_clones.sort(key=lambda x: x.costo)


# def GenerateMutation(muta_rate, clone):
        # n_clones = n_population * clone_rate
        # print("clones: ",n_clones)
        # for n in range(0,n_population):                
        #         Muta_rate = math.exp(mutationF*vpop[n].affinity)
        #         for m in range(0,int(n_clones)):
        #                 c1 = indv()
        #                 c1 = copy.deepcopy(vpop[n])
        #                 c1.affinity = 0
        #                 l = list(c1.Sbinary)
        #                 for i in range(0,len(l)):
        #                         r1 = random.uniform(0,1)
        #                         if(r1 < Muta_rate):
        #                                 if(l[i]=='1'):
        #                                         l[i]='0'
        #                                 else:
        #                                         l[i]='1'
        #                 c1.Sbinary = "".join(l)
        #                 # print(int(c1.Sbinary[:8],2),int(c1.Sbinary[8:],2))
        #                 c1.x1 = GenerateX(int(c1.Sbinary[:8],2))
        #                 c1.x2 = GenerateX(int(c1.Sbinary[8:],2))
        #                 # print(c1.x1,c1.x2)
        #                 c1.costo = Funtion(c1.x1,c1.x2)
        #                 vpop.append(c1)
        #                 ShowIndv(c1)

def main():
        pop = []
        # pop_select = []
        select_size = 4
        clone_rate = 0.25
        randomCells = 2
        pop = CreateRandomCells()
        pop.sort(key=lambda x: x.costo)
        # ShowPopulation(pop)
        for i in range(1,stopCond+1):
                for n in pop:
                        n.affinity = Affinity(n.costo)
                ShowPopulation(pop)
                pop_select = []
                pop_select = SelectPopulation(pop,select_size)
                pop_clones = []
                cant_clones = int(n_population * clone_rate)
                print("numero de clones:",cant_clones)
                for tmp in pop_select:
                        clone(tmp,cant_clones, pop_clones)
                ShowPopulation(pop_clones)
                pop.extend(pop_clones)
                pop = SelectPopulation(pop,select_size)
                print("Poblacion")
                ShowPopulation(pop)
                InsertAlet(pop,randomCells)
                SelectPopulation(pop,select_size)
                print("Poblacion")
                ShowPopulation(pop)
                print("Generacion: ",i)
                ShowIndv(pop[0])
                print("-----------------------------------------------")


main()
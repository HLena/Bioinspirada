import copy 
import numpy as np
import random 
import math
import json
from operator import attrgetter

# global variables
limInf = -10
limSup = 10
stopCond = 100
n_population = 5
problem_size= 2
n_clones= 5
n_rand = 2
affinity_th = 0.5
beta = 100
average_cost = 0
pop = []
# subpop= []
amax = 0
amin = 0

class indv:
    costo = 0.0
    norm_costo = 0.0
    x1 = 0.0
    x2 = 0.0
    aff = 0.0

Sbest = indv()

def Funtion(x1,x2):
    return -math.cos(x1)*math.cos(x2)*math.exp(-math.pow(x1-math.pi,2)-pow(x2-math.pi,2))


def ShowIndv(i):
    m = {'vector':[i.x1,i.x2],'costo':i.costo,'norm_costo':i.norm_costo,'aff':i.aff}
    print(json.dumps(m))

def ShowPopulation(l):
    for i in l:
        ShowIndv(i)

def GenerateIndv():
    ind = indv()
    ind.x1 = random.uniform(limInf,limSup)
    ind.x2 = random.uniform(limInf,limSup)
    ind.costo = Funtion(ind.x1,ind.x2)
    return ind




def InitializePopulation():
    print("-------Creando poblacion-----------")
    for t in range(0,n_population):
        pop.append(GenerateIndv())


def NormalizeFitPopulation():
        global average_cost
        global amax,amin
        # print("------------------------")
        amin = min(pop,key=attrgetter('costo'))
        # ShowIndv(amin)
        amax = max(pop,key=attrgetter('costo'))
        # ShowIndv(amax)
        for tmp in pop:
                tmp.norm_costo = 1.0-(tmp.costo/(amax.costo-amin.costo))
        print("Normalizar el costo")
        ShowPopulation(pop)
        global Sbest 
        Sbest = amin

def CalculateAverage(l):
        sum = 0.0
        for i in l:
                sum += i.costo
        return sum/len(l)

def alpha(f):
        return (1.0/beta)*math.exp(-f)

def CreateClones(tmp):
        clones = []
        for i in range(n_clones):
                # clones.append(copy.deepcopy(tmp))
                clones.append(indv())
        return clones


def MutateOfParent(clone,cell):
        clone.x1 = cell.x1 + alpha(cell.norm_costo) * np.random.normal(1.0)
        clone.x2 = cell.x2 + alpha(cell.norm_costo) * np.random.normal(1.0)
        clone.costo = Funtion(clone.x1, clone.x2)

def InsertAlet():
        print("Insercion Aleatorio")
        for i in range(0,n_rand):
                tmp = indv()
                tmp = GenerateIndv()
                # ShowIndv(tmp)
                pop.append(tmp)

def distance(c1,c2):
        sum = pow(c1.x1-c2.x1,2) + pow(c1.x2-c2.x2,2)
        return math.sqrt(sum)

def GetNeighborhood(t):
        neighbors = []
        for i in pop:
                if i != t and distance(i,t)<affinity_th:
                        neighbors.append(i)
        return neighbors

def Affinity(f):
    return 1.0 -(f/(amax.costo-amin.costo))

def affinitySupress(pop):
        subpop= []
        for cell in pop:
                cell.aff = Affinity(cell.costo)
                if(cell.aff > affinity_th):
                        subpop.append(cell)
        return subpop

def affinitySupress_(pop):
        subpop= []
        for cell in pop:
                neighbors = GetNeighborhood(cell)
                neighbors.sort(key=lambda x: x.costo)
                if not neighbors or cell == neighbors[0]:
                        subpop.append(cell)
        return subpop


def main():
        global average_cost
        global pop
        InitializePopulation()
        for i in range(1,stopCond+1):
                new_average = 0
                print('Iteracion ',i)
                ShowPopulation(pop)
                NormalizeFitPopulation()
                print("Obtener el mejor de la poblacion")
                ShowIndv(Sbest)
                progeny = []
                if not progeny:
                        print("Descendencia igual a null")
                cost_avg = CalculateAverage(pop)
                print("Calcular el promedio de costo")
                print(cost_avg)
                while True :
                        for cell in pop:
                                clones = []
                                clones = CreateClones(cell)
                                print("Clones")
                                ShowPopulation(clones)
                                for clone in clones:
                                        MutateOfParent(clone,cell)
                                clones.sort(key=lambda x: x.costo)
                                ShowPopulation(clones)
                                progeny.append(clones[0])
                        pro_avg = CalculateAverage(progeny)
                        if (pro_avg < cost_avg):
                                break
                        else: 
                                cost_avg = pro_avg
                print({'Promedio de clones':pro_avg})
                pop = progeny
                pop.sort(key=lambda x: x.costo)
                print('Clones Finales o Nueva Poblacion')
                ShowPopulation(pop)
                amin = min(pop,key=attrgetter('costo'))
                amax = max(pop,key=attrgetter('costo'))
                print('Nuevo promedio')
                cost_avg = pro_avg
                print(cost_avg)
                print('Poblacion despues del supresor de afinidad ')
                # pop = affinitySupress(pop)
                pop = affinitySupress_(pop)
                ShowPopulation(pop)
                print("Insertar Individuos Aleatorios")
                InsertAlet()
                ShowPopulation(pop)
                s = {'Generacion':i,'tam_poblacion':len(pop),'mejor_costo':Sbest.costo}
                print(s)

main()
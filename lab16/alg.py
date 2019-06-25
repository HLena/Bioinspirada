#!/usr/bin/env python
# coding: utf-8

# In[ ]:


import copy 
import numpy as np
import random 
import math
import json
from operator import attrgetter



class indv:
    fitness = 0.0
    inter = 0.0
    cost= 0.0
    health = 0.0
    norm_cost = 0.0
    x = []

lim_inf = -5
lim_sup = 5


def ShowIndv(i):
    if i.health == 0:
        m = {'vector':i.x,'costo':i.cost,'inter':i.inter,'fitness':i.fitness}
    else: 
        m = {'vector':i.x,'costo':i.cost,'inter':i.inter,'fitness':i.fitness,'sum_nut':i.health}
    print(json.dumps(m))

def ShowPopulation(l):
    for i in l:
        ShowIndv(i)


def Funtion(x1,x2):
    return math.pow(x1,2)+math.pow(x2,2)

def CreateStep(problem_size,search_space):
    x = []
    for j in range(problem_size):
        # print("j",j)
        x.append(random.uniform(search_space[0],search_space[1]))
    return x

def InitializePopulation(cells_num,problem_size,search_space):
    pop = []
    for i in range(cells_num):
        new_ind = indv()
        new_ind.x = CreateStep(problem_size,search_space)
        pop.append(new_ind)
    return pop

def Interaction(cell, pop,d_at,w_at,h_r,w_r):
    sum_s1 = 0
    sum_s2 = 0
    for other in pop:
        sum_p = 0
        for i in range(len(cell.x)):
            sum_p += math.pow(cell.x[i]-other.x[i],2)
        sum_s1 += -d_at*math.exp(-w_at*sum_p)
        sum_s2 += h_r * math.exp(-w_r*sum_p)
    return sum_s1 + sum_s2

def takeStep(random_cell,cell,step_size,search_space):
    x = copy.deepcopy(cell.x)
    for i in range(len(x)):
        x[i] = cell.x[i] + step_size * random_cell[i]
        if x[i] < search_space[0] : x[i] = search_space[0]
        if x[i] > search_space[1] : x[i] = search_space[1]
    return x

def evaluate(c,population,d_at,w_at,h_r,w_r):
    # ShowIndv(c)
    c.cost = Funtion(c.x[0],c.x[1])
    c.inter = Interaction(c,population,d_at,w_at,h_r,w_r)
    c.fitness = c.cost + c.inter


        
def ChemoTaxisSwim(population,problem_size,cells_num,Ns,step_size,d_at,w_at,h_r,w_r,search_space):
    # print("*")
    # ShowPopulation(population)
    for c in range(len(population)):
        sum_nutriente = 0.0
        evaluate(population[c],population,d_at,w_at,h_r,w_r)
        # population[c].health += population[c].fitness
        sum_nutriente += population[c].fitness
        # print("*")
        ShowIndv(population[c])
        new_cell = indv()
        for i in range(Ns):
            print("Swim ",i)
            randStepDirec = CreateStep(problem_size,[-1.0,1.0])
            new_cell.x= takeStep(randStepDirec,population[c],step_size,search_space)
            evaluate(new_cell,population,d_at,w_at,h_r,w_r)
            ShowIndv(new_cell)
            if(new_cell.fitness > population[c].fitness):
                break
            population[c] = copy.deepcopy(new_cell)
            sum_nutriente += new_cell.fitness
        population[c].health += sum_nutriente
        print("End-Swim")


def selecteByCellHealth(pop,n):

    # print(n,len(pop))
    del pop[int(n):]
    return pop






def main():
    problem_size = 2
    search_space = [-5,5]
    cells_num = 4
    Ned = 2
    Nre = 2
    Nc = 20
    Ns = 5
    step_size = 0.1
    d_attr = 0.1
    w_attr = 0.2
    h_rep = d_attr
    w_rep = 10
    Ped = 0.25
    CellBest = indv()
    population = InitializePopulation(cells_num,problem_size,search_space)
    print("Poblacion Inicial")
    ShowPopulation(population)
    for l in range(Ned):
        # break
        print("Ned:",l)
        for k in range(Nre):
            print("Nre:",k)
            for j in range(Nc):
                ChemoTaxisSwim(population,problem_size,cells_num,Ns,step_size,d_attr,w_attr,h_rep,w_rep,search_space)
                for cell in population:
                    if cell.cost <= CellBest.cost or CellBest.cost == 0:
                        CellBest = copy.deepcopy(cell)
                chemo = {'Chemo':j,'f':CellBest.fitness,'cost':CellBest.cost}
                print("-------------------",chemo)
            print("Best-fitness:",CellBest.fitness,CellBest.cost)
            print("Celulas antes de la Reproduccion")
            ShowPopulation(population)
            population.sort(key=lambda x: x.health)
            selected = selecteByCellHealth(population,cells_num/2)
            population = selected + selected
            print("Celulas despues de la Reproduccion")
            ShowPopulation(population)
            print("End-Nre")
        print("Celulas antes de la Eliminacion")
        ShowPopulation(population)
        for cell in population:
            if random.uniform(0.0,1.0) <= Ped:
                cell = CreateStep(problem_size,search_space)
        print("Celulas despues de la Eliminacion")
        ShowPopulation(population)
        print("End-Ned")
    print("Solucion Final")
    ShowIndv(CellBest)

        
main()


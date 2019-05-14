# import pathlib
import numpy as np
import scipy.stats as st
import random
import math
import copy as cp


poblacion = 10
desviacion = 0.3
iteraciones = 100
lim_inf = -10
lim_sup = 10
alfa = 2


individuos=[]
hijos=[]



file = open("lab7.txt","w")

class indv:
      x1=0
      x2=0
      d1=0
      d2=0
      fitness = 0

def ordenar( x, y ) :
    return  x.fitness > y.fitness

    
def funcion(x1, x2):
	return -math.cos(x1)*math.cos(x2)*math.exp(-math.pow(x1-math.pi,2)-pow(x2-math.pi,2))
	# return x1 - (x1/3)

def mostrarPoblacion(a):
      i=0
      if(a==1):
            file.write("----------P.INDIVIDUOS-----------\n")
            for x in individuos:
                  file.write(str(i)+"("+str(x.x1)+","+str(x.x2)+")f:"+str(x.fitness)+"  d1: "+str(x.d1)+" d2:"+str(x.d2)+"\n")
                  i+=1
      else:
            file.write("-----------P.HIJOS-----------\n")
            for x in hijos:
                  file.write(str(i)+"(" +str(x.x1)+","+str(x.x2)+")f:"+str(x.fitness)+"  d1: "+str(x.d1)+" d2:"+str(x.d2)+"\n")
                  i+=1

def metodoTorneo():
		p1 = random.randint(0,poblacion-1)
		p2 = random.randint(0,poblacion-1)
		while p1 == p2:
			p2 = random.randint(0,poblacion-1)
		if (individuos[p1].fitness < individuos[p2].fitness):
			return p1
		return p2;

def crearPoblacion():
      file.write("***CREANDO POBLACION INDIVIDUOS***\n")
      for x in xrange(0,poblacion):
            s1 = indv()
            s1.x2 = round(random.uniform(lim_inf,lim_sup),5)
            s1.x1 = round(random.uniform(lim_inf,lim_sup),5)
            s1.d1 = s1.d2 = 0.3
            s1.fitness = funcion(s1.x1, s1.x2)
            individuos.append(s1)
            # file.write("len_indi: "+str(len(individuos))+"\n")
      # file.write("Poblacion llena\n")

def evaluarPoblacion(opcion):
      if(opcion == 1):
            file.write("-ORDENANDO INDIVIDUOS-\n")
            individuos.sort(key=lambda x: x.fitness)
      else:
            file.write("-ORDENANDO HIJOS-\n")
            hijos.sort(key=lambda x: x.fitness)

def seleccionarPadres():
      p1 = metodoTorneo();
      file.write("PADRE ELEGIDO:"+str(p1)+"\n")
      return individuos[p1];

def mutacion(p):
      h = indv()
      h = cp.copy(p)
      # file.write("hijo es: ("+str(h.x1)+","+str(h.x2)+") ("+str(h.d1)+","+str(h.d2)+") fit:("+str(h.fitness)+")\n")
      file.write("***MUTACION***\n")
      pase = True
      while(pase == True or ((h.x1 < lim_inf and h.x1>lim_sup) and (h.x2<lim_inf and h.x2>lim_sup))):
            pase = False
            h.d1 = round(h.d1*(1 + alfa*np.random.normal(0.0,1)),5)       
            h.d2 = round(h.d2*(1 + alfa*np.random.normal(0.0,1)),5)
            h.x1 = round(h.x1 + h.d1* np.random.normal(0.0, 1),5)
            h.x2 = round(h.x2 + h.d2* np.random.normal(0.0, 1),5)
      h.fitness = funcion(h.x1,h.x2)
      file.write("hijo: ("+str(h.x1)+","+str(h.x2)+") ("+str(h.d1)+","+str(h.d2)+") fit:("+str(h.fitness)+")\n")
      file.write("---------------------------------------------------------------\n")
      hijos.append(h)

def procrearOffspring():
      file.write("***PROCREANDO OFFSPRING***\n")
      p1 = indv()
      while(len(hijos) < len(individuos)):
            p1 = seleccionarPadres()
            mutacion(p1)
            # hijos.append(h1)
      evaluarPoblacion(0)
      mostrarPoblacion(1)
      mostrarPoblacion(0)

def seleccionarSobrevivientes():
      # mostrarPoblacion(1)
      # mostrarPoblacion(0)
      file.write( "================SELECCION DE SOBREVIVIENTES======================\n")
      file.write( "INDIVIDUOS(MEJORES) + HIJOS(MEJORES)\n")
      u = (len(individuos)/2)    
      for x in xrange(0, u):
            individuos[x+u] = hijos[x]
      del hijos[:]
      file.write("hijos:"+str(len(hijos))+"\n")


def main():
      for x in xrange(0,iteraciones):
            file.write( "================ITERACION"+str(x)+"======================\n")
            if(len(individuos) > 0):
                  evaluarPoblacion(1)
                  mostrarPoblacion(1)
                  procrearOffspring()
                  seleccionarSobrevivientes()
                  # mostrarPoblacion(1)
            else:
                  crearPoblacion()
            # file.write("len_indi: "+str(len(individuos))+"\n")
      file.close()

main()






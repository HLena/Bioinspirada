
import numpy as np
import scipy.stats as st
import random
import math


poblacion = 0
desviacion = 0.3
iteraciones = 2000
lamda = 1
# lamda = 1
# lamda = 1

namesfile = ["u+1.txt","u+y.txt","u_y.txt"]
pop=[]
id = 0
file = open(namesfile[id],"w")
# file = open("u+y.txt","w")
# file = open("u_y.txt","w")

def ordenar( x, y ) :
    return  x.fitness > y.fitness

class indv:
	fitness = 0
	x1 = 0
	x2 = 0
	porcent = 0
	o1=0.3
	o2=0.3
    
def funcion(x1, x2):
	return -math.cos(x1)*math.cos(x2)*math.exp(-math.pow(x1-math.pi,2)-pow(x2-math.pi,2))
	# return x1 - (x1/3)

def mostrarPoblacion():
	file.write( "----------Poblacion-------------\n")
	for x in pop:
		cadena = "(" +str(x.x1 )+ "," +str( x.x2) +")f:"+str(x.fitness)+" o1: "+str(x.o1)+" o2:"+str(x.o2)
		file.write(cadena+"\n")

def metodoTorneo():
		# file.write( "torneo")
		# global poblacion
		p1 = random.randint(0,poblacion-1)
		p2 = random.randint(0,poblacion-1)
		# file.write( str(p1)+"-"+str(p2)+"\n")
		while p1 == p2:
			p2 = random.randint(0,poblacion-1)
		# file.write( "torneo2s")
		if (pop[p1].fitness < pop[p2].fitness):
			return p1
		return p2;

def operadorSeleccion():
	for x in xrange(0,lamda):
		pop.pop()

def crearPoblacion():
	# poblacion = random.randint(8,15)
	for x in xrange(0,poblacion):
		s1 = indv()
		s1.x2 =  round(random.uniform(-10,10),5)
		s1.x1 =  round(random.uniform(-10,10),5)
		s1.fitness = funcion(s1.x1, s1.x2)
		pop.append(s1)
	pop.sort(key=lambda x: x.fitness)

def adaptacionOperador(o):
	do = 1/math.sqrt(2*math.sqrt(poblacion));
	return o * math.exp(np.random.normal(0,do))
	

def mutacion(h):
	file.write("----------mutacion-----------\n")
	h.o1 = round(adaptacionOperador(h.o1),5)
	h.x1 = h.x1 + np.random.normal(0.0, h.o1)
	h.o2 = round(adaptacionOperador(h.o2),5)
	h.x2 = h.x2 + np.random.normal(0.0, h.o2)
	file.write("hijo: ("+str(h.x1)+","+str(h.x2)+") ("+str(h.o1)+","+str(h.o2)+")\n")
	pop.append(h)

def cruzamiento(a,b):
	file.write("----------cruzamiento-----------\n")
	h1 = indv()	
	h1.x1 = 0.5 * (a.x1+b.x1)
	h1.x2 = 0.5 * (a.x2+b.x2)
	h1.o1 =round(math.sqrt(a.o1 + b.o1),5)
	h1.o2 =round(math.sqrt(a.o2 + b.o2),5)
	h1.fitness = funcion(h1.x1, h1.x2)
	file.write("hijo: ("+str(h1.x1)+","+str(h1.x2)+") ("+str(h1.o1)+","+str(h1.o2)+")\n")
	mutacion(h1)

def seleccionarIndividuos():
	for x in xrange(0,lamda):
		file.write( "De: "+str(len(pop))+" individuos\n")		
		p1 = metodoTorneo()
		p2 = metodoTorneo()
		while p1==p2:
			p2 = metodoTorneo()
		file.write( "Seleccionando individuos\n")
		file.write( "p1: "+str(p1)+"\n")
		file.write( "p2: "+str(p2)+"\n")
		cruzamiento(pop[p1],pop[p2])
	# file.write( "oooooooooooooo1"
def menu():
	global lamda,poblacion,file
	print "Eliga una opcion"
	print "1) (u+1)"
	print "2) (u+y)"
	print "3) (uy)"
	numero = int(input("Introduce un numero: "))
	if(numero == 1):
		lamda = 1
		poblacion = 10
		file = open(namesfile[0],"w")
	elif(numero == 2):
		lamda = random.randint(5,10)
		poblacion = random.randint(lamda+1,15)
		file = open(namesfile[1],"w")
	else:
		lamda = random.randint(5,15)
		poblacion = random.randint(5,lamda)
		file = open(namesfile[2],"w")
	file.write("MU: "+str(poblacion)+" LAMBDA: "+ str(lamda)+"\n")


def main():

	menu()
	crearPoblacion()
	mostrarPoblacion()
	
	for x in xrange(1,iteraciones):
		file.write( "================Iteracion"+str(x)+"======================\n")
		file.write("desviacion: "+str(desviacion)+"\n")
		seleccionarIndividuos()
		pop.sort(key=lambda x: x.fitness)
		operadorSeleccion()
		mostrarPoblacion()
	file.close()



main()






// #include "p_evolutiva.h"
// #include "p_geneticaT.h"
#include "p_geneticaR.h"

int main(int argc, char const *argv[])
{
	srand(time(NULL));
	//---------LABORATORIO 5---------
    //reproduccion 10%
    //mutacion 20%
    pgenetica p(10,70,10,20,7,1000);
    p.inicio();

    //---------LABORATORIO 6---------
   	// p_evolutiva p1(100,1,-10,10,0.3);
    // p1.inicio();
    // return 0;
    
}

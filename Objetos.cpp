/********************************************
Implementación de Objetos Gráficos y de la
intersección con un rayo(origen,direccion)

A completar por el alumno

R.Vivo' 2013
********************************************/

#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include "Objetos.h"


//
// Clase Objeto -------------------------

Objeto::Objeto(void)
{
	colDifuso = Color::ROJO;
	colEspecular = Color::BLANCO;
	colEmisivo = Color::NEGRO;
	m = 1;
	id = -1;	// Identificador de objeto (no identificado)
}
void Objeto::setColor(Color cd,Color ce,int em,Color emi)
{// Fija las caracteristicas del material
	colDifuso = cd;
	colEspecular = ce;
	colEmisivo = emi;
	m = em;
}

bool Objeto::rayIntersection(Punto p, Vector v, Intersection &its) const
{// Virtual. Deberia ejecutarse siempre el derivado
	its.objectid = id;
	its.type = Intersection::NONE;
	return false;
}


//
// Clase Esfera -------------------------

Esfera::Esfera()
{// Construye la esfera unidad por defecto
	centro = Punto();
	radio = 1.0f;
}

Esfera::Esfera(Punto c,float r)
{
	centro = c;
	radio = r;
}

bool Esfera::rayIntersection(Punto p, Vector v, Intersection &its) const
// Calcula la interseccion del rayo con la esfera 
// No se considera el origen del rayo interior a la esfera, por lo que
// la interseccion o es NONE o EXTERIOR.
// ToDo: Modificar para traslucencia
{
	float alfa,beta,gamma;
	float raiz;
	float t1,t2;
	Vector pcentro = p-centro;
	
	// El primer punto, en el caso que haya 2, será el más cercano al inicio del rayo
	
	alfa = v*v;
	beta = 2*(pcentro*v);
	gamma = (pcentro*pcentro) - radio*radio;

	raiz = beta*beta-4*alfa*gamma;

	its.objectid = id;

	if ( raiz<0.0f ) {				// Raíces imaginarias
		its.type = Intersection::NONE;
		return false;
	}

	raiz = (float)sqrt(raiz);
	t2 = (-beta+raiz)/(2*alfa);
	t1 = (-beta-raiz)/(2*alfa);
	// Debido a que alfa>0, t1<t2

	if( t1 <= 0 ) {					// No hay intersecciones exteriores despues del origen del rayo
		its.type = Intersection::NONE;
		return false;
	}

	its.type = Intersection::EXTERIOR;
	its.distance = t1;
	its.point = p+v*t1;
	its.normal = (its.point-centro).unitary(); 
	
	return true;
}


//
//Clase Poliedro ----------------------

//ToDo: Implementar. Para rayIntersection usar el metodo de Haines

bool Poliedro::rayIntersection(Punto p, Vector v, Intersection &its) const{

	float te = 0;
	float ts = INFINITO;

	Vector normal;
	
	for(int i=0; i<ncaras; i++){
		
		Vector v1 = cara[i]->vertex(1) - cara[i]->vertex(0);
		Vector v2 = cara[i]->vertex(2) - cara[i]->vertex(0);
		Vector n = (v1^v2).unitary();

		Punto o = cara[i]->vertex(0);
		
		float rn = v * n;					// Calculo de r x n
		its.distance = (n*(o-p))/rn;		// Calculo de t

		if(rn == 0){						// r y n son perpendiculares
			if(((p-o)*n) > 0)				// El punto p es exterior
				return false;
		}else if(rn < 0){					// El producto de r x n es entrante
			te = max(te,its.distance);
			if(te == its.distance)
				normal = n;
		}else{								// El producto de r x n es saliente
			ts = min(ts,its.distance);
		}
	}

	if(te<=ts){
		its.distance = te;				// te es la interseccion mas cercana (y ts la menos)
		its.normal = normal;
		return true;
	}
	return false;
}

//
//Clase Caja ----------------------------

Caja::Caja(Transformacion T)
{
	//ToDo: Implementar
	cara.clear();
	ncaras = 0;

	// Puntos por defecto - Altura de la caja: 2
	Punto puntos[24]= {
		// Cara 1 - Normal hacia +x
		Punto( 1, 1,-1), Punto( 1, 1, 1), Punto( 1,-1, 1), Punto( 1,-1,-1),
		// Cara 2 - Normal hacia +y
		Punto(-1, 1,-1), Punto(-1, 1, 1), Punto( 1, 1, 1), Punto( 1, 1,-1),
		// Cara 3 - Normal hacia +z
		Punto(-1, 1, 1), Punto(-1,-1, 1), Punto( 1,-1, 1), Punto( 1, 1, 1),
		// Cara 4 - Normal hacia -x
		Punto(-1, 1,-1), Punto(-1,-1,-1), Punto(-1,-1, 1), Punto(-1, 1, 1),
		// Cara 5 - Normal hacia -y
		Punto(-1,-1,-1), Punto( 1,-1,-1), Punto( 1,-1, 1), Punto(-1,-1, 1),
		// Cara 6 - Normal hacia -z
		Punto(-1,-1,-1), Punto(-1, 1,-1), Punto( 1, 1,-1), Punto( 1,-1,-1)
	};

	for(int i=0; i<6; i++){
		Punto caras[4];
		for(int j=0; j<4; j++)
			caras[j] = puntos[j+i*4].transform(T);
		Poligono *c = new Poligono(4, caras);
		cara.push_back(c);
		ncaras++;
	}
}

//ToDo: Implementar
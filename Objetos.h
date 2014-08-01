/*
Definición de los objetos de la escena y de la geometría que los
construye. Todos los objetos responden a la intersección con un
rayo y saben devolver la normal en un punto.

A completar por el alumno.

R.Vivo' (2013)
*/

#ifndef _OBJETOS
#define _OBJETOS 2012

#include "Primitivas.h"
#include <vector>

using namespace std;

//ToDo: Ampliar la jerarquia
class Objeto;
  class Esfera;
  class Poliedro;
	class Caja;

// INTERSECTION: Clase util para describir la interseccion del rayo y el objeto
class Intersection
{
public:

	typedef enum Inttype {EXTERIOR,INTERIOR,INSIDE,NONE};	// Tipos de intersecciones rayo/objeto
	Inttype type;											// Tipo de la interseccion
	int objectid;											// Id del objeto alcanzado por el rayo
	float distance;											// Distancia al origen del rayo 
	Punto point;											// Punto donde se produce la interseccion
	Vector normal;											// Normal en el punto interseccion
	Intersection():type(NONE),objectid(-1),distance(INFINITO){}; // Constructor, por defecto interseccion vacia
	// Operadores de comparacion de intersecciones para el mismo rayo
	bool operator < (const Intersection& i) const {return distance<i.distance;};
	bool operator <=(const Intersection& i) const {return distance<=i.distance;};
	bool operator > (const Intersection& i) const {return distance>i.distance;};
	bool operator >=(const Intersection& i) const {return distance>=i.distance;};
};


// OBJETO: Clase madre de cualquier objeto gráfico
class Objeto
{
protected:

	int id;													// Id unico del objeto

public:

	Color colDifuso,colEspecular,colEmisivo;							// Colores difuso, especular y emisivo
	int m;																// Potencia de brillo de la reflexión especular
	Objeto();															// Constructor
	virtual bool rayIntersection(Punto, Vector, Intersection&) const;	// Devuelve la interseccion mas cercana
	void setId(int i){id=i;};
	int getId()const {return id;};
	void setColor(Color diffuse,Color specular=Color::BLANCO,
				  int shininess=1,Color emissive=Color::NEGRO);			// Caracteristicas del material
};

class Esfera: public Objeto
{
protected:

	Punto centro;
	float radio;

public:

	Esfera();
	Esfera(Punto centro, float radio);	// Constructor
	bool rayIntersection(Punto p, Vector v, Intersection& its) const;  // true: hay interseccion; false: NONE

};

class Poliedro: public Objeto
{
protected:

	vector <Poligono *> cara;
	int ncaras;

public:
	//ToDo: Definir metodos publicos
	bool rayIntersection(Punto p, Vector v, Intersection& its) const;
};

class Caja: public Poliedro
{
	//ToDo: Definir resto de la clase
public:
	Caja(Transformacion T=Transformacion());
};
	
//ToDo: Añadir más poliedros, otros objetos, etc

#endif
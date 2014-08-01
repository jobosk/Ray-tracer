/********************************************
Definicion de clases de geometria sencilla
para su uso con Algebra y de funcines de 
utilidad

R.Vivo'2012
*********************************************/

#ifndef _CPRIMITIVAS
#define _CPRIMITIVAS 2012

#include <GL/glut.h>
#include "Algebra.h"


// Funciones de utilidad ----------------------------------------
void draw_text(unsigned int x, unsigned int y, char *text, GLfloat *color = 0, void *font = GLUT_BITMAP_HELVETICA_18);
void drawWireCube(Color color, Transformacion t);
// --------------------------------------------------------------

class Primitiva;
	class Flecha;
	class Poligono;
		class Cuadrilatero;
		class Triangulo;

// Primitiva gráfica genérica
class Primitiva
{
protected:
	Color color;
public:
	Primitiva();
	void setcolor(Color c);
	virtual void draw() const=0;
};

// Dibujo de una flecha para representar un vector
class Flecha: public Primitiva
{
private:
	Vector vector;						// Dirección
public:
	Flecha(void);
	Flecha (Vector dir, Color col);
	void draw()const;					// Dibuja la flecha en el origen
	void drawAt(const Punto &en)const;	// Dibuja la flecha desde el Punto en
};

// Polígono de n vertices CONVEXO y ANTIHORARIO
class Poligono: public Primitiva
{
protected:
	Punto *vert;						// Array de vertices
	int nvertices;						// Numero de vertices
	Vector N;							// Normal exterior unitaria
public:
	Poligono();
	Poligono(int nvert, Punto *vertices);
	~Poligono();
	Punto vertex(int i)const;			// Devuelve el vértice i (empieza en 0)
	Punto centroid()const;				// Devuelve el centroide del poligono
	int nvertex() const;				// Devuelve el número de vértices
	Vector normal()const;				// Devuelve la normal exterior. SIEMPRE SENTIDO ANTIHORARIO
	int rayIntersection(Punto p, Vector v, float &t)const;	// Devuelve 1: interseccion en P(t); 0: rayo y plano paralelos
	void draw()const;					// Dibuja el poligono
	void drawWith(const Transformacion &T)const;			//Dibuja el poligono aplicando la transformacion T
	void print()const;					// Escribe los vertices por consola
};

// Poligono de cuatro lados
class Cuadrilatero: public Poligono
{
public:
	Cuadrilatero();
	Cuadrilatero (Punto p1,Punto p2,Punto p3,Punto p4, Transformacion t= Transformacion());  // Constructor
};

// Poligono de tres lados
class Triangulo: public Poligono
{
public:
	Triangulo();
	Triangulo(Punto p1,Punto p2,Punto p3, Transformacion t=Transformacion());
};

#endif 
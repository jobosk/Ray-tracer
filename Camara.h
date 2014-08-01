/*
	Definicion de la clase Camara y sus derivadas Ortografica y Perspectiva
	Las camaras mantienen las matrices del modelo, vista y proyeccion
	La camara calcula la matriz de transformacion de normales
	Se definen metodos de interaccion tipo trackball

	R.Vivo'2013
*/

#ifndef __CAMARA
#define __CAMARA

#include "Algebra.h"

// Clase Camara +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class Camara
{

protected:
	Punto eye, center;						// Posicion de la camara y punto de interes
	Vector u,v,w;							// Sistema de referencia de la camara

	float aspectRatio;						// Razon de aspecto 
	float near;								// Distancia al plano frontal 
	float far;								// Distancia al plano trasero 

	Color background;						// Color de fondo

	Transformacion viewmatrix;				// Matriz de transformacion al sistema de la vista
	Transformacion projectionmatrix;		// Matriz de transformacion al volumen ortografico canonico
	Transformacion projectionviewmatrix;	// Composicion de las dos matrices

	void setView();							// Actualiza la la matriz de la vista
	virtual void setProjection() =0;		// Virtualiza la projeccion y obliga a implementarla

public:

	Camara();								// Constructor por defecto

	Transformacion viewMatrix() const;		// Devuelve la matriz de la vista
	Transformacion projectionMatrix() const;// Devuelve la matriz de proyeccion (transformacion al volumen ortografico canonico)
	Transformacion projectionViewMatrix() const; // Devuelve la composicion de la projection y la view
	Transformacion normalMatrix				// Devuelve la matriz de transformacion de normales al sistema de la vista
		(const Transformacion &modelmatrix) const;			
	void lookAt(Punto eyepoint,				// Situa la camara como gluLookAt()
				Punto centerpoint,			
				Vector upvector); 
	Punto project(Punto) const;				// Devuelve el punto en el volumen canonico

	// Manejo de la camara como trackball

	void up(float degrees);					// Mueve la camara hacia arriba
	void down(float degrees);				// Mueve la camara hacia abajo
	void right(float degrees);				// Mueve la camara hacia la derecha
	void left(float degrees);				// Mueve la camara hacia la izquierda

};

// Clase Camara Ortografica +++++++++++++++++++++++++++++++++++++++++++++++++++++++

class CamaraOrtografica: public Camara
{
protected:
	float height;							// Altura de la foto 
	void setProjection();					// Actualiza la matriz de proyeccion
public:
	CamaraOrtografica();					// Constructor por defecto
	CamaraOrtografica(float ysize, float aspect, float neardistance, float fardistance); 
	void setFrustum(float ysize, float aspect, float neardistance, float fardistance);
};

// Clase Camara Perspectiva +++++++++++++++++++++++++++++++++++++++++++++++++++++++

class CamaraPerspectiva: public Camara
{
protected:
	float verticalAngle;					// Apertura vertical del objetivo
	void setProjection();					// Actualiza la matriz de proyeccion
public:
	CamaraPerspectiva();					// Constructor por defecto
	CamaraPerspectiva(float fovydegrees, float aspect, float neardistance, float fardistance);
	void setFrustum(float fovydegrees, float aspect, float neardistance, float fardistance);
	CamaraPerspectiva(Punto eyepoint, Punto centerpoint=Punto(),
					  Vector upvector=Vector(0,1,0), float fovyangle=60.0,
					  Color backgroundcolor= Color::NEGRO);
	Vector visualPixel(float i, float j, int ancho, int alto) const;
	Punto pov() const {return eye;};
	Punto poi() const {return center;};
	Color backColor() const {return background;};

};

#endif

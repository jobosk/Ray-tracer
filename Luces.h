/*
Definición de las fuentes de luz puntual, direccional y focalizada.

R.Vivo'(2012)
*/

#ifndef _LUCES
#define _LUCES 2012


#define ON 1
#define OFF 0

#include "Algebra.h"

class FuenteLuminosa;
  class Ambiental;
  class Puntual;
  class Direccional;
  class Focalizada;


class FuenteLuminosa 
{
protected:
	Color I;
	int encendida;
	Punto posicion;
public:	
	FuenteLuminosa();
	FuenteLuminosa(Color intensidad,Punto pos=Punto()); //Constructor 
	virtual Color intensity(Punto p)const;				//Devuelve la intensidad vista desde p
	virtual Vector L(Punto p)const;						//Vector unitario desde p hacia la luz
	void setColor(Color c);								//Fija el color (intensidad)
	Punto position() const;								//Devuelve la posición de la luz
	void setPosition(Punto pos);						//Fija la posicion de la luz
	void switchLight(int);								//Enciende (1) o apaga (0)
	int switchOn()const;								//Devuelve el estado de la luz (1) encendida, (0) apagada
};

class Ambiental: public FuenteLuminosa
{
public:
	Ambiental();
	Vector L(Punto p)const;
};

class Puntual: public FuenteLuminosa
{
public:
	Puntual();
	Puntual(Color c, Punto pos=Punto());
};

class Direccional: public FuenteLuminosa
{
protected:
	Vector direccion;
public:
	Direccional();
	Direccional(Color intens,Vector direc=Vector(0,-1,0));
	void setDirection(Vector d);
	Vector L(Punto p)const;
};

class Focalizada: public FuenteLuminosa
{
protected:
	Vector direccion;	// Dirección de central del foco
	double p;			// Parámetro que regula la distribución de la 
						// intensidad en el foco, según el modelo de Warn
	double semiapertura;// Coseno entre el eje y la generetriz del cono
public:
	Focalizada();
	Focalizada(Color inten,Punto posic,Vector direc,double concentracion,double angulo);
	Color intensity(Punto pt)const;
	void setShape(Vector dir,double concentracion,double angulo);
};

#endif
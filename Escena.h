/*
	Plantilla para la clase escena
	a completar segun requerimientos

	R.Vivo'2013
*/

#ifndef _ESCENA_H
#define _ESCENA_H 2013

#include "Luces.h"
#include "Objetos.h"
#include "Camara.h"
#include <vector>

using namespace std;

class Escena
{
protected:

	vector <Objeto *> objetos;				//Lista de objetos
	int n_objetos;							//Contador de objetos
	CamaraPerspectiva camara;				//Camara para ver la escena

	//ToDo: A�adir datos y metodos protegidos
	vector <FuenteLuminosa *> luces;
	int n_luces;
	bool shadows, reflexes;
	int profundidad;

public:
	
	Escena();										//Constructor
	~Escena();										//Destructor
	void reset(void);								//Limpia la escena
	int add(Objeto *o);								//A�ade un objeto y devuelve su id
	void setCamera(CamaraPerspectiva escenecamera);	//Fija la c�mara para esa escena
	CamaraPerspectiva getCamera() const;			//Devuelve la camara actual
	Color rayTrace(Punto inicio,Vector direccion) const; //Devuelve el color del primer objeto alcanzado

	//ToDo: A�adir metodos publicos
	Color reflexTrace(Punto inicio,Vector direccion, int nivel) const;
	int addLight(FuenteLuminosa *l);
	void switchLights(int state);
	void switchShadows(bool state){ shadows = state; }
	void switchReflexes(bool state){ reflexes = state; }
};

#endif
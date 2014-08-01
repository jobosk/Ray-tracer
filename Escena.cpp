/**************************************
	Implementacion de la clase escena
	a completar.

	R.Vivo'2013
**************************************/
#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include "Escena.h"

// Clase Escena ------------------------
Escena::Escena()
{
	n_objetos = 0;
	//ToDo: completar construccion
	n_luces = 0;
	shadows = true;
	reflexes = true;
	profundidad = 1;
}

Escena::~Escena()
{
	reset();
	//ToDo: completar destruccion
}

void Escena::setCamera(CamaraPerspectiva escenecamera){
	camara = escenecamera;
}

CamaraPerspectiva Escena::getCamera() const
{
	return camara;
}

void Escena::reset(void)
{
	objetos.clear();
	n_objetos=0;
	//ToDo: completar reset si se precisa
	luces.clear();
	n_luces=0;
	shadows = true;
	reflexes = true;
}

int Escena::add(Objeto *o)
{
	objetos.push_back(o);
	o->setId(n_objetos);
	n_objetos++;
	return n_objetos-1;
}

Color Escena::rayTrace(Punto inicio, Vector direccion) const
//Algoritmo de trazado de un rayo por la escena
{
	//ToDo: Implementar el trazador

	// 1. Calcular la interseccion mas proxima al 'inicio' en esa 'direccion' (ray casting)
	// 2. Si la interseccion es nula, devolver color negro
	// 3. Si no, calcular el color en el punto interseccion y devolverlo (iluminacion)

	Vector normal;
	
	int prox;
	float dist = INFINITO;
	for(int i=0; i<n_objetos; i++){
		Intersection its;
		if(objetos[i]->rayIntersection(inicio, direccion, its))
			if(its.distance < dist){
				prox = i;
				dist = its.distance;
				normal = its.normal;
			}
	}

	if(dist < INFINITO){
		
		Punto corte = inicio + direccion * dist;
		Color iluminacion = objetos[prox]->colEmisivo + luces[0]->intensity(corte) * objetos[prox]->colDifuso;
		
		for(int i=1; i<n_luces; i++){
			
			bool light = true;
			if(this->shadows){
				for(int o = 0; o < n_objetos; o++) {
					Intersection shadow;
					if(o != prox && objetos[o]->rayIntersection(corte + normal * EPSILON, luces[i]->position() - corte, shadow)) {
						light = false;
						break;
					}
				}
			}

			if(luces[i]->switchOn() == ON){

				Vector L = luces[i]->L(corte);
				Vector V = direccion.negated();
				Vector H = (L + V).unitary();

				float NL = normal * L;
				float NV = normal * V;
				float NH = normal * H;

				if(light)
					iluminacion = iluminacion + luces[i]->intensity(corte) * (objetos[prox]->colDifuso * NL + objetos[prox]->colEspecular * pow(NH, objetos[prox]->m));

				if(this->reflexes){
					
					Vector N2 = normal * 2.0;
					Vector R = N2 * NV - V;
					
					Intersection reflex;
					for(int o = 0; o < n_objetos; o++) {
						if(objetos[o]->rayIntersection(corte + normal * EPSILON, R, reflex)) {
							iluminacion = iluminacion + objetos[prox]->colEspecular * reflexTrace(corte + normal * EPSILON, R, 1);
						}
					}
				}
			}
		}
		return iluminacion;
	}
	return Color::NEGRO;
}

Color Escena::reflexTrace(Punto inicio, Vector direccion, int nivel) const {

	Vector normal;
	
	int prox;
	float dist = INFINITO;
	for(int i=0; i<n_objetos; i++){
		Intersection its;
		if(objetos[i]->rayIntersection(inicio, direccion, its))
			if(its.distance < dist){
				prox = i;
				dist = its.distance;
				normal = its.normal;
			}
	}

	if(dist < INFINITO){
		
		Punto corte = inicio + direccion * dist;
		Color iluminacion = objetos[prox]->colEmisivo + luces[0]->intensity(corte) * objetos[prox]->colDifuso;
		
		for(int i=1; i<n_luces; i++){
			
			bool light = true;
			if(this->shadows){
				for(int o = 0; o < n_objetos; o++) {
					Intersection shadow;
					if(o != prox && objetos[o]->rayIntersection(corte + normal * EPSILON, luces[i]->position() - corte, shadow)) {
						light = false;
						break;
					}
				}
			}

			if(luces[i]->switchOn() == ON){

				Vector L = luces[i]->L(corte);
				Vector V = direccion.negated();
				Vector H = (L + V).unitary();

				float NL = normal * L;
				float NV = normal * V;
				float NH = normal * H;

				if(light)
					iluminacion = iluminacion + luces[i]->intensity(corte) * (objetos[prox]->colDifuso * NL + objetos[prox]->colEspecular * pow(NH, objetos[prox]->m));

				if(this->reflexes && nivel < this->profundidad){
					
					Vector N2 = normal * 2.0;
					Vector R = N2 * NV - V;
					
					Intersection reflex;
					for(int o = 0; o < n_objetos; o++) {
						if(objetos[o]->rayIntersection(corte + normal * EPSILON, R, reflex)) {
							iluminacion = iluminacion + objetos[prox]->colEspecular * reflexTrace(corte + normal * EPSILON, R, nivel + 1);
						}
					}
				}
			}
		}
		return iluminacion;
	}
	return Color::NEGRO;
}

int Escena::addLight(FuenteLuminosa *l){
	luces.push_back(l);
	n_luces++;
	return n_luces-1;
}

void Escena::switchLights(int state){
	for(int i=0; i<n_luces; i++)
		luces[i]->switchLight(state);
}
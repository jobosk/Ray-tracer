
#include "Luces.h"
#include <math.h>

// Fuente Luminosa

FuenteLuminosa::FuenteLuminosa(){
	I = Color(1.0,1.0,1.0);
	encendida = ON;
	posicion = Punto(0.0,0.0,0.0);
}

FuenteLuminosa::FuenteLuminosa(Color intensidad,Punto pos){
	I = intensidad;
	encendida = ON;
	posicion = pos;
}

Color FuenteLuminosa::intensity(Punto p)const {
	return I;
}

Vector FuenteLuminosa::L(Punto p)const {
	return (posicion-p).unitary();
}

void FuenteLuminosa::setColor(Color c){
	I = c;
}

Punto FuenteLuminosa::position() const{
	return posicion;
}

void FuenteLuminosa::setPosition(Punto pos){
	posicion = pos;
}

void FuenteLuminosa::switchLight(int state){
	encendida = state;
}

int FuenteLuminosa::switchOn()const {
	return encendida;
}

// Luz Ambiental

Ambiental::Ambiental(){
	I = Color(1.0,1.0,1.0);
	encendida = ON;
	posicion = Punto(0.0,0.0,0.0);
}

Vector Ambiental::L(Punto p)const {
	return Vector(0.0,0.0,0.0);
}

// Luz Puntual

Puntual::Puntual(){
	I = Color(1.0,1.0,1.0);
	encendida = ON;
	posicion = Punto(0.0,0.0,0.0);
}

Puntual::Puntual(Color c, Punto pos){
	I = c;
	encendida = ON;
	posicion = pos;
}

// Luz Direccional

Direccional::Direccional(){
}

Direccional::Direccional(Color intens,Vector direc){
	I = intens;
	direccion = direc;
}

void Direccional::setDirection(Vector d){
	direccion = d;
}

Vector Direccional::L(Punto p)const {
	return (posicion - p).negated().unitary();
}

// Luz Focalizada

Focalizada::Focalizada(){
	I = Color(1.0,1.0,1.0);
	encendida = ON;
	posicion = Punto(0.0,0.0,0.0);
}

Focalizada::Focalizada(Color inten,Punto posic,Vector direc,double concentracion,double angulo){
	I = inten;
	encendida = ON;
	posicion = posic;
	direccion = direc;
	p = concentracion;
	semiapertura = cos(DEG2RAD(angulo/2));
}

Color Focalizada::intensity(Punto pt)const {
	Vector L = pt - posicion;
	float cosG = (L * direccion) / (L.module() * direccion.module());
	if(cosG < semiapertura)
		return Color::NEGRO;
	else
		return I * pow(cosG, (float)p);
}

void Focalizada::setShape(Vector dir,double concentracion,double angulo){
	direccion = dir;
	p = concentracion;
	semiapertura = cos(DEG2RAD(angulo/2));
}
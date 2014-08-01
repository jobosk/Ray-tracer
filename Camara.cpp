/*
	Implementación de la clase Camara y derivadas

	R.Vivo'2013

	Alumno: Jose Bosca Moya
*/

#include "Camara.h"
#include <math.h>

// class Camara +++++++++++++++++++++++++++++++++++++++++++++++++++++++

Camara::Camara(): eye(Punto()),center(Punto(0,0,-1)),u(Vector(1,0,0)),v(Vector(0,1,0)),w(Vector(0,0,1))
// Constructor por defecto comun a todas las camaras
{
	setView();
}

void Camara::setView()
// Calcula la transformación de la vista general
{
	// recuerda que la acumulacion es LIFO
	// coincidencia de origenes, giro de bases

	viewmatrix.reset();

	//ToDo: Construye la viewmatrix (sug: usa el metodo 'rotation(u,v,w)' de Transformacion)
	//		y actualiza la matriz projectionviewmatrix
	viewmatrix.rotation(u,v,w);
	viewmatrix.translation(eye.negated());

	projectionviewmatrix = projectionmatrix * viewmatrix ;

}

void Camara::lookAt(Punto eyepoint, Punto centerpoint, Vector upvector)
// Situa la camara y la orienta
{
	//ToDo: Actualiza eye, center, w, u, v. Los vectores deben ser unitarios
	eye = eyepoint;
	center = centerpoint;

	w = (center - eye).negated().unitary();
	u = (upvector^w).unitary();
	v = w^u;

	setView();
}

Transformacion Camara::viewMatrix() const
// Devuelve la matriz de la vista
{
	return viewmatrix;
}

Transformacion Camara::projectionMatrix() const
// Devuelve la transformacion de la proyeccion
{
	return projectionmatrix;
}

Transformacion Camara::projectionViewMatrix() const
// Devuelve la transformacion compuesta de la proyeccion y la vista
{
	return projectionviewmatrix;
}

Transformacion Camara::normalMatrix(const Transformacion &modelmatrix) const
// Devuelve la matriz de transformacion de normales dada la del modelo
{
	Transformacion modelview = viewmatrix*modelmatrix;
	return !(modelview.inverse3x3());
}

Punto Camara::project(Punto p) const
// Devuelve el punto en el volumen ortografico canonico
{
	return p.transform(projectionviewmatrix).homogen();
}

void Camara::up(float degrees)
// Mueve la camara en un arco hacia arriba
{
	//ToDo: Actualiza eye, u, v, w. ( Sug: usar Transformacion::rotation(angulo,eje,centrodegiro) )
	Transformacion T;
	T.rotation(degrees, u, center);

	eye = eye.transform(T);
	v = v.transform(T);

	lookAt(eye, center, v);

	setView();
}

void Camara::down(float degrees)
// Mueve la camara en un arco hacia abajo
{
	//ToDo: usar up()
	up(-degrees);

}

void Camara::right(float degrees)
// Mueve la camara en un arco hacia la derecha
{
	//ToDo: Actualiza eye, u, v, w. ( Sug: usar Transformacion::rotation(angulo,eje,centrodegiro) )
	Transformacion T;
	T.rotation(degrees, v.negated(), center);

	eye = eye.transform(T);
	v = v.transform(T);

	lookAt(eye, center, v);

	setView();
}

void Camara::left(float degrees)
// Mueve la camara en un arco hacia la izquierda
{
	//ToDo: usar right()
	right(-degrees);

}

// clase Camara Ortográfica +++++++++++++++++++++++++++++++++++++++++++++++++

CamaraOrtografica::CamaraOrtografica()
// Constructor por defecto de la camara ortografica
{
	height = 2;
	aspectRatio = 1;
	near = 1;
	far = 10;
	setProjection();
}

CamaraOrtografica::CamaraOrtografica(float ysize, float aspect, float neardistance, float fardistance):Camara()
// Constructor con frustum de la camara ortografica
{
	setFrustum(ysize,aspect,neardistance,fardistance);
}

void CamaraOrtografica::setFrustum(float ysize, float aspect, float neardistance, float fardistance)
// Modifica el frustum de la camara ortografica
{
	//ToDo: Actualizar height, aspectRatio, near, far
	height = ysize;
	aspectRatio = aspect;
	near = neardistance;
	far = fardistance;

	setProjection();
}

void CamaraOrtografica::setProjection()
// Escalado al ortoedro canonico
{
	projectionmatrix.reset(); //Inicializa la projection

	//ToDo: Calcular la projectionmatrix y actualizar la projectionviewmatrix

	float sy = 2.0/height;
	float sx = 2.0/(aspectRatio*height);
	float sz = 1.0/(far-near);

	projectionmatrix.scale(sx,sy,sz);
	projectionmatrix.translation(Real4(0,0,near));

	projectionviewmatrix = projectionmatrix * viewmatrix ;

	Camara::setView();

}

// clase Camara Perspectiva +++++++++++++++++++++++++++++++++++++++++++++

CamaraPerspectiva::CamaraPerspectiva()
// Constructor por defecto
{
	verticalAngle=60.0f;
	setProjection();
}

CamaraPerspectiva::CamaraPerspectiva(float fovydegrees, float aspect, float neardistance, float fardistance): Camara()
// Constructor con frustum perspectivo
{
	setFrustum(fovydegrees, aspect, neardistance, fardistance);
}

CamaraPerspectiva::CamaraPerspectiva(Punto eyepoint,Punto centerpoint,
			Vector upvector,float fovyangle, Color backgroundcolor)
// Constructor al uso para scripting
{
	setFrustum(fovyangle,1,1,10);
	lookAt(eyepoint,centerpoint,upvector);
	background = backgroundcolor;
}

void CamaraPerspectiva::setFrustum(float fovydegrees, float aspect, float neardistance, float fardistance)
// Constructor con frustum perspectivo
{
	//ToDo: Actualizar verticalAngle, aspectRatio, near, far
	verticalAngle = fovydegrees;
	aspectRatio = aspect;
	near = neardistance;
	far = fardistance;

	setProjection();
}

void CamaraPerspectiva::setProjection()
// Escalado al ortoedro canonico
{
	projectionmatrix.reset();	//identidad

	//ToDo: Construir la projectionmatrix y actualizar la projectionviewmatrix
	// a) Transformacion perspectiva
	// b) Escalado canonico. La relacion de aspecto es así: tan(fiw/2)= aspectRatio*tan(fih/2);
	// c) Actualizacion de la projectionviewmatrix

	float k = near/far;
	
	Matriz Mpo = Matriz(Real4(1, 0, 0, 0), 
					  Real4(0, 1, 0, 0), 
					  Real4(0, 0, 1/(1-k), k/(1-k)), 
					  Real4(0, 0,-1, 0));

	float fih = verticalAngle*PI/180.0;
	float h = tan(fih/2);
	float w = aspectRatio*h;

	float sy = 1.0/(far*h);
	float sx = 1.0/(far*w);
	float sz = 1.0/far;

	projectionmatrix.append(Mpo);
	projectionmatrix.scale(sx, sy, sz);

	projectionviewmatrix = viewmatrix * projectionmatrix;

	Camara::setView();

}

double f(double real, int viewport, int coord){
	return (real/(float)viewport)*((float)coord + 0.5)-(real/2.0);
}

Vector CamaraPerspectiva::visualPixel(float i, float j, int ancho, int alto) const
// Calculo la visual que pasa por i,j sobre un raster de ancho x alto centrado en center
{
	Vector visual;
	//ToDo practica 4.

	float fov = verticalAngle * PI/180.0;
	float pos_z = 3.0;
	
	double b = 2.0 * tan(fov/2.0) * pos_z;
	double a = ((float)ancho/alto) * b;

	visual = (center-eye) + u * f(a,ancho,i) + v * f(b,alto,j);

	return visual.unitary();
};
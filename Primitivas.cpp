/**************************************
Implementación de clases y funciones de
utilidad.

El alumno puede modificar/ampliar
para la escena a trazar en la practica 4

R.Vivo'2012
***************************************/
#include <iostream>
#include <cmath>
#include "Primitivas.h"

using namespace std;

// funciones de utilidad --------------------------------------------------------------

// Dibujo de texto
void draw_text(unsigned int x, unsigned int y, char *text, GLfloat *color, void *font)
// Muestra en la posición (x, y) la cadena de texto con la fuente y el color indicados
// Los dos últimos parámetros son opcionales. Ver en glut.h las posibles fuentes que se 
// pueden utilizar. 
// Por ejemplo: draw_text(30, 30, "Hola mundo");
{	
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);
	glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(viewport[0], viewport[2], viewport[1], viewport[3]);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glLoadIdentity();
	if (color==NULL) {
		GLfloat red[3]={1.0f, 0.0f, 0.0f};
		glColor3fv(red);
	}
	else
	{
		glColor3fv(color);
	}
	glRasterPos2f(x,y);
//	glRasterPos2f(x, winY-y);
	while (*text) 
	{
		glutBitmapCharacter(font, *text++);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
}


// Dibujo de un cubo alambrico con tranformacion
void drawWireCube(Color color, Transformacion t)
// Cubo alambrico de 2 x 2 x 2 centrado y transformado por t
{
	static Punto v[] = {Punto(-1,-1, 1), Punto( 1,-1, 1), Punto( 1, 1, 1), Punto(-1, 1, 1),
						Punto(-1,-1,-1), Punto( 1,-1,-1), Punto( 1, 1,-1), Punto(-1, 1,-1)};
	Punto vt[8];
	for( int i=0; i<8; i++ ) vt[i] = v[i].transform(t).homogen();
	glColor3f(color.r(),color.g(),color.b());
	glBegin(GL_LINE_LOOP);
		//0-1-2-3
		glVertex3f(vt[0].x(),vt[0].y(),vt[0].z());
		glVertex3f(vt[1].x(),vt[1].y(),vt[1].z());
		glVertex3f(vt[2].x(),vt[2].y(),vt[2].z());
		glVertex3f(vt[3].x(),vt[3].y(),vt[3].z());
	glEnd();
	glBegin(GL_LINE_LOOP);
		//7-6-5-4
		glVertex3f(vt[7].x(),vt[7].y(),vt[7].z());
		glVertex3f(vt[6].x(),vt[6].y(),vt[6].z());
		glVertex3f(vt[5].x(),vt[5].y(),vt[5].z());
		glVertex3f(vt[4].x(),vt[4].y(),vt[4].z());
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(vt[0].x(),vt[0].y(),vt[0].z());// 0-4
		glVertex3f(vt[4].x(),vt[4].y(),vt[4].z());
		glVertex3f(vt[1].x(),vt[1].y(),vt[1].z());// 1-5
		glVertex3f(vt[5].x(),vt[5].y(),vt[5].z());
		glVertex3f(vt[2].x(),vt[2].y(),vt[2].z());// 2-6
		glVertex3f(vt[6].x(),vt[6].y(),vt[6].z());
		glVertex3f(vt[3].x(),vt[3].y(),vt[3].z());// 3-7
		glVertex3f(vt[7].x(),vt[7].y(),vt[7].z());
	glEnd();
}



// Clase Primitiva -------------------------------------
Primitiva::Primitiva():color(Color::ROJO)
{
}
void Primitiva::setcolor(Color c)
{
	color=c;
}

// Clase Flecha ---------------------------------------
Flecha::Flecha():vector(Vector(1,0,0))
{
}

Flecha::Flecha(Vector dir, Color col)
{
	color = col;
	vector = dir;
}
void Flecha::draw() const
{
	drawAt(Punto());
}

void Flecha::drawAt(const Punto &en) const
{
	float largo= vector.module();
	Punto punta= en+vector;
	glColor3f(color.r(),color.g(),color.b());
	if(largo<0.001){ //Es muy pequeño o nulo; lo dibujamos como un punto
		glPointSize(4.0);
		glBegin(GL_POINTS);
			glVertex3d(punta.x(),punta.y(),punta.z());
		glEnd();
	}
	else { //Si es más grande como un flecha
		glBegin(GL_LINES);
			glVertex3d(en.x(),en.y(),en.z());
			glVertex3d(punta.x(),punta.y(),punta.z());
		glEnd();
		glPushMatrix();
		glTranslatef(punta.x(),punta.y(),punta.z());
		glRotatef(vector.longitude(),0.0,1.0,0.0);
		glRotatef(-vector.latitude(),1.0,0.0,0.0);
		glTranslatef(0.0,0.0,-largo/10.0);
		glutWireCone(largo/50.0,largo/10.0,10,1);
		glPopMatrix();
	};
}

//Clase Poligono ---------------------------Sentido antihorario y convexo
Poligono::Poligono()
//Construye un poligono vacio
{
	nvertices=0;
}
Poligono::Poligono(int nvert, Punto *vertices)
//Constructor por vértices
{
	if(nvert>2){
		vert= new Punto[nvert];
		nvertices=nvert;
		for(int i=0; i<nvertices; i++) vert[i]= vertices[i];
		N= ((vert[1]-vert[0]) ^ (vert[2]-vert[1])).unitary();
		if(N==Vector()) cerr<<"Poligono degenerado"<<endl;
	}
}
Poligono::~Poligono()
//Destructor
{
	delete[] vert;
}
int Poligono::nvertex()const
//Devuelve el numero de vertices del poligono
{
	return nvertices;
}
Punto Poligono::vertex(int i)const
//devuelve el vertice i
{
	return vert[i];
}
Punto Poligono::centroid()const
{//Devuelve el punto centroide
	Vector suma;
	Punto centroide;
	for(int i=0;i<nvertices;i++)
		suma= suma+vert[i].asVector();
	centroide= suma*(1.0f/nvertices);
	return centroide;
}
Vector Poligono::normal()const
{
	return N;
}
int Poligono::rayIntersection(Punto p, Vector v, float &t)const
//Calcula la intersección con el plano que contiene al poligono
{
	float Nv= N*v;
	if(abs(Nv) == 0.0) return 0; //Rayo y plano paralelos
	t= (N*(vert[0]-p)) / Nv;
	return 1;
}
void Poligono::draw()const
//Dibuja el poligono en alambrico (como Primitiva)
{	
	glColor3f(color.r(),color.g(),color.b());
	glBegin(GL_POLYGON);
	glNormal3f(N.x(),N.y(),N.z());
	for(int i=0; i< nvertices; i++)
		glVertex3f(vert[i].x(),vert[i].y(),vert[i].z());
	glEnd();
}
void Poligono::drawWith(const Transformacion &T)const
//Dibuja el poligono solido (como Primitiva)
{	
	Vector Nt = N.transform( !(T.inverse3x3()) );
	glColor3f(color.r(),color.g(),color.b());
	glBegin(GL_POLYGON);
	glNormal3f(Nt.x(),Nt.y(),Nt.z());
	for(int i=0; i< nvertices; i++){
		Punto vertt = vert[i].transform(T);
		glVertex3f(vertt.x(),vertt.y(),vertt.z());
	}
	glEnd();
}
void Poligono::print() const
{
	cout<<"Poligono: n_vertices= "<<nvertices<<endl;
	for(int i=0;i<nvertices;i++){
		cout<<"V"<<i<<"= ";vert[i].print();
	}
}
//
// Clase Cuadrilátero -----------------------
Cuadrilatero::Cuadrilatero()
{
	vert=new Punto[4];
	nvertices=4;
	N= Vector(0,0,1);

	vert[0]= Punto(0.0f,0.0f,0.0f);
	vert[1]= Punto(1.0f,0.0f,0.0f);
	vert[2]= Punto(1.0f,1.0f,0.0f);
	vert[3]= Punto(0.0f,1.0f,0.0f);
}
Cuadrilatero::Cuadrilatero(Punto p1,Punto p2,Punto p3,Punto p4, Transformacion t)
{
	vert=new Punto[4];
	nvertices=4;
	vert[0]= p1.transform(t);
	vert[1]= p2.transform(t);
	vert[2]= p3.transform(t);
	vert[3]= p4.transform(t);
	N= ((vert[1]-vert[0])^(vert[2]-vert[1])).unitary();
}

//
// Clase Triángulo --------------------------
Triangulo::Triangulo()
{
	vert=new Punto[3];
	nvertices= 3;
	vert[0]=Punto(0.0f,0.0f,0.0f);
	vert[1]=Punto(1.0f,0.0f,0.0f);
	vert[2]=Punto(1.0f,1.0f,0.0f);
	N= ((vert[1]-vert[0])^(vert[2]-vert[1])).unitary();

}
Triangulo::Triangulo(Punto p1,Punto p2,Punto p3, Transformacion t) 
{
	vert=new Punto[3];
	vert[0]= p1.transform(t);
	vert[1]= p2.transform(t);
	vert[2]= p3.transform(t);
	nvertices= 3;
	N= ((vert[1]-vert[0])^(vert[2]-vert[1])).unitary();

}

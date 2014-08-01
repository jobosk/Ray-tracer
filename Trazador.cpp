/********************************************
GPC PRACTICA 4

Plantilla para construir un trazador de rayos

R.Vivó(2013)
*********************************************/

//BIBLIOTECAS Y DEFINICIONES
#include <iostream>
#include <cmath>
#include <GL/Glut.h>
#include "Escena.h"
using namespace std;

//GLOBALES
unsigned char *raster = NULL;	// Vector que almacenará en los valores del raster, usando RGB
Escena *escena = NULL;			// Definición de la escena 
int ancho, alto;				// Tamaño del raster en píxeles
bool sucio = true;				// Flag que indica si hay que retrazar la imagen

void crearEscena()
//Construye la escena a trazar
{
	// Instancia una escena o limpia la que hay
	if ( escena==NULL ) escena = new Escena;
	escena->reset();

	//ToDo: Instanciar los objetos y añadirlos a la escena
	
	Objeto *esfera = new Esfera(Punto(0.0,0.1,-2.0), 0.8);
	//esfera->setColor(Color(0.0,0.0,0.3), Color(0.5,0.5,0.7), 40, Color(0.0,0.0,0.0));
	esfera->setColor(Color(0.1,0.1,0.5), Color(0.5,0.5,0.7), 40, Color(0.0,0.0,0.0));
	escena->add(esfera);

	Transformacion t_suelo;
	t_suelo.translation(Real4(0.0,-1.0,-1.5));
	t_suelo.scale(2.0,0.1,2.0);

	Objeto *suelo = new Caja(t_suelo);
	suelo->setColor(Color(0.3,0.3,0.3), Color(0.9,0.9,0.9), 100, Color(0.0,0.0,0.0));
	escena->add(suelo);

	Transformacion t_cubo;
	t_cubo.translation(Real4(-1.0,-0.3,-0.3));
	t_cubo.rotation(60, Vector(1.0,1.0,1.0));
	t_cubo.scale(0.5,0.5,0.5);

	Objeto *cubo = new Caja(t_cubo);
	cubo->setColor(Color(0.0,0.8,0.0), Color(0.1,0.1,0.1), 1, Color(0.0,0.3,0.0));
	escena->add(cubo);
	
	//ToDo: Instanciar una camara y asignarla a la escena
	
	escena->setCamera(CamaraPerspectiva(Punto(0.0,1.5,4.0), Punto(0.0,0.5,1.0), Vector(0,1,0), 50));

	//ToDo: Instanciar fuentes de luz y añadirlas a la escena

	FuenteLuminosa *ambiental = new Ambiental();
	ambiental->setColor(Color(0.7,0.7,0.7));
	escena->addLight(ambiental);

	FuenteLuminosa *puntual = new Puntual(Color(1,1,1), Punto(2.0,0.0,0.0));
	//FuenteLuminosa *puntual = new Puntual(Color(1,1,1), Punto(0,8,-2));
	escena->addLight(puntual);

	//FuenteLuminosa *focalizada = new Focalizada(Color(1,1,0), Punto(0,2,-1), Vector(0,-1,0), 3, 30);
	FuenteLuminosa *focalizada = new Focalizada(Color(1,1,0), Punto(0,2,-1), Vector(0,-1,0), 8, 60);
	//FuenteLuminosa *focalizada = new Focalizada(Color(1,1,0), Punto(0,8,-2), Vector(0,-1,0), 3, 30);
	escena->addLight(focalizada);
}

void myinit(void)
//Inicialización del trazado
{
	//Inicializa GL
    glClearColor (1.0, 1.0, 1.0, 1.0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	crearEscena();

	//ToDo: En caso necesario, inicializar características del trazado

	//ToDo: Salida por consola de opciones del trazado

}

void traza(void)
//Recorre el raster y calcula el color de cada pixel
{
	//MEMORIA RASTER
	// Si es la primera vez, o hay que redibujar la escena...
	if ( raster==NULL ) {
		raster = new unsigned char[3*ancho*alto];
		if ( raster==NULL ) {
			cerr<<"Sin memoria\n";
			exit(-1);
		}
	}

	// Puntero para recorrer el raster
	unsigned char *t;
	t=raster;

	//RECORRIDO DEL RASTER
	for ( int j=0; j<alto; j++ ) {
		for ( int i=0; i<ancho; i++ ){	
			// La camara se ocupa de calcular la visual que pasa por el pixel i,j
			Vector visual = escena->getCamera().visualPixel(i,j,ancho,alto);
			// La escena devuelve el color del pixel en esa visual
			Color color = escena->rayTrace(escena->getCamera().pov(),visual);
			// Si el color es negro, se pone al color del fondo
			if( color==Color::NEGRO ) color = escena->getCamera().backColor();
			// Se escribe el color en el raster
			*t++ = (unsigned char)(color.r()*255);
			*t++ = (unsigned char)(color.g()*255);
			*t++ = (unsigned char)(color.b()*255);
		}
	}

	// El raster esta cargado y es valido
	sucio = false;
}

void display(void)
//Recarga el raster en el buffer de color (area de dibujo)
{
	// Si el raster no es bueno, trazar
	if ( raster==NULL || sucio ) traza();

	// Copia el raster al frame buffer
	glRasterPos2i(0,0);
	glDrawPixels(ancho, alto, GL_RGB, GL_UNSIGNED_BYTE, raster);
	glFlush();
}

void myReshape(GLsizei w, GLsizei h)
//Establece las nuevas dimensiones del raster y el area de dibujo
{
    alto = (h == 0) ? 1 : h;
	ancho = (w == 0) ? 1 : w;
    glViewport(0, 0, ancho, alto);
    
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLfloat)ancho, 0.0, (GLfloat)alto);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	//El raster ya no es valido pues ha cambiado sus dimensiones
	delete [] raster;
	raster = NULL;
}

void myKeyboard(unsigned char tecla, int x, int y)
// Interfaz por teclado
{
	//ToDo: Seguir las especificaciones de la practica 4
	switch ( tecla ) {
	case 'A':
		//ToDo: Activar el antialiasing y redibujar
		break;
	case 'a':
		//ToDo: Desactivar el antialiasing y redibujar
		break;
	case 'S':
		//ToDo: Activar las sombras y redibujar
		escena->switchShadows(true);
		sucio = true;
		display();
		cout << "Activa sombras" << endl;
		break;
	case 's':
		//ToDo: Desactivar las sombras y redibujar
		escena->switchShadows(false);
		sucio = true;
		display();
		cout << "Desactiva sombras" << endl;
		break;
	case 'R':
		//ToDo: Activar los reflejos y redibujar
		escena->switchReflexes(true);
		sucio = true;
		display();
		cout << "Activa reflejos" << endl;
		break;
	case 'r':
		//ToDo: Desactivar los reflejos y redibujar
		escena->switchReflexes(false);
		sucio = true;
		display();
		cout << "Desactiva reflejos" << endl;
		break;
	case 'L':
		//ToDo: Activar todas las luces y redibujar
		escena->switchLights(ON);
		sucio = true;
		display();
		cout << "Activa luces" << endl;
		break;
	case 'l':
		//ToDo: Desactivar luces (excepto ambiental) y redibujar
		escena->switchLights(OFF);
		sucio = true;
		display();
		cout << "Desactiva luces" << endl;
		break;
	case 27:
		exit(0);
	default:;
	};
	return;
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB );
   glutInitWindowSize (500, 400); 
   glutInitWindowPosition (500, 100);
   glutCreateWindow ("Trazador de rayos GPC");

   myinit();
   glutDisplayFunc(display); 
   glutReshapeFunc(myReshape);
   glutKeyboardFunc(myKeyboard);

   glutMainLoop();
   return(0);
}
 
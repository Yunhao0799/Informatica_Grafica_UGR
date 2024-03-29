//**************************************************************************
// Práctica 1 usando objetos
//**************************************************************************

#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>
// Libreria necesaria para pintar de colores aleatorios
#include <ctime>

const float AXIS_SIZE=5000;
typedef enum{POINTS,EDGES,SOLID_CHESS,SOLID, SOLID_RANDOM} _modo;

struct colores_Aleatorios{
	int r, g, b;
};

//*************************************************************************
// clase punto
//*************************************************************************

class _puntos3D
{
public:

  
	_puntos3D();
void 	draw_puntos(float r, float g, float b, int grosor);

vector<_vertex3f> vertices;
};

//*************************************************************************
// clase triángulo
//*************************************************************************

class _triangulos3D: public _puntos3D
{
public:

	_triangulos3D();
void 	draw_aristas(float r, float g, float b, int grosor);
void    draw_solido(float r, float g, float b);
void 	draw_solido_ajedrez(float r1, float g1, float b1, float r2, float g2, float b2);
void 	draw_solido_aleatorio();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

vector<_vertex3i> caras;
vector<colores_Aleatorios> colores_cara;

};


//*************************************************************************
// clase cubo
//*************************************************************************

class _cubo: public _triangulos3D
{
public:

	_cubo(float tam=0.5);

//vector<colores_Aleatorios> colores_cara;

};


//*************************************************************************
// clase piramide
//*************************************************************************

class _piramide: public _triangulos3D
{
public:

	_piramide(float tam=0.5, float al=0.75);
};

//*************************************************************************
// clase objeto ply
//*************************************************************************

class _objeto_ply: public _triangulos3D
{
public:
   _objeto_ply();

int   parametros(char *archivo);
};

//************************************************************************
// objeto por revolución
//************************************************************************

class _rotacion: public _triangulos3D
{
public:
       _rotacion();
void  parametros(vector<_vertex3f> perfil1, int num1, int tipo);
// void  parametros(vector<_vertex3f> perfil1, float altura_tapa_superior, float altura_tapa_inferior, int num1);

vector<_vertex3f> perfil; 
int num;
};

// Lo que hay que hacer

class _cono: public _rotacion{
	
	public:
		_cono();
		_cono (float radio, float altura, int m);

		// void  parametros(vector<_vertex3f> perfil1, int num1);

	// vector<_vertex3f> perfil; 
	// int num;
};

class _esfera: public _rotacion{
	public:
		_esfera();
		_esfera(float radio, int n, int m);
		// _esfera (float radio, int n);
		//void  parametros(vector<_vertex3f> perfil1, int num1, int radio_size);

	// vector<_vertex3f> perfil; 
	// int num;
};

class _cilindro: public _rotacion{
	public:
		_cilindro(float radio, float altura, int m);
};


// Tapa inferior

// vertices[num_aux*num].x=0.0; 
//    if (tipo==0) vertices[num_aux*num].y=perfil[0].y; 
//    if (tipo==1) vertices[num_aux*num].y=0.0;
//    if (tipo==2) vertices[num_aux*num].y=-radio;
//    vertices[num_aux*num].z=0.0;





// Tapa superior

// vertices[num_aux*num+1].x=0.0; 
//    if (tipo==0) vertices[num_aux*num+1].y=perfil[num_aux-1].y; 
//    if (tipo==1) vertices[num_aux*num+1].y=altura; 
//    if (tipo==2) vertices[num_aux*num+1].y=radio; 
//    vertices[num_aux*num+1].z=0.0;


// Crear leer desde un ply
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

	// Tipo: cilindro-0, cono-1, esfera-2
	void  parametros(vector<_vertex3f> perfil1, int num1, int tipo);

	vector<_vertex3f> perfil; 
	int num;
};

// Lo que hay que hacer

class _cono: public _rotacion{
	
	public:
		// _cono();
		_cono (float radio=0.5, float altura=1, int m=24);
};

class _esfera: public _rotacion{
	public:
		// _esfera();
		_esfera(float radio=0.5, int n=24, int m=24);
};

class _cilindro: public _rotacion{
	public:
		// _cilindro();
		_cilindro(float radio=0.5, float altura=1, int m=24);
};

class _media_esfera: public _rotacion{
	public:
		// _esfera();
		_media_esfera(float radio=0.5, int n=24, int m=24);
};





class _brazo_izq : _triangulos3D{
	public:
		_brazo_izq();

	void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

	float anchura;
	float altura;

	protected:
		_cilindro cilindro;
};

class _brazo_dch : _triangulos3D{
	public:
		_brazo_dch();

	void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

	float anchura;
	float altura;

	protected:
		_cilindro cilindro;
};

class _pierna_izq: _triangulos3D{
	public:
		_pierna_izq();

	void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

	float anchura;
	float altura;

	protected:
		_cilindro cilindro;
		_esfera esfera;
};

class _pierna_dch : _triangulos3D{
	public:
		_pierna_dch();

	void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

	float anchura;
	float altura;

	protected:
		_cilindro cilindro;
		_esfera esfera;
};

class _cabeza : _triangulos3D{
	public: 
		_cabeza();

	void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

	protected:
		_media_esfera media_esfera;
		_cilindro cilindro;

};

class _tronco : _triangulos3D{
	public:
		_tronco();

	void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);
	void	move_left(float left);
	void 	move_right(float right);
	void	move_head(float left);


	float brazo_iq_mv; 
	float brazo_d_mv;
	float head;

	protected:
		_cubo cubo;
		_cilindro cilindro;	
		_brazo_izq brazo_izq;
		_brazo_dch brazo_dch;
		_cabeza cabeza;
};

class _tronco_inferior : _triangulos3D{
	public:
		_tronco_inferior();

	void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);


	protected:
		_cubo cubo;
		_cilindro cilindro;	
		_pierna_izq pierna_izq;
		_pierna_dch pierna_dch;
};


class _tronco_arbol:_triangulos3D{
	public:
		_tronco_arbol();

	void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);


	protected:
		_cilindro cilindro;
	
};

class _arbol:_triangulos3D{
	public:
		_arbol();

	void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);


	protected:
		_tronco_arbol tronco;
		_cono cono;

};


class _paisaje:_triangulos3D{
	public:
		_paisaje();

	void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);


	protected:
		_arbol arbol;
		_cubo cubo;
};




class _final : _triangulos3D{
	public:
		_final();

	void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);
	//void 	setLeft(float a);
	//void 	setRight(float a);
	void reset();

	float brazo_iq_mv; 
	float brazo_d_mv;
	float torso_rot;
	float max_torso;
	float inclinar;
	float inclinar_max;
	float brazo_max;
	float brazo_min;

	float rotar_cabeza;
	float rotar_cabeza_max;

	protected:
		_tronco tronco;
		_tronco_inferior tronco_inferior;
		_brazo_izq brazo_izq;
		_brazo_dch brazo_dch;
		_paisaje paisaje;
};






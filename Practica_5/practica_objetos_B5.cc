//**************************************************************************
// Práctica 1 usando objetos
//**************************************************************************

#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include "objetos_B5.h"


using namespace std;

// tipos
typedef enum{CUBO, PIRAMIDE, OBJETO_PLY, ROTACION, CILINDRO, ESFERA, MEDIA_ESFERA, CONO, ARTICULADO} _tipo_objeto;
_tipo_objeto t_objeto=ARTICULADO;
_modo   modo=SOLID;
int velocidad_brazo_izq = 1;
int velocidad_brazo_dch = 1;
int velocidad_rotacion_torso = 5;
int velocidad_inclinacion_torso = 5;
const int MAX_VELOCIDAD = 15;
int velocidad_cabeza = 5;
bool flag = 0;

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;
GLfloat Observer_angle_z;

int estadoRaton[3], xc, yc;
void pick_color(int x, int y);


// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Size_x,Size_y,Front_plane,Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int Window_x=50,Window_y=50,Window_width=800,Window_high=600;


// objetos
_cubo cubo;
_piramide piramide(0.85,1.3);
_objeto_ply  ply; 
_rotacion rotacion;
// Este crea cilindro
_cilindro cilindro (0.5, 2, 12); 
// Declaracion esfera
_esfera esfera(0.5, 6, 6);
// Declaracion cono
_cono cono(1, 2, 6); 
// _objeto_ply *ply1;

_media_esfera mEsfera(0.5, 24, 24);

_final final;


//**************************************************************************
//
//***************************************************************************

void clean_window()
{

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

void change_projection()
{

glMatrixMode(GL_PROJECTION);
glLoadIdentity();

// formato(x_minimo,x_maximo, y_minimo, y_maximo,plano_delantero, plano_traser)
//  plano_delantero>0  plano_trasero>PlanoDelantero)
glFrustum(-Size_x,Size_x,-Size_y,Size_y,Front_plane,Back_plane);
}

//**************************************************************************
// Funcion para definir la transformación*ply1 de vista (posicionar la camara)
//***************************************************************************

void change_observer()
{

	// posicion del observador
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,-Observer_distance);
	glRotatef(Observer_angle_x,1,0,0);
	glRotatef(Observer_angle_y,0,1,0);
	glRotatef(Observer_angle_z,0,0,1);
}

//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis()
{	
	glDisable(GL_LIGHTING);
	glLineWidth(2);
	glBegin(GL_LINES);
	// eje X, color rojo
	glColor3f(1,0,0);
	glVertex3f(-AXIS_SIZE,0,0);
	glVertex3f(AXIS_SIZE,0,0);
	// eje Y, color verde
	glColor3f(0,1,0);
	glVertex3f(0,-AXIS_SIZE,0);
	glVertex3f(0,AXIS_SIZE,0);
	// eje Z, color azul
	glColor3f(0,0,1);
	glVertex3f(0,0,-AXIS_SIZE);
	glVertex3f(0,0,AXIS_SIZE);
	glEnd();
}


//**************************************************************************
// Funcion que dibuja los objetos
//****************************2***********************************************

void draw_objects()
{

	switch (t_objeto){
		case CUBO: 		   cubo.draw(modo,0.0,0.0,1.0,0.0,1.0,0.0,2);break;
		case PIRAMIDE: piramide.draw(modo,1.0,0.0,0.0,0.0,1.0,0.0,6);break;
		case OBJETO_PLY:    ply.draw(modo,1.0,0.6,0.0,0.0,1.0,0.3,2);break;
		case ROTACION: rotacion.draw(modo,1.0,0.0,0.0,0.0,1.0,0.0,2);break;

		case CILINDRO: cilindro.draw(modo,1.0,0.0,0.0,0.0,1.0,0.0,2);break;
		case ESFERA:     esfera.draw(modo,1.0,0.0,1.0,0.0,1.0,0.0,6);break;
		case CONO:         cono.draw(modo,1.0,0.0,0.0,0.0,1.0,0.0,2);break;
		case MEDIA_ESFERA:         mEsfera.draw(modo,1.0,0.0,0.0,0.0,1.0,0.0,8);break;

		case ARTICULADO:   final.draw(modo,61/255.0,220/ 255.0, 132.0/255.0 ,0.0,1.0,0.0,2);break;
	}

}


//**************************************************************************
//
//***************************************************************************

void draw(void)
{



	glDrawBuffer(GL_FRONT);
	clean_window();
	change_observer();
	draw_axis();
	draw_objects();

	if(t_objeto == ARTICULADO)
	{
		glDrawBuffer(GL_BACK);
		clean_window();
		change_observer();
		// draw_axis();
		final.seleccion(100);
	}


	// if(t_objeto == ARTICULADO){
	// 	// glColor3f(153/255.0, 102/255.0, 51/255.0);
	// 	// glTranslatef(0, -0.6, 0);
	// 	// glRotatef(90, 1, 0, 0);
	// 	// glScalef(20.0, 20.0, 20.0);
	// 	// glRectf(-0.75f,0.75f, 0.75f, -0.75f);


	// 	glClearColor(0.0f, 89/255.0, 179/255.0,0.75);
	// }
	// else{
	// 	glClearColor(1,1,1,1);
	// }
	// glutSwapBuffers();



	


	glFlush();
}



//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion:
// nuevo ancho
// nuevo alto
//***************************************************************************

void change_window_size(int Ancho1,int Alto1)
{
float Aspect_ratio;

Aspect_ratio=(float) Alto1/(float )Ancho1;
Size_y=Size_x*Aspect_ratio;
change_projection();
glViewport(0,0,Ancho1,Alto1);
glutPostRedisplay();
}

/*** Idle fuction ***/

void idleFunction(){

	if(velocidad_brazo_dch > 0){
		if(flag == 0){
			if(final.brazo_d_mv < -final.brazo_max){
				final.brazo_d_mv = -final.brazo_max;
				final.brazo_iq_mv = -final.brazo_max;
				flag =1;
			}
			else{
				final.brazo_d_mv -= velocidad_brazo_dch;
				final.brazo_iq_mv -= velocidad_brazo_izq;
			}

		}
		

		if(flag == 1){
			final.brazo_d_mv += velocidad_brazo_izq;
			final.brazo_iq_mv += velocidad_brazo_izq;
			if(velocidad_brazo_dch > -final.brazo_min){
				final.brazo_d_mv = -final.brazo_min;
				final.brazo_iq_mv = -final.brazo_min;
			}
		}
		if(final.brazo_d_mv > -final.brazo_min && flag == 1){
			final.brazo_d_mv = -final.brazo_min;
			final.brazo_iq_mv = -final.brazo_min;
			flag = 0;
		}

		glutPostRedisplay();
	}
}

//**********-o*****************************************************************
// Funcion llamada cuando se aprieta una tecla normal
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton
//***************************************************************************

void normal_key(unsigned char Tecla1,int x,int y)
{
switch (toupper(Tecla1)){
	case 'Q':exit(0);
	case '1':modo=POINTS;		break;
	case '2':modo=EDGES;		break;
	case '3':modo=SOLID;		break;
	case '4':modo=SOLID_CHESS;	break;
	case '5':modo=SOLID_RANDOM; break;

	case 'P':t_objeto=PIRAMIDE;		break;
	case 'C':t_objeto=CUBO;			break;
	case 'O':t_objeto=OBJETO_PLY;	break;	
	case 'R':t_objeto=CILINDRO;		break;
	case 'E':t_objeto=ESFERA; 		break;
	case 'N':t_objeto=CONO; 		break;
	case 'A':t_objeto=ARTICULADO; 	break;
	case 'M':t_objeto=MEDIA_ESFERA; 	break;
	

	// Cambio de la velocidad de los grados de libertad
	//brazos
	case 'T': 
		velocidad_brazo_izq += 1;
		if(velocidad_brazo_izq > MAX_VELOCIDAD)
			velocidad_brazo_izq = MAX_VELOCIDAD;
		break;
	case 'Y': 
		if(velocidad_brazo_izq < -MAX_VELOCIDAD)
			velocidad_brazo_izq = -MAX_VELOCIDAD;
		velocidad_brazo_izq -= 1;
		break;

	case 'U': 
		if(velocidad_brazo_dch > MAX_VELOCIDAD)
			velocidad_brazo_dch = MAX_VELOCIDAD;
		velocidad_brazo_dch += 1;
		break;
	case 'I': 
		if(velocidad_brazo_dch < -MAX_VELOCIDAD)
			velocidad_brazo_dch = -MAX_VELOCIDAD;
		velocidad_brazo_dch -= 1;
		break;



	// Torso
	case 'S': 
		velocidad_rotacion_torso += 1;
		if(velocidad_rotacion_torso > MAX_VELOCIDAD)
			velocidad_rotacion_torso = MAX_VELOCIDAD;
		break;
	case 'D': 
		velocidad_rotacion_torso -= 1;
		if(velocidad_rotacion_torso < -MAX_VELOCIDAD)
			velocidad_rotacion_torso = -MAX_VELOCIDAD;
		break;
	
	case 'F': 
		velocidad_inclinacion_torso += 1;
		if(velocidad_inclinacion_torso > MAX_VELOCIDAD)
			velocidad_inclinacion_torso = MAX_VELOCIDAD;
		break;
	case 'G': 
		velocidad_inclinacion_torso -= 1;
		if(velocidad_inclinacion_torso < -MAX_VELOCIDAD)
			velocidad_inclinacion_torso = -MAX_VELOCIDAD;
		break;
	
	case 'H': 
		glutIdleFunc(NULL);
		break;

	case 'J':
		glutIdleFunc(idleFunction);
		break;

	case 'K': 
		Observer_distance=8*Front_plane;
		Observer_angle_x=3;
		Observer_angle_y=-2;
		Observer_angle_z=0;
		change_observer();
		break;

	// Cabeza
	case 'Z': 
		velocidad_cabeza += 1;
		if(velocidad_cabeza > MAX_VELOCIDAD)
			velocidad_cabeza = MAX_VELOCIDAD;
		break;

	case 'X': 
		velocidad_cabeza -= 1;
		if(velocidad_cabeza < -MAX_VELOCIDAD)
			velocidad_cabeza = -MAX_VELOCIDAD;
		break;

	case 'V':
		final.rotar_cabeza +=velocidad_cabeza;
		if(final.rotar_cabeza > final.rotar_cabeza_max){
			final.rotar_cabeza = final.rotar_cabeza_max;
		}
		break;

	case 'B':
		final.rotar_cabeza -=velocidad_cabeza;
		if(final.rotar_cabeza < -final.rotar_cabeza_max){
			final.rotar_cabeza = -final.rotar_cabeza_max;
		}
		break;

	case 'L':
		final.reset();
		break;

}
glutPostRedisplay();
}

//***************************************************************************
// Funcion l-olamada cuando se aprieta una tecla especial
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton

//***************************************************************************

void special_key(int Tecla1,int x,int y)
{

	switch (Tecla1){
		case GLUT_KEY_LEFT:Observer_angle_y--;break;
		case GLUT_KEY_RIGHT:Observer_angle_y++;break;
		case GLUT_KEY_UP:Observer_angle_x--;break;
		case GLUT_KEY_DOWN:Observer_angle_x++;break;
		case GLUT_KEY_PAGE_UP:Observer_distance*=1.2;break;
		case GLUT_KEY_PAGE_DOWN:Observer_distance/=1.2;break;

		// case GLUT_KEY_F7: Observer_angle_z--;break;
		// case GLUT_KEY_F8: Observer_angle_z++;break;


		// Establecemos movimiento de nuestra figura
		// Movimiento de los brazos
		// Brazo izq
		case GLUT_KEY_F1: 
			final.brazo_iq_mv+=velocidad_brazo_izq;
			if(final.brazo_iq_mv > -final.brazo_min )
				final.brazo_iq_mv = -final.brazo_min;
			break;
		// Brazo izq
		case GLUT_KEY_F2: 
			final.brazo_iq_mv-=velocidad_brazo_izq;
			if(final.brazo_iq_mv < -final.brazo_max )
				final.brazo_iq_mv = -final.brazo_max;
			break;

		// Movimiento de los brazos
		// Brazo dch
		case GLUT_KEY_F3: 
			final.brazo_d_mv+=velocidad_brazo_dch;
			if(final.brazo_d_mv > -final.brazo_min)
				final.brazo_d_mv = -final.brazo_min;
			break;
		// Brazo dch
		case GLUT_KEY_F4: 
			final.brazo_d_mv-=velocidad_brazo_dch;
			if(final.brazo_d_mv < -final.brazo_max )
				final.brazo_d_mv = -final.brazo_max;
			break;
		
		//Rotacion del torso
		case GLUT_KEY_F5: 
			final.torso_rot+=velocidad_rotacion_torso;
			if(final.torso_rot > final.max_torso)
				final.torso_rot = final.max_torso;
			break;
		
		case GLUT_KEY_F6: final.torso_rot-=velocidad_rotacion_torso;
			if(final.torso_rot < -final.max_torso)
				final.torso_rot = -final.max_torso;
			break;

		case GLUT_KEY_F7: final.inclinar+=velocidad_inclinacion_torso;
			if(final.inclinar > final.inclinar_max)
				final.inclinar = final.inclinar_max;
			break;

		case GLUT_KEY_F8: final.inclinar-=velocidad_inclinacion_torso;
			if(final.inclinar < -1)
				final.inclinar = -1;
			break;
		// movimiento de las piernas
	}

	glutPostRedisplay();
}


//***************************************************************************
// Funciones para manejo de eventos del ratón
//***************************************************************************

void clickRaton( int boton, int estado, int x, int y )
{
if(boton== GLUT_RIGHT_BUTTON) {
   if( estado == GLUT_DOWN) {
      estadoRaton[2] = 1;
      xc=x;
      yc=y;
     } 
   else estadoRaton[2] = 1;
   }
if(boton== GLUT_LEFT_BUTTON) {
  if( estado == GLUT_DOWN) {
      estadoRaton[2] = 2;
      xc=x;
      yc=y;
      pick_color(xc, yc);
    } 
  }
}


/*************************************************************************/

void getCamara (GLfloat *x, GLfloat *y)
{
	*x=Observer_angle_x;
	*y=Observer_angle_y;
}

/*************************************************************************/

void setCamara (GLfloat x, GLfloat y)
{
Observer_angle_x=x;
Observer_angle_y=y;
}



/*************************************************************************/

void RatonMovido( int x, int y )
{
float x0, y0, xn, yn; 
if(estadoRaton[2]==1) 
    {getCamara(&x0,&y0);
     yn=y0+(y-yc);
     xn=x0-(x-xc);
     setCamara(xn,yn);
     xc=x;
     yc=y;
     glutPostRedisplay();
    }
}

//***************************************************************************
// Funciones para la seleccion
//************************************************************************


void procesar_color(unsigned char color[3])
{

 
 switch (color[0]){
	case 100: 
		if (final.activo[0]==0) {
			final.activo[0]=1;
			final.cambiar[0]=1;
		}
		else {
			final.activo[0]=0;
			final.cambiar[0]=0;
		}
		break; 

	case 120: 
		if (final.activo[1]==0) {
			final.activo[1]=1;
			final.cambiar[1]=1;
		}
		else {
			final.activo[1]=0;
			final.cambiar[1]=0;
		}
		break; 

	case 140: 
		if (final.activo[2]==0) {
			final.activo[2]=1;
			final.cambiar[2]=1;
		}
		else {
			final.activo[2]=0;
			final.cambiar[2]=0;
		}
		break; 
                 
 }
}

void pick_color(int x, int y)
{
GLint viewport[4]; //Almacena tamaño de ventana
unsigned char pixel[3];

glGetIntegerv(GL_VIEWPORT, viewport); // Lee la ventana
glReadBuffer(GL_BACK);
glReadPixels(x,viewport[3]-y,1,1,GL_RGB,GL_UNSIGNED_BYTE,(GLubyte *) &pixel[0]);
printf(" valor x %d, valor y %d, color %d, %d, %d \n",x,y,pixel[0],pixel[1],pixel[2]);

procesar_color(pixel);
glutPostRedisplay();
}





//***************************************************************************
// Funcion de incializacion
//***************************************************************************

void initialize(void)
{

// se inicalizan la ventana y los planos de corte
Size_x=0.5;
Size_y=0.5;
Front_plane=1;
Back_plane=1000;

// se incia la posicion del observador, en el eje z
Observer_distance=8*Front_plane;
Observer_angle_x=3;
Observer_angle_y=-2;
Observer_angle_z=0;

// se indica cua*ply1l sera el color para limpiar la ventana	(r,v,a,al)
// blanco=(1,1,1,1) rojo=(1,0,0,1), ...
if(t_objeto == ARTICULADO){
	glClearColor(0.0f, 89/255.0, 179/255.0,0.75);
}
else
{
	glClearColor(1,1,1,1);
}


// se habilita el z-bufer
glEnable(GL_DEPTH_TEST);
change_projection();
glViewport(0,0,Window_width,Window_high);



}










//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************


int main(int argc, char *argv[] )
{
 





// -----------------------------------------

// se llama a la inicialización de glut
glutInit(&argc, argv);

// se indica las caracteristicas que se desean para la visualización con OpenGL
// Las posibilidades son:
// GLUT_SIMPLE -> memoria de imagen simple
// GLUT_DOUBLE -> memoria de imagen doble
// GLUT_INDEX -> memoria de imagen con color indizado
// GLUT_RGB -> memoria de imagen con componentes rojo, verde y azul para cada pixel
// GLUT_RGBA -> memoria de imagen con componentes rojo, verde, azul y alfa para cada pixel
// GLUT_DEPTH -> memoria de profundidad o z-bufer
// GLUT_STENCIL -> memoria de estarcido_rotation Rotation;
glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

// posicion de la esquina inferior izquierdad de la ventana
glutInitWindowPosition(Window_x,Window_y);

// tamaño de la ventana (ancho y alto)
glutInitWindowSize(Window_width,Window_high);

// llamada para crear la ventana, indicando el titulo (no se visualiza hasta que se llama
// al bucle de eventos)
glutCreateWindow("PRACTICA - 3");

// asignación de la funcion llamada "dibujar" al evento de dibujo
glutDisplayFunc(draw);
// asignación de la funcion llamada "change_window_size" al evento correspondiente
glutReshapeFunc(change_window_size);
// asignación de la funcion llamada "normal_key" al evento correspondiente
glutKeyboardFunc(normal_key);
// asignación de la funcion llamada "tecla_Especial" al evento correspondiente
glutSpecialFunc(special_key);

// eventos ratón
glutMouseFunc( clickRaton );
glutMotionFunc( RatonMovido );


glutIdleFunc(idleFunction);

// funcion de inicialización
initialize();

// creación del objeto ply
ply.parametros(argv[1]);

//ply1 = new _objeto_ply(argv[1]);

// inicio del bucle de eventos
glutMainLoop();
return 0;
}

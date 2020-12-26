//**************************************************************************
// Práctica 3 
//**************************************************************************

#include "objetos_B4.h"
#include "file_ply_stl.hpp"
#include <math.h>


//*************************************************************************
// _puntos3D
//*************************************************************************

_puntos3D::_puntos3D()
{
}

//*************************************************************************
// dibujar puntos
//*************************************************************************

void _puntos3D::draw_puntos(float r, float g, float b, int grosor)
{
  int i;
  glPointSize(grosor);
  glColor3f(r,g,b);
  glBegin(GL_POINTS);
  for (i=0;i< (int) vertices.size();i++){
    glVertex3fv((GLfloat *) &vertices[i]);
	}
  glEnd();
}


//*************************************************************************
// _triangulos3D
//*************************************************************************

_triangulos3D::_triangulos3D(){
  b_normales_caras=false;
  b_normales_vertices=false;

  ambiente_difusa=_vertex4f(0.2,0.4,0.9,1.0);  //coeficientes ambiente y difuso
  especular=_vertex4f(0.5,0.5,0.5,1.0);        //coeficiente especular
  brillo=40;                                   //exponente del brillo
}


//*************************************************************************
// dibujar en modo arista
//*************************************************************************

void _triangulos3D::draw_aristas(float r, float g, float b, int grosor)
{
int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
glLineWidth(grosor);
glColor3f(r,g,b);
glBegin(GL_TRIANGLES);
for (i=0;i<(int) caras.size();i++){
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();
}

//*************************************************************************
// dibujar en modo sólido
//*************************************************************************

void _triangulos3D::draw_solido(float r, float g, float b)
{
  int i;
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glColor3f(r,g,b);
  glBegin(GL_TRIANGLES);
  for (i=0;i<(int) caras.size();i++){
    
    glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
  }
  glEnd();
}

//*************************************************************************
// dibujar en modo sólido con apariencia de ajedrez
//*************************************************************************

void _triangulos3D::draw_solido_ajedrez(float r1, float g1, float b1, float r2, float g2, float b2)
{
  int i;
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glBegin(GL_TRIANGLES);
  for (i=0;i<(int) caras.size();i++){
    if(i%2 == 0) glColor3f(r1,g1,b1);
    else glColor3f(r2,g2,b2);

    glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
  }
  glEnd();
}

//*************************************************************************
// dibujar en modo sólido con colores aleatorios
//*************************************************************************
void _triangulos3D::draw_solido_aleatorio()
{
  int i;
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glBegin(GL_TRIANGLES);

  for (i=0;i<(int) caras.size();i++){


    glColor3ub(colores_cara[i].r, colores_cara[i].g, colores_cara[i].b);

    glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
  }
  glEnd();

}

//*************************************************************************
// dibujar con distintos modos
//*************************************************************************

void _triangulos3D::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
  switch (modo){
    case POINTS:draw_puntos(r1, g1, b1, grosor);break;
    case EDGES:draw_aristas(r1, g1, b1, grosor);break;
    case SOLID_CHESS:draw_solido_ajedrez(r1, g1, b1, r2, g2, b2);break;
    case SOLID:draw_solido(r1, g1, b1);break;
    case SOLID_RANDOM:draw_solido_aleatorio(); break;
    case SOLID_ILLUMINATED_FLAT:draw_iluminacion_plana(); break;
    case SOLID_ILLUMINATED_GOURAUD:draw_iluminacion_suave(); break;
    
	}
}

//*************************************************************************
// dibujar en modo iluminación plano
//*************************************************************************

void _triangulos3D::draw_iluminacion_plana( )
{
  int i;
  if (b_normales_caras==false) calcular_normales_caras();

  glShadeModel(GL_FLAT);  //GL_SMOOTH
  glEnable (GL_LIGHTING);
  glEnable(GL_NORMALIZE);

  glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,(GLfloat *) &ambiente_difusa);
	glMaterialfv(GL_FRONT,GL_SPECULAR,(GLfloat *) &especular);
	glMaterialfv(GL_FRONT, GL_SHININESS, &brillo);

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glBegin(GL_TRIANGLES);
    for (i=0;i<caras.size();i++){
      glNormal3fv((GLfloat *) &normales_caras[i]);
      glVertex3fv((GLfloat *) &vertices[caras[i]._0]);     
      glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
      glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
    }
  glEnd();

  glDisable(GL_LIGHTING);
}

//*************************************************************************
// calcular normales a caras
//*************************************************************************


void _triangulos3D::calcular_normales_caras(){  
  normales_caras.resize(caras.size());
  for(unsigned long i=0; i<caras.size(); i++){
	// obtener dos vectores en el triángulo y calcular el producto vectorial
    _vertex3f 
            a1=vertices[caras[i]._1]-vertices[caras[i]._0],
            a2=vertices[caras[i]._2]-vertices[caras[i]._0],
            n=a1.cross_product(a2);
    // modulo
    float m=sqrt(n.x*n.x+n.y*n.y+n.z*n.z);
    // normalización
    normales_caras[i]= _vertex3f(n.x/m, n.y/m, n.z/m);
	}
    
  b_normales_caras=true;
}





void _triangulos3D::draw_iluminacion_suave( )
{
	int i;
	// if (b_normales_caras==false) calcular_normales_caras();
	if (b_normales_vertices==false) calcular_normales_vertices();

	glEnable (GL_LIGHTING);
	glShadeModel(GL_SMOOTH);  //GL_SMOOTH
	glEnable(GL_NORMALIZE);

	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,(GLfloat *) &ambiente_difusa);
	glMaterialfv(GL_FRONT,GL_SPECULAR,(GLfloat *) &especular);
	glMaterialfv(GL_FRONT, GL_SHININESS, &brillo);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glBegin(GL_TRIANGLES);
		for (i=0;i<caras.size();i++){
			glNormal3fv((GLfloat *) &normales_vertices[caras[i]._0]);
			glVertex3fv((GLfloat *) &vertices[caras[i]._0]); 

			glNormal3fv((GLfloat *) &normales_vertices[caras[i]._1]);    
			glVertex3fv((GLfloat *) &vertices[caras[i]._1]);

			glNormal3fv((GLfloat *) &normales_vertices[caras[i]._2]);
			glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
		}
	glEnd();

	glDisable(GL_LIGHTING);
}


void _triangulos3D::calcular_normales_vertices(){  
  
  int i, n = vertices.size();

  int m  =  caras.size();

  normales_vertices.resize(n);

  for(i = 0; i < n; i++){
    normales_vertices[i].x = 0.0;
    normales_vertices[i].y = 0.0;
    normales_vertices[i].z = 0.0;
  }

  for(i = 0; i < m; i++){
    normales_vertices[caras[i]._0] += normales_caras[i];
    normales_vertices[caras[i]._1] += normales_caras[i];
    normales_vertices[caras[i]._2] += normales_caras[i];
  }

  b_normales_vertices=true;
}

//*************************************************************************
// clase cubo
//*************************************************************************

_cubo::_cubo(float tam)
{
  vertices.resize(8);
  vertices[0].x=-tam;vertices[0].y=-tam;vertices[0].z=tam;
  vertices[1].x=tam;vertices[1].y=-tam;vertices[1].z=tam;
  vertices[2].x=tam;vertices[2].y=tam;vertices[2].z=tam;
  vertices[3].x=-tam;vertices[3].y=tam;vertices[3].z=tam;
  vertices[4].x=-tam;vertices[4].y=-tam;vertices[4].z=-tam;
  vertices[5].x=tam;vertices[5].y=-tam;vertices[5].z=-tam;
  vertices[6].x=tam;vertices[6].y=tam;vertices[6].z=-tam;
  vertices[7].x=-tam;vertices[7].y=tam;vertices[7].z=-tam;

  // triangulos
  caras.resize(12);
  caras[0]._0=0;caras[0]._1=1;caras[0]._2=3;
  caras[1]._0=3;caras[1]._1=1;caras[1]._2=2;
  caras[2]._0=1;caras[2]._1=5;caras[2]._2=2;
  caras[3]._0=5;caras[3]._1=6;caras[3]._2=2;
  caras[4]._0=5;caras[4]._1=4;caras[4]._2=6;
  caras[5]._0=4;caras[5]._1=7;caras[5]._2=6;
  caras[6]._0=0;caras[6]._1=7;caras[6]._2=4;
  caras[7]._0=0;caras[7]._1=3;caras[7]._2=7;
  caras[8]._0=3;caras[8]._1=2;caras[8]._2=7;
  caras[9]._0=2;caras[9]._1=6;caras[9]._2=7;
  caras[10]._0=0;caras[10]._1=1;caras[10]._2=4;
  caras[11]._0=1;caras[11]._1=5;caras[11]._2=4;  

  srand((unsigned) time(0));
  //colores_cara.resize(12);
  for(int i = 0; i < (int) caras.size(); i++){
    colores_Aleatorios auxiliar;
    auxiliar.r = rand() % 255;
    auxiliar.g = rand() % 255;
    auxiliar.b = rand() % 255;
    colores_cara.push_back(auxiliar);
  }
}


//*************************************************************************
// clase piramide
//*************************************************************************

_piramide::_piramide(float tam, float al)
{

//vertices 
vertices.resize(5); 
vertices[0].x=-tam;vertices[0].y=0;vertices[0].z=tam;
vertices[1].x=tam;vertices[1].y=0;vertices[1].z=tam;
vertices[2].x=tam;vertices[2].y=0;vertices[2].z=-tam;
vertices[3].x=-tam;vertices[3].y=0;vertices[3].z=-tam;
vertices[4].x=0;vertices[4].y=al;vertices[4].z=0;

caras.resize(6);
caras[0]._0=0;caras[0]._1=1;caras[0]._2=4;
caras[1]._0=1;caras[1]._1=2;caras[1]._2=4;
caras[2]._0=2;caras[2]._1=3;caras[2]._2=4;
caras[3]._0=3;caras[3]._1=0;caras[3]._2=4;
caras[4]._0=3;caras[4]._1=1;caras[4]._2=0;
caras[5]._0=3;caras[5]._1=2;caras[5]._2=1;

  srand((unsigned) time(0));
  for(int i = 0; i < (int) caras.size(); i++){
    colores_Aleatorios auxiliar;
    auxiliar.r = rand() % 255;
    auxiliar.g = rand() % 255;
    auxiliar.b = rand() % 255;
    colores_cara.push_back(auxiliar);
  }
}

//*************************************************************************
// clase objeto ply
//*************************************************************************


_objeto_ply::_objeto_ply() 
{
   // leer lista de coordenadas de vértices y lista de indices de vértices
 
}



int _objeto_ply::parametros(char *archivo)
{
int n_ver,n_car;

vector<float> ver_ply ;
vector<int>   car_ply ;
int i;

_file_ply::read(archivo, ver_ply, car_ply );

n_ver=ver_ply.size()/3;
n_car=car_ply.size()/3;

printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

vertices.resize(n_ver);
caras.resize(n_car);

// Inicio codigo restante propio
for(i = 0; i < n_ver; i++){
  vertices[i].x = ver_ply[3*i];
  vertices[i].y = ver_ply[3*i+1];
  vertices[i].z = ver_ply[3*i+2];
}

for(i = 0; i < n_car; i++){
  caras[i]._0 = car_ply[3*i];
  caras[i]._1 = car_ply[3*i+1];
  caras[i]._2 = car_ply[3*i+2];
}

// Fin codigo restante propio

// Modo ajedrez diferentes colores
srand((unsigned) time(0));
for(int i = 0; i < (int) caras.size(); i++){
  colores_Aleatorios auxiliar;
  auxiliar.r = rand() % 255;
  auxiliar.g = rand() % 255;
  auxiliar.b = rand() % 255;
  colores_cara.push_back(auxiliar);
}

return(0);

}


//************************************************************************
// objeto por revolucion
//************************************************************************

_rotacion::_rotacion()
{

}


void _rotacion::parametros(vector<_vertex3f> perfil, int num, int tipo)
{
int i,j;
_vertex3f vertice_aux;
_vertex3i cara_aux;
int num_aux;
float radio, altura;

if(tipo == 2)
  radio = sqrt(perfil[0].x*perfil[0].x + perfil[0].y*perfil[0].y);

if(tipo==3){
  radio = sqrt(perfil[0].x*perfil[0].x + perfil[0].y*perfil[0].y);
}




// tratamiento de los vértice

num_aux=perfil.size();
   
if(tipo == 1){
  num_aux=1;
  altura=perfil[1].y;
}


vertices.resize(num_aux*num + 2);

if(tipo != 3){
  for (j=0;j<num;j++)
  {for (i=0;i<num_aux;i++)
     {
      vertice_aux.x=perfil[i].x*cos(2.0*M_PI*j/(1.0*num))+
                    perfil[i].z*sin(2.0*M_PI*j/(1.0*num));
      vertice_aux.z=-perfil[i].x*sin(2.0*M_PI*j/(1.0*num))+
                    perfil[i].z*cos(2.0*M_PI*j/(1.0*num));
      vertice_aux.y=perfil[i].y;
      vertices[i+j*num_aux]=vertice_aux;
     }
  }
  
}else{
  for (j=0;j<num;j++){
    for (i=0;i<num_aux;i++){
      vertice_aux.x=perfil[i].x*cos(2.0*M_PI*j/(1.0*num))+
                    perfil[i].z*sin(2.0*M_PI*j/(1.0*num));
      vertice_aux.z=-perfil[i].x*sin(2.0*M_PI*j/(1.0*num))+
                    perfil[i].z*cos(2.0*M_PI*j/(1.0*num));
      vertice_aux.y=perfil[i].y;
      vertices[i+j*num_aux]=vertice_aux;
    }
  }
}

// tratamiento de las caras 
// num es el numero de lados, num_aux, la cantidad de puntos
// el total de caras es la suma de los laterales mas las tapas

caras.resize((num_aux - 1) * 2 * num + 2 * num);

     
// Esta parte pinta los triangulos de las caras menos la que conecta el 
// ultimo punto con el inicial  
int c = 0;

if(tipo == 0 || tipo == 2){

  for ( j = 0; j < num; j++){
    for(i = 0; i < num_aux -1; i++){
      caras[c]._0 = j*num_aux+i;
      caras[c]._1 = j * num_aux+i+1; 
      caras[c]._2 = ((j+1)%num)*num_aux+i+1;
      c=c+1;
      caras[c]._0=((j+1)%num)*num_aux+i+1;
      caras[c]._1=((j+1)%num)*num_aux+i;
      caras[c]._2=j*num_aux+i;
      c=c+1;
    }
  }

}

if(tipo==3){
  for ( j = 0; j < num; j++){
    for(i = 0; i < num_aux -1; i++){
      caras[c]._0 = j*num_aux+i;
      caras[c]._1 = j * num_aux+i+1; 
      caras[c]._2 = ((j+1)%num)*num_aux+i+1;
      c=c+1;
      caras[c]._0=((j+1)%num)*num_aux+i+1;
      caras[c]._1=((j+1)%num)*num_aux+i;
      caras[c]._2=j*num_aux+i;
      c=c+1;
    }
  }
}


// cout << caras[c]._0 << " " << caras[c]._1 << " " << caras[c]._0 << " " << endl;

 // tapa inferior
if (fabs(perfil[0].x)>0.0)
  {  
    vertices[num_aux*num].x=0.0; 
    if (tipo==0) vertices[num_aux*num].y=perfil[0].y; 
    if (tipo==1) vertices[num_aux*num].y=0.0;
    if (tipo==2) vertices[num_aux*num].y=-radio;
    if (tipo==3) vertices[num_aux*num].y=-radio;

    vertices[num_aux*num].z=0.0;


    for ( j = 0; j < num; j++){
      caras[c]._0=num_aux*num;
      caras[c]._1=j*num_aux+0;
      caras[c]._2=((j+1)%num)*num_aux + 0;
      c=c+1;
    }

  }
 
 // tapa superior
 if (fabs(perfil[num_aux-1].x)>0.0)
  {

   vertices[num_aux*num+1].x=0.0; 
   if (tipo==0) vertices[num_aux*num+1].y=perfil[num_aux-1].y; 
   if (tipo==1) vertices[num_aux*num+1].y=altura; 
   if (tipo==2) vertices[num_aux*num+1].y=radio; 
   if (tipo==3) vertices[num_aux*num+1].y=radio; 
   vertices[num_aux*num+1].z=0.0;

   if (tipo==0 || tipo==2){
     for (j=0;j<num;j++)
     {
        caras[c]._0=num_aux*num+1;
        caras[c]._1=j*num_aux + num_aux -1;
        caras[c]._2=((j+1)%num)*num_aux+num_aux-1;
        c=c+1;
      
     }
  }

  if(tipo == 3){
    _vertex3f auxNew1(0.0, 0.0, 0.0);

    vertices.push_back(auxNew1);
    for (j=0;j<num;j++){
        caras[c]._0=num_aux*num+2;
        caras[c]._1=j*num_aux + num_aux -1;
        caras[c]._2=((j+1)%num)*num_aux+num_aux-1;
        c=c+1;
      
    }
  }

  if (tipo==1)
      {for (j=0;j<num;j++)
        {caras[c]._0=num_aux*num+1;
          caras[c]._1=j*num_aux;
          caras[c]._2=((j+1)%num)*num_aux;
          c=c+1;
        }
      }

  }





  srand((unsigned) time(0));
    //colores_cara.resize(12);
    for(int i = 0; i < (int) caras.size(); i++){
      colores_Aleatorios auxiliar;
      auxiliar.r = rand() % 255;
      auxiliar.g = rand() % 255;
      auxiliar.b = rand() % 255;
      colores_cara.push_back(auxiliar);
    }
  
}

//************************************************************************
// Cono
//************************************************************************

_cono::_cono(float radio, float altura, int m)
{
  vector<_vertex3f> perfil1;
  _vertex3f aux;

  aux.x = radio; 
  aux.y = 0.0; 
  aux.z = 0.0;
  perfil1.push_back(aux);
  

  aux.x = 0; 
  aux.y = altura; 
  aux.z = 0.0;
  perfil1.push_back(aux);

  parametros(perfil1, m, 1);
}

// _cono::_cono(){

// }



//************************************************************************
// Esfera
//************************************************************************

// _esfera::_esfera(){

// }

_esfera::_esfera(float radio, int n, int m){
  vector<_vertex3f> perfil1;
  _vertex3f aux;

  for(int i = 1; i < n; i++){
    aux.x = radio*cos(M_PI*i/n - M_PI/2.0);
    aux.y = radio*sin(M_PI*i/n - M_PI/2.0);
    aux.z = 0.0;
    perfil1.push_back(aux);
  }

  parametros(perfil1, m, 2);
}


_media_esfera::_media_esfera(float radio, int n, int m){
  vector<_vertex3f> perfil1;
  _vertex3f aux;

  bool continuar = true;
  if(n < 6){
    n = 6;
  }
  for(int i = 1; i < n && continuar; i++){
    
    
    
      aux.x = radio*cos(M_PI*i/n - M_PI/2.0);
      aux.y = radio*sin(M_PI*i/n - M_PI/2.0);
      aux.z = 0.0;
      perfil1.push_back(aux);

      if(M_PI*i/n - M_PI/2.0 == 0){
        continuar = false;
      }
    
  }

  // perfil1.erase(perfil1.begin() + 2);

  parametros(perfil1, m, 3);
}


// _cilindro::_cilindro(){
//   // 0.5, 2, 12
//   vector<_vertex3f> perfil1;
//   _vertex3f aux;

//   aux.x = 0.5; 
//   aux.y = -2/2.0; 
//   aux.z = 0.0;
//   perfil1.push_back(aux);

//   aux.x = 0.5; 
//   aux.y = 2/2.0; 
//   aux.z = 0.0;
//   perfil1.push_back(aux);

//   parametros(perfil1, 12, 0);

// }

// _cilindro::_cilindro(float radio=0.5, float altura=2, int m=12 ){
_cilindro::_cilindro(float radio, float altura, int m ){
  vector<_vertex3f> perfil1;
  _vertex3f aux;

  aux.x = radio; 
  aux.y = -altura/2.0; 
  aux.z = 0.0;
  perfil1.push_back(aux);

  aux.x = radio; 
  aux.y = altura/2.0; 
  aux.z = 0.0;
  perfil1.push_back(aux);

  parametros(perfil1, m, 0);
}



_brazo_izq::_brazo_izq(){
  // altura = 1.0;
  // anchura = 0.1;
  // cilindro.set(0.5, 2, 12);
}

void _brazo_izq::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor){
  // // Brazo izquierdo
  // glPushMatrix();
  // glRotatef(90.0,0,0,1);
  // glScalef(this -> altura, this->anchura, 0.1);
  // cubo.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  // glPopMatrix();

  // // Brazo derecho
  // glPushMatrix();
  // glTranslatef(1.1,0,0);
  // glRotatef(90.0,0,0,1);
  // glScalef(this -> altura, this->anchura, 0.1);
  // cubo.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  // glPopMatrix();

  glPushMatrix();
  glTranslatef(0.125,0.2,0);
  glRotatef(90.0,0,0,1);
  glScalef(0.05, 0.1, 0.05);
  cilindro.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

  // brazo izq
  glPushMatrix();
  //glRotatef(90.0,0,0,1);
  glScalef(0.2, 0.9, 0.2);
  cilindro.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

  
}

_brazo_dch::_brazo_dch(){
  // altura = 1.0;
  // anchura = 0.1;
  // cilindro.set(0.5, 2, 12);
}

void _brazo_dch::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor){
  

  // brazo dch
  glPushMatrix();
  glTranslatef(1.3,0,0);
  //glRotatef(90.0,0,0,1);
  glScalef(0.2, 0.9, 0.2);
  cilindro.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

   glPushMatrix();
  glTranslatef(1.20,0.2,0);
  glRotatef(90.0,0,0,1);
  glScalef(0.05, 0.1, 0.05);
  cilindro.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();
}

_cabeza::_cabeza(){

}

void _cabeza::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor){
  glPushMatrix();
  glTranslatef(0, 0.6,0.0);
  glRotatef(180, 1, 0, 0);
  media_esfera.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-0.41, 0.975, 0);
  glRotatef(45, 0, 0, 1);
  glScalef(0.05, 0.18, 0.05);
  cilindro.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(+0.41, 0.975, 0);
  glRotatef(45+90, 0, 0, 1);
  glScalef(0.05, 0.18, 0.05);
  cilindro.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();
}

_tronco::_tronco(){
  brazo_iq_mv = 0;
  brazo_d_mv = 0;
 
}

void _tronco::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor){
  glPushMatrix();
  glTranslatef(0,-0.05,0);
  glRotatef(head,0,1,0);
  glScalef(1, 1.0, 0.8);
  cabeza.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.65-0.65,0,0);
  // glRotatef(90.0,0,0,1);
  glScalef(1, 1.0, 0.8);
  cilindro.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-0.65,0,0);
  glTranslatef(-0.1250, +0.2, 0.0);
  glRotatef( brazo_iq_mv,1,0.0,0);
  glTranslatef(+0.1250, -0.2, 0.0);
  brazo_izq.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-0.65,0,0);
  glTranslatef(-1.2, +0.2, 0.0);
  glRotatef( brazo_d_mv,1,0.0,0);
  glTranslatef(1.2, -0.2, 0.0);
  brazo_dch.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();
}

void _tronco::move_left(float a){
  this -> brazo_iq_mv = a;
}

void _tronco::move_right(float a){
  this -> brazo_d_mv = a;
}

void _tronco::move_head(float a){
  this -> head = a;
}


_pierna_dch::_pierna_dch(){
  // altura = 1.0;
  // anchura = 0.1;
  // cilindro.set(0.5, 2, 12);
}

void _pierna_dch::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor){
  
  glPushMatrix();
  glTranslatef(0.85-0.65,-0.8,0);
  // glRotatef(90.0,0,0,1);
  glScalef(0.25, 0.4, 0.25);
  cilindro.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.85-0.65,-1.0,0);
  // glRotatef(90.0,0,0,1);
  glScalef(0.25, 0.25, 0.25);
  esfera.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();


}

_pierna_izq::_pierna_izq(){
  // altura = 1.0;
  // anchura = 0.1;
  // cilindro.set(0.5, 2, 12);
}

void _pierna_izq::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor){
  glPushMatrix();
  glTranslatef(0.45-0.65,-0.8,0);
  // glRotatef(90.0,0,0,1);
  glScalef(0.25, 0.4, 0.25);
  cilindro.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.45-0.65,-1.0,0);
  // glRotatef(90.0,0,0,1);
  glScalef(0.25, 0.25, 0.25);
  esfera.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

}

_tronco_inferior::_tronco_inferior(){
  // altura = 1.0;
  // anchura = 0.1;
  // cilindro.set(0.5, 2, 12);
}

void _tronco_inferior::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor){
  
  glPushMatrix();
  pierna_izq.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

  glPushMatrix();
  pierna_dch.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

}



_final::_final(){
  brazo_iq_mv = 0;
  brazo_d_mv = 0;
  torso_rot = 0;
  max_torso = 45;
  inclinar = 0;
  inclinar_max = 25;
  brazo_max = 95;
  brazo_min = -5;

  rotar_cabeza = 0;
  rotar_cabeza_max = 45;
}

void _final::reset(){
  brazo_iq_mv = 0;
  brazo_d_mv = 0;
  torso_rot = 0;
  max_torso = 45;
  inclinar = 0;
  inclinar_max = 25;
  brazo_max = 95;
  brazo_min = -5;

  rotar_cabeza = 0;
  rotar_cabeza_max = 45;
}

void _final::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor){

  // *********** Torso ***********  
  glPushMatrix();
  glTranslatef(0.0, 0.45, 0);
  glTranslatef(0.0, -0.3, 0.0);
  glRotatef(inclinar, 1, 0, 0);
  glRotatef( torso_rot,0,1,0);
  glTranslatef(0.0, 0.3, 0.0);
  glScalef(0.5, 0.5, 0.5);
  tronco.draw(modo, r1, g1, b1, r2, g2, b2, grosor);

  // Si se mueve los brazos, transmitir al tronco la instruccion de mover el tronco
  tronco.move_left(brazo_iq_mv);
  tronco.move_right(brazo_d_mv);
  tronco.move_head(rotar_cabeza);
  glPopMatrix();





  // *********** Piernas *********** 
  glPushMatrix();
  glTranslatef(0.0, 0.55, 0);
  // glTranslatef(-0.65, -0.0, 0.0);
  glScalef(0.5, 0.5, 0.5);
  tronco_inferior.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();



  glPushMatrix();
  glTranslatef(0.0, 0.5, 0);
  paisaje.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();
}

_tronco_arbol::_tronco_arbol(){

}

void _tronco_arbol::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor){
  glPushMatrix();
  glTranslatef(0, -0.2, 0);
  glScalef(0.2,0.5,0.2);
  cilindro.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();
}

_arbol::_arbol(){

}

void _arbol::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor){
  glPushMatrix();
  glScalef(1, 1.4, 1);
  cono.draw(modo, 51/255.0, 102/255.0, 0/255.0, r2, g2, b2, grosor);
  glPopMatrix();
  
  glPushMatrix();
  tronco.draw(modo, 77/255.0, 51/255.0, 25/255.0, r2, g2, b2, grosor);
  glPopMatrix();
}


_paisaje::_paisaje(){

}

void _paisaje::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor){


  float aux_d = 2.1;

  glPushMatrix();
  glTranslatef(1.0, -0.3, 1.2);
  glScalef(1/aux_d, 1/aux_d, 1/aux_d);
  arbol.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.5, -0.4, 1.2);
  glScalef(1/4.0, 1/4.0, 1/4.0);
  arbol.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-1, -0.4, 1.2);
  glScalef(1/4.0, 1/4.0, 1/4.0);
  arbol.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();


  glPushMatrix();
  glTranslatef(-1,-0.4, 5.4);
  glScalef(1/4.0, 1/4.0, 1/4.0);
  arbol.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

  aux_d = 4.1;

  glPushMatrix();
  glTranslatef(11.5, -0.41, 1.5);
  glScalef(1/aux_d, 1/aux_d, 1/aux_d);
  arbol.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

  aux_d = 3.1;

  glPushMatrix();
  glTranslatef(-1.5,-0.4, 4.56);
  glScalef(1/aux_d, 1/aux_d, 1/aux_d);
  arbol.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

  aux_d = 4.1;

  glPushMatrix();
  glTranslatef(1.9,-0.4, 2.56);
  glScalef(1/aux_d, 1/aux_d, 1/aux_d);
  arbol.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();


  aux_d = 5.3;

  glPushMatrix();
  glTranslatef(1.2, -0.4, 5.38);
  glScalef(1/aux_d, 1/aux_d, 1/aux_d);
  arbol.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();


  aux_d = 3.3;

  glPushMatrix();
  glTranslatef(-1.5, -0.4, 6.76);
  glScalef(1/aux_d, 1/aux_d, 1/aux_d);
  arbol.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

  // srand((unsigned) time(0));
  // float scaleeee;
  // for(int i = 0; i < 25; i++){
  //   glPushMatrix();
    
  //   glTranslatef(1.6 + (rand() + 0.1) + 2.5, -0.1, 0 - (rand()%10 + 0.1));
  //   scaleeee = rand()%5 * 1.0 + 0.1;
  //   glScalef(1/scaleeee, 1/scaleeee, 1/scaleeee);
  //   arbol.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  //   glPopMatrix();
  // }


  glPushMatrix();
  
  glTranslatef(0, -3.0,0);
  glScalef(50, 5, 50);
  cubo.draw(modo, 51/255.0, 26/255.0, 0/255.0, r2, g2, b2, grosor);
  glPopMatrix();

}


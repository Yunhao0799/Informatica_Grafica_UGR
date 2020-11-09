//**************************************************************************
// Práctica 1 usando objetos
//**************************************************************************

#include "objetos_B2.h"
#include "file_ply_stl.hpp"
#include <math.h>

// struct colores_Aleatorios{
// 	int r, g, b;
// };


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

_triangulos3D::_triangulos3D()
{
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
// glLineWidth(grosor);
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
// glLineWidth(grosor);
// glColor3f(r,g,b);
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
  // glLineWidth(grosor);
  // glColor3f(r,g,b);
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
	}
}

//*************************************************************************
// clase cubo
//*************************************************************************

_cubo::_cubo(float tam)
{
  //lista de vertices
  vertices.resize(8);

  vertices[0].x=   0;vertices[0].y=0;vertices[0].z=tam;
  vertices[1].x= tam;vertices[1].y=0;vertices[1].z=tam;
  vertices[2].x= tam;vertices[2].y=0;vertices[2].z=  0;
  vertices[3].x=   0;vertices[3].y=0;vertices[3].z=  0;

  vertices[4].x=   0;vertices[4].y=tam;vertices[4].z=tam;
  vertices[5].x= tam;vertices[5].y=tam;vertices[5].z=tam;
  vertices[6].x= tam;vertices[6].y=tam;vertices[6].z=  0;
  vertices[7].x=   0;vertices[7].y=tam;vertices[7].z=  0;

  // triangulos
  caras.resize(12);

  // Caras 0, 1, 5, 4
  caras[0]._0=0;caras[0]._1=1;caras[0]._2=4;
  caras[1]._0=1;caras[1]._1=5;caras[1]._2=4;

  // Caras 4, 5, 6, 7
  caras[2]._0=4;caras[2]._1=6;caras[2]._2=7;
  caras[3]._0=4;caras[3]._1=5;caras[3]._2=6;

  // Caras 1, 2, 6, 5
  caras[4]._0=1;caras[4]._1=2;caras[4]._2=6;
  caras[5]._0=1;caras[5]._1=6;caras[5]._2=5;

  // Caras 0, 3, 2, 1
  caras[6]._0=0;caras[6]._1=3;caras[6]._2=2;
  caras[7]._0=0;caras[7]._1=2;caras[7]._2=1;

  // Caras 3, 7, 6, 2
  caras[8]._0=3;caras[8]._1=7;caras[8]._2=6;
  caras[9]._0=3;caras[9]._1=6;caras[9]._2=2;

  // Caras 0, 4, 7, 3
  caras[10]._0=0;caras[10]._1=4;caras[10]._2=3;
  caras[11]._0=3;caras[11]._1=4;caras[11]._2=7;

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
//colores_cara.resize(12);
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


void _rotacion::parametros(vector<_vertex3f> perfil, int num)
{
int i,j;
_vertex3f vertice_aux;
_vertex3i cara_aux;
int num_aux;

// tratamiento de los vértice

num_aux=perfil.size();
vertices.resize(num_aux*num + 2);
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

// tratamiento de las caras 
// num es el numero de lados, num_aux, la cantidad de puntos
// el total de caras es la suma de los laterales mas las tapas

caras.resize((num_aux - 1) * 2 * num + 2 * num);
     
// Esta parte pinta los triangulos de las caras menos la que conecta el 
// ultimo punto con el inicial  
int c = 0;
for ( j = 0; j < num - 1; j++){
  caras[c]._0 = j*2;
  caras[c]._1 = j * 2 + 1;
  caras[c]._2 = (j+1)*2+1;
  c=c+1;
  caras[c]._0=(j+1)*2+1;
  caras[c]._1=(j+1)*2;
  caras[c]._2=j*2;
  c=c+1;
}

// cout << caras[c]._0 << " " << caras[c]._1 << " " << caras[c]._0 << " " << endl;

 // tapa inferior
if (fabs(perfil[0].x)>0.0)
  {
    vertices[num_aux*num].x = 0.0;
    vertices[num_aux*num].y = perfil[0].y;
    vertices[num_aux*num].z = 0.0;

    for ( j = 0; j < num - 1; j++){
      caras[c]._0=num_aux*num;
      caras[c]._1=j*2;
      caras[c]._2=(j+1)*2;
      c=c+1;
    }

  }
 
 // tapa superior
 if (fabs(perfil[num_aux-1].x)>0.0)
  {
    vertices[num_aux*num+1].x = 0.0;
    vertices[num_aux*num+1].y = perfil[num_aux-1].y;
    vertices[num_aux*num+1].z = 0.0;

    for ( j = 0; j < num - 1; j++){
      caras[c]._0=num_aux*num+1;
      caras[c]._1=j*2+1;
      caras[c]._2=(j+1)*2+1;
      c=c+1;
    }
  }

  // Crear aqui el ultimo lado que falta, y la ultima cara qeu falta 
  // de las tapas superiores e inferiores

  // Lado lateral
  caras[c]._0= num_aux*num-2;
  caras[c]._1= num_aux*num-1;
  caras[c]._2= 1;
  c=c+1;

  caras[c]._0 = 1;
  caras[c]._1 = 0;
  caras[c]._2 = num_aux*num-2;
  c=c+1;

  // Cerrar la cara de arriba
  caras[c]._0=num_aux*num+1;
  caras[c]._1=num_aux*num-1;
  caras[c]._2=1;
  c=c+1;

  // Cerrar la cara de abajo
  caras[c]._0=num_aux*num;
  caras[c]._1=num_aux*num-2;
  caras[c]._2=0;
  c=c+1;

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

_cono::_cono()
{

}

void _cono::parametros(vector<_vertex3f> perfil, int num){
  int i,j;
  _vertex3f vertice_aux;
  _vertex3i cara_aux;
  int num_aux;

  // tratamiento de los vértice
  num_aux=perfil.size();
  // +2 al final para el punto central de arriba y abajo
  vertices.resize(num_aux*num + 2);
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

  // Pintar el punto central de arriba
  vertices[num_aux*num].x = 0.0;
  // Cojo el valor absoluto ya que lo quiero poner mirando hacia
  // arriba, y mi perfil tiene los puntos abajo
  vertices[num_aux*num].y = fabs(perfil[num_aux-1].y);
  vertices[num_aux*num].z = 0.0;

  // Tratamiento de las caras 
  caras.resize((num_aux - 1)*2*num + 2*num );
  int c = 0;
  for(j = 0; j < num - 1; j++){
    caras[c]._0 = j;
    caras[c]._1 = num*num_aux;
    caras[c]._2 = j+1;
    c=c+1;
     
  }

  // Pintamos el lado lateral que falta
  caras[c]._0=num_aux*num - 1;
  caras[c]._1=num_aux*num;
  caras[c]._2=0;
  c = c+1; 

  // Ahora falta por pintar la base, para ello precisaremos de crear
  // el punto central
  vertices[num_aux*num + 1].x = 0.0;
  vertices[num_aux*num + 1].y = perfil[0].y;
  vertices[num_aux*num + 1].z = 0.0;

  // Tratamiento de las caras de la base
  for(i = 0; i < num - 1; i++){
    caras[c]._0 = i;
    caras[c]._1 = i+1;
    caras[c]._2 = num*num_aux + 1;
    c=c+1;
  }

  // Cerrar la cara que falta de la base
  caras[c]._0 = num - 1;
  caras[c]._1 = 0;
  caras[c]._2 = num*num_aux + 1;
  c=c+1;  

  srand((unsigned) time(0));
  //colores_cara.resize(12);
  for(i = 0; i < (int) caras.size(); i++){
    colores_Aleatorios auxiliar;
    auxiliar.r = rand() % 255;
    auxiliar.g = rand() % 255;
    auxiliar.b = rand() % 255;
    colores_cara.push_back(auxiliar);
  }
}


//************************************************************************
// Esfera
//************************************************************************

// _esfera::_esfera()
// {

// }

// void _esfera::parametros(vector<_vertex3f> perfil, int num, int radio_size){
//   int i, j;
//   _vertex3f vertice_aux;
//   _vertex3i cara_aux;
//   int num_aux;
//   // Generacionde puntos verticales
//   int vertical_points = num - 2;
//   // // vector<_vertex3f> starting_points;

//   // // Generar los puntos verticales iniciales
//   // // Formula circulo (x-h)² + (y-h)² = r²


//   // Tratamiento de los vertices
//   num_aux = num;
//   vertices.resize(num*num + 3);
//   // vertice_aux.x = 0.0;
//   // vertice_aux.y = 1.0;
//   // vertice_aux.z = 0.0; 
//   // vertices.push_back(vertice_aux);

//   double aux_point_x = 0;
//   // Generamos los puntos verticales

//   for(i = 1; i < vertical_points/2; i++){
//     aux_point_x = (i)*((perfil[0].x + radio_size)/((vertical_points*0.5)));
//     // Calculamos la x 
//     vertice_aux.x = aux_point_x;

//     // Calculamos ahora la y asociada
//     vertice_aux.y =  (perfil[0].y - sqrt(pow(radio_size,2) - pow((aux_point_x - perfil[0].x),2)));

//     // La z inicialmente sera 0
//     vertice_aux.z = 0.0; 
//     vertices[i - 1] = vertice_aux;
//   }


//   for(i = 1; i < vertical_points/2 ; i++){
//     aux_point_x = (i)*((perfil[0].x + radio_size)/(vertical_points*0.5));
//     // Calculamos la x 
//     vertice_aux.x = aux_point_x;

//     // Calculamos ahora la y asociada
//     vertice_aux.y =  fabs(perfil[0].y - sqrt(pow(radio_size,2) - pow((aux_point_x - perfil[0].x),2)));

//     // La z inicialmente sera 0
//     vertice_aux.z = 0.0; 
//     vertices[i + vertical_points/2 - 1] = vertice_aux;
//   }

//     // aux_point_x = 0.9;
//     // // Calculamos la x 
//     // vertice_aux.x = aux_point_x;

//     // // Calculamos ahora la y asociada
//     // vertice_aux.y =  (perfil[0].y - sqrt(pow(radio_size,2) - pow((aux_point_x - perfil[0].x),2)));

//     // // La z inicialmente sera 0
//     // vertice_aux.z = 0.0; 
//     // vertices.push_back(vertice_aux);





//   for(j = 0; j < num; j++){
//     for(i = 0; i < num_aux; i++){
//       vertice_aux.x=vertices[i].x*cos(2.0*M_PI*j/(1.0*num))+
//                     vertices[i].z*sin(2.0*M_PI*j/(1.0*num));

//       vertice_aux.y=vertices[i].y;

//       vertice_aux.z=-vertices[i].x*sin(2.0*M_PI*j/(1.0*num))+
//                     vertices[i].z*cos(2.0*M_PI*j/(1.0*num));
           
//       vertices[i+j*num_aux]=vertice_aux;

//     }
//   }




//   caras.resize((vertical_points - 1) * 2 * num + 2 * num);

//   int c = 0;
//   for ( j = 0; j < num - 1; j++){
//     caras[c]._0 = j*2;
//     caras[c]._1 = j * 2 + 1;
//     caras[c]._2 = (j+1)*2+1;
//     c=c+1;
//     caras[c]._0=(j+1)*2+1;
//     caras[c]._1=(j+1)*2;
//     caras[c]._2=j*2;
//     c=c+1;
//   }



//   //   // Adding top and botton point

//   // // top
//   // vertice_aux.x = 0.0;
//   // vertice_aux.y = radio_size;
//   // vertice_aux.z = 0.0; 
//   // vertices.push_back(vertice_aux);

//   // // botton
//   // vertice_aux.x = 0.0;
//   // vertice_aux.y = - radio_size;
//   // vertice_aux.z = 0.0; 
//   // vertices.push_back(vertice_aux);
// }

_esfera::_esfera(){


}


_esfera::_esfera(float radio, int n, int m){

}
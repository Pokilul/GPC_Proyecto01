/*
Graficos por Computadora
Nombre: Proyecto 1
Nombre del archivo: main.c
Fecha:  22-10-2022
Integrantes:
  Amavizca Cuellar Elizabeth / Morales Flores Gael Alexis / Manriquez Cobian Rogelio

Objetivo: Implementar una aplicación de dibujo por computadora de elementos geométricos básicos
          usando Lenguaje C y el estándar gráfico de OpenGL.
Compilar:
  gcc -c main.c
Enlazar:
  gcc main.o -o run -l glut32 -l opengl32
Ejecutar:
  ./run
*/

// IMPORTACIÓN DE LIBRERÍAS UTILIZADAS EN EL PROGRAMA.
#include<gl\glut.h>
#include<math.h>
#include<stdio.h>
#include <windows.h>
#define tamVentana 1000

// DEFINICIÓN DE PALETA DE COLORES
typedef enum{NEGRO, AZUL, VERDE, CYAN, ROJO, MAGENTA, AMARILLO, BLANCO, GRIS,
             GRIS_OSCURO, GRIS_CLARO}COLOR;

float PALETA_COLORES[11][3] = { 
  0, 0, 0, //NEGRO
  0, 0, 1, //AZUL
  0, 1, 0, //VERDE
  0, 1, 1, //CYAN
  1, 0, 0, //ROJO
  1, 0, 1, //MAGENTA
  1, 1, 0, //AMARILLO
  1, 1, 1, //BLANCO
  0.5,0.5,0.5, //GRIS
  0.25,0.25,0.25, //GRIS_OSC
  0.8,0.8,0.8 //GRIS_CLARO
}; 

// DEFINICIÓN DE ESTRUCTURAS PARA FIGURAS
typedef struct{
  float x;
  float y;
}PUNTO;

typedef struct{
  PUNTO ini;
  PUNTO fin;
  char gui;
  char tipoLinea;
  unsigned int grosor;
  float angulo;
  COLOR color;
}LINEA; 

typedef struct{
  PUNTO piv;
  float lado;
  char gui;
  char tipoLinea;
  unsigned int grosor;
  float angulo;
  COLOR color;
}CUADRADO;

typedef struct{
  PUNTO piv;
  float ancho;
  float alto;
  char gui;
  char tipoLinea;
  unsigned int grosor;
  float angulo;
  COLOR color;
}RECTANGULO;

typedef struct{
  PUNTO c;
  float radio;
  char gui;
  char tipoLinea;
  unsigned int grosor;
  float angulo;
  COLOR color;
}CIRCULO;

typedef struct{
  PUNTO c;
  float radio;
  int numLados;
  char gui;
  char tipoLinea;
  unsigned int grosor;
  float angulo;
  COLOR color;
}POLIGONO;

typedef struct{
  PUNTO c;
  float eje_mayor;
  float eje_menor;
  char gui;
  char tipoLinea;
  unsigned int grosor;
  float angulo;
  COLOR color;
}ELIPSE;

typedef struct{ 
  PUNTO piv;
  float base;
  float altura;
  char gui;
  char tipoLinea;
  unsigned int grosor;
  float angulo;
  COLOR color;
}TRIANGULO_ISO;

typedef struct{ 
  PUNTO piv;
  float base;
  float altura;
  char gui;
  char tipoLinea;
  unsigned int grosor;
  float angulo;
  COLOR color;
}TRIANGULO_RECT;

// FUNCIONES DE DIBUJADO DE FIGURAS
void linea(LINEA L);
void cuadrado(CUADRADO C);
void rectangulo(RECTANGULO R);
void poligono(POLIGONO P);
void circulo(CIRCULO CI);
void elipse(ELIPSE E);
void trianguloIso(TRIANGULO_ISO T);
void trianguloRect(TRIANGULO_RECT T);

// FUNCIONES RATÓN
void raton(int boton, int estado, int x, int y);
void ratonPasivo(int x, int y);
void ratonActivo(int x, int y);
void clicIzquierdo(PUNTO pt);
void clicDerecho(float x_, float y_);

// FUNCIONES DEL PLANO
void dibujarTexto(char *Texto, float x, float y);
void AsignaColor(COLOR c);
void display(void);

// FUNCIONES GENERALES
PUNTO CVent_a_Cplano(int x, int y,int AnchoV, int AltoV, float xini, float xfin, float yini, float yfin);
void cambiarColor(COLOR color);
void mostrarFiguras(void);
void borrarFiguras(void);
void borrarFigura(void);
void figurasGUI(void);
void mostrarGUI(void);

// FUNCIONES PARA TECLADO
void teclasNormales(unsigned char tecla, int x, int y);
void teclasEspeciales(int tecla, int x, int y);
void teclaIzquierda(void);
void teclaDerecha(void);
void teclaArriba(void);
void teclaAbajo(void);
void teclaL(void);
void masGrosor(void);
void menosGrosor(void);

/* VARIABLES PARA MANIPULACIÓN DE FIGURAS

  seleccionFigura values:
    0 -> Ninguna
    1 -> Línea
    2 -> Cuadrado
    3 -> Rectángulo
    4 -> Polígono
    5 -> Círculo
    6 -> Elipse
    7 -> Triángulo isóceles
    8 -> Triángulo rectángulo
*/
const float pi = 3.14159;
const float escalaArriba = 1.1,
            escalaAbajo = 0.9,
            angulo = 10;  
unsigned int seleccionFigura = 0;    
char coordenadas[40] = "(0,0)";
char primerPunto[8] = {1, 1, 1, 1, 1, 1, 1, 1};
char puntoFinal = 0;

// PUNTOS DE REFERENCIA
PUNTO puntoInicial;

// FIGURAS GEOMÉTRICAS PARA MANIPULAR
LINEA L, gL, gL1, gL2, gL3;
CUADRADO CU, gCU, gCU1;
RECTANGULO R, gR;
POLIGONO P, gP;
CIRCULO CI, gCI;
ELIPSE E, gE;
TRIANGULO_ISO TI, gTI;
TRIANGULO_RECT TR, gTR;

// FUNCIÓN MAIN
int main(int argc, char **argv){
  glutInit(&argc,argv);
  glutInitWindowSize(tamVentana, tamVentana);
  glutCreateWindow("Proyecto 1: aplicacion de dibujo asistido por computadora");
  // CONFIGURACIÓN FIGURAS GUI
  figurasGUI();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // DIVISIÓN DEL LIENZO EN 20x20
  glOrtho(-10, 10, -10, 10, 1.0, -1.0); 
  glMatrixMode(GL_MODELVIEW);
  // FUNCIÓN DE DISPLAY
  glutDisplayFunc(display);
  // FUNCIONES PARA EL RATÓN
  glutPassiveMotionFunc(ratonPasivo);
  glutMotionFunc(ratonActivo);
  glutMouseFunc(raton);
  // FUNCIONES PARA EL TECLADO
  glutSpecialFunc(teclasEspeciales);
  glutKeyboardFunc(teclasNormales);
  glutMainLoop();
  return 0;
}
  
void display(void){
  glClearColor(1,1,1,1);
  glClear(GL_COLOR_BUFFER_BIT);
  // MOSTRAR INTERFAZ Y FIGURAS  
  mostrarGUI();
  mostrarFiguras();
  glFlush();  
}

void mostrarGUI(){  
  // FONDO NEGRO PARA BOTONES DE FIGURAS
  AsignaColor(NEGRO);
  // BORDES SUPERIOR E INFERIORES
  glRectf(-10.0, 10.0, 10.0, 8.8);
  glRectf(-10.0, -10.0, -7.8, -9.5);    
  // BOTÓN LÍNEA
  AsignaColor(GRIS_OSCURO);
  glRectf(-9.5, 9.9, -8.5, 8.9);  
  linea(gL);  
  // BOTÓN CUADRADO
  AsignaColor(GRIS_OSCURO);
  glRectf(-8.4, 9.9, -7.4, 8.9);
  cuadrado(gCU);
  // BOTÓN RECTÁNGULO  
  AsignaColor(GRIS_OSCURO);
  glRectf(-7.3, 9.9, -6.3, 8.9);  
  rectangulo(gR);
  // BOTÓN POLÍGONO
  AsignaColor(GRIS_OSCURO);
  glRectf(-6.2, 9.9, -5.2, 8.9);  
  poligono(gP);
  // BOTÓN CÍRCULO
  AsignaColor(GRIS_OSCURO);
  glRectf(-5.1, 9.9, -4.1, 8.9);  
  circulo(gCI);
  // BOTÓN ELIPSE
  AsignaColor(GRIS_OSCURO);
  glRectf(-4.0, 9.9, -3.0, 8.9);  
  elipse(gE);
  // BOTÓN TRIÁNGULO ISÓSCELES
  AsignaColor(GRIS_OSCURO);
  glRectf(-2.9, 9.9, -1.9, 8.9);  
  trianguloIso(gTI);
  // BOTÓN TRIÁNGULO RECTÁNGULO
  AsignaColor(GRIS_OSCURO);
  glRectf(-1.8, 9.9, -0.8, 8.9);  
  trianguloRect(gTR);

  // BOTONES DE BORRADO INDIVIDUAL Y TOTAL
  // BOTÓN BORRADO INDIVIDUAL  
  AsignaColor(GRIS_OSCURO);
  glRectf(8.4, 9.9, 7.4, 8.9);  
  AsignaColor(BLANCO);
  glLineWidth(3.0);
  cuadrado(gCU1);
  linea(gL3);
  glLineWidth(1.0);
  // BOTÓN BORRADO TOTAL  
  AsignaColor(ROJO);
  glRectf(9.5, 9.9, 8.5, 8.9);
  AsignaColor(BLANCO);
  glLineWidth(3.0);
  linea(gL1);
  linea(gL2);
  glLineWidth(1.0);

  // FONDO NEGRO PARA BOTONES DE COLORES
  AsignaColor(NEGRO);
  glRectf(-10.0, 6.0, -9.0, -2.7);
  // BOTÓN 1
  AsignaColor(AZUL);
  glRectf(-9.9, 5.4, -9.1, 4.6);
  // BOTÓN 2
  AsignaColor(VERDE);
  glRectf(-9.9, 4.5, -9.1, 3.7);
  // BOTÓN 3
  AsignaColor(CYAN);
  glRectf(-9.9, 3.6, -9.1, 2.8);
  // BOTÓN 4
  AsignaColor(ROJO);
  glRectf(-9.9, 2.7, -9.1, 1.9);
  // BOTÓN 5
  AsignaColor(MAGENTA);
  glRectf(-9.9, 1.8, -9.1, 1.0);
  // BOTÓN 6
  AsignaColor(AMARILLO);
  glRectf(-9.9, 0.9, -9.1, 0.1);
  // BOTÓN 7
  AsignaColor(GRIS_CLARO);
  glRectf(-9.9, 0.0, -9.1, -0.8);
  // BOTÓN 8
  AsignaColor(GRIS);
  glRectf(-9.9, -0.9, -9.1, -1.7);
  // BOTÓN 9
  AsignaColor(GRIS_OSCURO);
  glRectf(-9.9, -1.8, -9.1, -2.6);
  AsignaColor(BLANCO); 
  dibujarTexto("Color", -9.98, 5.55);
  dibujarTexto("[ Figura seleccionada ]", 0, 9.5);
  switch(seleccionFigura){
    case 0:
      dibujarTexto("[ Ninguna ]", 0, 9.0);      
      break;
    case 1:
      dibujarTexto("[ Linea ]", 0, 9.0);
      break;
    case 2:
      dibujarTexto("[ Cuadrado ]", 0, 9.0);
      break;
    case 3:
      dibujarTexto("[ Rectangulo ]", 0, 9.0);
      break;
    case 4:
      dibujarTexto("[ Poligono ]", 0, 9.0);
      break;
    case 5:
      dibujarTexto("[ Circulo ]", 0, 9.0);
      break;
    case 6:
      dibujarTexto("[ Elipse ]", 0, 9.0);
      break;    
    case 7:
      dibujarTexto("[ Triangulo isosceles ]", 0, 9.0);
      break;
    case 8:
      dibujarTexto("[ Triangulo rectangulo ]", 0, 9.0);
      break;
  }  
  AsignaColor(BLANCO);   
  dibujarTexto(coordenadas, -9.8, -9.9);
}

void figurasGUI(void){
  // ATRIBUTOS LÍNEA
  gL.color = BLANCO;
  gL.ini.x = -9.4;
  gL.ini.y = 9.0;
  gL.fin.x = -8.6;
  gL.fin.y = 9.8;
  gL.gui= 1;
  gL.tipoLinea = 0;
  // LÍNEAS DE BORRAR
  gL1.color = BLANCO;
  gL1.ini.x = 9.4;
  gL1.ini.y = 9.8;
  gL1.fin.x = 8.6;
  gL1.fin.y = 9.0;
  gL1.gui= 1;
  gL2.color = BLANCO;
  gL2.ini.x = 8.6;
  gL2.ini.y = 9.8;
  gL2.fin.x = 9.4;
  gL2.fin.y = 9.0;
  gL2.gui= 1;
  gL3.color = BLANCO;
  gL3.ini.x = 8.3;
  gL3.ini.y = 9.8;
  gL3.fin.x = 7.5;
  gL3.fin.y = 9.0;
  gL3.gui= 1;
  // ATRIBUTOS CUADRADO
  gCU.color = BLANCO;
  gCU.piv.x = -8.3;
  gCU.piv.y = 9.0;
  gCU.lado = 0.8;
  gCU.gui= 1;  
  gCU1.color = BLANCO;
  gCU1.piv.x = 7.5;
  gCU1.piv.y = 9.0;
  gCU1.lado = 0.8;
  gCU1.gui= 1; 
  gCU1.grosor = 3; 
  // ATRIBUTOS RECTÁNGULO
  gR.color = BLANCO;
  gR.piv.x = -7.2;
  gR.piv.y = 9.2;
  gR.ancho = 0.8;
  gR.alto = 0.4;
  gR.gui= 1;
  // ATRIBUTOS POLÍGONO
  gP.color = BLANCO;
  gP.c.x = -5.7;
  gP.c.y = 9.4;
  gP.numLados = 6;
  gP.radio = 0.4;
  gP.gui= 1;
  // ATRIBUTOS CÍRCULO
  gCI.color = BLANCO;
  gCI.c.x = -4.6;
  gCI.c.y = 9.4;
  gCI.radio = 0.4;
  gCI.gui= 1;
  // ATRIBUTOS ELIPSE
  gE.color = BLANCO;
  gE.c.x = -3.5;
  gE.c.y = 9.4;
  gE.eje_mayor = 0.4;
  gE.eje_menor = 0.2;
  gE.gui= 1;
  // ATRIBUTOS TRIÁNGULO ISÓSCELES
  gTI.color = BLANCO;
  gTI.piv.x = -2.7;
  gTI.piv.y = 9.0;
  gTI.altura = 0.8;
  gTI.base = 0.6;
  gTI.gui= 1;
  // ATRIBUTOS TRIÁNGULO RECTÁNGULO
  gTR.color = BLANCO;
  gTR.piv.x = -1.7;
  gTR.piv.y = 9.0;
  gTR.altura = 0.8;
  gTR.base = 0.8;
  gTR.gui= 1;
}

// FUNCIONES DE DIBUJADO DE FIGURAS  

void linea(LINEA L){
  AsignaColor(L.color);
  glPushMatrix();  
  if(!L.gui)
    glRotatef(L.angulo, 0.0f, 0.0f, 1.0f);  
  glBegin(GL_LINE_STRIP);
  glVertex2f(L.ini.x,L.ini.y);
  glVertex2f(L.fin.x,L.fin.y);  
  glEnd();
  glPopMatrix();
}

void cuadrado(CUADRADO C){
  AsignaColor(C.color);  
  glPushMatrix();
  if(!C.gui)
    glRotatef(C.angulo, 0.0f, 0.0f, 1.0f);
  glBegin(GL_LINE_LOOP);  
  glVertex2f(C.piv.x,C.piv.y);
  glVertex2f(C.piv.x+C.lado, C.piv.y);
  glVertex2f(C.piv.x+C.lado, C.piv.y+C.lado);
  glVertex2f(C.piv.x,C.piv.y+C.lado);    
  glEnd(); 
  glPopMatrix(); 
}

void rectangulo(RECTANGULO R){
  AsignaColor(R.color);
  glPushMatrix();
  if(!R.gui)
    glRotatef(R.angulo, 0.0f, 0.0f, 1.0f);
  glBegin(GL_LINE_LOOP);
  glVertex2f(R.piv.x,R.piv.y);
  glVertex2f(R.piv.x+R.ancho, R.piv.y);
  glVertex2f(R.piv.x+R.ancho, R.piv.y +R.alto);
  glVertex2f(R.piv.x,R.piv.y+R.alto);
  glEnd();
  glPopMatrix();
}

void poligono(POLIGONO P){
  AsignaColor(P.color);
  float x, y, theta, Delta;
  Delta = 6.28/P.numLados;
  glPushMatrix();
  if(!P.gui)
    glRotatef(P.angulo, 0.0f, 0.0f, 1.0f);
  glBegin(GL_LINE_LOOP);
  for(theta=0; theta<6.28; theta+=Delta){
    x = P.c.x + P.radio*cos(theta);
    y = P.c.y + P.radio*sin(theta);
    glVertex2f(x,y);
  }
  glEnd();
  glPopMatrix();
}

void circulo(CIRCULO CI){
  AsignaColor(CI.color);
  float x,y,theta;
  glPushMatrix();
  if(!CI.gui)
    glRotatef(CI.angulo, 0.0f, 0.0f, 1.0f);
  glBegin(GL_LINE_LOOP);
  for(theta=0; theta<6.28; theta+=0.1){
    x=CI.c.x+CI.radio*cos(theta);
    y=CI.c.y+CI.radio*sin(theta);
    glVertex2f(x,y);
  }
  glEnd();
  glPopMatrix();
}

void elipse(ELIPSE E){
  AsignaColor(E.color);
  float x, y, theta;
  glPushMatrix();
  if(!E.gui)
    glRotatef(E.angulo, 0.0f, 0.0f, 1.0f);
  glBegin(GL_LINE_LOOP);
  for(theta=0; theta<6.28; theta+=0.1){
    x = E.c.x + E.eje_mayor*cos(theta);
    y = E.c.y + E.eje_menor*sin(theta);
    glVertex2f(x,y);
  }
  glEnd();
  glPopMatrix();
}

void trianguloIso(TRIANGULO_ISO T){
  AsignaColor(T.color);
  glPushMatrix();
  if(!T.gui)
    glRotatef(T.angulo, 0.0f, 0.0f, 1.0f);
  glBegin(GL_LINE_LOOP);
  glVertex2f(T.piv.x,T.piv.y);
  glVertex2f(T.piv.x + T.base, T.piv.y);
  glVertex2f(T.piv.x + T.base/2, T.piv.y + T.altura);
  glEnd();
  glPopMatrix();
}

void trianguloRect(TRIANGULO_RECT T){
  AsignaColor(T.color);
  glPushMatrix();
  if(!T.gui)
    glRotatef(T.angulo, 0.0f, 0.0f, 1.0f);
  glBegin(GL_LINE_LOOP);
  glVertex2f(T.piv.x,T.piv.y);
  glVertex2f(T.piv.x + T.base, T.piv.y);
  glVertex2f(T.piv.x, T.piv.y + T.altura);
  glEnd();
  glPopMatrix();
}

// FUNCIONES DEL RATÓN

void raton(int boton, int estado, int x, int y){
  PUNTO pt = CVent_a_Cplano(x,y,tamVentana,tamVentana,-10,10,-10,10);  
  // AL PRESIONAR EL BOTÓN
  if(estado == GLUT_DOWN)
    switch(boton){
      case GLUT_LEFT_BUTTON:        
        clicIzquierdo(pt);
        break;
      case GLUT_RIGHT_BUTTON:                
        clicDerecho(pt.x, pt.y);        
        break;
     }
  // AL SOLTAR EL BOTÓN
  if(estado == GLUT_UP)
    switch(boton){
      case GLUT_LEFT_BUTTON:
        if(seleccionFigura == 1 && puntoFinal == 0)                      
          puntoFinal = 1;          
        break;      
    }
}

void clicDerecho(float x_, float y_){
  // REGIONES DE FIGURAS GEOMÉTRICAS
  // REGIÓN DE LÍNEA
  if(((x_ >= -9.5)&&(x_ <= -8.5)) && ((y_ >= 8.9)&&(y_ <= 9.9)))    
    seleccionFigura = 1;  
  // REGIÓN DE CUADRADO
  else if(((x_ >= -8.4)&&(x_ <= -7.4)) && ((y_ >= 8.9)&&(y_ <= 9.9)))
    seleccionFigura = 2;
  // REGIÓN DE RECTÁNGULO  
  else if(((x_ >= -7.3)&&(x_ <= -6.3)) && ((y_ >= 8.9)&&(y_ <= 9.9)))
    seleccionFigura = 3;
  // REGIÓN DE POLÍGONO
  else if(((x_ >= -6.2)&&(x_ <= -5.2)) && ((y_ >= 8.9)&&(y_ <= 9.9)))        
    seleccionFigura = 4;  
  // REGIÓN DE CÍRCULO
  else if(((x_ >= -5.1)&&(x_ <= -4.1)) && ((y_ >= 8.9)&&(y_ <= 9.9)))    
    seleccionFigura = 5;
  // REGIÓN DE ELIPSE  
  else if(((x_ >= -4.0)&&(x_ <= -3.0)) && ((y_ >= 8.9)&&(y_ <= 9.9)))    
    seleccionFigura = 6;  
  // REGIÓN DE TRIÁNGULO ISÓSCELES
  else if(((x_ >= -2.9)&&(x_ <= -1.9)) && ((y_ >= 8.9)&&(y_ <= 9.9)))    
    seleccionFigura = 7;
  // REGIÓN DE TRIÁNGULO RECTÁNGULO  
  else if(((x_ >= -1.8)&&(x_ <= -0.8)) && ((y_ >= 8.9)&&(y_ <= 9.9)))    
    seleccionFigura = 8;  
  
  // REGIONES DE COLORES  
  if(((x_ >= -9.9)&&(x_ <= -9.1)) && ((y_ <= 5.4)&&(y_ >= 4.6)))
    cambiarColor(AZUL);      
  else if(((x_ >= -9.9)&&(x_ <= -9.1)) && ((y_ <= 4.5)&&(y_ >= 3.7)))
    cambiarColor(VERDE);    
  else if(((x_ >= -9.9)&&(x_ <= -9.1)) && ((y_ <= 3.6)&&(y_ >= 2.8)))
    cambiarColor(CYAN);      
  else if(((x_ >= -9.9)&&(x_ <= -9.1)) && ((y_ <= 2.7)&&(y_ >= 1.9)))
    cambiarColor(ROJO);      
  else if(((x_ >= -9.9)&&(x_ <= -9.1)) && ((y_ <= 1.8)&&(y_ >= 1.0)))
    cambiarColor(MAGENTA);    
  else if(((x_ >= -9.9)&&(x_ <= -9.1)) && ((y_ <= 0.9)&&(y_ >= 0.1)))
    cambiarColor(AMARILLO);    
  else if(((x_ >= -9.9)&&(x_ <= -9.1)) && ((y_ <= 0.0)&&(y_ >= -0.8)))
    cambiarColor(GRIS_CLARO);      
  else if(((x_ >= -9.9)&&(x_ <= -9.1)) && ((y_ <= -0.9)&&(y_ >= -1.7)))
    cambiarColor(GRIS);      
  else if(((x_ >= -9.9)&&(x_ <= -9.1)) && ((y_ <= -1.8)&&(y_ >= -2.6)))
    cambiarColor(GRIS_OSCURO);

  // REGIONES DE TRANSFORMACIONES GEOMÉTRICAS
  if(((x_ >= 8.5)&&(x_ <= 9.5)) && ((y_ <= 9.9)&&(y_ >= 8.9)))
    borrarFiguras();
  else if(((x_ >= 7.4)&&(x_ <= 8.4)) && ((y_ <= 9.9)&&(y_ >= 8.9)))
    borrarFigura();  
}

void clicIzquierdo(PUNTO pt){  
  puntoInicial = pt;
  switch(seleccionFigura){    
    case 1: // FIGURA LÍNEA
      if(primerPunto[0]){          
          L.color = NEGRO;
          L.ini = pt;
          L.gui= 0;
          L.grosor = 1;
          L.tipoLinea = 0;
          L.angulo = 0;
          primerPunto[0] = 0;
      }
      break;
    case 2: // FIGURA CUADRADO
      if(primerPunto[1]){
        CU.color = NEGRO;
        CU.piv = pt;
        CU.lado = 3;
        CU.gui= 0;
        CU.grosor = 1;
        CU.tipoLinea = 0;
        CU.angulo = 0;
        primerPunto[1] = 0;
      }
      break;
    case 3: // FIGURA RECTÁNGULO
      if(primerPunto[2]){
        R.color = NEGRO;
        R.piv = pt;
        R.alto = 2.5;
        R.ancho = 5;
        R.grosor = 1;
        R.tipoLinea = 0;
        R.angulo = 0;
        primerPunto[2] = 0;
      }
      break;
    case 4: // FIGURA POLÍGONO
      if(primerPunto[3]){
        P.color = NEGRO;
        P.c = pt;        
        P.numLados = 5;
        P.radio = 2;
        P.grosor = 1;
        P.tipoLinea = 0;
        P.angulo = 0;
        primerPunto[3] = 0;
      }
      break;
    case 5: // FIGURA CÍRCULO
      if(primerPunto[4]){
        CI.color = NEGRO;
        CI.c = pt;        
        CI.radio = 2;
        CI.grosor = 1;
        CI.tipoLinea = 0;
        CI.angulo = 0;
        primerPunto[4] = 0;
      }
      break;
    case 6: // FIGURA ELIPSE
      if(primerPunto[5]){
        E.color = NEGRO;
        E.c = pt;
        E.eje_mayor = 3;
        E.eje_menor = 1.5;
        E.grosor = 1;
        E.tipoLinea = 0;
        E.angulo = 0;
        primerPunto[5] = 0;
      }
      break;
    case 7: // FIGURA TRIÁNGULO ISÓSCELES
      if(primerPunto[6]){        
        TI.color = NEGRO;
        TI.altura = 4;
        TI.base = 3;
        TI.piv = pt;
        TI.grosor = 1;
        TI.tipoLinea = 0;
        TI.angulo = 0;    
        primerPunto[6] = 0;
      }
      break;
    case 8: // FIGURA TRIÁNGULO RECTÁNGULO
      if(primerPunto[7]){
        TR.color = NEGRO;
        TR.altura = 3;
        TR.base = 3;
        TR.piv = pt;
        TR.grosor = 1;
        TR.tipoLinea = 0; 
        TR.angulo = 0;
        primerPunto[7] = 0;
      }
      break;
  }   
}

void ratonPasivo(int x, int y){
  PUNTO pt=CVent_a_Cplano(x,y,tamVentana,tamVentana,-10,10,-10,10);
  sprintf(coordenadas,"(%.1f,%.1f)",pt.x,pt.y);  
}

void ratonActivo(int x, int y){
  PUNTO pt = CVent_a_Cplano(x, y, tamVentana, tamVentana, -10, 10, -10, 10);
  float tx, ty, theta;
  sprintf(coordenadas,"(%.1f,%.1f)",pt.x,pt.y);
  glutPostRedisplay();
  switch(seleccionFigura){    
    case 1: // FIGURA LÍNEA    
      if (puntoFinal == 1){
        tx = pt.x - puntoInicial.x;
        ty = pt.y - puntoInicial.y;
        puntoInicial = pt;
        L.ini.x += tx;
        L.ini.y += ty;
        L.fin.x += tx;
        L.fin.y += ty;       
      }
      else
        L.fin = pt;            
      break;
    case 2: // FIGURA CUADRADO
      if(!primerPunto[1])
        CU.piv = pt;    
      break;
    case 3: // FIGURA RECTÁNGULO
      if(!primerPunto[2])
        R.piv = pt;     
      break;
    case 4: // FIGURA POLÍGONO
      if(!primerPunto[3])
        P.c = pt;      
      break;
    case 5: // FIGURA CÍRCULO
      if(!primerPunto[4])
        CI.c = pt;      
      break;
    case 6: // FIGURA ELIPSE
      if(!primerPunto[5])
        E.c = pt; 
      break;
    case 7: // FIGURA TRIÁNGULO ISÓSCELES
      if(!primerPunto[6])
        TI.piv = pt;  
      break;
    case 8: // FIGURA TRIÁNGULO RECTÁNGULO
      if(!primerPunto[7])
        TR.piv = pt;  
      break;
  }       
}

// FUNCIONES PARA TECLADO

void teclasNormales(unsigned char tecla, int x, int y){    
    switch(tecla){
      case 27:
        exit(0);
        break;
      case 'l':
        teclaL();
        break;
      case 'q':
        if(P.numLados < 7 && seleccionFigura == 4)
          P.numLados++;
        break;
      case 'w':
        if(P.numLados > 4 && seleccionFigura == 4)
          P.numLados--;
        break;
      case '+':
        masGrosor();        
        break;
      case '-':
        menosGrosor();        
        break;
    }
}

void teclasEspeciales(int tecla, int x, int y){
    switch(tecla){
        case GLUT_KEY_LEFT:
          teclaIzquierda();
          break;
        case GLUT_KEY_RIGHT:
          teclaDerecha();
          break;
        case GLUT_KEY_UP:          
          teclaArriba();
          break;
        case GLUT_KEY_DOWN:
          teclaAbajo();
          break;
    }
    glutPostRedisplay();
}

void teclaArriba(void){
  switch(seleccionFigura){    
    case 1: // FIGURA LÍNEA
      L.ini.x *= escalaArriba;
      L.ini.y *= escalaArriba;
      L.fin.x *= escalaArriba;
      L.fin.y *= escalaArriba; 
      break;
    case 2: // FIGURA CUADRADO
      CU.lado *= escalaArriba;    
      break;
    case 3: // FIGURA RECTÁNGULO
      R.ancho *= escalaArriba;   
      R.alto *= escalaArriba;
      break;
    case 4: // FIGURA POLÍGONO
      P.radio *= escalaArriba;      
      break;
    case 5: // FIGURA CÍRCULO
      CI.radio *= escalaArriba; 
      break;
    case 6: // FIGURA ELIPSE
      E.eje_mayor *= escalaArriba;
      E.eje_menor *= escalaArriba;
      break;
    case 7: // FIGURA TRIÁNGULO ISÓSCELES
      TI.base *= escalaArriba;
      TI.altura *= escalaArriba;
      break;
    case 8: // FIGURA TRIÁNGULO RECTÁNGULO
      TR.base *= escalaArriba;
      TR.altura *= escalaArriba; 
      break;
  }  
}

void teclaAbajo(void){
  switch(seleccionFigura){    
    case 1: // FIGURA LÍNEA
      L.ini.x *= escalaAbajo;
      L.ini.y *= escalaAbajo;
      L.fin.x *= escalaAbajo;
      L.fin.y *= escalaAbajo; 
      break;
    case 2: // FIGURA CUADRADO
      CU.lado *= escalaAbajo;    
      break;
    case 3: // FIGURA RECTÁNGULO
      R.ancho *= escalaAbajo;   
      R.alto *= escalaAbajo;
      break;
    case 4: // FIGURA POLÍGONO
      P.radio *= escalaAbajo;      
      break;
    case 5: // FIGURA CÍRCULO
      CI.radio *= escalaAbajo; 
      break;
    case 6: // FIGURA ELIPSE
      E.eje_mayor *= escalaAbajo;
      E.eje_menor *= escalaAbajo;
      break;
    case 7: // FIGURA TRIÁNGULO ISÓSCELES
      TI.base *= escalaAbajo;
      TI.altura *= escalaAbajo;
      break;
    case 8: // FIGURA TRIÁNGULO RECTÁNGULO
      TR.base *= escalaAbajo;
      TR.altura *= escalaAbajo; 
      break;
  }  
}

void teclaIzquierda(void){
  switch(seleccionFigura){    
    case 1: // FIGURA LÍNEA
      L.angulo -= angulo;
      break;
    case 2: // FIGURA CUADRADO
      CU.angulo -= angulo;
      break;
    case 3: // FIGURA RECTÁNGULO
      R.angulo -= angulo;
      break;
    case 4: // FIGURA POLÍGONO
      P.angulo -= angulo;      
      break;
    // case 5: // FIGURA CÍRCULO
    //   CI.angulo -= angulo;
    //   break;
    case 6: // FIGURA ELIPSE
      E.angulo -= angulo;
      break;
    case 7: // FIGURA TRIÁNGULO ISÓSCELES
      TI.angulo -= angulo;
      break;
    case 8: // FIGURA TRIÁNGULO RECTÁNGULO
      TR.angulo -= angulo; 
      break;
  }  
}

void teclaDerecha(void){
  switch(seleccionFigura){    
    case 1: // FIGURA LÍNEA
      L.angulo += angulo;
      break;
    case 2: // FIGURA CUADRADO
      CU.angulo += angulo;
      break;
    case 3: // FIGURA RECTÁNGULO
      R.angulo += angulo;
      break;
    case 4: // FIGURA POLÍGONO
      P.angulo += angulo;      
      break;
    // case 5: // FIGURA CÍRCULO
    //   CI.angulo += angulo;
    //   break;
    case 6: // FIGURA ELIPSE
      E.angulo += angulo;
      break;
    case 7: // FIGURA TRIÁNGULO ISÓSCELES
      TI.angulo += angulo;
      break;
    case 8: // FIGURA TRIÁNGULO RECTÁNGULO
      TR.angulo += angulo; 
      break;
  }  
}

void teclaL(void){
  switch(seleccionFigura){    
    case 1: // FIGURA LÍNEA
      L.tipoLinea = !L.tipoLinea;
      break;
    case 2: // FIGURA CUADRADO
      CU.tipoLinea = !CU.tipoLinea;
      break;
    case 3: // FIGURA RECTÁNGULO
      R.tipoLinea = !R.tipoLinea;
      break;
    case 4: // FIGURA POLÍGONO
      P.tipoLinea = !P.tipoLinea;
      break;
    case 5: // FIGURA CÍRCULO
      CI.tipoLinea = !CI.tipoLinea;
      break;
    case 6: // FIGURA ELIPSE
      E.tipoLinea = !E.tipoLinea;
      break;
    case 7: // FIGURA TRIÁNGULO ISÓSCELES
      TI.tipoLinea = !TI.tipoLinea;
      break;
    case 8: // FIGURA TRIÁNGULO RECTÁNGULO
      TR.tipoLinea = !TR.tipoLinea;
      break;
  }  
}

void masGrosor(void){
  switch(seleccionFigura){    
    case 1: // FIGURA LÍNEA
      L.grosor += 1;
      break;
    case 2: // FIGURA CUADRADO
      CU.grosor += 1;
      break;
    case 3: // FIGURA RECTÁNGULO
      R.grosor += 1;
      break;
    case 4: // FIGURA POLÍGONO
      P.grosor += 1;
      break;
    case 5: // FIGURA CÍRCULO
      CI.grosor += 1;
      break;
    case 6: // FIGURA ELIPSE
      E.grosor += 1;
      break;
    case 7: // FIGURA TRIÁNGULO ISÓSCELES
      TI.grosor += 1;
      break;
    case 8: // FIGURA TRIÁNGULO RECTÁNGULO
      TR.grosor += 1;
      break;
  }  
}

void menosGrosor(void){
  switch(seleccionFigura){    
    case 1: // FIGURA LÍNEA
      L.grosor -= 1;
      break;
    case 2: // FIGURA CUADRADO
      CU.grosor -= 1;
      break;
    case 3: // FIGURA RECTÁNGULO
      R.grosor -= 1;
      break;
    case 4: // FIGURA POLÍGONO
      P.grosor -= 1;
      break;
    case 5: // FIGURA CÍRCULO
      CI.grosor -= 1;
      break;
    case 6: // FIGURA ELIPSE
      E.grosor -= 1;
      break;
    case 7: // FIGURA TRIÁNGULO ISÓSCELES
      TI.grosor -= 1;
      break;
    case 8: // FIGURA TRIÁNGULO RECTÁNGULO
      TR.grosor -= 1;
      break;
  }  
}

// FUNCIONES ADICIONALES

void borrarFiguras(void){
  int k;    
  puntoFinal = 0;
  for(k = 0; k<8; k++)
    primerPunto[k] = 1;    
}

void borrarFigura(void){
  switch(seleccionFigura){    
    case 1: // FIGURA LÍNEA      
      puntoFinal = 0;
      primerPunto[0] = 1;      
      break;
    case 2: // FIGURA CUADRADO
      primerPunto[1] = 1;      
      break;
    case 3: // FIGURA RECTÁNGULO
      primerPunto[2] = 1;      
      break;
    case 4: // FIGURA POLÍGONO
      primerPunto[3] = 1;       
      break;
    case 5: // FIGURA CÍRCULO
      primerPunto[4] = 1;      
      break;
    case 6: // FIGURA ELIPSE
      primerPunto[5] = 1;      
      break;
    case 7: // FIGURA TRIÁNGULO ISÓSCELES
      primerPunto[6] = 1;      
      break;
    case 8: // FIGURA TRIÁNGULO RECTÁNGULO
      primerPunto[7] = 1;      
      break;
  }  
}

void mostrarFiguras(void){
  if(!primerPunto[0]){
    glLineWidth(L.grosor);
    if(L.tipoLinea){      
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0x000F);      
    }
    linea(L);
    if(L.tipoLinea)
        glDisable(GL_LINE_STIPPLE);    
    glLineWidth(1.0);
  }    
  if(!primerPunto[1]){
    glLineWidth(CU.grosor);
    if(CU.tipoLinea){      
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0x000F);      
    }
    cuadrado(CU);
    if(CU.tipoLinea)
        glDisable(GL_LINE_STIPPLE);  
    glLineWidth(1.0);
  }      
  if(!primerPunto[2]){
    glLineWidth(R.grosor);
    if(R.tipoLinea){      
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0x000F);      
    }
    rectangulo(R);
    if(R.tipoLinea)
        glDisable(GL_LINE_STIPPLE);  
    glLineWidth(1.0); 
  }    
  if(!primerPunto[3]){
    glLineWidth(P.grosor);
    if(P.tipoLinea){      
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0x000F);      
    }
    poligono(P);
    if(P.tipoLinea)
        glDisable(GL_LINE_STIPPLE);  
    glLineWidth(1.0); 
  }    
  if(!primerPunto[4]){
    glLineWidth(CI.grosor);
    if(CI.tipoLinea){      
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0x000F);      
    }
    circulo(CI);
    if(CI.tipoLinea)
        glDisable(GL_LINE_STIPPLE);  
    glLineWidth(1.0);
  }      
  if(!primerPunto[5]){
    glLineWidth(E.grosor);
    if(E.tipoLinea){      
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0x000F);      
    }
    elipse(E);
    if(E.tipoLinea)
        glDisable(GL_LINE_STIPPLE);  
    glLineWidth(1.0);
  }
  if(!primerPunto[6]){
    glLineWidth(TI.grosor);
    if(TI.tipoLinea){      
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0x000F);      
    }
    trianguloIso(TI);
    if(TI.tipoLinea)
        glDisable(GL_LINE_STIPPLE);  
    glLineWidth(1.0); 
  }     
  if(!primerPunto[7]){
    glLineWidth(TR.grosor);
    if(TR.tipoLinea){      
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0x000F);      
    }
    trianguloRect(TR);
    if(TR.tipoLinea)
        glDisable(GL_LINE_STIPPLE);  
    glLineWidth(1.0);
  }
  glutPostRedisplay();      
}

void cambiarColor(COLOR color){
  switch(seleccionFigura){
    case 1: // COLOR LÍNEA
      L.color = color;            
      break;    
    case 2: // COLOR CUADRADO
      CU.color = color;
      break;
    case 3: // COLOR RECTÁNGULO
      R.color = color;
      break;
    case 4: // COLOR POLÍGONO
      P.color = color;
      break;
    case 5: // COLOR CÍRCULO
      CI.color = color;
      break;
    case 6: // COLOR ELIPSE
      E.color = color;
      break;
    case 7: // COLOR TRIÁNGULO ISÓSCELES
      TI.color = color;
      break;
    case 8: // COLOR TRIÁNGULO RECTÁNGULO
      TR.color = color;
      break;
  }  
}

void dibujarTexto(char *Texto, float x, float y){
  int k=0;
  glRasterPos2f(x,y);
  while(Texto[k]!='\0'){
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, Texto[k]);
    k++;
  }
}

PUNTO CVent_a_Cplano(int x, int y,int AnchoV, int AltoV, 
                     float xini, float xfin, float yini, float yfin){
  PUNTO pos;
  float RangoX = xfin - xini;
  float RangoY = yfin - yini;
  float porcentaje;
  porcentaje = ((float)x/AnchoV);
  pos.x = (porcentaje*(RangoX)) - (RangoX)/2;
  porcentaje = ((float)y/AltoV);
  pos.y = ((1-porcentaje) * (RangoY)) - (RangoX)/2;
  return pos;
}

void AsignaColor(COLOR c){
  glColor3f(PALETA_COLORES[c][0],PALETA_COLORES[c][1],PALETA_COLORES[c][2]);
}
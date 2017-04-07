//
//  main.cpp
//  Projecto
//
//  Created by Miguel Enrique Guerra Topete on 01/04/17.
//  Copyright © 2017 Miguel Enrique Guerra Topete. All rights reserved.
//

#include <iostream>
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctime>
#include "List.h"
#include "Planeta.h"

Planeta *planetaHover;
List<Planeta*> *planetas;

GLuint estreallas;
double multiplicarTiempo = 0.05;
double contadorReloj = 0;
double zoom = 5;
double arrastrarX = -1;
double arrastrarY = -1;
double mouseX;
double mouseY;
double eliminarA = 0;
double eliminarB = 0;
GLfloat distanciaAEstrealas = 600;

using namespace std;

void intPlanetas();
void menu(unsigned char key);
void initSistema();
void rWin(int w, int h);
void dibujar();
void botones(unsigned char key, int A, int B);
void irAPlaneta(int index);
void movimientoMouse(int x, int y);
void orbita();
void mouse(int boton, int estado, int x, int y);
void agregarTextura();
GLuint LoadTexture(const char * filename);

void initPlanetas()
{
    planetas= new List<Planeta*>();
    
    /*Planeta(string nombre, double aphelion, double perihelion, double periodoOrbital,
    char* direccionTextura, double tamano, double inclinacion, Planeta *sateliteDe = NULL);
     
    - aphelion y perihelion es que tan cerca esta orbitando del sol
    - *sateliteDe seria la luna y especificando a quien pertenece.
    */
    
    planetas->Add(new Planeta("Sol", 0, 0, 0, "sun.jpg", 109/30, 1.0));
    planetas->Add(new Planeta("Tierra", 30, 30, 1 , "Tierra.bmp", 1, 1.0));
    planetas->Add(new Planeta("Luna", 8, 8, 0.07, "Luna.bmp", 0.18, 15.0, planetas->Get(1)));
    
    planetaHover = planetas->Get(0);
}

void menu(unsigned char key)
{
    for(int i = 0; i < planetas->Length() - 1; i++)
        cout << "Presiona " << i << " para ver " << planetas->Get(i)->nombre << endl;
    
    cout << "Presiona W para hacer zoom in" << endl;
    cout << "Presiona S para hacer zoom out" << endl;
    cout << "Presiona + para acelerar la velocidad" << endl;
    cout << "Presiona - para disminuir la velocidad" << endl;
    cout << "Presiona sin soltar el click izquierdo del mouse y jalalo para mover la camara" << endl;
    cout << "--------------------------------------" << endl;
    cout << "--------------------------------------" << endl;
    
    if(key >= '0' && key <= planetas->Length())
        cout << "Este es el " << planetas->Get(key - '0')->nombre;
    
}

void rWin(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, w/h, 1.0, 1500.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void dibujar()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    double x, y, z;
    const double mitadPI = 3.1416 / 180;
    
    x = sin((-mouseX + arrastrarX) * mitadPI) * cos((mouseY - arrastrarY) * mitadPI) * zoom;
    y = sin((mouseY - arrastrarY) * mitadPI) * zoom;
    z = cos((-mouseX + arrastrarX) * mitadPI) * cos((mouseY - arrastrarY) * mitadPI) * zoom;
    
    gluLookAt(planetaHover->pX + x, planetaHover->pY + y, planetaHover->pZ + z,
              planetaHover->pX, planetaHover->pY, planetaHover->pZ, 0.0, 1.0, 0.0);
    
    glPushMatrix();
    for(int i = 0; i < planetas->Length(); i++)
        planetas->Get(i)->Dibujar();
    glPopMatrix();
    
    glutSwapBuffers();
}

void irAPlaneta(int index)
{
    Planeta * planeta;
    planeta = planetas->Get(index);
    planetaHover = planeta;
}

void botones(unsigned char key, int A, int B)
{
    switch(key)
    {
        case 'w': zoom--;
            break;
        case 's': zoom++;
            break;
        case 'a': eliminarA += 0.1;
            break;
        case 'd': eliminarB += 0.1;
            break;
        case '0': irAPlaneta(0);
            break;
        case '1': irAPlaneta(1);
            break;
        case '-': multiplicarTiempo /= multiplicarTiempo < 0.01 ? 1 : 2;
            break;
        case '+': multiplicarTiempo *= multiplicarTiempo > 10 ? 1 : 2;
            break;
        case 27:  exit(0);
            break;
        default:  break;
    }
}

void mouse(int boton, int estado, int x, int y)
{
    switch(boton)
    {
        case 0:
            if(estado == 0)
            {
                if(arrastrarX == -1)
                {
                    arrastrarX = x;
                    arrastrarY = y;
                }
                else
                {
                    arrastrarX += x - mouseX;
                    arrastrarY += y - mouseY;
                }
                mouseX = x;
                mouseY = y;
            }
            break;
        case 1:
            break;
        default:
            break;
    }
}

void movimientoMouse(int x, int y)
{
    mouseX = x;
    mouseY = y;
}

void orbita()
{
    long nuevoContadorReloj = clock();
    
    for(int i = 0; i < planetas->Length(); i++)
        planetas->Get(i)->Orbita(multiplicarTiempo * ((nuevoContadorReloj - contadorReloj) / 20), 30);
    
    contadorReloj = nuevoContadorReloj;
    glutPostRedisplay();
}

void agregarTextura()
{
    
}

void initSistema()
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Proyecto");
    
    glutReshapeFunc(rWin);
    agregarTextura();
    glutDisplayFunc(dibujar);
    glutKeyboardFunc(botones);
    glutMotionFunc(movimientoMouse);
    glutMouseFunc(mouse);
    
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    glutIdleFunc(orbita);
    glutMainLoop();
    
}


int main(int argc, char **argv)
{
    initPlanetas();
    menu(0);
    glutInit(&argc, argv);
    initSistema();
    return 0;
}

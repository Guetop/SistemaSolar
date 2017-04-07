//
//  Planeta.cpp
//  Projecto
//
//  Created by Miguel Enrique Guerra Topete on 01/04/17.
//  Copyright © 2017 Miguel Enrique Guerra Topete. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <GLUT/glut.h>
#include "Planeta.h"

Planeta::Planeta()
{
    ubicacionActualSistema = 0;
}

Planeta::Planeta(string nombre, double aphelion, double perihelion, double periodoOrbital,
                 char* direccionTextura, double tamano, double inclinacion, Planeta *sateliteDe)
{
    this->nombre = nombre;
    this->aphelion = aphelion;
    this->ubicacionActualEje = 0;
    this->perihelion = perihelion;
    this->periodoOrbital = periodoOrbital;
    this->direccionTextura = direccionTextura;
    this->tamano = tamano;
    this->sateliteDe = sateliteDe;
    this->inclinacionZ = inclinacion;
    this->inclinacionY = rand() % 180;
}

void Planeta::Orbita(double grado, double escalaRotacionEje)
{
    ubicacionActualEje += grado * escalaRotacionEje;
    ubicacionActualSistema += grado / (periodoOrbital == 0 ? 1 : periodoOrbital);
    
    ubicacionActualSistema = ubicacionActualSistema > 360 ? ubicacionActualSistema - 360 : ubicacionActualSistema;
    ubicacionActualEje = ubicacionActualEje > 360 ? ubicacionActualEje - 360 : ubicacionActualEje;
}

void Planeta::Dibujar()
{
    GLfloat posicion[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat luzAmbiente[] = {0.3, 0.3, 0.3, 1.0};
    
    const double pi = 3.1416;
    const double gradosARadianes = 57.29577951;
    double trasladoSistemaX = 0.05 * (aphelion + perihelion);
    double trasladoSistemaZ = 0.05 * (aphelion + perihelion);
    
    if(nombre == "Sol")
        pX = -2;
    else
        pX = -cos((double)(ubicacionActualSistema / gradosARadianes + pi)) * trasladoSistemaX;
    
    pY = 0;
    
    if(sateliteDe == NULL)
        pZ = sin((double)(ubicacionActualSistema / gradosARadianes + pi)) * trasladoSistemaZ;
    else
        pZ = sin((double)(ubicacionActualSistema / gradosARadianes + pi)) * trasladoSistemaX;
    
    
    if(nombre == "Sol")
    {
        glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
        glLightfv(GL_LIGHT0, GL_AMBIENT, posicion);
    }
    else
    {
        if(sateliteDe == NULL)
        {
            pX += -(trasladoSistemaX + trasladoSistemaZ) / 3;
        }
    }
    
    if(sateliteDe != NULL)
    {
        glPushMatrix();
        glTranslated(sateliteDe->pX, sateliteDe->pY, sateliteDe->pZ);
    }
    
    //se crea la orbita
    glPushMatrix();
    if(sateliteDe == NULL)
    {
        glTranslated(-(trasladoSistemaX + trasladoSistemaZ) / 3, 0, 0);
        glScaled(1, 1, trasladoSistemaZ / trasladoSistemaX);
        glRotated(90, 1.0, 0.0, 0.0);
        glutSolidTorus(0.01, trasladoSistemaX, 4, trasladoSistemaX * (trasladoSistemaZ < 50 ? 15 : 5));
    }
    glPopMatrix();
    
    glPushMatrix();
    if(sateliteDe != NULL)
    {
        glRotated(inclinacionY, 0, 1, 0);
        glRotated(inclinacionZ, 1, 0, 0);
    }
    glTranslated(pX, pY, pZ);
    
    //se crea el planeta
    GLUquadric *qobj = gluNewQuadric();
    gluQuadricTexture(qobj, GL_TRUE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textura);
    glRotated(270.0f, 1.0f, 0.0f, 0.0f);
    
    glPushMatrix();
    glRotated(ubicacionActualEje, 0, 0, 1);
    gluSphere(qobj, tamano/3, 30, 30);
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
    gluDeleteQuadric(qobj);
    
    if(nombre == "Sol")
    {
        glEnable(GL_LIGHTING);
    }
    glPopMatrix();
    
    if(sateliteDe != NULL)
    {
        glPopMatrix();
    }
}

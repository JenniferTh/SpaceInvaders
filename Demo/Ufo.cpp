/*
 * Ufo.cpp
 *
 *  Created on: 05.07.2015
 *      Author: Malte
 */

#include "Ufo.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Ufo.h"

#include "vec3.hpp"

double PunktA[3];
double PunktB[3];
double PunktC[3];
double PunktD[3];

double Spitze[3];


Ufo::Ufo() {
	// TODO Auto-generated constructor stub

}

void Ufo::MoveUfo(){
	//PunktA

}

// Date member function
void Ufo::DrawUfo()
{

	double Spitze[] = {0.0f , 0.6f ,10};

	glPushMatrix();
	glScalef(1, 1, 1);
	glBegin(GL_TRIANGLES);
	//glColor3f(1*0.55,1*0.55,1*0.55);
	//Punkt A
		glVertex3f(Spitze[0]-0.6f, Spitze[1]-1.f , Spitze[2]);
	    glVertex3f(Spitze[0], Spitze[1]-0.8f, Spitze[2]);
	    glVertex3f(Spitze[0], Spitze[1], Spitze[2]);

	    glVertex3f(0.6f, -0.4f, Spitze[2]);
	    glVertex3f(0.f, -0.2f, Spitze[2]);
	    glVertex3f(Spitze[0], Spitze[1], Spitze[2]);

	    glVertex3f(Spitze[0]-0.6f, Spitze[1]-1.0f, Spitze[2]);
	    glVertex3f(Spitze[0], Spitze[1]-0.8f, Spitze[2]);
	    glVertex3f(Spitze[0] +0.6f, Spitze[1] -1.f, Spitze[2]);

	glEnd();
	glPopMatrix();
}

double Ufo::getSpitze(){
	return Spitze;
}


void Ufo::setSpitze(double x, double y, double z){
	Spitze[0] = x;
	Spitze[1] = y;
	Spitze[2] = z;
}


Ufo::~Ufo() {
	// TODO Auto-generated destructor stub
}



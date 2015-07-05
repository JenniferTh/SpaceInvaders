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


Ufo::Ufo() {
	// TODO Auto-generated constructor stub

}

void Ufo::MoveUfo(){
	//PunktA
	PunktA[0] = PunktA[0]+1;
	PunktA[1] = PunktA[1]+1;
	PunktA[2] = PunktA[2]++;
}

// Date member function
void Ufo::DrawUfo()
{

	double PunktA[] = {-0.6f, -0.4f, 10.f};
	double PunktB[] = {PunktA[0]+0.6f, PunktA[1] +0.2f,  10.f};
	double PunktC[] = {PunktA[0]+0.6f, PunktA[1] +1.2f,  10.f};
	double PunktD[] = {PunktA[1]+1.0f, PunktA[1], PunktA[2]};

	glPushMatrix();
	glScalef(1, 1, 1);

	glBegin(GL_TRIANGLES);
	//glColor3f(1*0.55,1*0.55,1*0.55);
	//Punkt A
	glVertex3f(PunktA[0],PunktA[1],PunktA[2]);
	//Punkt B
	glVertex3f(PunktB[0],PunktB[1],PunktB[2]);
	//Punkt C
	glVertex3f(PunktC[0],PunktC[1],PunktC[2]);

	//Punkt D
	glVertex3f(PunktD[0],PunktD[1],PunktD[2]);
	//Punkt B
	glVertex3f(PunktB[0],PunktB[1],PunktB[2]);
	//Punkt C
	glVertex3f(PunktC[0],PunktC[1],PunktC[2]);

	//Punkt A
	glVertex3f(PunktA[0],PunktA[1],PunktA[2]);
	//Punkt B
	glVertex3f(PunktB[0],PunktB[1],PunktB[2]);
	//Punkt D
	glVertex3f(PunktD[0],PunktD[1],PunktD[2]);
	glEnd();
	glPopMatrix();
}




Ufo::~Ufo() {
	// TODO Auto-generated destructor stub
}



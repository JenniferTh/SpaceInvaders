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

Vec3 Spitze;


Ufo::Ufo() {
	// TODO Auto-generated constructor stub
	Spitze(0, 0.6 ,10);
}

void Ufo::MoveUfo(){
	//PunktA

}

// Date member function
void Ufo::DrawUfo()
{

	glPushMatrix();
	glScalef(1, 1, 1);
	glBegin(GL_TRIANGLES);
	//glColor3f(1*0.55,1*0.55,1*0.55);
	//Punkt A
		glVertex3f(Spitze.p[0]-0.6f, Spitze.p[1]-1.f , Spitze.p[2]);
	    glVertex3f(Spitze.p[0], Spitze.p[1]-0.8f, Spitze.p[2]);
	    glVertex3f(Spitze.p[0], Spitze.p[1], Spitze.p[2]);

	    glVertex3f(Spitze.p[0]+0.6f, Spitze.p[1]+1.2f, Spitze.p[2]);
	    glVertex3f(Spitze.p[0], Spitze.p[1]+0.8f, Spitze.p[2]);
	    glVertex3f(Spitze.p[0], Spitze.p[1], Spitze.p[2]);

	    glVertex3f(Spitze.p[0]-0.6f, Spitze.p[1]-1.0f, Spitze.p[2]);
	    glVertex3f(Spitze.p[0], Spitze.p[1]-0.8f, Spitze.p[2]);
	    glVertex3f(Spitze.p[0] +0.6f, Spitze.p[1] -1.f, Spitze.p[2]);

	glEnd();
	glPopMatrix();
}


void Ufo::SetSpitze(Vec3 tempSpitze)
{
	Spitze.p[0] = tempSpitze.p[0];
	Spitze.p[1] = tempSpitze.p[1];
	Spitze.p[2] = tempSpitze.p[2];
	printf("Hallo wkj<bgblkb");
}

Vec3 Ufo::GetSpitze()
{
return Spitze;
}

Ufo::~Ufo() {
	// TODO Auto-generated destructor stub
}



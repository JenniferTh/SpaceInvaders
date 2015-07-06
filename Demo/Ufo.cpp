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
Vec3 D;
Vec3 C;
Vec3 B;
Vec3 A;

Vec3 UfoSpeed;


Ufo::Ufo() {
	Vec3 s(0.0,1.0,10.0);
	Spitze = s;
	Vec3 a(-0.4,0,10.0);
	Vec3 b(0.0,-0.2,10.0);
	Vec3 c(0.4,0,10.0);
	Vec3 d(0.0,0,10.0);
	A = a;
	B = b;
	C = c;
	D = d;
}

void Ufo::MoveUfo(Vec3 UfoSpeed){
	A.p[0] = A.p[0]*UfoSpeed.p[0];
	B.p[0] = B.p[0]*UfoSpeed.p[0];
	C.p[0] = C.p[0]*UfoSpeed.p[0];
	D.p[0] = D.p[0]*UfoSpeed.p[0];
	A.p[1] = A.p[1]*UfoSpeed.p[1];
	B.p[1] = B.p[1]*UfoSpeed.p[1];
	C.p[1] = C.p[1]*UfoSpeed.p[1];
	D.p[1] = D.p[1]*UfoSpeed.p[1];
}

// Date member function
void Ufo::DrawUfo()
{
	glPushMatrix();
	glScalef(1, 1, 1);
	glBegin(GL_TRIANGLES);
	//glColor3f(1*0.55,1*0.55,1*0.55);
	//Fläche Links
		glVertex3f(Spitze.p[0]-0.4, Spitze.p[1]-1 , Spitze.p[2]);
	    glVertex3f(Spitze.p[0], Spitze.p[1]-1, Spitze.p[2]);
	    glVertex3f(Spitze.p[0], Spitze.p[1], Spitze.p[2]);
	//Fläche Rechts
		glVertex3f(Spitze.p[0]+0.4, Spitze.p[1]-1 , Spitze.p[2]);
	    glVertex3f(Spitze.p[0], Spitze.p[1]-1, Spitze.p[2]);
	    glVertex3f(Spitze.p[0], Spitze.p[1], Spitze.p[2]);
	//Fläche Unten
	    glVertex3f(Spitze.p[0]-0.4, Spitze.p[1]-1, Spitze.p[2]);
	    glVertex3f(Spitze.p[0]+0.4, Spitze.p[1]-1, Spitze.p[2]);
	    glVertex3f(Spitze.p[0], Spitze.p[1]-1.2, Spitze.p[2]);
	glEnd();
	glPopMatrix();
}

void Ufo::SetSpitze(Vec3 tempSpitze, Vec3 Speed)
{
	Spitze.p[0] = tempSpitze.p[0];
	Spitze.p[1] = tempSpitze.p[1];
	Spitze.p[2] = tempSpitze.p[2];
	MoveUfo(Speed);
}

Vec3 Ufo::GetSpitze(){
	return Spitze;
}
Vec3 Ufo::GetB(){

}

Ufo::~Ufo() {
	// TODO Auto-generated destructor stub
}



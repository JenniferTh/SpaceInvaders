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

Vec3 UfoSpeed;


Ufo::Ufo() {
	Vec3 s(0,-5.0,10.0);
	Spitze = s;
}


// Date member function
void Ufo::DrawUfo()
{
	glPushMatrix();
	glScalef(1, 1, 1);
	glBegin(GL_TRIANGLES);
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

void Ufo::SetSpitze(Vec3 tempSpitze, Vec3 Speed){
	Spitze.p[0] = tempSpitze.p[0];
	Spitze.p[1] = tempSpitze.p[1];
	Spitze.p[2] = tempSpitze.p[2];
}

Vec3 Ufo::GetSpitze(){
	return Spitze;
}
bool Ufo::collision(Vec3 ball, double r){
	double distance = (ball-Spitze-Vec3(0,-0.4,0)).LengthXY();
	if (distance < 0) { distance = distance * -1; }
	if(distance <= r + 0.4){
		return true;
	}
	return false;
	//Prüft ob der übergebene Asteroid mit dem der Sphere um das Raumschiff kollidiert
}

Ufo::~Ufo() {
	// TODO Auto-generated destructor stub
}



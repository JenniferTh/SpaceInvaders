/*
 * Asteroid.cpp
 *
 *  Created on: 06.07.2015
 *      Author: Jenni
 */

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vec3.hpp"
#include "Asteroid.h"

	double radiusA;
	Vec3 center(0,0,10);
	Vec3 speedA(0,0,0);

	Asteroid::Asteroid() {
		double r = randomDoubleBetween(0.8,1.2);
		radiusA = r;
		center.p[0] = randomDoubleBetween(-9, 9);
		center.p[1] = randomDoubleBetween(-14, 14);
		speedA.p[0] = randomDoubleBetween(-0.005, 0.005);
		speedA.p[1] = randomDoubleBetween(-0.005, 0.005);
	}
	Asteroid::Asteroid(Vec3 vel, double x, double y) {
		radiusA = 0.6;
		center.p[0] = x;
		center.p[1] = y;
		speedA.p[0] = vel.p[0];
		speedA.p[1] = vel.p[1];
	}
	Vec3 Asteroid::getCenter(){
		return center;
	}
	Vec3 Asteroid::getSpeed(){
		return speedA;
	}
	void Asteroid::setSpeed(Vec3 vel){
		speedA = vel;
	}
	void Asteroid::setCenter(Vec3 cen){
		center = cen;
	}
	double Asteroid::getRadius(){
		return radiusA;
	}
	void Asteroid::elasticCollision(Asteroid a){
		double mass1 = radiusA;
		double mass2 = a.getRadius();
		double newSpeedX2 = (2*mass1*speedA.p[0]+a.getSpeed().p[0]*(mass2-mass1)) / (mass1 + mass2);
		double newSpeedX1 = (2*mass2*a.getSpeed().p[0]+speedA.p[0]*(mass1-mass2)) / (mass1 + mass2);
		double newSpeedY2 = (2*mass1*speedA.p[1]+a.getSpeed().p[1]*(mass2-mass1)) / (mass1 + mass2);
		double newSpeedY1 = (2*mass2*a.getSpeed().p[1]+speedA.p[1]*(mass1-mass2)) / (mass1 + mass2);
		Vec3 newSpeedA(newSpeedX2, newSpeedY2, 0);
		Vec3 newCenterA((a.getCenter().p[0]+newSpeedX2),(a.getCenter().p[1]+newSpeedY2),10);
		speedA.p[0] = newSpeedX1;
		speedA.p[1] = newSpeedY1;
		a.setSpeed(newSpeedA);

		center.p[0] += newSpeedX1;
		center.p[1] += newSpeedY1;
		a.setCenter(newCenterA);
	}
 	double Asteroid::randomDoubleBetween(double XMin, double XMax){
		double X = XMin + rand() * (XMax - XMin) / RAND_MAX;
		return X;
	}
	Asteroid::~Asteroid() {
		// TODO Auto-generated destructor stub
	}




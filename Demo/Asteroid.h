/*
 * Asteroid.h
 *
 *  Created on: 06.07.2015
 *      Author: Jenni
 */
#include "Vec3.hpp"
#ifndef ASTEROID_H_
#define ASTEROID_H_


class Asteroid {
public:
	Asteroid();
	Asteroid(Vec3 vel, double x, double y);
	double randomDoubleBetween(double XMin, double XMax);
	Vec3 getCenter();
	Vec3 getSpeed();
	void setSpeed(Vec3 vel);
	void setCenter(Vec3 cen);
	void elasticCollision(Asteroid a);
	double getRadius();
	virtual ~Asteroid();
};



#endif /* ASTEROID_H_ */

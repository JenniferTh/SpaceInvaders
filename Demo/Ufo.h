/*
 * Ufo.h
 *
 *  Created on: 05.07.2015
 *      Author: Malte
 */

#include "vec3.hpp"

#ifndef UFO_H_
#define UFO_H_

class Ufo {
public:
	Vec3 Spitze;
	Vec3 D;
	Vec3 C;
	Vec3 B;
	Vec3 A;

	Vec3 UfoSpeed;
	void draw();
	Ufo();
	virtual ~Ufo();
    void DrawUfo();
    void MoveUfo(Vec3 UfoSpeed);

    //getters
    Vec3 GetSpitze();
    Vec3 GetB();
    //setters
    void SetSpitze(Vec3 topVector, Vec3 speed);
};

#endif /* UFO_H_ */

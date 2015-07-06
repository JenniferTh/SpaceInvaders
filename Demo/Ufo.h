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
	void draw();
	Ufo();
	virtual ~Ufo();
    void DrawUfo();
    void MoveUfo();

    //getters
    Vec3 GetSpitze();

    //setters
    void SetSpitze(Vec3 topVector);

};

#endif /* UFO_H_ */

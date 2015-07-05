/*
 * Ufo.h
 *
 *  Created on: 05.07.2015
 *      Author: Malte
 */

#ifndef UFO_H_
#define UFO_H_

class Ufo {
public:
	void draw();
	Ufo();
	virtual ~Ufo();
    void DrawUfo();
    void MoveUfo();

    double PunktA[];
    double PunktB[];
    double PunktC[];
    double PunktD[];


    const double* getPunktA() const {
    	return PunktA;
    }

	const double* getPunktB() const {
		return PunktB;
	}

	const double* getPunktC() const {
		return PunktC;
	}

	const double* getPunktD() const {
		return PunktD;
	}



};

#endif /* UFO_H_ */

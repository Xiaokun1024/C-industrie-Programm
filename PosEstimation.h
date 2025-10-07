/*
 * PosEstimation.h
 *
 *  Created on: 06.10.2025
 *      Author: pi
 */

#ifndef POSESTIMATION_H_
#define POSESTIMATION_H_

class PosEstimation{
private:
	double dx[3];	//x[0]:x-Koordinate  x[1]:y-Koordinate	x[2]:Raumrichtung
	double dVelAverage;

public:
	PosEstimation();
	~PosEstimation();
	void Reset();
	void PredictPosition(double dSpeedR, double dSpeedL, double dTimestep);
	double* GetPosition();
};


#endif /* POSESTIMATION_H_ */

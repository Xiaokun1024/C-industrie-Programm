/*
 * PosEstimation.cpp
 *
 *  Created on: 06.10.2025
 *      Author: pi
 */
#define _USE_MATH_DEFINES
#include "PosEstimation.h"
#include <cmath>


PosEstimation::PosEstimation(){
	for(auto i = 0; i < 3; i++){
		dx[i] = 0;
	}
	dVelAverage = 0;
}

PosEstimation::~PosEstimation()
{}

void PosEstimation::PredictPosition(double SpeedR, double SpeedL, double Timestep){

	dx[0] += dVelAverage * Timestep * cos(dx[2]);
	dx[1] += dVelAverage * Timestep * sin(dx[2]);
	dx[2] += (SpeedR - SpeedL) *  Timestep / 0.23;

	dx[2] = fmod(dx[2], 2 * M_PI);
	if(dx[2] > M_PI)	dx[2] -= 2 * M_PI;
	if(dx[2] < -M_PI)	dx[2] += 2 * M_PI;

	dVelAverage = (SpeedR + SpeedL) / 2;
}

double* PosEstimation::GetPosition(){
	return dx;
}

void PosEstimation::Reset(){
	for(auto i = 0; i < 3; i++){
		dx[i] = 0;
	}
	dVelAverage = 0;
}





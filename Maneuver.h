/*
 * Maneuver.h
 *
 *  Created on: 06.10.2025
 *      Author: pi
 */

#ifndef MANEUVER_H_
#define MANEUVER_H_

#include <list>
#include <string>

struct Coord
{
	double dX;
	double dY;
	double dZ;
	Coord(){
		dX=0;
		dY=0;
		dZ=0;
	}
	Coord (double X, double Y, double Z):dX(X),dY(Y),dZ(Z){}
};

class Maneuver{
private:

//	Coord dcoord;

	std::list<Coord> CoordList;
	std::list<Coord>::iterator iter;

	bool bIsRunning;
	double adWishSpeed[2];
	double dMaxSpeed;
	double dPosDifference;

public:
	Maneuver();
	~Maneuver();
	void CalcCircle(double dRadius,double dSpeed,double dTimestep);
	void CalcEight(double dRadius,double dSpeed,double dTimestep);
	void LogList(std::string sDatei);
	bool isRunning();
	void Start();
	void Stop();
	void Proceed();
	void CalcManeuverSpeed(double dX, double dY, double dW);
	double* GetManeuverSpeed();

};



#endif /* MANEUVER_H_ */

/*
 * RobotControl.h
 *
 *  Created on: 28.09.2023
 *      Author: pi
 */

#ifndef ROBOTCONTROL_H_
#define ROBOTCONTROL_H_
#include "PIDController.h"
#include "InterfaceSIM.h"
#include "Maneuver.h"
#include "PosEstimation.h"
#include "ncurses.h"
#include <iostream>

class RobotControl
{
private:
	double dIstgeschwendigkeit[2]; //1.Rechts 2.Links
	bool bIsActive;

	InterfaceSIM interface; // Erstellt ein Objekt in der Klasse
	Maneuver maneuver;
	PosEstimation posEstimation;
	PIDController Motor_rechts;
	PIDController Motor_links;
	int iSignallaenge[2];

public:
	RobotControl();
	~RobotControl();
	bool isActive();

	static RobotControl *transferPointer;
	static void transferFunction();

	void Communicate();
	void Step();

};



#endif /* ROBOTCONTROL_H_ */

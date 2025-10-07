/*
 * RobotControl.cpp
 *
 *  Created on: 28.09.2023
 *      Author: pi
 */

#include "PIDController.h"
//#include "InterfaceSIM.h"
#include "InterfaceHW.h"
#include "InterfaceHW.cpp"
#include "InterfaceHWArdupi.h"
#include "InterfaceHWArdupi.cpp"
#include "InterfaceHWHall.h"
#include "InterfaceHWHall.cpp"
#include "InterfaceHWSpeed.h"
#include "InterfaceHWSpeed.cpp"
#include "Maneuver.h"
#include "PosEstimation.h"
#include "ncurses.h"
#include <iostream>
#include "RobotControl.h"
using namespace std;
RobotControl* RobotControl::transferPointer;

void RobotControl::transferFunction()
{
	transferPointer->Step();
}

RobotControl::RobotControl()
{
	dIstgeschwendigkeit[0]=0;
	dIstgeschwendigkeit[1]=0;

	Motor_rechts=PIDController(500.0,100.0,0.0,0.04);
	Motor_links=PIDController(500.0,100.0,0.0,0.04);

	bIsActive=false;
	transferPointer=this;
}

RobotControl::~RobotControl()
{}


void RobotControl::Communicate()
{
	interface.Initialize(0.04);
	interface.SetTimerfunction(0,0,04,transerFunction);
	cout<<"Soll ein neues Manöver gefahren werden?    j  für ja;   n für nein"<<endl;
	char ant;
	cin>>ant;
	if(ant=='n')
	{bIsActive=false;}
	else if(ant=='j')
	{
		cout<<"Geben Sie der Radius und die Geschwindigkeit ein: "<<endl;
		double dRadius,dVel;
		cout << "Radius: ";
		cin>> dRadius;
		cout << "dVel: ";
		cin>> dVel;
		cout<<"Geben Sie die Nummer der gewünschten Manöver ein: 1.Kreis, 2.Acht."<<endl;
		int wahl;
		cout <<"Sie haben die Wahl: ";
		cin >> wahl;
		if(wahl==1)
		{
			maneuver.CalcCircle(dRadius,dVel,0.04);
			maneuver.LogList("circle.txt");
		}
		else if (wahl==2)
		{
			maneuver.CalcEight(dRadius,dVel,0.04);
			maneuver.LogList("Eight.txt");
		}

	posEstimation.Reset();

	//sigprocmask(SIG_UNBLOCK, &interface.mask, nullptr);
	initscr();
	nodelay(stdscr,TRUE);
	noecho();


	printw("w: Start, s: Stop, f:proceed, q:quit. \n");
	printw("Input:");
	char ch;

	while (ch!='q')
	{
		ch = getch();
		if (ch != -1)
		{
			clear();
			printw("w: Start, s: Stop, f:proceed, q:quit. \n");
			printw("Input: %c \n", ch);
			switch(ch)
			{case'w':
				maneuver.Start();
				break;

			case's':
				maneuver.Stop();
				break;

			case'f':
				maneuver.Proceed();
				break;

			default:
			printw("Bitte geben eine gültige Zeichnung ein. \n");
			break;}
		}
	}

	maneuver.Stop();
	double *dIstgeschwendigkeit=interface.GetInput();
	while((dIstgeschwendigkeit[0]>0.01&&(dIstgeschwendigkeit[1]>0.01)))
	{}
	printf("Roboter steht");
	//sigprocmask(SIG_BLOCK, &interface.mask, nullptr);
	endwin();
	}
}

bool RobotControl::isActive()
{
	return bIsActive;
}

void RobotControl::Step()
{
	double *pdInput = interface.GetInput();

	dIstgeschwendigkeit[0] = *(pdInput);
	dIstgeschwendigkeit[1] = *(pdInput+1);

	if(maneuver.isRunning())
	{
		posEstimation.PredictPosition(*(dIstgeschwendigkeit),*(dIstgeschwendigkeit+1),0.04);
		double *pos=posEstimation.GetPosition();

		maneuver.CalcManeuverSpeed(*pos,*(pos+1),*(pos+2));
		double *dSollgeschwendigkeit=maneuver.GetManeuverSpeed();


		Motor_rechts.CalculateU(dSollgeschwendigkeit[0], dIstgeschwendigkeit[0]);
		Motor_links.CalculateU(dSollgeschwendigkeit[1], dIstgeschwendigkeit[1]);

		iSignallaenge[0] = int(Motor_rechts.GetU() + 1500);
		iSignallaenge[1] = int(Motor_links.GetU() + 1500);

		if(iSignallaenge[1]>=2000){iSignallaenge[1]=2000;}
		if(iSignallaenge[1]<=1000){iSignallaenge[1]=1000;}
		if(iSignallaenge[0]>=2000){iSignallaenge[0]=2000;}
		if(iSignallaenge[0]<=1000){iSignallaenge[0]=1000;}


	}
	else
	{
		iSignallaenge[0]=1500;
		iSignallaenge[1]=1500;
	}
	interface.SetOutputs(iSignallaenge);
}

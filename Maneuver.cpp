/*
 * Maneuver.cpp
 *
 *  Created on: 28.09.2023
 *      Author: pi
 */


#include "Maneuver.h"
#include <cmath>
#include <string>
#include <fstream>
#include <list>

Maneuver::Maneuver()
{
	bIsRunning=false;
	adWishSpeed[0]=0.0;
	adWishSpeed[1]=0.0;
	dMaxSpeed=0.5;
	dPosDifference=0.02;
}

Maneuver::~Maneuver()
{}



void Maneuver::CalcCircle(double dRadius,double dSpeed,double dTimestep)
{
	CoordList.clear();
	for(int counter=0; counter<(int)((2*M_PI)/((dSpeed/dRadius)*dTimestep));counter++)
	{
		double x=dRadius * sin(counter*(dSpeed/dRadius)*dTimestep);
		double y=dRadius *(1-cos(counter*(dSpeed/dRadius)*dTimestep));
		CoordList.push_back(Coord(x,y,dSpeed));
	}
}

void Maneuver::CalcEight(double dRadius,double dSpeed,double dTimestep)
{
	CoordList.clear();
	for(int counter=1; counter<(int)((2*M_PI)/((dSpeed/dRadius)*dTimestep));counter++)
	{
		double x=dRadius * sin(counter*(dSpeed/dRadius)*dTimestep);
		double y=dRadius *(1-cos(counter*(dSpeed/dRadius)*dTimestep));
		CoordList.push_back(Coord(x,y,dSpeed));
	}
	for(int counter=1; counter<(int)((2*M_PI)/((dSpeed/dRadius)*dTimestep));counter++)
	{
		double x=dRadius*sin(counter*(dSpeed/dRadius)*dTimestep);
		double y=(-dRadius)*(1-cos(counter*(dSpeed/dRadius)*dTimestep));
		CoordList.push_back(Coord(x,y,dSpeed));
	}
}

void Maneuver::LogList(std::string sDatei)
{
	std::fstream ofile;
	ofile.open(sDatei,std::ios::out);
	for(iter=CoordList.begin();iter!=CoordList.end();iter++)
	{
		ofile<<iter->dX<<"\t"<<iter->dY<<std::endl;
	}
	ofile.close();
}

bool Maneuver::isRunning()
{
	return bIsRunning;
}

void Maneuver::Start()
{
	bIsRunning = true;
	iter=CoordList.begin();
}

void Maneuver::Stop()
{
	bIsRunning=false;
}

void Maneuver::Proceed()
{
	bIsRunning=true;
}

void Maneuver::CalcManeuverSpeed(double dX, double dY, double dW) //nach  UML-Zustandsdiagramm
{//Positionsvergleich
	dPosDifference = sqrt(pow((iter->dX-dX),2)+pow((iter->dY-dY),2));
	if(dPosDifference<0.02)
	{
		iter++;
	}

		if(iter!=CoordList.end())
		{
			//Winkel zwischen Soll- und Istposition berechnen
			double phi = atan2((iter->dY-dY),(iter->dX-dX));
			//Winkeldifferenz berechnen
			double delta_phi=phi-dW;
			//Winkeldifferenz auf ]-π, π]begrenzen
			delta_phi=fmod(delta_phi, 2*M_PI);
			if(delta_phi>M_PI){delta_phi-=2*M_PI;}
			else if(delta_phi<=-M_PI){delta_phi+=2*M_PI;}
			//Rotationsanteil bestimmen
			double dRot=2*delta_phi;
			if(dRot>0.5){dRot=0.5;}
			else if(dRot<-0.5){dRot=-0.5;}
			//Translationsanteil übernehmen
			double dTra=iter->dZ;
			//Geschwindigkeitsanteile überprüfen
			if (dTra*dRot > 0.0)
				{
					if (dTra + dRot > dMaxSpeed)  {dTra = dMaxSpeed - dRot;}
					else if (dTra + dRot < -dMaxSpeed)  {dTra = -dMaxSpeed - dRot;}
				}
			else
				{
					if (dTra - dRot > dMaxSpeed)  {dTra = dMaxSpeed + dRot;}
					else if (dTra - dRot < -dMaxSpeed)  {dTra = -dMaxSpeed + dRot;}
				}
			//Geschwindigkeitsanteile summieren
			adWishSpeed[0] = dTra + dRot;
			adWishSpeed[1] = dTra - dRot;

		}
		else //Manöverfahrt beenden
			{
			adWishSpeed[0]=0.0;  //right
			adWishSpeed[1]=0.0;  //left
			Stop();
			}
}

double* Maneuver::GetManeuverSpeed()//einen Pointer auf die Adresse des Geschwindigkeitsarrays übergibt.
{
	return adWishSpeed;
}

#include "PIDController.h"
#include <cmath>


PIDController::PIDController(double Kp, double Ki, double Kd, double Ta):dKp(Kp),dKi(Ki),dKd(Kd),dTa(Ta)
{
	dEsum=0;
	deold=0;
	dU=0;
}

PIDController::PIDController()
{
	dKp=0;
	dKi=0;
	dKd=0;
	dTa=0;
	dEsum=0;
	deold=0;
	dU=0;
}

void PIDController::CalculateU(double dW, double dY){
    double denow = dW - dY;
    dEsum = dEsum + denow;

    dU = dKp * denow + dKi * dTa * dEsum + dKd * (denow - deold) / dTa;

    deold = denow;
}

double PIDController::GetU(){
    return dU;
}


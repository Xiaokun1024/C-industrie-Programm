#include "PIDController.h"
#include <cmath>


PIDController::PIDController(double Kp, double Ki, double Kd, double Ta){
    dKp = Kp;   dKi = Ki;   dKd = Kd;   dTa = Ta;
}

PIDController::~PIDController(){

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
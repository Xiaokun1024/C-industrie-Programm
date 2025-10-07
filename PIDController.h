#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

class PIDController
{
private:
    double dKp, dKi, dKd, dTa;
    double dEsum = 0, deold = 0, dU = 0;

public:
    PIDController(double Kp, double Ki, double Kd, double Ta);
    PIDController();

    void CalculateU(double dW, double dY);
    double GetU();
};


#endif

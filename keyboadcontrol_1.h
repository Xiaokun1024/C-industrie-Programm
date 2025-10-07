//KeyboardControl Class

#ifndef KEYBOARDCONTROL_H
#define KEYBOARDCONTROL_H

#include <array>
#include"InterfaceSIM.h"

class KeyboardControl{

public:
    static void transferFunction;
    static KeyboardControl* transferPointer;
    std::array<double, 2> Communicate(std::array<double, 2>){};
    std::array<double, 2> Step(std::array<double, 2>){};
private:
    int signalMicros_[2];
    double Ist_geschwindigkeit[2];
    double Sollgeschwindigkeit[2];
    InterfaceSim sim_;

};

#endif

//KeyboardControl Class

#ifndef KEYBOARDCONTROL_H
#define KEYBOARDCONTROL_H

#include <array>
#include "InterfaceSIM.h"

class KeyboardControl{

public:

    static void transferFunction();
    static KeyboardControl* transferPointer;
    KeyboardControl();
    ~KeyboardControl();
    void Communicate();
    void Step();
    InterfaceSIM sim;

private:
    int signalMicros_[2];
    double Ist_Geschwindigkeit[2];
    double Soll_Geschwindigkeit[2];
    

};

#endif
//KeyboardControl Class

#ifndef KEYBOARDCONTROL_H
#define KEYBOARDCONTROL_H

#include <array>

class KeyboardControl{

public:

    std::array<double, 2> Communicate(std::array<double, 2>){};
    std::array<double, 2> Step(std::array<double, 2>){};

};

#endif

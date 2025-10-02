#include<array>
#include <cmath>
#include"keyboardcontrol.h"
#include <algorithm> 
KeyboardControl* KeyboardControl::transferPointer=nullptr;
//构造函数
KeyboardControl::KeyboardControl(){
    sim_Initialize(dt,&KeyboardControl::transferFunction); //初始化intializeSim
    transferPointer = this;
}

void KeyboardControl::transferFunction(){
    if(transferPointer){
        transferPointer->Step();
    }
}
void KeyboardControl::Step()
{
    // read acutual velocity from simulation 
    double* pIn = sim_.GetInput();          // pIn[0]=右轮 m/s, pIn[1]=左轮 m/s
    Ist_geschwindigkeit[0] = pIn[0];
    Ist_geschwindigkeit[1] = pIn[1];

    // velocity ->signals
    int velToMicros(double v) {
        if(v>0.5) v=0.5;
        if(v<-0.5) v=-0.5;
    
        double micros = 1500.0 + v * 1000.0;       // 0.5 -> 2000，-0.5 -> 1000
        // 四舍五入并再次确保边界
        int mi = static_cast<int>(std::lround(micros));
        if (mi < 1000) mi = 1000;
        if (mi > 2000) mi = 2000;
        return mi;
    }

    //Compute two outputs Signals 
    signalMicros_[0] = velToMicros(desired_[0]);   // 右
    signalMicros_[1] = velToMicros(desired_[1]);   // 左

    //Send Signals to Roboter
    sim_.SetOutputs(signalMicros_);
}

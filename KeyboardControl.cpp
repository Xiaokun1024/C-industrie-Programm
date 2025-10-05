
#include <ncurses.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include "KeyboardControl.h"
#include <algorithm>
#include <vector>
#include "InterfaceSIM.cpp"
#include "PIDController.cpp" 

double dt = 0.04;
PIDController links(500, 1850, 0, 0.04),rechts(500, 1850, 0, 0.04);

KeyboardControl* KeyboardControl::transferPointer=nullptr;
KeyboardControl::KeyboardControl(){
    transferPointer = this;
}

KeyboardControl::~KeyboardControl(){
}

void KeyboardControl::transferFunction(){
    if(transferPointer){
        transferPointer->Step();
    }
}

int velToMicros(double v){
    double micros = 1500.0 + v * 1000.0;       // 0.5 -> 2000，-0.5 -> 1000

    int mi = static_cast<int>(std::lround(micros));
    if (mi < 1000) mi = 1000;
    if (mi > 2000) mi = 2000;
    return mi;
}

void KeyboardControl::Step()
{
    // read acutual velocity from simulation 
    double* pIn = sim.GetInput();          // pIn[0]=rechts m/s, pIn[1]=links m/s
    Ist_Geschwindigkeit[0] = pIn[0];
    Ist_Geschwindigkeit[1] = pIn[1];

    std::vector<double> PID_Geschwindigkeit; 
    rechts.CalculateU(Soll_Geschwindigkeit[0], Ist_Geschwindigkeit[0]);
    links.CalculateU(Soll_Geschwindigkeit[1], Ist_Geschwindigkeit[1]);
    PID_Geschwindigkeit[0] = rechts.GetU();     
    PID_Geschwindigkeit[1] = links.GetU();      


    //Compute two outputs Signals 
    //signalMicros_[0] = velToMicros(Soll_Geschwindigkeit[0]);   // rechts
    //signalMicros_[1] = velToMicros(Soll_Geschwindigkeit[1]);   // links
    signalMicros_[0] = velToMicros(PID_Geschwindigkeit[0]);   // rechts
    signalMicros_[1] = velToMicros(PID_Geschwindigkeit[1]);   // links

    //Send Signals to Roboter
    sim.SetOutputs(signalMicros_);
}

void KeyboardControl::Communicate(){
    sim.Initialize(0.04,transferFunction);
    // NCurses initialisieren
    initscr();              // Bildschirm initialisieren
    cbreak();               // Zeichenweises Einlesen (ohne Buffer)
    noecho();               // Keine automatische Ausgabe der Eingabe
    keypad(stdscr, TRUE);   // Spezielle Tasten aktivieren
    nodelay(stdscr, TRUE);  // Nicht-blockierende Eingabe

    sigprocmask(SIG_UNBLOCK, &sim.mask, nullptr);
    int ch;
    bool running = true;
    char lastkey = ' ';

    while(running){
        
        ch = getch();

        if (ch >= 32 && ch <= 126){
            lastkey = char(ch);
            
            switch(ch){
                case 'q' :{
                    Soll_Geschwindigkeit[0] = 0;
                    Soll_Geschwindigkeit[1] = 0;
                    running = false;
                    break;
                }

                case 'b' :{
                    Soll_Geschwindigkeit[0] = 0;
                    Soll_Geschwindigkeit[1] = 0;
                    break;
                }

                case 'w':{
                    if(fabs(Soll_Geschwindigkeit[0]) < 0.5 && fabs(Soll_Geschwindigkeit[1]) < 0.5){
                        Soll_Geschwindigkeit[0] += 0.01;     //rechts Motor
                        Soll_Geschwindigkeit[1] += 0.01;     //links Motor
                    }
                    break;
                }

                case 's':{
                    if(fabs(Soll_Geschwindigkeit[0]) < 0.5 && fabs(Soll_Geschwindigkeit[1]) < 0.5){
                        Soll_Geschwindigkeit[0] -= 0.01;     //rechts Motor
                        Soll_Geschwindigkeit[1] -= 0.01;     //links Motor
                    }
                    break;
                }

                case 'a':{
                    if(fabs(Soll_Geschwindigkeit[0]) < 0.5 && fabs(Soll_Geschwindigkeit[1]) < 0.5){
                        Soll_Geschwindigkeit[0] += 0.005;     //rechts Motor
                        Soll_Geschwindigkeit[1] -= 0.005;     //links Motor
                    }
                    break;
                }

                case 'd':{
                    if(fabs(Soll_Geschwindigkeit[0]) < 0.5 && fabs(Soll_Geschwindigkeit[1]) < 0.5){
                        Soll_Geschwindigkeit[0] -= 0.005;     //rechts Motor
                        Soll_Geschwindigkeit[1] += 0.005;     //links Motor
                    }
                    break;
                }
            }

        }
        
        clear();
        printw("Motor_Steuerung: w(vor), s(rueck), a(links), d(rechts), q und b(quit)\n");
        printw("Soll: R = %.3f L = %.3f\n", Soll_Geschwindigkeit[0], Soll_Geschwindigkeit[1]);
        printw("Ist: R = %.3f L = %.3f\n", Ist_Geschwindigkeit[0], Ist_Geschwindigkeit[1]);
        printw("Letzte Taste: %c\n", lastkey);  
        refresh();
        napms(50);
    }


    while(fabs(Ist_Geschwindigkeit[0]) > 0.0001 || fabs(Ist_Geschwindigkeit[1]) > 0.0001){
        clear();
        printw("Warte auf Stillstand.\n"); 
        printw("Ist: R=%.3f L=%.3f\n", Ist_Geschwindigkeit[0], Ist_Geschwindigkeit[1]);
        refresh();
        napms(50);
    }

    
    sigprocmask(SIG_BLOCK, &sim.mask, nullptr);
    // ncurses ausschalten
    endwin();
    std::cout << "Programm beendet." << std::endl;
}

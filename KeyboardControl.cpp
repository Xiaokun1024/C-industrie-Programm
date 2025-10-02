//comminicate()

#include <ncurses.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include "KeyboardControl.h"

std::array<double, 2> Soll_Geschwindigkeit = {0,0};


void KeyboardControl::Communicate(std::array<double, 2> Ist_Geschwindigkeit){
    // NCurses initialisieren
    initscr();              // Bildschirm initialisieren
    cbreak();               // Zeichenweises Einlesen (ohne Buffer)
    noecho();               // Keine automatische Ausgabe der Eingabe
    keypad(stdscr, TRUE);   // Spezielle Tasten aktivieren
    nodelay(stdscr, TRUE);  // Nicht-blockierende Eingabe

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
        printw("Letzte Taste: %c\n", lastkey);  // 这里会一直显示上次按下的键
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

    
    // ncurses ausschalten
    endwin();
    std::cout << "Programm beendet." << std::endl;
}

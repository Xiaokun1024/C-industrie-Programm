/*
 * main.cpp
 *
 *  Created on: 07.10.2025
 *      Author: pi
 */

#include <pigpio.h>
#include <ncurses/curses.h>
#include <unistd.h>
#include <cmath>

int velToMicros(double v){
    double micros = 1500.0 + v * 1000.0;       // 0.5 -> 2000，-0.5 -> 1000
    if (micros < 1000) micros = 1000;
    if (micros > 2000) micros = 2000;
    return micros;
}

int main(){
	//init GPIO
	gpioInitialise();
	int irechts, ilinks;
	char ch;
	bool running;
	initscr();
	nodelay(stdscr,TRUE);
	noecho();

	while(running){
		ch = getch();
		if (ch >= 32 && ch <= 126){
			switch(ch){
	                case 'q' :{
	                    irechts = 0;
	                    ilinks = 0;
	                    running = false;
	                    break;
	                }

	                case 'b' :{
	                	irechts = 0;
	                    ilinks = 0;
	                    break;
	                }

	                case 'w':{
	                    if(1){
	                        irechts += 0.02;     //rechts Motor
	                        ilinks += 0.02;     //links Motor
	                    }
	                    break;
	                }

	                case 's':{
	                    if(1){
	                    	irechts -= 0.02;     //rechts Motor
	                    	ilinks -= 0.02;     //links Motor
	                    }
	                    break;
	                }

	                case 'a':{
	                    if(1){
	                    	irechts += 0.01;     //rechts Motor
	                    	ilinks -= 0.01;     //links Motor
	                    }
	                    break;
	                }

	                case 'd':{
	                    if(1){
	                    	irechts -= 0.01;     //rechts Motor
	                    	ilinks += 0.01;     //links Motor
	                    }
	                    break;
	                }
			}
		}

	irechts = velToMicros(irechts);
	ilinks = velToMicros(ilinks);
	gpioServo(18, irechts);
	gpioServo(17, ilinks);

	sleep(1);
	endwin();
	gpioTerminate();

}



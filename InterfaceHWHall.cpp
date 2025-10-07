#include "InterfaceHWHall.h"

//Constructor zum Erstellen des Objektes
InterfaceHWHall::InterfaceHWHall(void)
{
    _iInstance	= 0;
    _iPinA 		= 0;
    _iPinB 		= 0;
    _iPinAI		= 0;
    _iPinBI		= 0;

}

/*
Funktion zum Initialisieren der Klasse
Parameter:  iPinA fuer Kanal A des Encoders
            iPinB fuer Kanal B des Encoders
            iInstance fuer Instanz der Klasse
            iInstance = 1 fuer rechten Motor (in Fahrtrichtung)
            iInstance = 0 fuer linken Motor (in Fahrtrichtung)
*/
void InterfaceHWHall::Initialize(int iPinA, int iPinB, int iInstance)
{
    //Eingabeparameter abspeichern
    _iInstance = iInstance;
    _iPinA = iPinA;
    _iPinB = iPinB;

    //Pins an arduPi anpassen (siehe Pintabelle)
    /*
      Pintabelle

      Breakboard Pin-Nr.| arduPi Pin-Nr.
      ++++++++++++++++++++++++++++++++++
      4                 | 6
      17                | 7
      18                | 2
      21/27             | 8
      22                | 9
      23                | 3
      24                | 4
      25                | 5

    */
    //Kanal A
    switch(iPinA)
    {   case 18: _iPinAI=2; break;
        case 23: _iPinAI=3; break;
        case 24: _iPinAI=4; break;
        case 25: _iPinAI=5; break;
        case 4 : _iPinAI=6; break;
        case 17: _iPinAI=7; break;
        case 27: _iPinAI=8; break;
        case 22: _iPinAI=9; break;
     }
    //Kanal B
    switch(iPinB)
    {   case 18: _iPinBI=2; break;
        case 23: _iPinBI=3; break;
        case 24: _iPinBI=4; break;
        case 25: _iPinBI=5; break;
        case 4 : _iPinBI=6; break;
        case 17: _iPinBI=7; break;
        case 27: _iPinBI=8; break;
        case 22: _iPinBI=9; break;
     }

    //Anzahl Impulse initialisieren
    _lPulse = 0;

    //Digitale Eingaenge auf INPUT und PULLUP schalten fuer Hallauswertung (unter Verwendung von PIGPIO-Library)
    gpioSetMode(_iPinA, PI_INPUT);
    gpioSetMode(_iPinB, PI_INPUT);
    gpioSetPullUpDown(_iPinA, PI_PUD_UP);
    gpioSetPullUpDown(_iPinB, PI_PUD_UP);

    //Interrupts abhaengig von Instanz des Objekts initialisieren (unter Verwendung von arduPi-Library)
    //Interrupt bei steigender Flanke --> 784 Interrupts pro Umdrehung (siehe auch Speed.cpp)
    if( _iInstance == 0)
    {
        attachInterrupt(_iPinAI, ISRopen0A, BOTH);
        ISR_opener0A = this;
        //attachInterrupt(_iPinB, ISRopen0B, RISING);
        //ISR_opener0B = this;
    }
    if( _iInstance == 1)
    {
        attachInterrupt(_iPinBI, ISRopen1A, BOTH);
        ISR_opener1A = this;
        //attachInterrupt(_iPinB, ISRopen1B, RISING);
        //ISR_opener1B = this;
    }
}

//Zeiger auf Klasse Hall initialisieren
InterfaceHWHall * InterfaceHWHall::ISR_opener0A;
//InterfaceHWHall * InterfaceHWHall::ISR_opener0B;
InterfaceHWHall * InterfaceHWHall::ISR_opener1A;
//InterfaceHWHall * InterfaceHWHall::ISR_opener1B;

//Interrupt-Service-Routinen
void InterfaceHWHall::ISRopen0A(void)
{
  ISR_opener0A->ISR_HallA0();
}
/*void InterfaceHWHall::ISRopen0B(void)
{
  ISR_opener0B->ISR_HallB();
}*/
void InterfaceHWHall::ISRopen1A(void)
{
  ISR_opener1A->ISR_HallA1();
}
/*void InterfaceHWHall::ISRopen1B(void)
{
    ISR_opener1B->ISR_HallB();
}
*/

//Hallsondenauswertung innerhalb ISR fuer Kanal A
void InterfaceHWHall::ISR_HallA0(void)
{
    //wenn Signale gleichlaeufig Drehrichtung fuer linken Motor (Instanze 0) positiv,
    //Impulsanzahl entsprechend Drehrichtung erhoehen
    if(gpioRead(_iPinB)==gpioRead(_iPinA)) _lPulse++;

    //wenn Signale gegenlaeufig Drehrichtung fuer linken Motor (Instanze 0) negativ
    //Impulsanzahl entsprechend Drehrichtung erniedrigen
    else _lPulse--;
}

void InterfaceHWHall::ISR_HallA1(void)
{
    //wenn Signale gleichlaeufig Drehrichtung fuer rechten Motor (Instanze 1) positiv, Impulsanzahl entsprechend Drehrichtung erhoehen
    if(gpioRead(_iPinA) == gpioRead(_iPinB) )_lPulse++;
    //wenn Signale gegenlaeufig Drehrichtung fuer rechten Motor (Instanze 1) negativ, Impulsanzahl entsprechend Drehrichtung erniedrigen
    else _lPulse--;
}

//Hallsondenauswertung innerhalb ISR fuer Kanal B
/*void InterfaceHWHall::ISR_HallB(void)
{
        if(_iInstance==0)
        {
            //wenn Signale gleichlaeufig Drehrichtung fuer rechten Motor (Instanze 0) positiv, Impulsanzahl entsprechend Drehrichtung erhoehen
            if(digitalRead(_iPinA) == HIGH) _lPulse++;
            //wenn Signale gegenlaeufig Drehrichtung fuer rechten Motor (Instanze 0) negativ, Impulsanzahl entsprechend Drehrichtung erniedrigen
            else _lPulse--;
        }

        if(_iInstance==1)
        {
            //wenn Signale gleichlaeäufig Drehrichtung fuer linken Motor (Instanze 1) positiv, Impulsanzahl entsprechend Drehrichtung erhoehen
            if(digitalRead(_iPinA) == HIGH) _lPulse++;
            //wenn Signale gegenlaeufig Drehrichtung fuer linken Motor (Instanze 1) negativ, Impulsanzahl entsprechend Drehrichtung erniedrigen
            else _lPulse--;
        }
}*/

//Lesefunktion fuer Impulsanzahl (Impulsanzahl wird ausgegeben und dann auf 0 gesetzt)
long InterfaceHWHall::GetPulse(void)
{
    long lTemp=_lPulse;
    _lPulse = 0;
    return lTemp;
}

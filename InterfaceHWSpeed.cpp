#include "InterfaceHWSpeed.h"


//Constructor zum Erstellen des Objektes
InterfaceHWSpeed::InterfaceHWSpeed(void)
{
	_iTimebase = 0;
}

/*
Funktion zum Initialisieren des Objektes
Parameter:  iTimebase fuer Zeitbasis
*/
void InterfaceHWSpeed::Initialize(int iTimebase)
{
    //Einganbeparameter abspeichern
    _iTimebase = iTimebase;
}

//Geschwindigkeitsberechnung und Rueckgabe
//Paraketer: lPulse fuar Anzahl gemessener Impulse durch Motorencoder (Hall-Sensoren)
//Achtung: Anzahl Impuls pro Umdrehung muss an Auswertung der Encoder angepasst werden
double InterfaceHWSpeed::GetSpeed(long lPulse)
{
    //Geschwindigkeit = (Impulse * Radumfang/Umdrehung[m] / 784 [Impulse/Umdrehung]) * (1000 [ms/s] / Zeitbasis [ms])
    float fSpeed=( ( ( ( (float)lPulse)  * 0.23) / 784) * (1000.0 / ( (float) _iTimebase) ) );
    return fSpeed;
}

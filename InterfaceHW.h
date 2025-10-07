#ifndef INTERFACEHW_H_
#define INTERFACEHW_H_
#include "InterfaceHWHall.h"
#include "InterfaceHWSpeed.h"
#include "InterfaceHWArdupi.h"
#include <pigpio.h>

#define MOTORRECHTS 18
#define MOTORLINKS 17

class InterfaceHW
{
public:
    //Constructor
	InterfaceHW(void);

    //Destructor
    ~InterfaceHW(void);

    /*
    Methode zum Initialisieren des Hardwareobjektes
    Parameter:	fZeitbasis  Zeitbasis mit der Steuerung arbeitet
    */
    void Initialize(double dTimestep);

    //Timerfunktion setzten
    /*
    Methode um Timerfunktion zu setzten
    Parameter:	fTimer      Timer der benutzt werden soll; 10 Timer verfuegbar (0-9)
                fZeitbasis  Zeitbasis mit Funktion aufgerufen werden soll
                *pFunction  Funktionszeiger auf Funktion die mit Timer ausgefuehrrt wrden soll
    */
    void SetTimerfunction(int iTimer, double dTimestep, void (*pFunction)());

    /*
    Methode zur Erhalten der Inputs (Geschindigkeiten der Motoren)
    */
    double* GetInput(void);

    /*
    Methode zum Setzen der Outputs (Servosignale)
    Parameter:	*iMicors    Zeiger auf Array mit Servosignallaenge fuer rechten und linken Motor
                            erstes Element rechter Motor, zweites Element linker Motor
    */
    void SetOutputs(int *iMicros);

private:
    //Hallobjekte fuer rechten und linken Motor erstellen
    InterfaceHWHall Hall_R;
    InterfaceHWHall Hall_L;
    //Geschwindigkeitsobjekte fuer rechten und linken Motor erstellen
    InterfaceHWSpeed Speed_R;
    InterfaceHWSpeed Speed_L;
    //Array fuer Ausgaenge
    double _adOutput[2];
};

#endif // INTERFACEHW_

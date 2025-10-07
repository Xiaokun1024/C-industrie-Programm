#include "InterfaceHW.h"

//Constructor fuer HardwareObjekt
InterfaceHW::InterfaceHW(void)
{

}

/*
Funktion zum Initialisieren des Hardwareobjektes
Parameter:	iZeitbasis  Zeitbasis mit der Steuerung arbeitet
*/
void InterfaceHW::Initialize(double dTimestep)
{
    //PIGPIO-Library initialisieren
    gpioInitialise();
    //Hallobjekte initialisieren
    Hall_L.Initialize(22, 23, 0); //(Kanal A: Pin 22, Kanal B: Pin 23, Instanz: 0)
    Hall_R.Initialize(24, 25, 1); //(Kanal A: Pin 24, Kanal B: Pin 25, Instanz: 1)
    //Speedobjekte initialisieren
    Speed_R.Initialize((int) (dTimestep*1000.0));
    Speed_L.Initialize((int) (dTimestep*1000.0));
    //Outputs auf null initialisieren
    _adOutput[0]=0.0;
    _adOutput[1]=0.0;
}

//Destructor fuer HardwareObjekt
InterfaceHW::~InterfaceHW(void)
{
    //PIGPIO-Library loeschen
    gpioTerminate();
}

/*
 *Funktion erstellt per Timer ausgefuehrte Funktion
 *Parameter:    iTimer      = Timer der verwendet werden soll; 10 Timer unterstuetzt (0-9)
 *              iZeitbasis  = Zeit zwischen den aufrufen der Funktion [ms]
 *              pFunction   = Zeiger auf auszufuehrende Funktion
 */
void InterfaceHW::SetTimerfunction(int iTimer, double dTimestep, void (*pFunction)())
{
    gpioSetTimerFunc(iTimer, (int) (dTimestep*1000), (*pFunction));
}

/*
Funktion zur Erhalten der Inputs (Geschindigkeiten der Motoren)
*/
double* InterfaceHW::GetInput(void)
{
   //Geschwindigkeit der rechten Motors wird berechnet
   _adOutput[0] = Speed_R.GetSpeed(Hall_R.GetPulse());
   //geschwindigkeit des linken Motors wird berechnet
   _adOutput[1] = Speed_L.GetSpeed(Hall_L.GetPulse());
   //Zeiger auf Array mit Geschwindigkeitswerten wird uebergegen
   return _adOutput;
}

/*
Funktion zum Setzen der Outputs (Servosignale)
Parameter:	*iMicors    Zeiger auf Array mit Servosignallaenge fuer rechten und linken Motor
                        erstes Element rechter Motor, zweites Element linker Motor
*/
void InterfaceHW::SetOutputs(int *iMicros)
{
    //Servosignal setzen
    gpioServo (MOTORRECHTS, *iMicros);     //rechter Motor
    gpioServo (MOTORLINKS, *(iMicros+1));  //linker  Motor
}

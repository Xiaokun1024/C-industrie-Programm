#ifndef INTERFACEHWHALL_H_
#define CSCHNITTSTELLEHWHALL_H_

#include <inttypes.h>
#include "InterfaceHWArdupi.h"
#include <pigpio.h>


class InterfaceHWHall
{
    public:

        //Constructor zum Erstellen der Klasse
        InterfaceHWHall(void);

        /*
        Funktion zum Initialisieren der Klasse
        Parameter:  iPinA fuer Kanal A des Encoders
                    iPinB fuer Kanal B des Encoders
                    iInstance fuer Instanz der Klasse
                    iInstance = 0 fuer rechten Motor (in Fahrtrichtung)
                    iInstance = 1 fuer linken Motor (in Fahrtrichtung)
        */
        void Initialize(int iPinA, int iPinB, int iInstance);

        //Interrupt-Service-Routinen jeweils fuer Kanal A und B
        static void ISRopen0A(void);
        static void ISRopen0B(void);
        static void ISRopen1A(void);
        static void ISRopen1B(void);

        //Zeiger auf Klasse Hall jeweils fuer Kanal A und B
        static InterfaceHWHall *ISR_opener0A;
        static InterfaceHWHall *ISR_opener0B;
        static InterfaceHWHall *ISR_opener1A;
        static InterfaceHWHall *ISR_opener1B;

        //Hallsondenauswertung als ISR fuer Kanal A unf B
        void ISR_HallA0(void);
        void ISR_HallA1(void);
        void ISR_HallB(void);

        //Lesefunktion fuer Impulsanzahl
        long GetPulse(void);

    private:
        //Pin-Variablen fuer Kanal A und B
        int _iPinA;
        int _iPinB;
        int _iPinAI;
        int _iPinBI;

        //Ausgabe fuer Anzahl Impulse (Interrupts)
        volatile long _lPulse;

        //Instanz
        int _iInstance;
};


#endif // INTERFACEHWHALL_H_

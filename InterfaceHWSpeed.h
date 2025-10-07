#ifndef INTERFACEHWSPEED_H_
#define INTERFACEHWSPEED_H_

class InterfaceHWSpeed
{
    public:
        //Constructor zum Erstellen des Objektes
        InterfaceHWSpeed(void);

        /*
        Funktion zum Initialisieren des Objektes
        Parameter:  iTimebase fuer Zeitbasis
        */
        void Initialize(int iTimebase);
        //Gewindigkeitsausgabefunktion
        double GetSpeed(long lPulse);

    private:
        //Zeitbasis
        int _iTimebase;
};

#endif // INTERFACEHWSPEED_H_

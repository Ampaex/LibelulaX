#include <Arduino.h>

class PIDcontrol 
{
    public:
        PIDcontrol(float Kp, float Ki, float Kd, double objetivo)
        {
            this->Kp = Kp;
            this->Ki= Ki;
            this->Kd = Kd;
            this->objetivo = objetivo;
        };

        double recalcular(double entrada);

        void setObjetivo(double objetivo);

        double getUltimaSalida(){return ultimaSalida;};

    private:
        float Kp;
        float Ki;
        float Kd;

        double ultimaSalida, objetivo;

        unsigned long tiempoActual, tiempoAnterior;
        double tiempoTranscurrido;

        double error, ultimoError, errorAcum, ratioError;

};
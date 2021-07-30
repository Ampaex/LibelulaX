#include <PIDcontrol.h>

double PIDcontrol::recalcular(double entrada)
{
    tiempoActual = millis();                                        //Obtiene el tiempo actual
    tiempoTranscurrido = (double)(tiempoActual - tiempoAnterior);   //Calcula el tiempo transcurrido desde el último cálculo

    error = objetivo - entrada;                                     // Determina el error con respecto al objetivo (Parte proporcional)
    errorAcum += error * tiempoTranscurrido;                        // Cáculo de la integral
    ratioError = (error - ultimoError)/tiempoTranscurrido;          // Cálculo de la derivada

    double out = Kp*error + Ki*errorAcum + Kd*ratioError;           //Salida del PID

    ultimoError = error;                                            //Almacena este error como el último
    tiempoAnterior = tiempoActual;                                  //Almacena este tiempo como el último

    ultimaSalida = out;                                             //Recordamos esta última salida

    return out;                                                     //Devuelve el valor PID calculado
}

void PIDcontrol::setObjetivo(double objetivo)
{
    this->objetivo = objetivo;
}
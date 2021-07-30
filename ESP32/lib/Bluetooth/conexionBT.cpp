#include "conexionBT.h"

//Inicializa la conexión bluetooth classic
bool conexionBT::iniciar(String nombre_bt)
{
  SerialBT.begin(nombre_bt);
}

//Envía una cadena al terminal
void conexionBT::enviaString(String texto)
{
    for(char caracter: texto){
        SerialBT.write(caracter);
    }
}

//Comprueba si existen datos disponibles y los recoge
String conexionBT::recibeString()
{
    String texto;
    if (SerialBT.available())
    {
        texto = SerialBT.readString();
    }

    return texto;
}
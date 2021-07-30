#include <conf.h>
#include <Arduino.h>
#include <BluetoothSerial.h>
#include <conexionBT.h>
#include <PIDcontrol.h>


conexionBT SerialBT;

struct coordGPS
{
  float latitud;
  float longitud;
  float altitud;
};


PIDcontrol elevador = PIDcontrol(2, 0, 2, 20);

void setup() 
{
  #ifdef DEBUG
    Serial.begin(115200);
  #endif

  #ifdef BLUETOOTH
    SerialBT.iniciar("LIBÉLULA BLUE");
  #endif

  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);

}

void loop()  
{
  //SerialBT.enviaString(SerialBT.recibeString());

  if (Serial.available())
  {
    elevador.recalcular(Serial.readString().toDouble());
    Serial.println(elevador.getUltimaSalida());
  }
  delay(400);

}

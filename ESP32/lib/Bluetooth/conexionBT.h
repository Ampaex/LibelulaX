#include <Arduino.h>
#include <BluetoothSerial.h>

class conexionBT
{
    public:
        bool iniciar(String nombre_bt);
        void enviaString(String texto);
        String recibeString();

    private:
        BluetoothSerial SerialBT;

};
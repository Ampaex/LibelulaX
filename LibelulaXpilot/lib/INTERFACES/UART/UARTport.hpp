#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <driver/uart.h>
#include <esp_log.h>
#include <esp_event.h>

class UARTport
{
private:
    uart_port_t _uartNum;

public:
    QueueHandle_t _eventQueue;

    /*Writes data to TX buffer.
    @param dataBuffer Pointer to buffer for retrieving data
    @param bufferSize Size of buffer
    @return Number of bytes written. ( -1 if error )
    */
    int writeData(char* dataBuffer, size_t bufferSize);

    /*Reads data from RX buffer. - Blocking function -
    @param dataBuffer Pointer to buffer for storing data
    @param bufferSize Size of buffer
    @return Number of bytes retrieved. ( -1 if error )
    */
    int readData(char *dataBuffer, size_t bufferSize);

    UARTport(uart_port_t uartPort, int baudRate);
    ~UARTport();
};
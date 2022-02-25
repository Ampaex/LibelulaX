#include <UARTport.hpp>

UARTport::UARTport(uart_port_t uartNum = UART_NUM_0, int baudRate = 115200) : _uartNum(uartNum)
{
    const char *TAG = "UARTport";
    if(uart_is_driver_installed(uartNum))
    {
        ESP_LOGE(TAG, "You can only use one UARTport for each UART number");
        abort();
    }
    
    uart_config_t uart_config = {
        .baud_rate = baudRate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    ESP_ERROR_CHECK(uart_driver_install(_uartNum, 2048, 2048, 10, &_eventQueue, 0));

    ESP_ERROR_CHECK(uart_set_mode(_uartNum, UART_MODE_UART));

    ESP_ERROR_CHECK(uart_param_config(_uartNum, &uart_config));

    ESP_ERROR_CHECK(uart_set_pin(_uartNum, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
}

int UARTport::writeData(char *dataBuffer, size_t bufferSize)
{
    return uart_write_bytes(_uartNum, dataBuffer, bufferSize);
}

int UARTport::readData(char *dataBuffer, size_t bufferSize)
{
    const char *TAG = "UARTport-readData";
    uart_event_t event;

    int ret = -1;

    while (1)
    {
        if (xQueueReceive(_eventQueue, &event, portMAX_DELAY))
        {
            switch (event.type)
            {
            // Event of UART receving data
            case UART_DATA:
                ESP_LOGI(TAG, "[UART DATA]: %d", event.size);
                ret = uart_read_bytes(_uartNum, dataBuffer, bufferSize, 100);
                break;
            // Event of HW FIFO overflow detected
            case UART_FIFO_OVF:
                ESP_LOGW(TAG, "hw fifo overflow");
                // If fifo overflow happened, you should consider adding flow control for your application.
                // The ISR has already reset the rx FIFO,
                // As an example, we directly flush the rx buffer here in order to read more data.
                uart_flush_input(_uartNum);
                xQueueReset(_eventQueue);
                break;
            // Event of UART ring buffer full
            case UART_BUFFER_FULL:
                ESP_LOGW(TAG, "ring buffer full");
                // If buffer full happened, you should consider encreasing your buffer size
                // As an example, we directly flush the rx buffer here in order to read more data.
                uart_flush_input(_uartNum);
                xQueueReset(_eventQueue);
                break;
            // Event of UART RX break detected
            case UART_BREAK:
                ESP_LOGW(TAG, "uart rx break");
                break;
            // Event of UART parity check error
            case UART_PARITY_ERR:
                ESP_LOGW(TAG, "uart parity error");
                break;
            // Event of UART frame error
            case UART_FRAME_ERR:
                ESP_LOGW(TAG, "uart frame error");
                break;
            // Others
            default:
                ESP_LOGW(TAG, "uart event type: %d", event.type);
                break;
            }
        }
    }
    return ret;
}

UARTport::~UARTport()
{
    ESP_ERROR_CHECK(uart_driver_delete(_uartNum));
}
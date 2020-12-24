#include "board_comm.h"
#include "app_comm.h"
#include "app_uart.h"
#include "ble_nus.h"
#include "ble_gatt.h"

#if defined(UART_PRESENT)
#include "nrf_uart.h"
#endif
#if defined(UARTE_PRESENT)
#include "nrf_uarte.h"
#endif

uint16_t m_ble_nus_max_data_len = BLE_GATT_ATT_MTU_DEFAULT - 3; /**< Maximum length of data (in bytes) that can be transmitted to the peer by the Nordic UART service module. */
#define UART_TX_BUF_SIZE 256                                    /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 256                                    /**< UART RX buffer size. */
/**@brief   Function for handling app_uart events.
 *
 * @details This function will receive a single character from the app_uart module and append it to
 *          a string. The string will be be sent over BLE when the last character received was a
 *          'new line' '\n' (hex 0x0A) or if the string has reached the maximum data length.
 */
/**@snippet [Handling the data received over UART] */
void uart_event_handle(app_uart_evt_t *p_event)
{
    static uint8_t data_array[BLE_NUS_MAX_DATA_LEN];
    static uint8_t index = 0;
    uint32_t err_code;

    switch (p_event->evt_type)
    {
    case APP_UART_DATA_READY:
        UNUSED_VARIABLE(app_uart_get(&data_array[index]));
        index++;

        if ((data_array[index - 1] == '\n') ||
            (data_array[index - 1] == '\r') ||
            (index >= m_ble_nus_max_data_len))
        {
            if (index > 1)
            {
                NRF_LOG_DEBUG("Ready to send data over BLE NUS");
                NRF_LOG_HEXDUMP_DEBUG(data_array, index);

                do
                {
                    uint16_t length = (uint16_t)index;
                    err_code = app_nus_send(data_array, &length, m_conn_handle);
                    if ((err_code != NRF_ERROR_INVALID_STATE) &&
                        (err_code != NRF_ERROR_RESOURCES) &&
                        (err_code != NRF_ERROR_NOT_FOUND))
                    {
                        APP_ERROR_CHECK(err_code);
                    }
                } while (err_code == NRF_ERROR_RESOURCES);
            }

            index = 0;
        }
        break;

    case APP_UART_COMMUNICATION_ERROR:
        APP_ERROR_HANDLER(p_event->data.error_communication);
        break;

    case APP_UART_FIFO_ERROR:
        APP_ERROR_HANDLER(p_event->data.error_code);
        break;

    default:
        break;
    }
}
/**@snippet [Handling the data received over UART] */

/**@brief  Function for initializing the UART module.
 */
/**@snippet [UART Initialization] */
void bsp_uart_init(void)
{
    uint32_t err_code;
    app_uart_comm_params_t const comm_params =
    {
        .rx_pin_no = RX_PIN_NUMBER,
        .tx_pin_no = TX_PIN_NUMBER,
        .rts_pin_no = RTS_PIN_NUMBER,
        .cts_pin_no = CTS_PIN_NUMBER,
        .flow_control = APP_UART_FLOW_CONTROL_DISABLED,
        .use_parity = false,
#if defined(UART_PRESENT)
        .baud_rate = NRF_UART_BAUDRATE_115200
#else
        .baud_rate = NRF_UARTE_BAUDRATE_115200
#endif
    };

    APP_UART_FIFO_INIT(&comm_params,
                       UART_RX_BUF_SIZE,
                       UART_TX_BUF_SIZE,
                       uart_event_handle,
                       APP_IRQ_PRIORITY_LOWEST,
                       err_code);
    APP_ERROR_CHECK(err_code);
}

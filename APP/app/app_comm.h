#ifndef APP_COMM_H__
#define APP_COMM_H__
#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include <string.h>

#include "bsp.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

    void app_ble_init(void);
    void services_init(void);
    uint32_t app_ble_qwr_conn_handle_assign(uint16_t conn_handle);
    uint32_t app_nus_send(uint8_t *p_data, uint16_t *p_length, uint16_t conn_handle);

    uint32_t app_advertising_restart_without_whitelist(void);

    void sleep_mode_enter(void);

    extern uint16_t m_conn_handle;
    extern uint16_t m_ble_nus_max_data_len;
#endif

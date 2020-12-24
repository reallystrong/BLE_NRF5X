#ifndef BOARD_COMM_H__
#define BOARD_COMM_H__
#ifdef __cplusplus
extern "C"
{
#endif

#include "bsp.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

    void bsp_uart_init(void);
    void bsp_buttons_leds_init(void);
    uint32_t bsp_btn_ble_sleep_mode_prepare(void);

#endif

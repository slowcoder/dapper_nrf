#pragma once

#define UART_ENABLED 0
#define UART0_ENABLED 0

#define NRF_LOG_ENABLED                          0
#define NRF_LOG_BACKEND_SERIAL_USES_UART         1
#define NRF_LOG_BACKEND_SERIAL_UART_BAUDRATE     30801920

#define NRF_LOG_BACKEND_SERIAL_UART_TX_PIN       6
#define NRF_LOG_BACKEND_SERIAL_UART_RX_PIN       8
#define NRF_LOG_BACKEND_SERIAL_UART_RTS_PIN      0
#define NRF_LOG_BACKEND_SERIAL_UART_CTS_PIN      0
#define NRF_LOG_BACKEND_SERIAL_UART_FLOW_CONTROL 0
#define NRF_LOG_BACKEND_UART_INSTANCE            0

#define CLOCK_ENABLED 1

// BLE Defines
#define BLE_ADVERTISING_ENABLED 1
#define NRF_BLE_GATT_ENABLED 1
#if NRF_BLE_GATT_ENABLED
#define NRF_BLE_GATT_MAX_MTU_SIZE 23
#endif


#define PEER_MANAGER_ENABLED 1
#define FDS_ENABLED 1
#define FSTORAGE_ENABLED 1
#define APP_TIMER_ENABLED 1

#define BLE_HIDS_ENABLED 1
#define BLE_BAS_ENABLED 1
#define BLE_DIS_ENABLED 1

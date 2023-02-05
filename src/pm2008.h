/***********************************************************************************************************************
 * File Name    : uart_ep.h
 * Description  : Contains function declaration of uart_ep.c and Macros.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/

#ifndef PM2008_H_
#define PM2008_H_

#include "common_utils.h"

/* Macro definition */
#define CARRIAGE_ASCII            (13u)     /* Carriage return */
#define ZERO_ASCII                (48u)     /* ASCII value of zero */
#define NINE_ASCII                (57u)     /* ASCII value for nine */
#define DATA_LENGTH               (64u)      /* Expected Input Data length */
#define UART_ERROR_EVENTS         (UART_EVENT_BREAK_DETECT | UART_EVENT_ERR_OVERFLOW | UART_EVENT_ERR_FRAMING | \
                                    UART_EVENT_ERR_PARITY)    /* UART Error event bits mapped in registers */
#define UART_MSG_NOTSET            -1


#define PM2008_DEBUG                0

// Control modes
#define PM2008_UART_START_SYMBOL                0x11
#define PM2008_UART_RESPONSE_SYMBOL             0x16

//Command Table of Serial Protocol
#define PM2008_UART_CMD_READ_PARTICLE                     0x0B
#define PM2008_UART_CMD_OPEN_CLOSE_PARTICLE_MEASUREMENT   0x0C
#define PM2008_UART_CMD_SETUP_PARTICLE_MEASUREMENT        0x0D
#define PM2008_UART_CMD_SETUP_TIMING_MEASUREMENT          0x05
#define PM2008_UART_CMD_SETUP_DYNAMIC_WORKING             0x06
#define PM2008_UART_CMD_SETUP_CALIBRATED_COEFFICIENT      0x07
#define PM2008_UART_CMD_OPEN_CLOSE_LASER_DIODE            0x08
#define PM2008_UART_CMD_READ_SW_VERSION                   0x1E
#define PM2008_UART_CMD_READ_SERIAL_NUMBER                0x1F


#define PM2008_05UM_THRESHOLD             10

/* Function declaration releated uart definitions */
fsp_err_t uart_send_cmd(uint8_t *p_msg, uint8_t length);
fsp_err_t uart_initialize(void);
void deinit_uart(void);
void uart_rx_mesage(void);

/* Function Decleartion based on PM Setting */
void pm2008_tx_read_measurement(void);
void pm2008_tx_open_particle_measurement(void);
void pm2008_tx_close_particle_measurement(void);
void pm2008_tx_setup_continuous_particle_measuring(void);
void pm2008_openning_sequence(void);
void pm2008_sleep_mode_on(void);
void pm2008_sleep_mode_off(void);
void pm2008_action(void);

typedef struct {
    uint8_t reserved : 4;
    uint8_t low_working_temperature: 1;
    uint8_t high_working_temperature: 1;
    uint8_t fan_low_speed :1;
    uint8_t fan_high_speed :1;
}tsPM2008Alarm;

typedef struct {
  uint8_t     status;
  uint16_t    measuring_mode;
  uint16_t    measuring_time;
  uint16_t    pm1p0_grimm;
  uint16_t    pm2p5_grimm;
  uint16_t    pm10_grimm;
  uint16_t    pm1p0_tsi;
  uint16_t    pm2p5_tsi;
  uint16_t    pm10_tsi;
  uint16_t    number_of_0p3_um;
  uint16_t    number_of_0p5_um;
  uint16_t    number_of_1_um;
  uint16_t    number_of_2p5_um;
  uint16_t    number_of_5_um;
  uint16_t    number_of_10_um;
  tsPM2008Alarm alarm_flag;
}tsPM2008;



#endif /* PM2008_H_ */

/***********************************************************************************************************************
 * File Name    : uart.c
 * Description  : Contains UART functions definition.
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

#include "pm2008.h"

/*******************************************************************************************************************//**
 * @addtogroup r_sci_uart_ep
 * @{
 **********************************************************************************************************************/

/*
 * External Global Variables
 */
/* LED Structure used to blink on board LED */
extern bsp_leds_t g_bsp_leds;

/*
 * Private function declarations
 */

/*
 * Private global variables
 */
/* Temporary buffer to save data from receive buffer for further processing */
static uint8_t g_temp_buffer[DATA_LENGTH] = { RESET_VALUE };

/* Rx buffer to save data from temporary buffer for decoding */
static uint8_t g_rx_buffer[DATA_LENGTH] = { RESET_VALUE };

/* Temporary buffer to save data for transmit buffer for further processing */
static uint8_t g_tx_buffer[DATA_LENGTH] = { RESET_VALUE };

/* Counter to update g_temp_buffer index */
static volatile uint8_t g_counter_var = RESET_VALUE;

/* Variable buffer length for g_temp_buffer index */
static volatile int8_t g_buff_len = UART_MSG_NOTSET;

/* Flag to check whether data is received or not */
uint8_t g_data_received_flag = false;

/* Flag for user callback */
static volatile uint8_t g_uart_event = RESET_VALUE;

/* Struct value for pm2008 */
tsPM2008 pm2008 = { RESET_VALUE };

/* Static Function Declerations */
static uint8_t calculateCRC(const uint8_t *const buffer, int8_t dataLength);
static void resetUartRXVariables(void);
static fsp_err_t pm2008_decode_message(const uint8_t *const buffer,int8_t dataLength);
static uint16_t byte_2_short(const uint8_t *const buffer, uint8_t index);

/*******************************************************************************************************************//**
 * @brief       Initialize  UART.
 * @param[in]   None
 * @retval      FSP_SUCCESS         Upon successful open and start of timer
 * @retval      Any Other Error code apart from FSP_SUCCESS  Unsuccessful open
 ***********************************************************************************************************************/
fsp_err_t uart_initialize(void)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Initialize UART channel with baud rate 9600 */
    err = R_SCI_UART_Open (&g_uart_ctrl, &g_uart_cfg);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("\r\n**  R_SCI_UART_Open API failed  **\r\n");
    }
    return err;
}

/*****************************************************************************************************************
 *  @brief       send command to terminal rs terminal
 *  @param[in]   p_msg
 *  @retval      FSP_SUCCESS                Upon success
 *  @retval      FSP_ERR_TRANSFER_ABORTED   Upon event failure
 *  @retval      Any Other Error code apart from FSP_SUCCESS,  Unsuccessful write operation
 ****************************************************************************************************************/
fsp_err_t uart_send_cmd(uint8_t *p_msg, uint8_t length)
{
    fsp_err_t err = FSP_SUCCESS;
    uint8_t msg_len = length;
    uint32_t local_timeout = (DATA_LENGTH * UINT16_MAX);

    /* Reset callback capture variable */
    g_uart_event = RESET_VALUE;

    /* Writing to terminal */
    err = R_SCI_UART_Write (&g_uart_ctrl, p_msg, msg_len);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("\r\n**  R_SCI_UART_Write API Failed  **\r\n");
        return err;
    }
#if PM2008_DEBUG
    APP_PRINT("Send Data: ");
    for (int i = 0; i < length; i++)
    {
        APP_PRINT(" 0x%x", *(p_msg + i));
    }
    APP_PRINT("\n");
#endif
    /* Check for event transfer complete */
    while ((UART_EVENT_TX_COMPLETE != g_uart_event) && (--local_timeout))
    {
        /* Check if any error event occurred */
        if (UART_ERROR_EVENTS == g_uart_event)
        {
            APP_ERR_PRINT("\r\n**  UART Error Event Received  **\r\n");
            return FSP_ERR_TRANSFER_ABORTED;
        }
    }
    if (RESET_VALUE == local_timeout)
    {
        err = FSP_ERR_TIMEOUT;
    }
    return err;
}

/*******************************************************************************************************************//**
 *  @brief       Deinitialize SCI UART module
 *  @param[in]   None
 *  @retval      None
 **********************************************************************************************************************/
void deinit_uart(void)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Close module */
    err = R_SCI_UART_Close (&g_uart_ctrl);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("\r\n**  R_SCI_UART_Close API failed  ** \r\n");
    }
}

/*****************************************************************************************************************
 *  @brief      UART user callback
 *  @param[in]  p_args
 *  @retval     None
 ****************************************************************************************************************/
void user_uart_callback(uart_callback_args_t *p_args)
{
    /* Logged the event in global variable */
    g_uart_event = (uint8_t) p_args->event;

    if (UART_EVENT_RX_CHAR == p_args->event)
    {
        /* Reset g_temp_buffer index if it exceeds than buffer size */
        if (DATA_LENGTH == g_counter_var)
        {
            g_counter_var = RESET_VALUE;
            memset (g_temp_buffer, RESET_VALUE, DATA_LENGTH);
        }

        // Detect variable length for
        if (g_counter_var == 1 && (g_temp_buffer[0]== 0x16))
        {
            g_buff_len = (int8_t ) p_args->data + 3; // Data length(p->arg) + HEADER + LENGTH + CRC
        }

        g_temp_buffer[g_counter_var++] = (uint8_t ) p_args->data;
    }

    //uart_rx_mesage();
}

/*****************************************************************************************************************
 *  @brief       print received message segger terminal
 ****************************************************************************************************************/
void uart_rx_mesage(void)
{
    if ((int8_t) g_counter_var == g_buff_len)
    {
        uint8_t crc = calculateCRC (&g_temp_buffer[g_counter_var - g_buff_len], g_buff_len - 1);
        if (crc == g_temp_buffer[g_counter_var])
        {
            g_data_received_flag = true;
            pm2008_decode_message(g_temp_buffer,g_buff_len);
        }
        else
        {
            resetUartRXVariables();
        }
    }else if((int8_t) g_counter_var > g_buff_len){
        resetUartRXVariables();
    }

    if (g_data_received_flag)
    {
#if PM2008_DEBUG
        APP_PRINT("Received Data: ");
        for (int i = 0; i < g_buff_len; i++)
        {
            APP_PRINT(" 0x%x", g_temp_buffer[g_counter_var - g_buff_len + i]);
        }
        APP_PRINT("\n");
#endif
        g_data_received_flag = false;
        resetUartRXVariables();
    }
}


/*****************************************************************************************************************
 *  @brief       Calculates crc. Formula:
 *               Cumulative sum of data = 256- (HEAD+LEN+CMD+DATA)
 ****************************************************************************************************************/
static uint8_t calculateCRC(const uint8_t *const buffer, int8_t dataLength)
{
    const int coefficient = 256;
    uint8_t sum = 0;
    for (int i = 0; i <= dataLength; i++)
    {
        sum += *(buffer + i);
    }
    return (uint8_t) (coefficient - sum);
}

/*****************************************************************************************************************
 *  @brief       Resest ISR variables for UART
 ****************************************************************************************************************/
static void resetUartRXVariables(void)
{
     g_counter_var = RESET_VALUE;
     g_buff_len = UART_MSG_NOTSET;
     /* Clear data_valid flag as data is not valid, Clear the buffer and break the loop */
     memset (g_temp_buffer, RESET_VALUE, DATA_LENGTH);
}

/*******************************************************************************************************************//**
 * @} (end addtogroup r_sci_uart_ep)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup pm2008 functions
 * @{
  **********************************************************************************************************************/
/*****************************************************************************************************************
 *  @brief       Read Command for pm2008
 ****************************************************************************************************************/
void pm2008_tx_read_measurement(void){
    fsp_err_t err = FSP_SUCCESS;

    g_tx_buffer[0] = 0x11;
    g_tx_buffer[1] = 0x02; // frame length
    g_tx_buffer[2] = 0x0B;
    g_tx_buffer[3] = 0x07;
    g_tx_buffer[4] = 0xDB;

    err = uart_send_cmd(&g_tx_buffer[0], 5);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("** UART - Read Command FAILED ** \r\n");
    }else{
        APP_PRINT("** UART - Read Command SUCCESSFULL ** \r\n");
    }
}

/*****************************************************************************************************************
 *  @brief       Open particle measurement for pm2008
 ****************************************************************************************************************/
void pm2008_tx_open_particle_measurement(void){
    fsp_err_t err = FSP_SUCCESS;

    g_tx_buffer[0] = 0x11;
    g_tx_buffer[1] = 0x03; // frame length
    g_tx_buffer[2] = 0x0C;
    g_tx_buffer[3] = 0x02;
    g_tx_buffer[4] = 0x1E;
    g_tx_buffer[5] = 0xC0;

    err = uart_send_cmd(&g_tx_buffer[0], 6);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT ("** UART - Open Particle Measurement FAILED ** \r\n");
    }else{
        APP_PRINT ("** UART - Open Particle Measurement SUCCESSFULL ** \r\n");
    }
}

/*****************************************************************************************************************
 *  @brief       Close particle measurement for pm2008
 ****************************************************************************************************************/
void pm2008_tx_close_particle_measurement(void){
    fsp_err_t err = FSP_SUCCESS;

    g_tx_buffer[0] = 0x11;
    g_tx_buffer[1] = 0x03; // frame length
    g_tx_buffer[2] = 0x0C;
    g_tx_buffer[3] = 0x01;
    g_tx_buffer[4] = 0x1E;
    g_tx_buffer[5] = 0xC1;

    err = uart_send_cmd(&g_tx_buffer[0], 6);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT ("** UART - Close Particle Measurement FAILED ** \r\n");
    }else{
        APP_PRINT ("** UART - Close Particle Measurement SUCCESSFULL ** \r\n");
    }
}

/*****************************************************************************************************************
 *  @brief       Set up and read particle measurement time for pm2008
 ****************************************************************************************************************/
void pm2008_tx_setup_continuous_particle_measuring(void){
    fsp_err_t err = FSP_SUCCESS;

    g_tx_buffer[0] = 0x11;
    g_tx_buffer[1] = 0x03; // frame length
    g_tx_buffer[2] = 0x0D;
    g_tx_buffer[3] = 0xFF;
    g_tx_buffer[4] = 0xFF;
    g_tx_buffer[5] = 0xE1;

    err = uart_send_cmd(&g_tx_buffer[0], 6);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT ("** UART - Setup continuous Particle Measurement FAILED ** \r\n");
    }else{
        APP_PRINT ("** UART - Setup continuous Particle Measurement SUCCESSFULL ** \r\n");
    }
}

/*****************************************************************************************************************
 *  @brief       Performs required openning sequence for pm2008
 ****************************************************************************************************************/
void pm2008_openning_sequence(void){
    /* OPEN Particle measurement mode in sensor */
    pm2008_tx_open_particle_measurement();
    R_BSP_SoftwareDelay(500, BSP_DELAY_UNITS_MILLISECONDS);

    /* Setup continuous measurement mode in sensor */
    pm2008_tx_setup_continuous_particle_measuring();
    R_BSP_SoftwareDelay(500, BSP_DELAY_UNITS_MILLISECONDS);

    APP_PRINT ("** PM2008 - Openning Sequence Completed ** \r\n");
}

/*****************************************************************************************************************
 *  @brief      puts pm2008 on sleep mode
 ****************************************************************************************************************/
void pm2008_sleep_mode_on(void){
    R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) BSP_IO_PORT_01_PIN_09, BSP_IO_LEVEL_LOW);
    APP_PRINT ("** PM2008 - Sleep Mode Activated ** \r\n");
}
/*****************************************************************************************************************
 *  @brief      puts pm2008 on normal mode, wakes up sensor
 ****************************************************************************************************************/
void pm2008_sleep_mode_off(void){
    R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) BSP_IO_PORT_01_PIN_09, BSP_IO_LEVEL_HIGH);
    APP_PRINT ("** PM2008 - Normal Mode Activated** \r\n");
}

/*****************************************************************************************************************
 *  @brief      takes action based on pm2008 on normal mode
 ****************************************************************************************************************/
void pm2008_action(void){
    // Based on the value of pm particle, led is either turned on either turned off
    if(pm2008.number_of_0p5_um > PM2008_05UM_THRESHOLD){
        R_IOPORT_PinWrite (&g_ioport_ctrl, (bsp_io_port_pin_t)g_bsp_leds.p_leds[0], BSP_IO_LEVEL_HIGH);
    }else{
        R_IOPORT_PinWrite (&g_ioport_ctrl, (bsp_io_port_pin_t)g_bsp_leds.p_leds[0], BSP_IO_LEVEL_LOW);
    }

}

/*****************************************************************************************************************
 *  @brief       decode pm2008 measurement data
 ****************************************************************************************************************/

static fsp_err_t pm2008_decode_message(const uint8_t *const buffer,int8_t dataLength){
    fsp_err_t err = FSP_SUCCESS;
    memcpy(g_rx_buffer,buffer,(size_t)dataLength);

    // Check if 1st address is 0x16
    if(g_rx_buffer[0] != PM2008_UART_RESPONSE_SYMBOL){
        err = FSP_ERR_INVALID_LINKED_ADDRESS;
        return err;
    }

    switch(g_rx_buffer[2]){
        case PM2008_UART_CMD_READ_PARTICLE:
            pm2008.pm1p0_grimm = byte_2_short(g_rx_buffer,3);
            pm2008.pm2p5_grimm= byte_2_short(g_rx_buffer,7);
            pm2008.pm10_grimm = byte_2_short(g_rx_buffer,11);
            pm2008.pm1p0_tsi = byte_2_short(g_rx_buffer,15);
            pm2008.pm2p5_tsi= byte_2_short(g_rx_buffer,19);
            pm2008.pm10_tsi = byte_2_short(g_rx_buffer,23);
            pm2008.number_of_0p3_um = byte_2_short(g_rx_buffer,27);
            pm2008.number_of_0p5_um = byte_2_short(g_rx_buffer,31);
            pm2008.number_of_1_um = byte_2_short(g_rx_buffer,35);
            pm2008.number_of_2p5_um = byte_2_short(g_rx_buffer,39);
            pm2008.number_of_5_um = byte_2_short(g_rx_buffer,43);
            pm2008.number_of_10_um = byte_2_short(g_rx_buffer,47);
            pm2008.alarm_flag = *((tsPM2008Alarm*)&g_rx_buffer[51]);

            APP_PRINT ("**Received PM2008 Sensor DATA:** \r\n");
            //APP_PRINT ("**PM1.0 Grimm: %d, PM2.5 Grimm: %d, PM10.0 Grimm: %d** \r\n",pm2008.pm1p0_grimm,pm2008.pm2p5_grimm,pm2008.pm10_grimm);
            //APP_PRINT ("**PM1.0 TSI: %d, PM2.5 TSI: %d, PM10.0 TSI: %d** \r\n",pm2008.pm1p0_tsi,pm2008.pm2p5_tsi,pm2008.pm10_tsi);
            APP_PRINT ("**#Number 0.3 um: %d, **#Number 0.5 um: %d, **#Number 1.0 um: %d, ** \r\n",pm2008.number_of_0p3_um,pm2008.number_of_0p5_um,pm2008.number_of_1_um);
            break;
        case PM2008_UART_CMD_OPEN_CLOSE_PARTICLE_MEASUREMENT:
            pm2008.status = g_rx_buffer[3];
            APP_PRINT ("**OPEN/CLOSE Status: %x ** \r\n",pm2008.status);
            break;

        case PM2008_UART_CMD_SETUP_PARTICLE_MEASUREMENT:
            pm2008.measuring_time = (uint16_t)((((uint16_t)g_rx_buffer[3])<< 8) | ((uint16_t)g_rx_buffer[4]));
            APP_PRINT ("**Measuring time: %x ** \r\n",pm2008.measuring_time);
            break;

        default:
            break;

    }


    return err;
}

/*****************************************************************************************************************
 *  @brief       func for PM2008_UART_CMD_READ_PARTICLE. 4 bytes 1 short value
 ****************************************************************************************************************/

static uint16_t byte_2_short(const uint8_t *const buffer, uint8_t index){
    return (uint16_t)((((uint16_t)buffer[index])<< 24) | (((uint16_t)buffer[index+1])<< 16) | (((uint16_t)buffer[index+2])<< 8) | ((uint16_t)buffer[index+3]));
}

/*******************************************************************************************************************//**
 * @} (end addtogroup rpm2008 functions)
 **********************************************************************************************************************/

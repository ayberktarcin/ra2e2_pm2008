/***********************************************************************************************************************
 * File Name    : timer_setup.c
 * Description  : Contains data structures and functions used in timer_setup.c
 **********************************************************************************************************************/

#include "common_utils.h"
#include "timer_setup.h"
#include "wdt.h"

/*******************************************************************************************************************//**
 * @addtogroup r_wdt_ep
 * @{
 **********************************************************************************************************************/

/*
 * External Global Variables
 */
/* LED Structure used to blink on board LED */
extern bsp_leds_t g_bsp_leds;

/*
 * Global Variables
 */
/* Variable to capture timer overflow counts and print WDT status */
volatile bool g_timer_callback_flag = false;

/*******************************************************************************************************************//**
 * @brief     Initialize the GPT timer in Periodic mode.
 * @param[IN]   None
 * @retval FSP_SUCCESS                  Upon successful open
 * @retval Any Other Error code apart from FSP_SUCCES  Unsuccessful open
 ***********************************************************************************************************************/
fsp_err_t init_gpt_module(void)
{
    /* variable to track error and return values */
    fsp_err_t err = FSP_SUCCESS;

    /* Open GPT module */
    err = R_GPT_Open(&g_timer_ctrl, &g_timer_cfg);
    if (FSP_SUCCESS != err)
    {
        /* Print Error on RTT console */
        APP_ERR_PRINT ("\r\n ** R_GPT_TimerOpen API failed ** \r\n");
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief    Start the GPT HAL timer and return the error to the Application.
 * @param[IN]   None
 * @retval FSP_SUCCESS                 Timer driver closed successfully.
 * @retval Any Other Error code apart from FSP_SUCCES  Unsuccessful to start timer
 ***********************************************************************************************************************/
fsp_err_t timer_start(void)
{
    /* variable to track error and return values */
    fsp_err_t err = FSP_SUCCESS;

    /* Start GPT timer */
    err = R_GPT_Start(&g_timer_ctrl);
    if (FSP_SUCCESS != err)
    {
        /* Print Error on RTT console */
        APP_ERR_PRINT ("\r\n ** R_GPT_Start API failed ** \r\n");
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief    Start the GPT HAL timer and return the error to the Application.
 * @param[IN]   None
 * @retval FSP_SUCCESS                 Timer driver closed successfully.
 * @retval Any Other Error code apart from FSP_SUCCES  Unsuccessful to start timer
 ***********************************************************************************************************************/
fsp_err_t timer_stop(void)
{
    /* variable to track error and return values */
    fsp_err_t err = FSP_SUCCESS;

    /* Stop GPT timer */
    err = R_GPT_Stop(&g_timer_ctrl);
    if (FSP_SUCCESS != err)
    {
        /* Print Error on RTT console */
        APP_ERR_PRINT("\r\n ** R_GPT_Stop API failed ** \r\n");
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief    Close the GPT HAL driver and Handle the return closing API error, to the Application.
 * @param[IN]   None
 * @retval      None
 ***********************************************************************************************************************/
void deinit_gpt_module(void)
{
    /* variable to track error and return values */
    fsp_err_t err = FSP_SUCCESS;

    /* Close the GPT module */
    err=  R_GPT_Close(&g_timer_ctrl);
    /* handle error */
    if (FSP_SUCCESS != err)
    {
        /* Print Error on RTT console */
        APP_ERR_PRINT ("\r\n ** R_GPT_Close API failed ** \r\n");
    }
}

/*******************************************************************************************************************//**
 * This function is called when gpt timer's counter wrapped around.
 * @brief    Refresh WDT counter and toggle LED state.
 * @param[IN]   p_args   Callback function parameter data
 * @retval      None
 **********************************************************************************************************************/
void gpt_callback(timer_callback_args_t *p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);

    static bsp_io_level_t level_led = BSP_IO_LEVEL_HIGH;
    /* Toggle LED */
    level_led ^= BSP_IO_LEVEL_HIGH;
    R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t)g_bsp_leds.p_leds[1], level_led);
    APP_PRINT ("** R_GPT_Callback API successful!** \r\n");

    g_timer_callback_flag =true;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup r_wdt_ep)
 **********************************************************************************************************************/

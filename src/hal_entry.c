/***********************************************************************************************************************
 * File Name    : hal_entry.c
 * Description  : Contains data structures and functions used in hal_entry.c.
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

#include "common_utils.h"
#include "timer_setup.h"
#include "ext_irq_setup.h"
#include "pm2008.h"
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
extern volatile uint8_t g_timer_overflow_counter;

/*
 * function declarations
 */
void R_BSP_WarmStart(bsp_warm_start_event_t event);

/*
 * private function declarations
 */
/* function used to read RTT input and proceed */
static fsp_err_t read_Input_from_RTT(void);
/* function used to check if reset is caused by wdt or not */
static void check_reset_status(void);

/*******************************************************************************************************************//**
 * The RA Configuration tool generates main() and uses it to generate threads if an RTOS is used.  This function is
 * called by main() when no RTOS is used.
 **********************************************************************************************************************/
void hal_entry(void)
{
    fsp_pack_version_t version = {RESET_VALUE};
    fsp_err_t err = FSP_SUCCESS;

    /* Version get API for FLEX pack information */
    R_FSP_VersionGet(&version);

    /* Example Project information printed on the Console */
    APP_PRINT (BANNER_1);
    APP_PRINT (BANNER_2);
    APP_PRINT (BANNER_3, EP_VERSION);
    APP_PRINT (BANNER_4, version.version_id_b.major, version.version_id_b.minor, version.version_id_b.patch);
    APP_PRINT (BANNER_5);

    APP_PRINT ("\r\n FSP Modules used in this project: gpt, lpm, uart, ext_irq\n");

    /* If this board has no LEDs then trap here */
    if (RESET_VALUE == g_bsp_leds.led_count)
    {
        APP_PRINT ("\r\nNo on-board LEDs Found");
        APP_ERR_TRAP (FSP_ERR_UNSUPPORTED);
    }

    /* Initializing for detection of reset status */
    check_reset_status();

    /* Initializing GPT in PWM Mode */
    err = init_gpt_module();

    /* Initializing External IRQ */
    err = init_icu_module();

    /* Enable WDT to count and generate NMI or Reset when the debugger(JLink) is connected. */
    enable_wdt_count_in_debug_mode();

    /* Open WDT. For every GPT timeout, wdt will get refreshed. */
    err = init_wdt_module();

    /* Start GPT timer in Periodic mode */
    err = timer_start();


    while(true)
    {
        /* Refresh WDT, if user has not pressed the push button */
        wdt_refresh();

//        /* Process input only when User has provided one */
//        if(APP_CHECK_DATA)
//        {
//            err = read_Input_from_RTT();
//            if(FSP_SUCCESS != err)
//            {
//                /* Close timer and external IRQ module */
//                deinit_gpt_module();
//                deinit_icu_module();
//                APP_ERR_TRAP (err);
//            }
//        }

        R_BSP_SoftwareDelay(1000, BSP_DELAY_UNITS_MILLISECONDS);
    }
}

/*******************************************************************************************************************//**
 * This function is called to read user input from RTT using RTT API.
 * @brief    Read user input from RTT. Handle the Error internally with Proper Message. Application handles the rest.
 * @param[IN]   None
 * @retval      Any Other Error code apart from FSP_SUCCES  Unsuccessful to perform operation
 **********************************************************************************************************************/
fsp_err_t read_Input_from_RTT(void)
{
    fsp_err_t err = FSP_SUCCESS;     // Error status

    /* Variable to store user input */
    uint8_t rByte[BUFFER_SIZE] = { RESET_VALUE };
    uint8_t user_data = RESET_VALUE;

    /* Read First byte of data provided by user */
    APP_READ(rByte);

    user_data = (uint8_t) atoi((char *)&rByte);

    switch (user_data)
    {
        case ENABLE_WDT:
        {
            /* Enable WDT to count and generate NMI or Reset when the debugger(JLink) is connected. */
            enable_wdt_count_in_debug_mode();

            /* Open WDT. For every GPT timeout, wdt will get refreshed. */
            err = init_wdt_module();

            /* Start GPT timer in Periodic mode */
            err = timer_start();
            if (FSP_SUCCESS != err)
            {
                APP_ERR_PRINT ("\r\n ** GPT TIMER START FAILED ** \r\n");
                return err;
            }

            /* Enable External IRQ */
            err = enable_icu_module();
            if(FSP_SUCCESS != err)
            {
                APP_ERR_PRINT ("\r\n ** EXTERNAL IRQ ENABLE FAILED ** \r\n");
                return err;
            }

            /* Print message to indicate user about application status. */
            APP_PRINT ("\r\nWDT initialized, GPT Timer Started")
            APP_PRINT("\r\nTo stop WDT counter from refreshing, press the push button\r\n");
        }
        break;

        default:
        {
            /* Menu for User Selection */
            APP_PRINT ("\r\nInvalid Input");
            APP_PRINT ("\r\nPlease provide input from below option");
            APP_PRINT ("\r\nEnter 1 to Enable WDT\r\n");
            APP_PRINT ("User Input:  ");
        }
        break;
    }
    user_data = RESET_VALUE;
    return err;
}

/*******************************************************************************************************************//**
 * This function is called to check whether reset is caused by WDT.
 * @brief    Check if reset caused by WDT or not.
 * @param[IN]   None
 * @retval      None
 **********************************************************************************************************************/
static void check_reset_status(void)
{
    /* Check if reset was caused by the WDT? If it was, turn on LED to indicate WDT reset triggered. */
    if (SYSTEM_RSTSR1_WDTRF_DETECT_WDT_RESET == R_SYSTEM->RSTSR1_b.WDTRF)
    {
        /* Clear the flag once read the value */
        R_SYSTEM->RSTSR1_b.WDTRF = RESET_VALUE;

        /* Turn ON LED */
#if defined (BOARD_RA4W1_EK) || defined (BOARD_RA6T1_RSSK)
        R_IOPORT_PinWrite (&g_ioport_ctrl, (bsp_io_port_pin_t)g_bsp_leds.p_leds[0], BSP_IO_LEVEL_LOW);
#else
        R_IOPORT_PinWrite (&g_ioport_ctrl, (bsp_io_port_pin_t)g_bsp_leds.p_leds[0], BSP_IO_LEVEL_HIGH);
#endif
        APP_PRINT ("\r\n************************ WDT Reset detected ************************\r\n");
    }
}

/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/
void R_BSP_WarmStart(bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        R_IOPORT_Open (&g_ioport_ctrl, &g_bsp_pin_cfg);
    }
}

/*******************************************************************************************************************//**
 * @} (end addtogroup r_wdt_ep)
 **********************************************************************************************************************/

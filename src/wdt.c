/***********************************************************************************************************************
 * File Name    : wdt.c
 * Description  : Contains watchdog related functions
 **********************************************************************************************************************/

#include "common_utils.h"
#include "wdt.h"

/*
 * External Global Variables
 */
/* LED Structure used to blink on board LED */
extern bsp_leds_t g_bsp_leds;

/*
 * Global Variables
 */
/* Variable to capture timer overflow counts and print WDT status */
volatile uint32_t g_wdt_counter = RESET_VALUE;


fsp_err_t init_wdt_module(void){
    fsp_err_t err = FSP_SUCCESS;     // Error status
    err = R_WDT_Open (&g_wdt_ctrl, &g_wdt_cfg);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT ("\r\n ** R_WDT_Open API Failed ** \r\n");
        return err;
    }
    return err;
}

fsp_err_t wdt_refresh(void){
    fsp_err_t err = FSP_SUCCESS;     // Error status
    static bsp_io_level_t level_led = BSP_IO_LEVEL_HIGH;

    /* Refresh WDT*/
    err = R_WDT_Refresh(&g_wdt_ctrl);
    if (FSP_SUCCESS != err)
    {
        /* Turn ON LED to indicate error, along with output on RTT*/
#if defined (BOARD_RA4W1_EK) || defined (BOARD_RA6T1_RSSK)
        R_IOPORT_PinWrite (&g_ioport_ctrl, (bsp_io_port_pin_t)g_bsp_leds.p_leds[0], BSP_IO_LEVEL_LOW);
#else
        R_IOPORT_PinWrite (&g_ioport_ctrl, (bsp_io_port_pin_t)g_bsp_leds.p_leds[0], BSP_IO_LEVEL_HIGH);
#endif
        /* Print Error on RTT console */
        APP_ERR_PRINT ("\r\n ** R_WDT_Refresh API failed ** \r\n");
    }
    else
    {
        /* Counter is used to count the number of times GPT callback triggered. */
        g_wdt_counter++;

        /* Toggle LED */
        level_led ^= BSP_IO_LEVEL_HIGH;
        R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t)g_bsp_leds.p_leds[0], level_led);
        /* Print Error on RTT console */
        APP_PRINT ("** R_WDT_Refresh API successful!** \r\n");
    }
    return err;
}

/*******************************************************************************************************************//**
 * This function is called to enable WDT counter in debug mode.
 * @brief    Enables WDT counter to count in debug mode.
 * @param[IN]   None
 * @retval      None
 **********************************************************************************************************************/
void enable_wdt_count_in_debug_mode(void)
{
    /* As per hardware manual's DBGREG module, section 2.6.4.2:- Clear this bit to enable WDT Reset/NMI in debug mode */
    R_DEBUG->DBGSTOPCR_b.DBGSTOP_WDT = RESET_VALUE;
}

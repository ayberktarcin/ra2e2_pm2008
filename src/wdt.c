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

    return FSP_SUCCESS;

}

fsp_err_t wdt_refresh(void){

    return FSP_SUCCESS;

}

void deinit_wdt_module(void){
    return FSP_SUCCESS;
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

/***********************************************************************************************************************
 * File Name    : hal_entry.c
 * Description  : Contains data structures and functions used in hal_entry.c.
 **********************************************************************************************************************/

#include "common_utils.h"
#include "timer_setup.h"
#include "ext_irq_setup.h"
#include "pm2008.h"
#include "wdt.h"
#include "lpm.h"

/*******************************************************************************************************************//**
 * @addtogroup r_wdt_ep
 * @{
 **********************************************************************************************************************/


/*
 * External Global Variables
 */
/* LED Structure used to blink on board LED */
extern bsp_leds_t g_bsp_leds;
extern volatile bool g_timer_callback_flag;

/*
 * function declarations
 */
void R_BSP_WarmStart(bsp_warm_start_event_t event);

/*
 * private function declarations
 */
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

    /* Enable External IRQ */
    err = enable_icu_module();

    /* Enable WDT to count and generate NMI or Reset when the debugger(JLink) is connected. */
    enable_wdt_count_in_debug_mode();

    /* Open Watchdog Timer*/
    err = init_wdt_module();

    /* Initialize Low Power Module */
    err = lpm_init();

    /* Initialize UART perihperal */
    err = uart_initialize();

    /* Start GPT timer in Periodic mode */
    err = timer_start();

    /* Openning and setting configuration for PM2008 */
    pm2008_openning_sequence();


    while(true)
    {
        /* Refresh WDT */
        wdt_refresh();

        if(g_timer_callback_flag){
            pm2008_tx_read_measurement();
            uart_rx_mesage();
            g_timer_callback_flag = false;
        }

        /*Put Sensor & MCU in low-power mode*/
        pm2008_sleep_mode_on();
        lpm_mode_enter(APP_LPM_SW_STANDBY_STATE, &g_lpm_ctrl);

        /*MCU listens GPT callback to open. MCU halts at the above pointer if timer callback is not triggered*/
        /*Put sensor in normal power mode*/
        pm2008_sleep_mode_off();

        R_BSP_SoftwareDelay(500, BSP_DELAY_UNITS_MILLISECONDS);
    }
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
        APP_PRINT ("\r\n************************ WDT Reset detected ************************\r\n\r\n");
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

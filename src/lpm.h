/***********************************************************************************************************************
 * File Name    : lpm.h
 * Description  : Contains declarations of data structures and functions used in hal_entry.c.
 **********************************************************************************************************************/

#ifndef LPM_H_
#define LPM_H_

/* generic headers */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hal_data.h"
#include "bsp_pin_cfg.h"
#include "r_ioport.h"

/*
 * Low Power Mode Definitions for LPM ep
 * Since there are no Normal mode definition in LPM driver, use this enum to keep LPM app state including:
 * Sleep, SW Standby, SW Standby with Snooze enabled, Deep SW Standby, Normal.
 */
typedef enum e_app_lpm_state
{
    APP_LPM_SLEEP_STATE = 0,             ///< Sleep mode
    APP_LPM_SW_STANDBY_STATE,            ///< SW Standby mode
    APP_LPM_SW_STANDBY_SNOOZE_STATE,     ///< SW Standby mode with Snooze enabled
    APP_LPM_DEEP_SW_STANDBY_STATE,       ///< Deep SW Standby mode
    APP_LPM_NORMAL_STATE                 ///< Normal mode
} app_lpm_states_t;


fsp_err_t lpm_init(void);
fsp_err_t lpm_deinit(void);
fsp_err_t lpm_mode_enter			(app_lpm_states_t lpm_mode, lpm_instance_ctrl_t * const g_lpm_ctrl_instance_ctrls);

#endif /* LPM_H_ */

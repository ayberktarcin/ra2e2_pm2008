/***********************************************************************************************************************
 * File Name    : lpm.c
 * Description  : Contains function definition.
 **********************************************************************************************************************/
#include "common_utils.h"
#include "lpm.h"

/*******************************************************************************************************************//**
 * @addtogroup lpm_ep
 * @{
 **********************************************************************************************************************/

fsp_err_t lpm_init(void){
    fsp_err_t err = FSP_SUCCESS;
    err = R_LPM_Open(&g_lpm_ctrl, &g_lpm_cfg);
    if (FSP_SUCCESS != err)
    {
        /* Print Error on RTT console */
        APP_ERR_PRINT ("\r\n ** R_LPM_Open API failed ** \r\n");
    }
    return err;
}
fsp_err_t lpm_deinit(void){
    fsp_err_t err = FSP_SUCCESS;
    err = R_LPM_Close(&g_lpm_ctrl);
    if (FSP_SUCCESS != err)
    {
        /* Print Error on RTT console */
        APP_ERR_PRINT ("\r\n ** R_LPM_Close API failed ** \r\n");
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief       This function enables and puts the MCU in sleep mode.
 * @param[IN]   None
 * @retval      FSP_SUCCESS                  Upon successful entering sleep mode
 * @retval      Any Other Error code apart from FSP_SUCCESS
 **********************************************************************************************************************/
fsp_err_t lpm_mode_enter(app_lpm_states_t lpm_mode,lpm_instance_ctrl_t * const p_current_ctrl)
{
    fsp_err_t err = FSP_SUCCESS;

    switch(lpm_mode)
    {
        case APP_LPM_SW_STANDBY_STATE:
            /* Enter SW Standby mode */
            err = R_LPM_LowPowerModeEnter(p_current_ctrl);
            if (FSP_SUCCESS != err)
            {
                /* Print Error on RTT console */
                APP_ERR_PRINT ("\r\n** R_LPM_LowPowerEnter API failed ** \r\n");
            }else{
                /* Print on RTT console */
                APP_PRINT ("** R_LPM_LowPowerEnter API successful ** \r\n");
            }
            break;

        default:
            /* return error */
            err = FSP_ERR_INVALID_MODE;
            break;
    }
    return err;
}
/*******************************************************************************************************************//**
 * @} (end addtogroup lpm_ep)
 **********************************************************************************************************************/

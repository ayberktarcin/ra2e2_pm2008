/***********************************************************************************************************************
 * File Name    : wdt.h
 * Description  : Contains watchdog related function declerations and variables
 **********************************************************************************************************************/


#ifndef WDT_H_
#define WDT_H_

/* Number of counts for printing WDT Refresh status */
#define WDT_REFRESH_COUNTER_VALUE                   (3u)

/* User command input value */
#define ENABLE_WDT                                  (1u)


fsp_err_t init_wdt_module(void);
fsp_err_t wdt_refresh(void);
void deinit_wdt_module(void);
/* function used to enable wdt in debug mode */
void enable_wdt_count_in_debug_mode(void);

#endif /* WDT_H_ */

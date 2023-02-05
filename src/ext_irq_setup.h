/***********************************************************************************************************************
 * File Name    : ext_irq_setup.h
 * Description  : Contains data structures and functions used in ext_irq_setup.c
 **********************************************************************************************************************/

#ifndef EXT_IRQ_SETUP_H_
#define EXT_IRQ_SETUP_H_


/*
 * function declarations
 */
fsp_err_t init_icu_module(void);
fsp_err_t enable_icu_module(void);
void deinit_icu_module(void);

#endif /* EXT_IRQ_SETUP_H_ */

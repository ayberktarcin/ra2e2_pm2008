/* generated configuration header file - do not edit */
#ifndef BSP_PIN_CFG_H_
#define BSP_PIN_CFG_H_
#include "r_ioport.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

#define VREFH0_PMOD1_GPIO9 (BSP_IO_PORT_00_PIN_10)
#define VREFL0_PMOD1_GPIO10 (BSP_IO_PORT_00_PIN_11)
#define PMOD1_MIKROBUS_ARDUINO_RESET (BSP_IO_PORT_00_PIN_14)
#define LED3_RED_ARDUINO_D2_PMOD2_RESET (BSP_IO_PORT_00_PIN_15)
#define PM2008TX (BSP_IO_PORT_01_PIN_00)
#define PM2008RX (BSP_IO_PORT_01_PIN_01)
#define PM2008Sleep (BSP_IO_PORT_01_PIN_09)
#define LED1_BLUE (BSP_IO_PORT_01_PIN_12)
#define Push_Button (BSP_IO_PORT_02_PIN_05)
#define IIC_SCL0 (BSP_IO_PORT_04_PIN_00)
#define IIC_SDA0 (BSP_IO_PORT_04_PIN_01)
#define LED2_GREEN (BSP_IO_PORT_09_PIN_14)
extern const ioport_cfg_t g_bsp_pin_cfg; /* EK-RA2E2.pincfg */

void BSP_PinConfigSecurityInit();

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif /* BSP_PIN_CFG_H_ */

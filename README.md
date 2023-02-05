# Description
The objective of this project is to integrate PM 2008 sensor and Renesas development board for air quality monitoring and control system. The PM 2008 sensor is capable of measuring fine particulate matter (PM2.5) in the air, which has a significant impact on human health. The Renesas development board, on the other hand, provides a platform for rapid prototyping and testing of embedded systems.

The project will involve interfacing the PM 2008 sensor with the Renesas development board, which will allow the system to acquire real-time air quality data. This data will be processed and analyzed by the board, and relevant information will be displayed on an attached display. 

Overall, this project aims to provide a low-cost, yet reliable and efficient solution for monitoring air quality in various environments. It has applications in homes, offices, industrial settings, and public places, among others. The integration of PM 2008 sensor and Renesas development board provides a versatile and scalable platform for further development and integration with other systems.

# Summary of Tools
* Board:   EK-RA2E2 
* Device:   R7FA2E2A72DNK 
* Toolchain:   GCC ARM Embedded 
* Toolchain Version:   10.3.1.20210824 
* FSP Version:   4.2.0 https://github.com/renesas/fsp/releases/tag/v4.2.0
* IDE: e2tudio: https://www.renesas.com/eu/en/software-tool/e-studio
* Sensor: Cubic PM2008 - https://en.gassensor.com.cn/ParticulateMatterSensor/info_itemid_237.html

# Setup
## Block Configuration of EK-RA2E2 and PM2008
![](Documents/RA2E2_PM2008_UART_Congig.png)

## Pin Configuration
![](Documents/PinConfig.png)

## Photo of EK-RA2E2 and PM2008 Connection
![](Documents/SetupPhoto.jpeg)

# RTT Viewer Logs
SEGGER RTT tool is used in this project for monitoring. 
Note: SEGGER Real-Time Terminal (RTT) Viewer is a powerful tool for debugging embedded systems. It provides a convenient way to view and analyze real-time data from an embedded system, making it an essential tool for software engineers and developers. It can be used with a J-link Arm Debugger.

# Running Application Log
![](Documents/RTTViewerOutput.PNG)

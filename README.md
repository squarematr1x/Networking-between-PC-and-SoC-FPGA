# Networking between PC and SoC-FPGA

The source code in this repo can be used to send images from PC to SoC-FPGA. The processed images are sent back to PC and saved on a hard drive.

## Instructions

* Download Vivado Design Tools **version 2018.2**
* Create project by using this [Zybo-Z7-20-DMA Demo template](https://github.com/Digilent/Zybo-Z7-20-DMA)
* Implement the image processing HDL modules of your choise in Vivado
* Create new application project in Xilinx SDK and select lwIP Echo Server template
* Remove  **`main.c`** and **`echo.c`** from the created project and copy the content of the server folder and paste it into src folder of your project in Xilinx SDK

The client folder has the source code for the TCP client running on PC. The server must be running on the SoC before images can be sent from the client.

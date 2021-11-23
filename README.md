# Networking between PC and SoC-FPGA

The source code in this repo can be used to send images from PC to SoC-FPGA. The processed images are sent back to PC and saved on a hard drive.

## Instructions

* Download Vivado Design Tools **version 2018.2**
* Create project by using this [Zybo-Z7-20-DMA Demo template](https://github.com/Digilent/Zybo-Z7-20-DMA)
* Implement the image processing HDL modules of your choice in Vivado
* Create new application project in Xilinx SDK and select lwIP Echo Server template
* Remove  **`main.c`** and **`echo.c`** from the created project and copy the content of the server folder and paste it into src folder of your project in Xilinx SDK
* Modify BSP settings and from tcp_options set **`tcp_mss`** from 1460 to 1444 and **`tcp_wnd`** from 2048 to 5776 (these settings will affect the speed of the networking and can be further modified later)

The client folder has the source code for the TCP client running on PC. The server must be running on the SoC before images can be sent from the client.

## Demo

![screenshot](https://github.com/squarematr1x/Networking-between-PC-and-SoC-FPGA/blob/master/screenshot/demo.png?raw=true)

### Explanation

Every *RGB* pixel of the image is multiplied by nonuniform scale *A*. The resulting pixel *y*:

![equation](https://github.com/squarematr1x/Networking-between-PC-and-SoC-FPGA/blob/master/screenshot/eq.png?raw=true)

This matrix multiplication is done on the FPGA. Note that A-channel is not modified. This particular matrix multiplication results in peculiar looking images.

## Known limitations

* Only works with png files (RGBA).
* The image size should not be altered (image size in = image size out).

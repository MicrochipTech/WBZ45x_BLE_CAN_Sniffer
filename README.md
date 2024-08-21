# WBZ45x BLE Peripheral Example : CAN Bus Sniffer

<img src="media/IoT-Made-Easy-Logo.png" width=100>

> "IoT Made Easy!" 

Devices: **| PIC32CXBZ2 | WBZ45x |**<br>
Features: **| CAN | BLE |**


## ⚠ Disclaimer

<p><span style="color:red"><b>
THE SOFTWARE ARE PROVIDED "AS IS" AND GIVE A PATH FOR SELF-SUPPORT AND SELF-MAINTENANCE. This repository contains example code intended to help accelerate client product development. </br>

For additional Microchip repos, see: <a href="https://github.com/Microchip-MPLAB-Harmony" target="_blank">https://github.com/Microchip-MPLAB-Harmony</a>

Checkout the <a href="https://microchipsupport.force.com/s/" target="_blank">Technical support portal</a> to access our knowledge base, community forums or submit support ticket requests.
</span></p></b>

## Introduction

This document describes how to program a Microchip WBZ45x BLE SoC certified RF module to connect to a smartphone while monitoring messages received over its connected CAN bus. A sample smartphone app is provided to communicate with the SoC via BLE and display the CAN messages received by the BLE SoC. A Host PC running a terminal emulator serves as a debug window to display diagnostic messages sent by the BLE SoC module.

<img src=".//media/WBZ45x_BLE_CAN_BlockDiagram.png" width=700/>

## Table of Contents

- [Introduction](#introduction)
- [Hardware Requirements](#hardware-requirements)
- [Demo Platform Assembly](#demo-platform-assembly)
- [Software Requirements](#software-requirements)
- [Program Demo Firmware](#program-demo-firmware)
- [Connect to Smartphone App](#connect-to-smartphone-app)
- [Testing Procedure](#testing-procedure)

## Hardware Requirements

* BLE SoC Certified RF Module: ["WBZ451 Curiosity Development Board"](https://www.microchip.com/en-us/development-tool/ev96b94a)

    <img src=".//media/ev96b94a.jpg" width=375/>

* High Speed CAN FD Controller + Transceiver: ["MCP251863 Click"](https://www.mikroe.com/mcp251863-click?srsltid=AfmBOoo5cWWs8CO75NdETyiO2E0TmTjOKCkVMExN7PqwAvgfHuu7hJCp) featuring Microchip's [MCP251863](https://www.microchip.com/en-us/product/mcp251863)

    <img src=".//media/mcp251863-click.jpg" width=250/>

* USB-to-CAN Adapter for Host PC: ["PCAN-USB FD Adapter"](https://phytools.com/collections/peak-system-technik/products/pcan-usb-fd-adapter) manufactured by [phytools](https://phytools.com) (or equivalent diagnostic tool that can generate CAN messages)

    <img src=".//media/PCAN-USB-FD_Adapter.png" width=300/>

    <img src=".//media/phytools_logo.png" width=300/>

## Demo Platform Assembly

1. Choose one of the following methods to connect the MCP251863 Click board to the WBZ451 Curiosity development board:

    <img src=".//media/connect_method_01.png" width=650/>
    <img src=".//media/connect_method_02.png" width=650/>

2. Connect the Host PC to the WBZ451 Curiosity development board using a micro-USB cable and note the new Virtual COM port number that is associated with your connection (e.g. use the Windows Device Manager and look for `USB Serial Device`).

    <img src=".//media/Win10_DeviceManager.png" width=400/>

3. Using a DB-9 female-to-female serial cable, connect the PCAN-USB FD Adapter to the MCP251863 Click board, then connect the adapter's USB cable to an available USB port of the Host PC.

## Software Requirements

Embedded software development tools need to be installed in order to properly program (and interact with) the WBZ451 Curiosity Development Board.

1. Microchip `MPLAB X` tool chain for embedded code development on 32-bit architecture MCU/MPU platforms (made up of 3 major components)

    - [MPLAB X IDE](https://www.microchip.com/mplab/mplab-x-ide) (when prompted, enable the installation of the [MPLAB IPE](https://www.microchip.com/en-us/tools-resources/production/mplab-integrated-programming-environment) too)
        
        NOTE: This demonstration project was last tested successfully with MPLAB X **v6.20**. If a previous version of the IDE is needed, download it from the [MPLAB Development Ecosystem Downloads Archive](https://www.microchip.com/en-us/tools-resources/archives/mplab-ecosystem) (to fall back to the version Microchip successfully tested prior to release). 

    - [MPLAB XC32 Compiler](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers#tabs)

        NOTE: This demonstration project was tested successfully with XC32 **v4.45**, and in general should work with later versions of the compiler as they become available. If you encounter issues building the project with a newer version of the compiler, it is recommended to download the compiler version that was last tested successfully from the [MPLAB Development Ecosystem Downloads Archive](https://www.microchip.com/en-us/tools-resources/archives/mplab-ecosystem) (to fall back to the version Microchip successfully tested prior to release). 

     - [MPLAB Harmony Software Framework](https://microchipdeveloper.com/harmony3:mhc-overview)
	    - csp version: v3.14.0
	    - core version: v3.11.1
	    - CMSIS-FreeRTOS: v10.4.6
	    - wireless_pic32cxbz_wbz: v1.1.0
	    - dev_packs: v3.14.0
	    - wolfssl version: v4.7.0
	    - crypto version: v3.7.6
	    - wireless_ble: v1.0.0

2. [Git](https://git-scm.com) (a free and open source distributed version control system)

    - Download/install the latest version of [Git for Windows, macOS, or Linux](https://git-scm.com/downloads)

    - Verify working operation of the `Git Bash` prompt (e.g. for Windows: click `Start` > type `Git Bash`)

3. Any [Terminal Emulator](https://en.wikipedia.org/wiki/List_of_terminal_emulators) program of your choice

4. [Microchip Bluetooth Data](https://play.google.com/store/apps/details?id=com.microchip.bluetooth.data&hl=en_US&pli=1) smartphone app

5. PCAN-USB FD Adapter [Device Driver for Windows](https://www.peak-system.com/quick/DrvSetup) 

## Program Demo Firmware

1. Clone/download the MPLAB X demo project by issuing the following command line in a `Command Prompt` or `PowerShell` window.

    ```bash
    git clone https://github.com/MicrochipTech/WBZ45x_BLE_CAN_Sniffer.git
    ```

2. Launch the MPLAB X IDE.

3. Open the demo project:

    - From the MPLAB X mail toolbar, select `File > Open Project`
    - Navigate to the `WBZ45x_BLE_CAN_Sniffer\firmware` folder
    - Select (click on) the `bleCan_Peripheral.X` project folder
    - Click on the `Open Project` button

4. Clean and build the project:

    - Click on the `Clean and Build Main Project` icon in the MPLAB X main toolbar

        <img src=".//media/mplab_clean_build.png" width=450/>

5. Program the WBZ451 BLE SoC:

    - Click on the Make and Program Device Main Project icon in the MPLAB X main toolbar 
        <img src=".//media/mplab_make_program.png" width=375/>

    - Verify that the programming phase was successful
        <img src=".//media/mplab_prog_success.png" width=600/>

## Connect to Smartphone App

1. Open the Microchip MBD Application on Mobile phone and click on BLE Smart to scan for the available devices.

    ![MBD_APP_1](media/MBD_APP_1.png)

2. The MBD App will scan for the available devices and the scan result will be shown as below. Select "CAN BLE Bridge Device" from the scan list.

    ![MBD_APP_2](media/MBD_APP_2.png)

3. Click on the Connect button to establish a BLE Connection with the device.

    ![MBD_APP_3](media/MBD_APP_3.png)

4. Once BLE Connection is established and service discovery is completed, Transparent UART Service is discovered and shown as Microchip Data Service as shown below.

    ![MBD_APP_4](media/MBD_APP_4.png)

5. To view the CAN messages transmitted via BLE, select the Transparent UART TX Characteristic (UUID ending with 9616) as shown below.

    ![MBD_APP_5](media/MBD_APP_5.png)

6. In the Characteristics page, enable notify/indicate as shown below.

    ![MBD_APP_6](media/MBD_APP_6.png)

## Testing Procedure

1. Open a serial terminal window (e.g. TeraTerm) and connect to the COM port corresponding to your board (e.g. choose `File` &gt; choose `New connection`&gt; choose `Serial`&gt; select `USB Serial Device`).

    <img src=".//media/TeraTerm_NewConnection.png" width=400/>

2. Set the baud rate for the COM port connection at `115200 baud` (e.g. choose `Setup` &gt; choose `Serial port`&gt; type in `115200` for the speed &gt; click `New setting`).

    <img src=".//media/TeraTerm_SerialPort.png" width=400/>

3. Press the RESET button on the WBZ451 Curiosity development board and confirm that the application firmware outputs a couple of opening messages to the serial terminal window.

    <img src=".//media/terminal_reset.png" width=600/>

4. Launch the PCAN-View PC application.

5. In the `CAN Setup` tab, select `SAE J2284-4 (500k/2M)` for the Bit Rate Preset:
    <img src=".//media/PCAN-View_CAN_Setup.png" width=400/>

6. Create/transmit a new **standard** CAN message by selecting `File > New Message` in the PCAN-View's main toolbar. In the **New Transmit Message** pop-up window:

    - Enter an ID that's less than 0x800
    - Select the number of data bytes in the message and set the value of each byte as desired 
    - Under Message Type, ensure that only the boxes checked are for `CAN FD` and `Bit Rate Switch`
    - Enter a cycle time to send out the same message periodically (in milliseconds)
    - Click on the OK button to start the transmission of repeating messages

        <img src=".//media/PCAN-View_StandardMesg.png" width=400/>

7. Confirm that the same periodic CAN message transmissions are received and displayed in the Microchip Bluetooth Data (MBD) smartphone app.

    <img src=".//media/MBD_APP_Characteristics.png" width=300/>

The following video shows the overall process of connecting to the Smartphone and viewing messages received on the CAN bus connected to the BLE SoC:

![Alt Text](media/MBD_CAN.gif)
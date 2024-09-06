/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

/*******************************************************************************
  Application BLE Profile Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_trsps_handler.c

  Summary:
    This file contains the Application BLE functions for this project.

  Description:
    This file contains the Application BLE functions for this project.
 *******************************************************************************/


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include <stdint.h>
#include "ble_trsps/ble_trsps.h"

#include "app.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************

uint16_t conn_hdl; // connection handle info captured @BLE_GAP_EVT_CONNECTED event

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

void APP_TRSPS_command(uint8_t* pWrBuffer, size_t size)
{           
    if (strstr(pWrBuffer, "+LED_RED:ON"))
    {
        RED_LED_Set();
        return;
    }
    if (strstr(pWrBuffer, "+LED_RED:OFF"))
    {
        RED_LED_Clear();
        return;
    }
    if (strstr(pWrBuffer, "+LED_GREEN:ON"))
    {
        GREEN_LED_Set();
        return;
    }
    if (strstr(pWrBuffer, "+LED_GREEN:OFF"))
    {
        GREEN_LED_Clear();
        return;
    }
    if (strstr(pWrBuffer, "+LED_BLUE:ON"))
    {
        BLUE_LED_Set();
        return;
    }
    if (strstr(pWrBuffer, "+LED_BLUE:OFF"))
    {
        BLUE_LED_Clear();
        return;
    }
    if (strstr(pWrBuffer, "+MSG:"))
    {
        pWrBuffer += strlen("+MSG:");
        size -= strlen("+MSG:");   
        return;
    }
    if (strstr(pWrBuffer, "+STATUS:")) // Send status of RGB LED back to BLE device
    {
        uint8_t buffer[25];
        // e.g. response format = "RGB: R g B" (where upper case = ON & lower case = OFF)
        sprintf(buffer, "+RGB: %s %s %s", RED_LED_Get() == 1 ? "R" : "r", GREEN_LED_Get() == 1 ? "G" : "g", BLUE_LED_Get() == 1 ? "B" : "b");
        size = strlen(buffer);
        BLE_TRSPS_SendData(conn_hdl, size, buffer);
        return;
    }
    if (strstr(pWrBuffer, "+CAN:TX")) // Transmit a sample CAN message to the CAN controller
    {
        CAN_MSG_t canMsg;
        // Set example CAN message parameters
        canMsg.msgObj.txObj.bF.id.SID = 0x1;
        canMsg.msgObj.txObj.bF.ctrl.DLC = 0x8;
        canMsg.can_data[0] = 0xB;
        canMsg.can_data[1] = 0xA;
        canMsg.can_data[2] = 0xB;
        canMsg.can_data[3] = 0xE;
        canMsg.can_data[4] = 0xC;
        canMsg.can_data[5] = 0xA;
        canMsg.can_data[6] = 0xF;
        canMsg.can_data[7] = 0xE;
        // Add CAN message to the Tx queue
        APP_TransmitMessageQueue(&canMsg);
        APP_LED_BLUE_pulse();
        return;
    }
}

void APP_TrspsEvtHandler(BLE_TRSPS_Event_T *p_event)
{
    switch(p_event->eventId)
    {
        case BLE_TRSPS_EVT_CTRL_STATUS:
        {
            /* TODO: implement your application code.*/
        }
        break;
        
        case BLE_TRSPS_EVT_TX_STATUS:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_TRSPS_EVT_CBFC_ENABLED:
        {
            /* TODO: implement your application code.*/
        }
        break;
        
        case BLE_TRSPS_EVT_CBFC_CREDIT:
        {
            /* TODO: implement your application code.*/
        }
        break;
        
        case BLE_TRSPS_EVT_RECEIVE_DATA:
        {
            uint8_t *data;
            uint16_t data_len = 0;
            APP_Msg_T appCANTxMsg;

            // Retrieve length of received data
            BLE_TRSPS_GetDataLength(p_event->eventField.onReceiveData.connHandle, &data_len);
            appCANTxMsg.msgData[0] = data_len;
            // Retrieve the received data
            BLE_TRSPS_GetData(p_event->eventField.onReceiveData.connHandle, &appCANTxMsg.msgData[1]);
            appCANTxMsg.msgId = APP_MSG_BLE_RX_CAN_TX_EVT;
            OSAL_QUEUE_SendISR(&appData.appQueue, &appCANTxMsg);

            // Allocate memory according to data length to process a potential command string
            data = OSAL_Malloc(data_len);
            if(data == NULL) {
                break;
            }
            data = &appCANTxMsg.msgData[1];
#ifdef ENABLE_CONSOLE_PRINT
            SYS_CONSOLE_PRINT("[TRSPS] Received Message: ");            
            for (int index = 0; index < data_len; index++)
            {
                SYS_CONSOLE_PRINT("%c", data[index]);
            }
            SYS_CONSOLE_PRINT("\r\n");   
#endif
            APP_TRSPS_command(data, data_len);
            OSAL_Free(data);
        }
        break;
        
        case BLE_TRSPS_EVT_VENDOR_CMD:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_TRSPS_EVT_ERR_UNSPECIFIED:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_TRSPS_EVT_ERR_NO_MEM:
        {
            /* TODO: implement your application code.*/
        }
        break;

        default:
        break;
    }
}
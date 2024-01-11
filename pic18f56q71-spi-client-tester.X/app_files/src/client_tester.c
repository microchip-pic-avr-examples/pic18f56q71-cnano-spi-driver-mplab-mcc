/**
 * Client Tester Application Source File
 * 
 * @file client_tester.c
 * 
 * @ingroup clienttester
 * 
 * @brief Tester for implementation.
 * 
 * @version Client Tester Version v1.0.0
 * 
 */

/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS "AS IS". 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

// Included files
#include <stdint.h>

#include "../../mcc_generated_files/system/pins.h"
#include "../../mcc_generated_files/spi/spi_interface.h"
#include "../client_tester.h"


// Functions
void TesterInitialize(void)
{
    CLIENTTESTER.Initialize();
    CLIENTTESTER.Open(TESTER);
}

bool AssertDevice(void)
{
    bool returnValue;
    if (CLIENTTESTER.IsTxReady() == true)
    {
        CS1_SetLow();
        returnValue = true;
    }
    else
    {
        returnValue = false;
    }
    return returnValue;
}

bool DeassertDevice(void)
{
    bool returnValue;
    if (CLIENTTESTER.IsTxReady() == true)
    {
        CS1_SetHigh();
        returnValue = true;
    }
    else
    {
        returnValue = false;
    }
    return returnValue;
}

void TesterCallback(void)
{
    LED0_Toggle();
}

bool ButtonPress(void)
{   
    static uint8_t debounce = 10u;
    static uint8_t buttonState = 0u;
    static bool buttonPressEnabled = true;
    bool returnValue = false;

    if(0u == SW0_GetValue())
    {       
        if(buttonState < debounce)
        {
            buttonState++;
        }
        else if(true == buttonPressEnabled)
        {
            buttonPressEnabled = false;
            returnValue = true;
        }
        else
        {
            // Do Nothing
        }
    }
    else if(buttonState > 0u)
    {
        buttonState--;
    }
    else
    {
        buttonPressEnabled = true;
    }
    return returnValue;
}
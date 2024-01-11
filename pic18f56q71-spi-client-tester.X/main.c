/*
 * MAIN Generated Driver File
 *
 * @file main.c
 *
 * @defgroup main MAIN
 *
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0
 */

/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip
    software and any derivatives exclusively with Microchip products.
    You are responsible for complying with 3rd party license terms
    applicable to your use of 3rd party software (including open source
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.?
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR
    THIS SOFTWARE.
 */
#include "mcc_generated_files/system/system.h"
#include "app_files/client_tester.h"
#include "mcc_generated_files/timer/delay.h"

/*
    Main application
 */

// Macros
#define EXPECTED_RESULT1    0xCD
#define EXPECTED_RESULT2    0xEF
#define CORRECTBYTE         0xAB

int main(void)
{
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    //    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    uint8_t receivedValue = 0, dataToWrite = (uint8_t) EXPECTED_RESULT1;

    LED0_SetHigh();

    TesterInitialize();

    // Send EXPECTED_RESULT twice using ByteExchange to prime the client
    while (AssertDevice() == false)
    {
        // Wait for operation to finish
    }
    CLIENTTESTER.ByteExchange((uint8_t) EXPECTED_RESULT1);
    while (DeassertDevice() == false)
    {
        // Wait for operation to finish
    }

    while (AssertDevice() == false)
    {
        // Wait for operation to finish
    }
    CLIENTTESTER.ByteExchange((uint8_t) EXPECTED_RESULT1);
    while (DeassertDevice() == false)
    {
        // Wait for operation to finish
    }

    // Toggle LED0 on every data transmission
    CLIENTTESTER.RxCompleteCallbackRegister(TesterCallback);

    while (1)
    {
        while (AssertDevice() == false)
        {
            // Wait for operation to finish
        }
        receivedValue = CLIENTTESTER.ByteExchange(dataToWrite);
        while (DeassertDevice() == false)
        {
            // Wait for operation to finish
        }
        if ((uint8_t) CORRECTBYTE == receivedValue)
        {
            dataToWrite = (uint8_t) EXPECTED_RESULT1;
        }
        else
        {
            dataToWrite = (uint8_t) EXPECTED_RESULT2;
        }
        DELAY_milliseconds(500);
    }
}
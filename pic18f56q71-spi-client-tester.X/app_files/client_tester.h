/**
 * Client Tester Application Header File
 * 
 * @file client_tester.h
 * 
 * @ingroup clienttester clienttester
 * 
 * @brief Tester for communication with a client device using SPI.
 * Operates in interrupt mode. Works for client in interrupt or polling mode.
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

#ifndef CLIENT_TESTER_H
#define	CLIENT_TESTER_H

// Included files
#include "../mcc_generated_files/system/system.h"

// Functions
/**
 * @ingroup clienttester
 * @brief Initializes SPI and opens configuration fro 7-segment display.
 * @param None.
 * @return None.
 */
void TesterInitialize(void);

/**
 * @ingroup clienttester
 * @brief Pulls Chip Select line low if the device is ready to be used.
 * @param None.
 * @return true If Chip Select is pulled low successfully
 * @return false If Chip Select could not be pulled low as device is not idle.
 */
bool AssertDevice(void);

/**
 * @ingroup clienttester
 * @brief Pulls Chip Select line high if the device is idle.
 * @param None.
 * @return true If Chip Select is pulled high successfully.
 * @return false If Chip Select could not be pulled high as device is not idle.
 */
bool DeassertDevice(void);

/**
 * @ingroup clienttester
 * @brief Callback after transfer completion.
 * @param None.
 * @return None.
 */
void TesterCallback(void);

/**
 * @ingroup clienttester
 * @brief Handles debouncing on button press.
 * @param None.
 * @retval True Button is pressed
 * @retval False Button is not pressed
 */
bool ButtonPress(void);

#endif	/* CLIENT_TESTER_H */


/**
 * Interrupt Demo Application Header File
 * 
 * @file host_interrupt_demo.h
 * 
 * @ingroup hostinterruptdemo HOSTINTERRUPTDEMO
 * 
 * @brief SPI Interrupt mode demo.
 * 
 * @version Interrupt Demo Application Version v1.0.0
 * 
 */

/*
© [2023] Microchip Technology Inc. and its subsidiaries.

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

#ifndef HOST_INTERRUPT_DEMO_H
#define	HOST_INTERRUPT_DEMO_H

// Included files
#include "../mcc_generated_files/system/system.h"

// Functions
/**
 * @ingroup hostinterruptdemo
 * @brief Initializes SPI and opens configuration fro 7-segment display.
 * @param None.
 * @return None.
 */
void DemoInitialize(void);

/**
 * @brief Pulls Chip Select line low if the device is ready to be used.
 * @param None.
 * @return true If Chip Select is pulled low successfully
 * @return false If Chip Select could not be pulled low as device is not idle.
 */
bool AssertDevice(void);

/**
 * @brief Pulls Chip Select line high if the device is idle.
 * @param None.
 * @return true If Chip Select is pulled high successfully.
 * @return false If Chip Select could not be pulled high as device is not idle.
 */
bool DeassertDevice(void);

/**
 * @ingroup hostinterruptdemo
 * @brief Displays a custom character number on the 7-segment
 * @param input Single character input
 * @return None.
 * @note Character displayed on left display first and shifted to right display
 * on second input.
 */
void DisplayCustomCharacter(uint8_t input);

/**
 * @ingroup hostinterruptdemo
 * @brief Displays a 2 digit number on the 7-segment.
 * @param numberInput Two digit number
 * @return None.
 */
void DisplayNumber(uint8_t numberInput);

/**
 * @ingroup hostinterruptdemo
 * @brief Callback after transfer completion.
 * @param None.
 * @return None.
 */
void DisplayCompleteCallback(void);

/**
 * @ingroup hostinterruptdemo
 * @brief Handles debouncing on button press.
 * @param None.
 * @retval True Button is pressed
 * @retval False Button is not pressed
 */
bool ButtonPress(void);

#endif	/* HOST_INTERRUPT_DEMO_H */


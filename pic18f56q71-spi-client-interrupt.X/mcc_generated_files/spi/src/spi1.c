/**
  * SPI1 Generated Driver File
  *
  * @file spi1.c
  *
  * @ingroup spi1
  *
  * @brief This file contains the driver code for the SPI1 module.
  *
  * @version SPI1 Driver Version v3.0.0.
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

#include <xc.h>
#include "../spi_interface.h"
#include "../spi1.h"
#include "../spi_interrupt_types.h"

/**
 * @ingroup spi1
 * @struct SPI_INTERFACE SPI1
 * @brief Defines an object for SPI_INTERFACE functions.
 */ 
 
const struct SPI_INTERFACE SPI_Client = {
    .Initialize = SPI1_Initialize,
    .Deinitialize = SPI1_Deinitialize,
    .Open = SPI1_Open,
    .Close = SPI1_Close,
    .BufferExchange = SPI1_BufferExchange,
    .BufferWrite = SPI1_BufferWrite,
    .BufferRead = SPI1_BufferRead,	
    .ByteExchange = SPI1_ByteExchange,
    .ByteWrite = SPI1_ByteWrite,
    .ByteRead = SPI1_ByteRead,
    .IsTxReady = SPI1_IsTxReady,
    .IsRxReady = SPI1_IsRxReady,
    .RxCompleteCallbackRegister = SPI1_RxCompleteCallbackRegister,
    .TxCompleteCallbackRegister = SPI1_TxCompleteCallbackRegister
};

static void (*SPI1_RxCompleteCallback)(void);
static void (*SPI1_TxCompleteCallback)(void);

static spi_descriptor_t spi1_descriptor = {
    .transmitBuffer 	= NULL,
    .receiveBuffer 	= NULL,
    .bytesToTransmit   	= 0,
    .bytesToReceive   	= 0,
    .status 		= SPI_RESET
};

//con0 == SPI1CON0, con1 == SPI1CON1, con2 == SPI1CON2, baud == SPI1BAUD, clksel == SPI1CLKSEL
static const spi_configuration_t spi1_configuration[] = {   
    { 0x0, 0x40, 0x4, 0x3f, 0x00 },
    { 0x0, 0x40, 0x4, 0x0, 0x0 }
};

void SPI1_Initialize(void)
{
    //Disable the interrupts
    PIE3bits.SPI1IE 		= 0U;
    PIE3bits.SPI1TXIE 	= 0U;
    PIE3bits.SPI1RXIE 	= 0U;
	
	//Clear all registers to reset value
    SPI1CON0 = 0x00U;
    SPI1CON1 = 0x00U;
    SPI1CON2 = 0x00U;
    SPI1BAUD = 0x00U;
    SPI1CLK = 0x00U;      


	
    //Set Interrupt handler
	SPI1_RxCompleteCallbackRegister(NULL);
	SPI1_TxCompleteCallbackRegister(NULL);	
	//Set status as SPI_RESET
	spi1_descriptor.status = SPI_RESET;
	
}

void SPI1_Deinitialize(void)
{
    	SPI1CON0bits.EN = 0U;
	//Set status as SPI_RESET
	spi1_descriptor.status = SPI_RESET;
}

bool SPI1_Open(uint8_t spiConfigIndex)
{
	bool returnValue = false; 
	if(false == SPI1CON0bits.EN)
	{
		spi1_descriptor.status = SPI_IDLE;
        SPI1CON0 = spi1_configuration[spiConfigIndex].con0;
        SPI1CON1 = spi1_configuration[spiConfigIndex].con1;
	    SPI1CON2 = spi1_configuration[spiConfigIndex].con2 & ~(_SPI1CON2_SPI1RXR_MASK) & ~(_SPI1CON2_SPI1TXR_MASK);
        SPI1BAUD = spi1_configuration[spiConfigIndex].baud;        
        SPI1CLK = spi1_configuration[spiConfigIndex].clksel;        
		TRISCbits.TRISC6 = 1;
		//Enable the interrupt
		PIE3bits.SPI1IE 	= 1U;
        SPI1CON0bits.EN = 1U;
		
		returnValue = true;
	}
	else
	{
		returnValue = false;
	}
	return returnValue;
}

void SPI1_Close(void)
{
	SPI1CON0bits.EN = 0U;
	
	//Disable the interrupts
	PIE3bits.SPI1IE 		= 0U;
    PIE3bits.SPI1TXIE 	= 0U;
    PIE3bits.SPI1RXIE 	= 0U;
	
	//Set status as SPI_RESET
	spi1_descriptor.status = SPI_RESET;
}

void SPI1_BufferExchange(void *bufferData, size_t bufferSize)
{
	uint8_t* bufferInput = bufferData;
	if (SPI_IDLE == spi1_descriptor.status)
    	{
		//Set both SPI as full duplex mode for buffer exchange operation
        SPI1CON2 = (SPI1CON2 | _SPI1CON2_SPI1RXR_MASK) | _SPI1CON2_SPI1TXR_MASK;
		
		//Load the spi1_descriptor
		spi1_descriptor.transmitBuffer 	= bufferInput;
		spi1_descriptor.receiveBuffer 	= bufferInput;
        spi1_descriptor.bytesToTransmit 	= bufferSize;
        spi1_descriptor.bytesToReceive 	= bufferSize;
        spi1_descriptor.status 			= SPI_BUSY;

		//Clear the buffers
        SPI1STATUSbits.CLRBF = 1U;
        //Load the transfer count register
		SPI1TCNTL = (uint8_t)((uint8_t)0xFF & (uint8_t)bufferSize);
		//Set the interrupts
		PIE3bits.SPI1RXIE 	= 1U;
		PIE3bits.SPI1TXIE 	= 1U;
	}
	else
	{
		// No operation when module is already in use
	}
}

void SPI1_BufferWrite(void *bufferData, size_t bufferSize)
{
	uint8_t* bufferInput = bufferData;
	if (SPI_IDLE == spi1_descriptor.status)
    	{
		//Set both SPI as transmit only mode for buffer write operation
        	SPI1CON2 = (SPI1CON2 & ~_SPI1CON2_SPI1RXR_MASK) | _SPI1CON2_SPI1TXR_MASK;
		//Load the spi1_descriptor
		spi1_descriptor.transmitBuffer 	= bufferInput;
		spi1_descriptor.receiveBuffer 	= NULL;
        spi1_descriptor.bytesToTransmit 	= bufferSize;
	    spi1_descriptor.bytesToReceive 	= 0U;
        spi1_descriptor.status 			= SPI_BUSY;

		//Clear the buffers
		SPI1STATUSbits.CLRBF = 1U;
        //Load the transfer count register
		SPI1TCNTL = bufferSize;
		//Set the interrupts
		PIE3bits.SPI1TXIE 	= 1U;
	}
	else
	{
		// No operation when module is already in use
	}
}

void SPI1_BufferRead(void *bufferData, size_t bufferSize)
{
	uint8_t* bufferInput = bufferData;
	if (SPI_IDLE == spi1_descriptor.status)
    {
		//Set both SPI as receive only mode for buffer read operation
        SPI1CON2 = (SPI1CON2 | _SPI1CON2_SPI1RXR_MASK) & ~_SPI1CON2_SPI1TXR_MASK;
		
		//Load the spi1_descriptor
		spi1_descriptor.transmitBuffer 	= NULL;
		spi1_descriptor.receiveBuffer 	= bufferInput;
	    spi1_descriptor.bytesToTransmit 	= 0U;
        spi1_descriptor.bytesToReceive 	= bufferSize;
	    spi1_descriptor.status 			= SPI_BUSY;

		//Clear the buffers
		SPI1STATUSbits.CLRBF = 1U;
         //Load the transfer count register
		SPI1TCNTL = bufferSize;
		//Set the interrupts
		PIE3bits.SPI1RXIE 	= 1U;
	}
	else
	{
		// No operation when module is already in use
	}
}

uint8_t SPI1_ByteExchange(uint8_t byteData)
{
	uint8_t returnValue = 0U;
	while(SPI_IDLE != spi1_descriptor.status)
    {
        // Waiting for the current operation to finish
    }
	//Set both SPI as full duplex mode for byte exchange operation
	SPI1CON2 = (SPI1CON2 | _SPI1CON2_SPI1RXR_MASK) | _SPI1CON2_SPI1TXR_MASK;	
	//Load the spi1_descriptor
	spi1_descriptor.transmitBuffer 		= &byteData;
	spi1_descriptor.receiveBuffer 		= &byteData;
    spi1_descriptor.bytesToTransmit 		= 1;
	spi1_descriptor.bytesToReceive 		= 1;
	spi1_descriptor.status 				= SPI_BUSY;

	//Clear the buffers
	SPI1STATUSbits.CLRBF = 1U;
    //Load the transfer count register
	SPI1TCNTL = 1;
	//Set the interrupts
	PIE3bits.SPI1RXIE 	= 1U;
	PIE3bits.SPI1TXIE 	= 1U;
	
	//Wait on transmit complete
	while(SPI_BUSY == spi1_descriptor.status){
	};
	
	return byteData;
}

void SPI1_ByteWrite(uint8_t byteData)
{
	if(SPI_IDLE == spi1_descriptor.status)
	{
		SPI1CON2 = (SPI1CON2 | _SPI1CON2_SPI1RXR_MASK) | _SPI1CON2_SPI1TXR_MASK;
        //Load the transfer count register
		SPI1TCNTL = 1;
		//Write input data to SPI transmit buffer register
		SPI1TXB = byteData;
	}
	else
	{
		//Do nothing
	}
}

uint8_t SPI1_ByteRead(void)
{
	uint8_t returnValue = 0x00U;
	if(SPI_IDLE == spi1_descriptor.status)
	{
		SPI1CON2 = (SPI1CON2 | _SPI1CON2_SPI1RXR_MASK) | _SPI1CON2_SPI1TXR_MASK;
		//Store received data  from receive buffer register 
		returnValue = SPI1RXB;
	}
	else
	{
		returnValue = 0x00U;
	}
	return returnValue;
}


bool SPI1_IsTxReady(void)
{
	bool returnValue = false;
    	if (SPI_IDLE == spi1_descriptor.status)
    	{
        	returnValue = true;
    	}
    	else 
    	{
        	returnValue = false;
    	}
    	return returnValue;
}

bool SPI1_IsRxReady(void)
{
	bool returnValue = false;
    	if((spi1_descriptor.status == SPI_IDLE) && (1U == PIR3bits.SPI1RXIF))
    	{
        	returnValue = true;
    	}
    	else 
    	{
        	returnValue = false;
    	}
    	return returnValue;
}

void SPI1_RxCompleteCallbackRegister(void (*rxCompleteCallbackHandler)(void))
{
	 SPI1_RxCompleteCallback = rxCompleteCallbackHandler;
}

void SPI1_TxCompleteCallbackRegister(void (*txCompleteCallbackHandler)(void))
{
	 SPI1_TxCompleteCallback = txCompleteCallbackHandler;
}

void SPI1_Receive_ISR(void)
{
	if(spi1_descriptor.bytesToReceive > (size_t)0)
	{
		//Read the receive buffer register
		*spi1_descriptor.receiveBuffer = SPI1RXB;
		spi1_descriptor.receiveBuffer++;
		spi1_descriptor.bytesToReceive = spi1_descriptor.bytesToReceive - 1U;
	}
	else
    {
        //In case of operation for Byte Write
        //To clear the interrupt flag, perform a dummy read
        (void)SPI1RXB;
    }
	if((size_t)0 == spi1_descriptor.bytesToReceive)
	{
        //If no more bytes to receive disable the interrupts
			PIE3bits.SPI1RXIE 	= 0U;
        //Check to see if more bytes are there to transmit
        //In case of Buffer Read there wont be any bytes to transmit
        //It should set it the status to SPI_IDLE
        if((size_t)0 == spi1_descriptor.bytesToTransmit)
        {
	    spi1_descriptor.status = SPI_IDLE;	
    	}
	else
	{
		//Do nothing
	}
	if (SPI1_RxCompleteCallback != NULL)
        {
        	SPI1_RxCompleteCallback();
        }
        else
        {
        	// No callback exists
        }
	}
	else
	{
		//Do nothing
	}
}

void SPI1_Transmit_ISR(void)
{
	if(spi1_descriptor.bytesToTransmit > (size_t)0)
	{
        //Transmit the data on transmit buffer register
		SPI1TXB = *spi1_descriptor.transmitBuffer;
		spi1_descriptor.transmitBuffer++;
		spi1_descriptor.bytesToTransmit = spi1_descriptor.bytesToTransmit - 1U;
	}
	else
	{
		//Do nothing
	}
	
	//The execution comes here to check if there is no more bytes to transmit
	//This condition is true in case of last byte to be transmitted
	if((size_t)0 == spi1_descriptor.bytesToTransmit)
	{
		//Disable the interrupt
		PIE3bits.SPI1TXIE 	= 0U;
        //Check to see if more bytes are there to receive
        //In case of Buffer Write there wont be any bytes to receive
        //It should set it the status to SPI_IDLE
        if((size_t)0 == spi1_descriptor.bytesToReceive)
	{
			spi1_descriptor.status = SPI_IDLE;
	}
	else
	{
		//Do nothing
	}
	if (SPI1_TxCompleteCallback != NULL)
        {
        	SPI1_TxCompleteCallback();
        }
        else
        {
        	// No callback exists
        }
	}
}

void SPI1_ISR(void)
{
	//This ISR will be for reporting errors
}
//*****************************************************************************
//
// Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  Redistributions of source code must retain the above copyright
//  notice, this list of conditions and the following disclaimer.
//
//  Redistributions in binary form must reproduce the above copyright
//  notice, this list of conditions and the following disclaimer in the
//  documentation and/or other materials provided with the
//  distribution.
//
//  Neither the name of Texas Instruments Incorporated nor the names of
//  its contributors may be used to endorse or promote products derived
//  from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// uart_driver.c
//
// 		Description: Source code for the implementation of the Bosch BoosterPack with
// 		an MSP432P401R launchpad for low power consumption. BoosterPack includes:
// 		- Inertial Measurement Unit (IMU) sensor with an accelerometer and gyroscope
// 		- Magnetometer
// 		- Environmental sensor with pressure, ambient temperature and humidity
// 		- Ambient light sensor
// 		- Infrared temperature sensor
//
// Adapted by Michael Arriete
//****************************************************************************

#include <ti/devices/msp432p4xx/inc/msp432.h>
#include "stdio.h"
#include "string.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "uart_driver.h"
int CNT=1;
#define SMCLK_FREQUENCY_HZ			(12000000)

s_test test = {
	FALSE,
	"",
	""
};
const eUSCI_UART_ConfigV1 uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        78,                                     // BRDIV = 78
        2,                                       // UCxBRF = 2
        0,                                       // UCxBRS = 0
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
        EUSCI_A_UART_8_BIT_LEN                  // 8 bit data length
};
const eUSCI_UART_ConfigV1 uartConfig1 =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        78,                                     // BRDIV = 78
        2,                                       // UCxBRF = 2
        0,                                       // UCxBRS = 0
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
        EUSCI_A_UART_8_BIT_LEN                  // 8 bit data length
};

void uartInit()
{

    MAP_WDT_A_holdTimer();

    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);

    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

    MAP_UART_initModule(EUSCI_A2_BASE, &uartConfig);
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);
    MAP_UART_enableModule(EUSCI_A2_BASE);
    MAP_UART_enableModule(EUSCI_A0_BASE);

    MAP_SysTick_enableModule();
    MAP_SysTick_setPeriod(3000000);

    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_SysTick_enableInterrupt();

    /* Enabling MASTER interrupts */
    MAP_Interrupt_enableMaster();
}


void sendText(){
   unsigned int i;

	for (i = 0; i < MAX_STR_LENGTH; ++i)
    {
	   // wait until UART ready
	   while (!(UCA2IFG & UCTXIFG)) {
	       ;// USCI_A2 TX buffer ready?
	   }
		   if (test.txString[i] != 0)
		  {
		       UART_transmitData(EUSCI_A2_BASE, test.txString[i]);
//			  EUSCI_A_UART_transmitData(EUSCI_A2_BASE, test.txString[i]);
		  }
		  else{
			  break;
		  }
    }
}
void SysTick_Handler(void)
{
    MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN0);
    if(CNT%2==0)
    {
//        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN2);
    }
    if(CNT%3==0)
    {
//        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN1);
    }

    CNT++;
}


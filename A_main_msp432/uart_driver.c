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

char SSID[9]="MI10";
char PASSD[11]="123456789";
char CLDSTA[22]="13357965877591237946";
char CLDPASSD[10]="12345678";
int CNT=1;
static volatile uint16_t curADCResultX,curADCResultY;
static volatile float normalizedADCRes;

char str;
#define SMCLK_FREQUENCY_HZ			(12000000)

s_test test = {
	FALSE,
	"",
	""
};
const eUSCI_UART_ConfigV1 uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        6,                                     // BRDIV = 78
        8,                                       // UCxBRF = 2
        32,                                       // UCxBRS = 0
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
        EUSCI_A_UART_8_BIT_LEN                  // 8 bit data length
};
void delay(int interval)
{
    int i,j;
    for(i=0;i<interval;i++)
        for(j=0;j<1025;j++);
}
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
//
//    MAP_UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
//    MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
//    MAP_Interrupt_enableInterrupt(INT_EUSCIA2);
//    MAP_Interrupt_enableInterrupt(INT_EUSCIA0);
//    MAP_Interrupt_enableSleepOnIsrExit();
//    MAP_Interrupt_enableMaster();
//    MAP_SysTick_enableModule();
//    MAP_SysTick_setPeriod(3000000);
//
//    MAP_Interrupt_enableSleepOnIsrExit();
//    MAP_SysTick_enableInterrupt();

    /* Enabling MASTER interrupts */
//    MAP_Interrupt_enableMaster();
}


void sendTextEsp(){
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
//		       UART_transmitData(EUSCI_A0_BASE, test.txString[i]);
//			  EUSCI_A_UART_transmitData(EUSCI_A2_BASE, test.txString[i]);
		  }
		  else{
			  break;
		  }
    }
}
void sendTextPc(){
   unsigned int i;

    for (i = 0; i < MAX_STR_LENGTH; ++i)
    {
       // wait until UART ready
       while (!(UCA2IFG & UCTXIFG)) {
           ;// USCI_A2 TX buffer ready?
       }
           if (test.txString[i] != 0)
          {
//               UART_transmitData(EUSCI_A2_BASE, test.txString[i]);
               UART_transmitData(EUSCI_A0_BASE, test.txString[i]);
//            EUSCI_A_UART_transmitData(EUSCI_A2_BASE, test.txString[i]);
          }
          else{
              break;
          }
    }
}
void ESPInit()
{
//    char str='';
//    snprintf(test.txString, 60,"AT\r\n");sendTextEsp();

//    while(str!='P')
//    {
//        str=MAP_UART_receiveData(EUSCI_A2_BASE);
//    }
//    str=MAP_UART_receiveData(EUSCI_A2_BASE);

//    snprintf(test.txString, 60,"AT+CWMODE=1\r\n");sendTextEsp();sendTextPc();
    snprintf(test.txString, 60,"AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PASSD);sendTextEsp();sendTextPc();
    delay(6000);
//    snprintf(test.txString, 60,"AT+CWDHCP_DEF=1,1\r\n");sendTextEsp();sendTextPc();
//    delay(3);
    snprintf(test.txString, 60,"AT+ATKCLDSTA=\"%s\",\"%s\"\r\n",CLDSTA,CLDPASSD);sendTextEsp();sendTextPc();
    delay(4000);
}
//void EUSCIA2_IRQHandler(void)
//{
//    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A2_BASE);
//
//    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
//    {
////        MAP_UART_transmitData(EUSCI_A2_BASE, MAP_UART_receiveData(EUSCI_A2_BASE));
//        snprintf(test.txString, 60,"there is PC");
////        sendTextEsp();
//        sendTextPc();
//    }
//
//}
//void EUSCIA0_IRQHandler(void)
//{
//    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);
//
//    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
//    {
////        MAP_UART_transmitData(EUSCI_A0_BASE, MAP_UART_receiveData(EUSCI_A0_BASE));
//        snprintf(test.txString, 60,"there is PC");sendTextEsp();
//    }
//
//}

void adcInit(void)
{
    /*
     * VRX-->5.5
     * VRY-->5.4
     * SW -->6.4*/
    MAP_REF_A_setReferenceVoltage(REF_A_VREF2_5V);
    MAP_REF_A_enableReferenceVoltage();
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 1);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 1);
    MAP_PCM_setPowerState(PCM_AM_LDO_VCORE1);
    MAP_FPU_enableModule();
        MAP_FPU_enableLazyStacking();
        MAP_ADC14_enableModule();
        MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1,
                0);
        MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN5,
        GPIO_TERTIARY_MODULE_FUNCTION);
        MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN4,
        GPIO_TERTIARY_MODULE_FUNCTION);
        /* Configuring ADC Memory */
        MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, false);
         MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_INTBUF_VREFNEG_VSS,
         ADC_INPUT_A0, false);
         MAP_ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_INTBUF_VREFNEG_VSS,
         ADC_INPUT_A1, false);
        MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P6, GPIO_PIN4);
        MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, GPIO_PIN4);
        MAP_GPIO_enableInterrupt(GPIO_PORT_P6, GPIO_PIN4);
        MAP_Interrupt_enableInterrupt(INT_PORT6);
        MAP_SysCtl_enableSRAMBankRetention(SYSCTL_SRAM_BANK1);
        /* Enabling interrupts */
//        MAP_ADC14_enableInterrupt(ADC_INT1);
//        MAP_Interrupt_enableInterrupt(INT_ADC14);
        MAP_Interrupt_enableSleepOnIsrExit();
        MAP_Interrupt_enableMaster();
        MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

        /* Triggering the start of the sample */
        MAP_ADC14_enableConversion();
        MAP_ADC14_toggleConversionTrigger();
}
void ADC14_IRQHandler(void)
{
    uint64_t status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);

    if (ADC_INT1 & status)
    {
        curADCResultX = MAP_ADC14_getResult(ADC_MEM0);
        curADCResultY= MAP_ADC14_getResult(ADC_MEM1);
       /* curADCResult = MAP_ADC14_getResult(ADC_MEM0);
        adc=curADCResult;
        MAP_UART_transmitData(EUSCI_A0_BASE, 48+adc/10000);

                MAP_UART_transmitData(EUSCI_A0_BASE, 48+(adc/1000)%10);
                MAP_UART_transmitData(EUSCI_A0_BASE, 48+(adc/100)%10);
                MAP_UART_transmitData(EUSCI_A0_BASE, 48+(adc/10)%10);
                MAP_UART_transmitData(EUSCI_A0_BASE, 48+(adc%10));
                        MAP_UART_transmitData(EUSCI_A0_BASE, ' ');
                curADCResult = MAP_ADC14_getResult(ADC_MEM1);
                adc=curADCResult;
                        MAP_UART_transmitData(EUSCI_A0_BASE, 48+adc/10000);
                        MAP_UART_transmitData(EUSCI_A0_BASE, 48+(adc/1000)%10);
                        MAP_UART_transmitData(EUSCI_A0_BASE, 48+(adc/100)%10);
                        MAP_UART_transmitData(EUSCI_A0_BASE, 48+(adc/10)%10);
                        MAP_UART_transmitData(EUSCI_A0_BASE, 48+(adc%10));

                MAP_UART_transmitData(EUSCI_A0_BASE, '\r');
                MAP_UART_transmitData(EUSCI_A0_BASE, '\n');*/
        MAP_ADC14_toggleConversionTrigger();
    }
//    delay(1000);
}
//void PORT6_IRQHandler(void)
//{
//    uint32_t status;
//
//    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P6);
//    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, status);
//    if(status & GPIO_PIN4)
//    {
//        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
//        UART_transmitData(EUSCI_A0_BASE, 'j');
//    }
//    delay(500);
//}

//void SysTick_Handler(void)
//{
////    MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN0);
//    if(CNT%2==0)
//    {
////        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN2);
//    }
//    if(CNT%3==0)
//    {
////        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN1);
//    }
//
//    CNT++;
//}


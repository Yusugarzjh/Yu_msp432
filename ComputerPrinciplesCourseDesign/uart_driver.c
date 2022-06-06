#include <ti/devices/msp432p4xx/inc/msp432.h>
#include "stdio.h"
#include "string.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "uart_driver.h"
char SSID[9]="MI10";//WIFI名
char PASSD[11]="123456789";//WIFI密码
char CLDSTA[22]="13357965877591237946";//设备编号
char CLDPASSD[10]="12345678";//密码
static volatile uint16_t curADCResultX,curADCResultY;
static volatile float normalizedADCRes;
char str;
// #define SMCLK_FREQUENCY_HZ			(12000000)
s_test test = {
	FALSE,
	"",
	""
};
//串口设置115200波特率
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
void delay(int interval)//延迟函数
{
    int i,j;
    for(i=0;i<interval;i++)
        for(j=0;j<1025;j++);
}
void uartInit()//初始化串口模块
{
    
    MAP_WDT_A_holdTimer();
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);
    MAP_UART_initModule(EUSCI_A2_BASE, &uartConfig);
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);
    MAP_UART_enableModule(EUSCI_A2_BASE);
    MAP_UART_enableModule(EUSCI_A0_BASE);
    MAP_Interrupt_enableMaster();
}
void sendTextEsp()//发送信息至原子云
{
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

		  }
		  else{
			  break;
		  }
    }
}
void sendTextPc()//发送信息至串口
{
   unsigned int i;

    for (i = 0; i < MAX_STR_LENGTH; ++i)
    {
       // wait until UART ready
       while (!(UCA0IFG & UCTXIFG)) {
           ;// USCI_A2 TX buffer ready?
       }
           if (test.txString[i] != 0)
          {
               UART_transmitData(EUSCI_A0_BASE, test.txString[i]);
          }
          else{
              break;
          }
    }
}
void ESPInit()//连接原子云
{
    snprintf(test.txString, 60,"AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PASSD);sendTextEsp();sendTextPc();
    delay(6000);//延迟等待连接
    snprintf(test.txString, 60,"AT+ATKCLDSTA=\"%s\",\"%s\"\r\n",CLDSTA,CLDPASSD);sendTextEsp();sendTextPc();
    delay(6000);//延迟等待连接
}
void adcInit(void)//adc模块初始化
{
    /*引脚连接
      VRX-->5.5
      VRY-->5.4
      SW -->6.4*/
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
    MAP_SysCtl_enableSRAMBankRetention(SYSCTL_SRAM_BANK1);
    MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
    /* Triggering the start of the sample */
    MAP_ADC14_enableConversion();
    MAP_ADC14_toggleConversionTrigger();
}

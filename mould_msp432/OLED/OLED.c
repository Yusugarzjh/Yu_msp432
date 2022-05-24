/**
  ******************************************************************************
  * @file    Project/Template/main.c 
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    04/06/2009
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
//#include "stm32f10x.h"
//#include "stm32f10x_conf.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <LQ12864.h>
#define LCD_SCL_PORT GPIO_PORT_P4
#define LCD_SCL_PIN  GPIO_PIN1
#define LCD_SDA_PORT GPIO_PORT_P4
#define LCD_SDA_PIN  GPIO_PIN2
#define LCD_RST_PORT GPIO_PORT_P4
#define LCD_RST_PIN  GPIO_PIN3
#define LCD_DC_PORT  GPIO_PORT_P4
#define LCD_DC_PIN   GPIO_PIN4
#define LCD_CS_PORT  GPIO_PORT_P1
#define LCD_CS_PIN   GPIO_PIN5
#define LCD_SCL(x)  ((x)?(GPIO_setOutputHighOnPin(LCD_SCL_PORT,LCD_SCL_PIN)):(GPIO_setOutputLowOnPin(LCD_SCL_PORT,LCD_SCL_PIN)))
#define LCD_SDA(x)  ((x)?(GPIO_setOutputHighOnPin(LCD_SDA_PORT,LCD_SDA_PIN)):(GPIO_setOutputLowOnPin(LCD_SDA_PORT,LCD_SDA_PIN)))
#define LCD_RST(x)  ((x)?(GPIO_setOutputHighOnPin(LCD_RST_PORT,LCD_RST_PIN)):(GPIO_setOutputLowOnPin(LCD_RST_PORT,LCD_RST_PIN)))
#define LCD_DC(x)   ((x)?(GPIO_setOutputHighOnPin(LCD_DC_PORT,LCD_DC_PIN)):(GPIO_setOutputLowOnPin(LCD_DC_PORT,LCD_DC_PIN)))
#define LCD_CS(x)   ((x)?(GPIO_setOutputHighOnPin(LCD_CS_PORT,LCD_CS_PIN)):(GPIO_setOutputLowOnPin(LCD_CS_PORT,LCD_CS_PIN )))
//extern unsigned char lqTFT12864[] ;
/*************************************************
函数: void RCC_Configuration(void)
功能: 复位和时钟控制 配置
参数: 无
返回: 无
**************************************************/
//void RCC_Configuration(void)
//{
//  ErrorStatus HSEStartUpStatus;                    //定义外部高速晶体启动状态枚举变量
//  RCC_DeInit();                                    //复位RCC外部设备寄存器到默认值
//  RCC_HSEConfig(RCC_HSE_ON);                       //打开外部高速晶振
//  HSEStartUpStatus = RCC_WaitForHSEStartUp();      //等待外部高速时钟准备好
//  if(HSEStartUpStatus == SUCCESS)                  //外部高速时钟已经准别好
//  {
//    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //开启FLASH预读缓冲功能，加速FLASH的读取。所有程序中必须的用法.位置：RCC初始化子函数里面，时钟起振之后
//    FLASH_SetLatency(FLASH_Latency_2);                    //flash操作的延时
//
//    RCC_HCLKConfig(RCC_SYSCLK_Div1);               //配置AHB(HCLK)时钟等于==SYSCLK
//    RCC_PCLK2Config(RCC_HCLK_Div1);                //配置APB2(PCLK2)钟==AHB时钟
//    RCC_PCLK1Config(RCC_HCLK_Div2);                //配置APB1(PCLK1)钟==AHB1/2时钟
//
//    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  //配置PLL时钟 == 外部高速晶体时钟 * 9 = 72MHz
//    RCC_PLLCmd(ENABLE);                                   //使能PLL时钟
//
//    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)    //等待PLL时钟就绪
//    {
//    }
//    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);            //配置系统时钟 = PLL时钟
//    while(RCC_GetSYSCLKSource() != 0x08)                  //检查PLL时钟是否作为系统时钟
//    {
//    }
//  }
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
//}
int main(void)
{
//   RCC_Configuration();
    MAP_WDT_A_holdTimer();
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);
   LCD_init();               //OLED模块初始化 
	 
   while(1)
   {
        
//       show_pic(1,0,128,64,lqTFT12864);
//       time_delay_ms(3000);
////       display_pic();
//       time_delay_ms(2000);      //延时
//
//       LCD_CLS();                //OLED清屏
//       LCD_draw_line(2,1,XMAX-3,YMAX,GREEN);
//       LCD_draw_line(2,YMAX,XMAX-3,1,GREEN);
//       LCD_draw_circle(64,32,20,WHITE);
//       LCD_draw_circle(64,32,25,YELLOW);
//       LCD_draw_circle(64,32,30,BLUE);
//       LCD_draw_rectangle(2,1,XMAX-3,YMAX,GREEN);
       
//       LCD_draw_part( 5, 2,126,62,RED);
//       LCD_draw_part(10, 5,119,59,GREEN);
//       LCD_draw_part(15, 8,114,56,BLUE);
//       LCD_draw_part(20,11,109,53,WHITE);
//       LCD_draw_part(25,14,104,50,BLACK);
//       LCD_draw_part(30,17, 99,47,ORANGE);
//       LCD_draw_part(35,20, 94,42,PURPLE);
//       LCD_draw_part(40,23, 89,37,YELLOW);
//       LCD_draw_part(45,26, 84,32,CYAN);
       
//       time_delay_ms(2000);      //延时
      
       LCD_CLS();                //OLED清屏    
       LCD_P16X16Str(1,0,"龙邱智能科技",RED,BLACK);       //显示汉字字符串       
       LCD_P6X8Str(2,7,"LongQiu.taobao.com",CYAN,BLACK);   //显示英文6*8字符串            
       LCD_P8X16Str(2,2,"www.LQIST.cn",BLUE,BLACK);         //显示英文8*16字符串
       LCD_P16X16Str(1,1,"液晶校准测试",YELLOW,BLACK);     //显示汉字字符串
       while(1);
//       time_delay_ms(1000);
    }
   
  } 




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
//#include "picture.h"
#include "lqTFT12864.h"
/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <LQ12864.h>
#define LCD_SCL_PORT GPIO_PORT_P4
#define LCD_SCL_PIN GPIO_PIN1
#define LCD_SDA_PORT GPIO_PORT_P4
#define LCD_SDA_PIN GPIO_PIN2
#define LCD_RST_PORT GPIO_PORT_P4
#define LCD_RST_PIN GPIO_PIN3
#define LCD_DC_PORT GPIO_PORT_P4
#define LCD_DC_PIN GPIO_PIN4
#define LCD_CS_PORT GPIO_PORT_P1
#define LCD_CS_PIN GPIO_PIN5
#define LCD_SCL(x) ((x) ? (GPIO_setOutputHighOnPin(LCD_SCL_PORT, LCD_SCL_PIN)) : (GPIO_setOutputLowOnPin(LCD_SCL_PORT, LCD_SCL_PIN)))
#define LCD_SDA(x) ((x) ? (GPIO_setOutputHighOnPin(LCD_SDA_PORT, LCD_SDA_PIN)) : (GPIO_setOutputLowOnPin(LCD_SDA_PORT, LCD_SDA_PIN)))
#define LCD_RST(x) ((x) ? (GPIO_setOutputHighOnPin(LCD_RST_PORT, LCD_RST_PIN)) : (GPIO_setOutputLowOnPin(LCD_RST_PORT, LCD_RST_PIN)))
#define LCD_DC(x) ((x) ? (GPIO_setOutputHighOnPin(LCD_DC_PORT, LCD_DC_PIN)) : (GPIO_setOutputLowOnPin(LCD_DC_PORT, LCD_DC_PIN)))
#define LCD_CS(x) ((x) ? (GPIO_setOutputHighOnPin(LCD_CS_PORT, LCD_CS_PIN)) : (GPIO_setOutputLowOnPin(LCD_CS_PORT, LCD_CS_PIN)))
/*
    OLED引脚接线
    GND --> GND
    VCC --> 3.3V
    CK  --> 4.1
    SD  --> 4.2
    RST --> 4.3
    AO  --> 4.4
    画图要用到的函数都在LQ12864.c中
    更改某个像素点的颜色
    LCD_draw_dot(x,y,color(颜色));

*/
int main(void)
{
  //   RCC_Configuration();
  MAP_WDT_A_holdTimer();
  CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);
  LCD_init(); // OLED模块初始化

  while (1)
  {

//           show_pic(1,0,128,64,lqTFT12864);            //显示图片LQ12864.h
//           time_delay_ms(3000);                        //延时
//           display_pic();                              //显示图片picture.h
//           time_delay_ms(2000);                        //延时
//
//           LCD_CLS();                                  //OLED清屏
//           LCD_draw_line(2,1,XMAX-3,YMAX,GREEN);       //画直线，前两个参数为第一个端点的坐标，然后是后面两个端点的坐标，最后一个参数是颜色
//
//           LCD_draw_circle(64,32,20,WHITE);            //画圆，前两个参数为圆心的坐标，第三个是半径，最后颜色
//
//           LCD_draw_rectangle(2,1,XMAX-3,YMAX,GREEN);  //画矩形框，前两个参数为左上角的坐标，然后是后面右下角的坐标，最后颜色
//
//           LCD_draw_part(45,26, 84,32,CYAN);           //画实心矩形，前两个参数为左上角的坐标，然后是后面右下角的坐标，最后颜色
//
//           time_delay_ms(2000);                        //延时


    LCD_CLS();                                           // OLED清屏
//    LCD_P16X16Str(1, 0, "龙邱智能科技", RED, BLACK);     //显示汉字字符串
    LCD_P16X16Str(2, 2, "病胶囊人", RED, BLACK);
//    LCD_P6X8Str(2, 7, "LongQiu.taobao.com", CYAN, BLACK);//显示英文6*8字符串
//    LCD_P8X16Str(2, 2, "www.LQIST.cn", BLUE, BLACK);     //显示英文8*16字符串
//    LCD_P16X16Str(1, 1, "液晶校准测试", YELLOW, BLACK);  //显示汉字字符串
    while (1)
      ;
    //       time_delay_ms(1000);//延时
  }
}

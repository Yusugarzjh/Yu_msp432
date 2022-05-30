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
    OLED���Ž���
    GND --> GND
    VCC --> 3.3V
    CK  --> 4.1
    SD  --> 4.2
    RST --> 4.3
    AO  --> 4.4
    ��ͼҪ�õ��ĺ�������LQ12864.c��
    ����ĳ�����ص����ɫ
    LCD_draw_dot(x,y,color(��ɫ));

*/
int main(void)
{
  //   RCC_Configuration();
  MAP_WDT_A_holdTimer();
  CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);
  LCD_init(); // OLEDģ���ʼ��

  while (1)
  {

//           show_pic(1,0,128,64,lqTFT12864);            //��ʾͼƬLQ12864.h
//           time_delay_ms(3000);                        //��ʱ
//           display_pic();                              //��ʾͼƬpicture.h
//           time_delay_ms(2000);                        //��ʱ
//
//           LCD_CLS();                                  //OLED����
//           LCD_draw_line(2,1,XMAX-3,YMAX,GREEN);       //��ֱ�ߣ�ǰ��������Ϊ��һ���˵�����꣬Ȼ���Ǻ��������˵�����꣬���һ����������ɫ
//
//           LCD_draw_circle(64,32,20,WHITE);            //��Բ��ǰ��������ΪԲ�ĵ����꣬�������ǰ뾶�������ɫ
//
//           LCD_draw_rectangle(2,1,XMAX-3,YMAX,GREEN);  //�����ο�ǰ��������Ϊ���Ͻǵ����꣬Ȼ���Ǻ������½ǵ����꣬�����ɫ
//
//           LCD_draw_part(45,26, 84,32,CYAN);           //��ʵ�ľ��Σ�ǰ��������Ϊ���Ͻǵ����꣬Ȼ���Ǻ������½ǵ����꣬�����ɫ
//
//           time_delay_ms(2000);                        //��ʱ


    LCD_CLS();                                           // OLED����
//    LCD_P16X16Str(1, 0, "�������ܿƼ�", RED, BLACK);     //��ʾ�����ַ���
    LCD_P16X16Str(2, 2, "��������", RED, BLACK);
//    LCD_P6X8Str(2, 7, "LongQiu.taobao.com", CYAN, BLACK);//��ʾӢ��6*8�ַ���
//    LCD_P8X16Str(2, 2, "www.LQIST.cn", BLUE, BLACK);     //��ʾӢ��8*16�ַ���
//    LCD_P16X16Str(1, 1, "Һ��У׼����", YELLOW, BLACK);  //��ʾ�����ַ���
    while (1)
      ;
    //       time_delay_ms(1000);//��ʱ
  }
}

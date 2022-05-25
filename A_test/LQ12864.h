/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�MK66FX1M0VLQ18���İ�
����    д��CHIUSIR
����    ע��
������汾��V1.0
�������¡�2016��08��20��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
���������䡿chiusir@163.com
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#ifndef __LQ12864_H__
#define __LQ12864_H__
//#include "stm32f10x.h"
/*******************�ӿڶ���******************************/
					  
#define	LQTFT_A0	PDout(3)
#define	LQTFT_RST	PDout(2) 
#define	LQTFT_SDA	PDout(1) 
#define	LQTFT_SCK	PDout(0)
#define	LQTFT_CS	PDout(4) 


#define	RED	0xf800
#define	GREEN	0x07e0
#define	BLUE	0x001f
#define	PURPLE	0xf81f
#define	YELLOW	0xffe0
#define	CYAN	0x07ff 		//����ɫ
#define	ORANGE	0xfc08
#define	BLACK	0x0000
#define	WHITE	0xffff

#define	XMAX	132//132
#define	YMAX	65//64

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
/*****************˽�к�������*********************************/
void time_delay_ms(unsigned int ms)	 ;
void LCD_init(void);									//LCD��ʼ��

void LCD_write_command(unsigned char cmd);						//���Ϳ�����

void LCD_write_para8(unsigned char dat);						//�������ݲ���

void LCD_write_para16(unsigned int dat);						//����������ʾ����

void LCD_address_rst(void);							    //DDRAM��ַ����

void LCD_SetPos(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye);	//��λ��ʾ����λ��
void LCD_display_full(unsigned int color);					//ȫ����ʾĳ����ɫ
void LCD_CLS(void);

void LCD_draw_part(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned int color_dat);
															//��������������ʼ����ֹ���꣬����ʼ����ֹ���꣬��ɫ
void LCD_draw_line(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned int color_dat);
															//���ߣ�����ʼ����ֹ���꣬����ʼ����ֹ���꣬��ɫ
void LCD_draw_rectangle(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned int color_dat);
															//�����α߿�����ʼ����ֹ���꣬����ʼ����ֹ���꣬��ɫ
void LCD_draw_circle(unsigned char x,unsigned char y,unsigned char r,unsigned int color_dat);
															//��Բ�α߿�Բ�ĺ����ꡢ�����꣬�뾶����ɫ
void LCD_draw_dot(unsigned char x,unsigned char y,unsigned int color_dat);	//���㣬�����꣬�����꣬��ɫ

void display_pic();//��ʾͼƬ

void LCD_P8X16(unsigned char x, unsigned char y, unsigned char c_dat,unsigned int word_color,unsigned int back_color);

void LCD_P8X16Str(unsigned char x, unsigned char y, unsigned char *s_dat,unsigned int word_color,unsigned int back_color);

void LCD_P6X8(unsigned char x, unsigned char y, unsigned char c_dat,unsigned int word_color,unsigned int back_color);

void LCD_P6X8Str(unsigned char x, unsigned char y, unsigned char *s_dat,unsigned int word_color,unsigned int back_color);

void LCD_P6X8NUM(unsigned char x, unsigned char y, unsigned int num, unsigned char num_bit,unsigned int word_color,unsigned int back_color);

void LCD_P8X8(unsigned char x, unsigned char y, unsigned char c_dat,unsigned int word_color,unsigned int back_color);

void LCD_P8X8Str(unsigned char x, unsigned char y, unsigned char *s_dat,unsigned int word_color,unsigned int back_color);

void LCD_P8X8NUM(unsigned char x, unsigned char y, unsigned int num, unsigned char num_bit,unsigned int word_color,unsigned int back_color);

void LCD_P16X16Str(unsigned char x,unsigned char y,unsigned char *s_dat,unsigned int word_color,unsigned int back_color);
void LCD_P16X12Str(unsigned char x,unsigned char y,unsigned char *s_dat,unsigned int word_color,unsigned int back_color);
void show_pic(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned char pic[]);

#endif /*SGP18T_LQTFTB.h*/

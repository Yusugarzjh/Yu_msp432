
#include <SPI.h>
#include <LCD_screen.h>
#include <LCD_screen_font.h>  
#include <LCD_utilities.h>
#include <Screen_HX8353E.h>
#include <Terminal12e.h>
#include <Terminal6e.h>
#include <Terminal8e.h>
Screen_HX8353E myScreen;



// notes in the melody:



const int buttonOne = 33;     // the number of the pushbutton pin
const int buttonTwo = 32;     // the number of the pushbutton pin
const int ledGreen =  38;      // the number of the LED pin
const int ledBlue =  37;      // the number of the LED pin

// variables will change:
int buttonOneState = 0;         // variable for reading the pushbutton #1 status
int buttonTwoState = 0;         // variable for reading the pushbutton #2 status

// Define variables and constants


#define joystickX 2
#define joystickY 26
uint16_t x, y, x00, y00;
uint16_t colour;
//uint32_t z;
int h=1,z=4;

void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200); // for LCD debug output

    // By default MSP432 has analogRead() set to 10 bits. 
    // This Sketch assumes 12 bits. Uncomment to line below to set analogRead()
    // to 12 bit resolution for MSP432.
    analogReadResolution(12);

    myScreen.begin();
    x00 = 0;
    y00 = 0;
      // initialize the LED pins as  output:
  pinMode(ledGreen, OUTPUT);     
  pinMode(ledBlue, OUTPUT);     

   pinMode(buzzerPin,OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonOne, INPUT_PULLUP);     
  pinMode(buttonTwo, INPUT_PULLUP); 
}
//推箱子小游戏

int mymap[8][8]=           //定义一个8x8的地图，用数组表示各单元格元素
{{0,0,0,1,1,1,1,0},       //0 blank
 {0,1,1,1,5,0,1,0},       //1 wall
 {0,1,0,0,4,0,1,0},       //3 destination
 {1,1,0,1,0,1,1,1},       //4 box
 {1,0,0,1,0,1,3,1},       //5 person
 {1,0,1,0,0,4,3,1},       //7 box+destination
 {1,0,4,0,0,0,3,1},       //8 person+destination
 {1,1,1,1,1,1,1,1}
  
  };
int * p=&mymap[0][0];
void change()
{
int mymapo[8][8]=           //定义一个8x8的地图，用数组表示各单元格元素
{{1,1,1,1,1,1,1,1},       //0 blank
 {1,0,0,0,1,0,0,1},       //1 wall
 {1,0,1,0,1,4,3,1},       //3 destination
 {1,0,0,0,0,4,3,1},       //4 box
 {1,0,1,0,1,4,3,1},       //5 person
 {1,0,0,0,1,0,0,1},       //7 box+destination
 {1,1,1,1,1,5,0,1},       //8 person+destination
 {0,0,0,0,1,1,1,1}
  
  };
  
 
   
    for(int i=0;i<8;i++)
        {for(int j=0;j<8;j++)
          {
           mymap[i][j]= mymapo[i][j];
            }
            
          }

  };



/*int mymap[8][8]=           //定义一个8x8的地图，用数组表示各单元格元素
{{1,1,1,1,1,1,1,1},       //0 blank
 {1,0,0,0,1,0,0,1},       //1 wall
 {1,0,1,0,1,4,3,1},       //3 destination
 {1,0,0,0,0,4,3,1},       //4 box
 {1,0,1,0,1,4,3,1},       //5 person
 {1,0,0,0,1,0,0,1},       //7 box+destination
 {1,1,1,1,1,5,0,1},       //8 person+destination
 {0,0,0,0,1,1,1,1}
  
  };*/
 void paint()             //绘制初始地图
 {
  for(int i=0;i<8;i++)
    { for(int j=0;j<8;j++)
        {
            switch(mymap[j][i])
            { case 1:for(int k=1;k<15;k++){myScreen.dRectangle(i*16+k,j*16+k,16-k,16-k,redColour);};break;
              case 3:myScreen.triangle(i*16,j*16,i*16,j*16+16,i*16+16,j*16+8,redColour);break;
              case 4: myScreen.dRectangle(i*16+1,j*16+1,15,15,redColour);break;
              case 5:myScreen.circle(i*16+8,j*16+8,8,redColour);break;
              case 7:myScreen.dRectangle(i*16+1,j*16+1,15,15,redColour);break;
              case 8:myScreen.circle(i*16+8,j*16+8,8,redColour);break;
              }
          }
      }
  };

 void play()                //玩游戏
 {delay(500);               //控制小人移动速度
    x = map(analogRead(joystickX), 0, 4096, 0, 128);
    y = map(analogRead(joystickY), 0, 4096, 128, 0);
    if (x < 1)      x = 1;
    if (x > 126)    x = 126;
    if (y < 1)      y = 1;
    if (y > 126)    y = 126;

    if(x>100)
      {
        if(mymap[h][z+1]==0||mymap[h][z+1]==3)
          {cls(h,z);
          cls(h,z+1);
            mymap[h][z]-=5;
          mymap[h][z+1]+=5;
          add(h,z);
          add(h,z+1);
            z++;
            }
     else if((mymap[h][z+1]==4||mymap[h][z+1]==7)&&(mymap[h][z+2]==0||mymap[h][z+2]==3))
               {cls(h,z);
               cls(h,z+1);
               cls(h,z+2);
              mymap[h][z]-=5;
              mymap[h][z+1]+=1;
              mymap[h][z+2]+=4;
              add(h,z);
               add(h,z+1);
               add(h,z+2);
               z++;}
        }
     else if(x<28)
     {
            if(mymap[h][z-1]==0||mymap[h][z-1]==3)
          {cls(h,z);
          cls(h,z-1);
            mymap[h][z]-=5;
          mymap[h][z-1]+=5;
          add(h,z);
          add(h,z-1);
            z--;
            }
        else if((mymap[h][z-1]==4||mymap[h][z-1]==7)&&(mymap[h][z-2]==0||mymap[h][z-2]==3))
               {cls(h,z);
               cls(h,z-1);
               cls(h,z-2);
              mymap[h][z]-=5;
              mymap[h][z-1]+=1;
              mymap[h][z-2]+=4;
              add(h,z);
               add(h,z-1);
               add(h,z-2);
               z--;
             }
          
      }
    else if(y>100)
    {
        if(mymap[h+1][z]==0||mymap[h+1][z]==3)
          {cls(h,z);
          cls(h+1,z);
            mymap[h][z]-=5;
          mymap[h+1][z]+=5;
          add(h,z);
          add(h+1,z);
            h++;
            }
             else if((mymap[h+1][z]==4||mymap[h+1][z]==7)&&(mymap[h+2][z]==0||mymap[h+2][z]==3))
               {cls(h,z);
               cls(h+1,z);
               cls(h+2,z);
              mymap[h][z]-=5;
              mymap[h+1][z]+=1;
              mymap[h+2][z]+=4;
              add(h,z);
               add(h+1,z);
               add(h+2,z);
               h++;}
      }
  else if (y<28)
    { 
       if(mymap[h-1][z]==0||mymap[h-1][z]==3)
          {cls(h,z);
          cls(h-1,z);
            mymap[h][z]-=5;
          mymap[h-1][z]+=5;
          add(h,z);
          add(h-1,z);
            h--;
            }
              else if((mymap[h-1][z]==4||mymap[h-1][z]==7)&&(mymap[h-2][z]==0||mymap[h-2][z]==3))
               {cls(h,z);
               cls(h-1,z);
               cls(h-2,z);
              mymap[h][z]-=5;
              mymap[h-1][z]+=1;
              mymap[h-2][z]+=4;
              add(h,z);
               add(h-1,z);
               add(h-2,z);
               h--;}
      }

      
    
  };
 void add(int m,int n)        //绘制一个单元格图像
 {
       switch(mymap[m][n])
            {
              case 3:myScreen.triangle(n*16,m*16,n*16,m*16+16,n*16+16,m*16+8,redColour);break;
              case 4: myScreen.dRectangle(n*16+1,m*16+1,15,15,redColour);break;
              case 5:myScreen.circle(n*16+8,m*16+8,8,redColour);break;
              case 7:myScreen.dRectangle(n*16+1,m*16+1,15,15,redColour);break;
              case 8:myScreen.circle(n*16+8,m*16+8,8,redColour);break;
            }

  
     
  };
void cls(int m,int n)         //清除一个单元格图像
{
             switch(mymap[m][n])
            {
              case 3:myScreen.triangle(n*16,m*16,n*16,m*16+16,n*16+16,m*16+8,blackColour);break;
              case 4: myScreen.dRectangle(n*16+1,m*16+1,15,15,blackColour);break;
              case 5:myScreen.circle(n*16+8,m*16+8,8,blackColour);break;
              case 7:myScreen.dRectangle(n*16+1,m*16+1,15,15,blackColour);break;
              case 8:myScreen.circle(n*16+8,m*16+8,8,blackColour);break;
            }



  };
void digital()                    //绘制数字 100
{
     myScreen.circle(0,0,2,redColour);    //0  2   5          
   myScreen.line(2,0,5,0,redColour);      //2  
   myScreen.line(0,2,0,4,redColour);      //4  4   4          
   myScreen.line(0,4,2,4,redColour);      //                        
   myScreen.line(2,4,2,10,redColour);     //            
   myScreen.line(2,10,5,10,redColour);    //  
   myScreen.line(5,10,5,0,redColour);     //  10  10          
   myScreen.circle(12,5,5,redColour);
    myScreen.circle(12,5,3,redColour);
   myScreen.circle(24,5,5,redColour);
    myScreen.circle(24,5,3,redColour);
  };
 void clear()               //清除整个游戏地图
 {
  for(int i=0;i<8;i++)
    { for(int j=0;j<8;j++)
        {
            switch(mymap[j][i])
            { case 1:for(int k=1;k<15;k++){myScreen.dRectangle(i*16+k,j*16+k,16-k,16-k,blackColour);};break;
              case 3:myScreen.triangle(i*16,j*16,i*16,j*16+16,i*16+16,j*16+8,blackColour);break;
              case 4: myScreen.dRectangle(i*16+1,j*16+1,15,15,blackColour);break;
              case 5:myScreen.circle(i*16+8,j*16+8,8,blackColour);break;
              case 7:myScreen.dRectangle(i*16+1,j*16+1,15,15,blackColour);break;
              case 8:myScreen.circle(i*16+8,j*16+8,8,blackColour);break;
              }
          }
      }
  };
void sentence1()
{
   myScreen.gText(6,64,"welcome to our game",
                   redColour);
                   delay(1000);
         myScreen.gText(6,64,"welcome to our game",
                   blackColour);
      myScreen.gText(32,64,"let's begin",
                   redColour);
                   delay(1000);
          myScreen.gText(32,64,"let's begin",
                   blackColour);
  };
 void sentence2()
{
   myScreen.gText(16,64,"you are so good",
                   redColour);
                   delay(1000);
         myScreen.gText(16,64,"you are so good",
                   blackColour);
      myScreen.gText(2,64,"let's begin the next",
                   redColour);
                   delay(1000);
          myScreen.gText(2,64,"let's begin the next",
                   blackColour);
  };
void loop() {
sentence1();
paint();
   while(!(mymap[4][6]==7&&mymap[5][6]==7&&mymap[6][6]==7))
  {
    play();
    };
    clear();
    sentence2();
    change();
h=6,z=5;
  paint();
  
 while(!(mymap[2][6]==7&&mymap[3][6]==7&&mymap[4][6]==7))
  {

    play();
    };
clear();
while(1);

}

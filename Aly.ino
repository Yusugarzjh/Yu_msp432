#include <ESP8266WiFi.h>
/* 依赖 PubSubClient 2.4.0 */
#include <PubSubClient.h>
/* 依赖 ArduinoJson 5.13.4 */
#include <ArduinoJson.h>
#include <SparkFun_SHTC3.h>
#include <DHT.h>        //调用DHT库
 
/* 连接您的WIFI SSID和密码 */
#define WIFI_SSID         "MI10" //要改的地方你的WiFi名(热点)
#define WIFI_PASSWD       "123456789"//要改的地方你的WiFi密码
 
/**
 * {
  "ProductKey": "gq2j5ZKAIeD",
  "DeviceName": "ESP8266",
  "DeviceSecret": "ca205372da50722c2977bcd79104b8ca"
}
 */
/* 设备的三元组信息*/
#define PRODUCT_KEY       "ha84B0y65Ko" //要改的地方
#define DEVICE_NAME       "esp8266" //要改的地方
#define DEVICE_SECRET     "2d3a383664b4896e70a7212d44820907"//要改的地方
#define REGION_ID         "cn-shanghai"
 
/* 线上环境域名和端口号，不需要改 */
#define MQTT_SERVER       PRODUCT_KEY ".iot-as-mqtt." REGION_ID ".aliyuncs.com"
#define MQTT_PORT         1883
#define MQTT_USRNAME      DEVICE_NAME "&" PRODUCT_KEY
 
#define CLIENT_ID         "FESA234FBDS24|securemode=3,signmethod=hmacsha1,timestamp=789|" //要改的地方
// 算法工具: http://iot-face.oss-cn-shanghai.aliyuncs.com/tools.htm 进行加密生成password
// password教程 https://www.yuque.com/cloud-dev/iot-tech/mebm5g
#define MQTT_PASSWD       "9a829f36748b1f0fef9707b32a9765f4b59dc6e1"  //要改的地方
 
#define ALINK_BODY_FORMAT         "{\"id\":\"dht11\",\"version\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":%s}"
#define ALINK_TOPIC_PROP_POST     "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"
 
unsigned long lastMs = 0;
float RH,T,RH_sum,T_sum;
unsigned char count=0;
WiFiClient espClient;
PubSubClient  client(espClient);
//SHTC3 mySHTC3;
DHT dht(1,DHT11);      //设置Data引脚所接IO口和传感器类型
 
 
void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    payload[length] = '\0';
    Serial.println((char *)payload);
 
}
 
void wifiInit()                  
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWD);   //连接WiFi
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("WiFi not Connect");
    }
    Serial.println("Connected to AP");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());  
    Serial.print("espClient [");
    client.setServer(MQTT_SERVER, MQTT_PORT);   /* 连接WiFi之后，连接MQTT服务器 */
    client.setCallback(callback);
}
 
 
void mqttCheckConnect()
{
    while (!client.connected())
    {
        Serial.println("Connecting to MQTT Server ...");
        if (client.connect(CLIENT_ID, MQTT_USRNAME, MQTT_PASSWD))
 
        {
 
            Serial.println("MQTT Connected!");
 
        }
        else
        {
            Serial.print("MQTT Connect err:");
            Serial.println(client.state());
            delay(5000);
        }
    }
}
 
 
void mqttIntervalPost()
{
    char param[32];
    char jsonBuf[128];
    sprintf(param, "{\"CurrentTemperature\":%f}",T_sum/count);
    sprintf(jsonBuf, ALINK_BODY_FORMAT, param);
    Serial.println(jsonBuf);
    boolean d = client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
    if(d){
      Serial.println("publish Temperature success"); 
    }else{
      Serial.println("publish Temperature fail"); 
    }
    //https://blog.zeruns.tech   
    sprintf(param, "{\"CurrentHumidity\":%f}",RH_sum/count);
    sprintf(jsonBuf, ALINK_BODY_FORMAT, param);
    Serial.println(jsonBuf);
    d = client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
    if(d){
      Serial.println("publish Humidity success"); 
    }else{
      Serial.println("publish Humidity fail"); 
    }
}
 
void errorDecoder(SHTC3_Status_TypeDef message) // The errorDecoder function prints "SHTC3_Status_TypeDef" resultsin a human-friendly way
{
  switch(message)
  {
    case SHTC3_Status_Nominal : Serial.print("Nominal"); break;
    case SHTC3_Status_Error : Serial.print("Error"); break;
    case SHTC3_Status_CRC_Fail : Serial.print("CRC Fail"); break;
    default : Serial.print("Unknown return code"); break;
  }
}
//https://blog.zeruns.tech
void setup() 
{
    /* initialize serial for debugging */
    Serial.begin(115200);
    dht.begin();   
    Serial.println("Demo Start");
    wifiInit();
    //Wire.begin(D5,D6);           //初始化Wire（IIC）库
    unsigned char i=0;
   // errorDecoder(mySHTC3.begin());
}
 
 
// the loop function runs over and over again forever
void loop()
{
  delay(1000);                  //延时1000毫秒

    float RH = dht.readHumidity();   //读取湿度数据
    float T = dht.readTemperature();//读取温度数据
    RH_sum+=RH;
    T_sum+=T;
    count+=1;                

  Serial.print("Humidity:");  //向串口打印 Humidity:
  Serial.print(RH);           //向串口打印湿度数据
  Serial.print("%");
  Serial.print("  Temperature:");
  Serial.print(T);            //向串口打印温度数据
  Serial.println("C"); 

  if (millis() - lastMs >= 1000)
  {
    lastMs = millis();
    mqttCheckConnect(); 
 
    /* 上报 */
    mqttIntervalPost();
    count=0;
    RH_sum=0;
    T_sum=0;
  }
  client.loop();
}

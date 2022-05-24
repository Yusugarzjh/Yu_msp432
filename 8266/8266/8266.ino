#include <ESP8266WiFi.h>
#include "PubSubClient.h"
#include <ArduinoJson.h>
#define WIFI_SSID         "MI10" 
#define WIFI_PASSWD       "123456789"
#define PRODUCT_KEY       "ha84B0y65Ko" 
#define DEVICE_NAME       "esp8266" 
#define DEVICE_SECRET     "2d3a383664b4896e70a7212d44820907"
#define REGION_ID         "cn-shanghai"
 

#define MQTT_SERVER       PRODUCT_KEY ".iot-as-mqtt." REGION_ID ".aliyuncs.com"
#define MQTT_PORT         1883
#define MQTT_USRNAME      DEVICE_NAME "&" PRODUCT_KEY
 
#define CLIENT_ID         "FESA234FBDS24|securemode=3,signmethod=hmacsha1,timestamp=789|" 

#define MQTT_PASSWD       "9a829f36748b1f0fef9707b32a9765f4b59dc6e1"  
 
#define ALINK_BODY_FORMAT         "{\"id\":\"dht11\",\"version\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":%s}"
#define ALINK_TOPIC_PROP_POST     "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"
 
unsigned long lastMs = 0;
float RH,T,RH_sum,T_sum;
unsigned char count=0;
WiFiClient espClient;
PubSubClient  client(espClient);

 
 
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
    WiFi.begin(WIFI_SSID, WIFI_PASSWD);  
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("WiFi not Connect");
    }
    Serial.println("Connected to AP");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());  
    Serial.print("espClient [");
    client.setServer(MQTT_SERVER, MQTT_PORT);   
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
void setup() 
{
    
    Serial.begin(115200);
    Serial.println("Demo Start");
    wifiInit();
    unsigned char i=0;
}
 
 
// the loop function runs over and over again forever
void loop()
{
  delay(1000);                  //延时1000毫秒

    float RH = dht.readHumidity();   
    float T = dht.readTemperature();
    RH_sum+=RH;
    T_sum+=T;
    count+=1;                
  if (millis() - lastMs >= 1000)
  {
    lastMs = millis();
    mqttCheckConnect(); 
    mqttIntervalPost();
    count=0;
    RH_sum=0;
    T_sum=0;
  }
  client.loop();
}
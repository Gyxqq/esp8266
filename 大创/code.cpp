#include <EEPROM.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Arduino.h>
#define region_num_now 6
#define steer_num_now 2
bool region_init(region *region0);                              // 初始化函数
void reconnect();                                               // 断线重连
void callback(char *topic, byte *payload, unsigned int length); // 消息处理
const char *ssid = "G";                                         // wifi名称
const char *password = "gyx200404";                             // wifi密码
const char *mqtt_server = "120.46.204.140";                     // mqtt服务器地址，如本地ip192.168.31.22
int steer_stats[steer_num_now] = {};                            // 0-90 表示百叶窗张开角度
struct region / 区域结构体
{
public:
  int region_id;          // 区域编号
  int region_light;       // 区域亮度
  int region_pin;         // 地区控制针脚
  int region_steer_pin;   // 地区舵机控制针脚
  int region_led_stats;   // 0-100 区域灯光亮度状态
  int region_steer_stats; // 地区百叶窗状态
  static int region_num;
};
/
    int region::region_num = region_num_now;
WiFiClient espClient;
PubSubClient client(espClient);
region region_now[region_num_now];
void setup()
{
  region_init();
  WiFi.begin(ssid, password);
  Serial.begin(9600);
  client.setServer(mqtt_server, 1883); // 连接mqtt
  client.setCallback(callback);        // 设置回调，控制led灯
  for (int i = 0; i < region_num_now; i++)
  {

    region_now[i].region_id = i;
    pinMode(region_now[i].region_pin, OUTPUT);
  }
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  Serial.print()
}
bool region_init(region *region0)
{ // 初始化函数
  region0[0].region_pin = 1;
  region0[1].region_pin = 2;
  region0[2].region_pin = 3;
  region0[3].region_pin = 4;
  region0[4].region_pin = 5;
  region0[5].region_pin = 6;
  region0[0].region_steer_pin = 7;
  region0[0].region_steer_pin = 7;
  region0[0].region_steer_pin = 7;
  region0[0].region_steer_pin = 7;
  region0[0].region_steer_pin = 8;
  region0[0].region_steer_pin = 8;
  for (int i = 0; i < region_num_now; i++)
  {
    ledcAttachPin(region_now[i].region_pin, i);
  }
}
void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266 Client1"))
    {
      Serial.println("connected");
      client.subscribe("test"); //
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void callback(char *topic, byte *payload, unsigned int length) // 消息处理
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  int light_now=0;
  int light_exp=0;
  int regio_id = 0;
  for (i = 0; i < length; i++)
  {
    if (topic[i] == '&')
    {
      while (topic[i] != '#')
      {
        region_id += regio_id * 10 + topic[i] - '0';
        i++;
      }
      i++;
       while (topic[i] != '#')
      {
       light_now+=light_now*10+topic[i]-'0';
        i++;
      }
      i++;
       while (topic[i] != '&')
      {
       light_exp+=light_exp*10+topic[i]-'0';
        i++;
      }
      i--;

    }
  }
}

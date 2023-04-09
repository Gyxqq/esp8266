#include <EEPROM.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Arduino.h>
#include <math.h>
#define region_num_now 6
#define steer_num_now 2
//#define steer_light 1
bool region_init(); // 初始化函数
void reconnect();
void set_steer(int angl, int channel);                          // 断线重连
void callback(char *topic, byte *payload, unsigned int length); // 消息处理
const char *ssid = "G";                                         // wifi名称
const char *password = "gyx200404";                             // wifi密码
const char *mqtt_server = "120.46.204.140";                     // mqtt服务器地址，如本地ip192.168.31.22
int steer_stats[steer_num_now] = {};
int steer_channel[steer_num_now] = {};                          // 0-90 表示百叶窗张开角度
struct region // 区域结构体
{
  public:
    int region_id;           // 区域编号
    int region_light;        // 区域亮度
    int region_light_exp;    // 区域期望亮度
    int region_pin;          // 地区控制针脚
    int region_steer_pin;    // 地区舵机控制针脚
    int region_steer_pin_id; // 针脚编号
    int region_led_stats;    // 0-255 区域灯光亮度状态
    int pwm_channel;         // pwm通道
    int region_steer_stats;  // 地区百叶窗状态
    static int region_num;
};

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
  // Serial.print("11");
}
bool region_init()
{ // 初始化函数
  region_now[0].region_pin = 13;
  region_now[1].region_pin = 12;
  region_now[2].region_pin = 14;
  region_now[3].region_pin = 27;
  region_now[4].region_pin = 26;
  region_now[5].region_pin = 25;
  region_now[0].region_steer_pin = 15;
  region_now[1].region_steer_pin = 15;
  region_now[2].region_steer_pin = 15;
  region_now[3].region_steer_pin = 15;
  region_now[4].region_steer_pin = 2;
  region_now[5].region_steer_pin = 2;
  region_now[0].region_steer_pin_id = 0;
  region_now[1].region_steer_pin_id = 0;
  region_now[2].region_steer_pin_id = 0;
  region_now[3].region_steer_pin_id = 0;
  region_now[4].region_steer_pin_id = 1;
  region_now[5].region_steer_pin_id = 1;
  for (int i = 0; i < region_num_now; i++)
  {
    ledcAttachPin(region_now[i].region_pin, i);
    ledcSetup(i, 5000, 8); // 设置pwm频道和分辨率
    region_now[i].pwm_channel = i;
  }
  steer_channel[0] = 6;
  steer_channel[1] = 7;
  ledcAttachPin(15, 6);
  ledcSetup(6, 50, 12);
  ledcAttachPin(2, 7);
  ledcSetup(7, 50, 12); // 舵机的pwm设置
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
  int light_now = 0;
  int light_exp = 0;
  int region_id = 0;
  Serial.print("gyx\n");
  Serial.println(length);
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  for (int i = 0; i < length; i++)
  {

    if (payload[i] == '&' && payload[i + 1] == '\"')break;
    if (payload[i] == '&')
    {
      i++;
      while (payload[i] != '#')
      {
          Serial.print((char)payload[i]);
        Serial.print((char)payload[i]);
        region_id += region_id * 10 + payload[i] - '0';
        i++;
      }
      i++;
      while (payload[i] != '#')
      {
        Serial.print((char)payload[i]);
        light_now += light_now * 10 + payload[i] - '0';
        i++;
      }
      i++;
      Serial.print("\nid=");
      Serial.println(region_id);
      while (payload[i] != '&')
      {
        Serial.print((char)payload[i]);
        light_exp += light_exp * 10 + payload[i] - '0';
        i++;
      }
      i--;

      region_now[region_id].region_light = light_now;

      region_now[region_id].region_light_exp = light_exp;
//      region_id=0;
//      light_now=0;
//      light_exp=0;
    }
  } // 消息处理



  struct steer_light0
  {
    int num = 0;
    int steer_light = 0;
  } steer_light[steer_num_now];
  Serial.print("gyx\n");
  for (int i = 0; i < region_num_now; i++)
  {
    steer_light[region_now[i].region_steer_pin_id].steer_light += region_now[i].region_light_exp;
    steer_light[region_now[i].region_steer_pin_id].num++;
  }
  for (int i = 0; i < steer_num_now; i++)
  {
    steer_light[i].steer_light /= steer_light[i].num;
    set_steer(steer_light[i].steer_light, steer_channel[i]);
  } // 计算舵机的角度
  for (int i = 0; i < region_num_now; i++)
  {
    if (abs(region_now[i].region_light - region_now[i].region_light_exp) < 20)
      continue;
    if (region_now[i].region_light - region_now[i].region_light_exp > 0)
    {
      if (region_now[i].region_led_stats >= 0 && region_now[i].region_led_stats <= 255)
      {
        region_now[i].region_led_stats--;
        ledcWrite(region_now[i].pwm_channel, region_now[i].region_led_stats);
      }
    }
    if (region_now[i].region_light - region_now[i].region_light_exp < 0)
    {
      if (region_now[i].region_led_stats >= 0 && region_now[i].region_led_stats <= 255)
      {
        region_now[i].region_led_stats++;
        ledcWrite(region_now[i].pwm_channel, region_now[i].region_led_stats);
      }
    }
  } // led亮度调整
}

void set_steer(int angl, int channel)
{

  double a = angl * 0.9;
  a = 100 + a * 200 / 90;
  int c = a;
  ledcWrite(channel, c);

}
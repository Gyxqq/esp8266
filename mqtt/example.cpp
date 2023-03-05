// mqtt + esp8266
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>


const char* ssid = "G";  // wifi名称
const char* password = "gyx200404"; // wifi密码
const char* mqtt_server = "120.46.204.140";  // mqtt服务器地址，如本地ip192.168.31.22

WiFiClient espClient;
PubSubClient client(espClient);

const byte ledPin = D4; // 需要控制的led灯

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++) {
    char receivedChar = (char)payload[i];
    Serial.print(receivedChar); // 打印mqtt接收到消息

    if (receivedChar == '1') {  // 收到消息是 '1' 点亮LED灯
      digitalWrite(ledPin, HIGH);
    }
    if (receivedChar == '0')    // 收到消息是 '0' 关闭LED灯
      digitalWrite(ledPin, LOW);
  }
  Serial.println();
}


void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266 Client1")) {
      Serial.println("connected");
      client.subscribe("test");  // 
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup()
{
  WiFi.begin(ssid,password);
  Serial.begin(9600);

  client.setServer(mqtt_server, 1883); // 连接mqtt
  client.setCallback(callback);        // 设置回调，控制led灯

  pinMode(ledPin, OUTPUT);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

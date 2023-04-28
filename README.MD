# 一些arduino函数:
    delayMicroseconds(int i) //微秒级延时
    delay(int i) //毫秒级延迟
    digitalWrite(int pin,LOW/HIGH);//设置端口高低电平
    pinMode(int pin,INPUT/OUTPUT);//设置GPIO读或者写
     ledcAttachPin(GPIO, channel);      //输出pwm信号的GPIO，产生信号的通道。(对于esp32，通道在0-16之间)  
    ledcWrite(channel, dutycycle);       // PWM信号的通道和占空比   
    ledcSetup(Channel, freq, resolution);//设置pwm频道的pwm波频率和分辨率
    resolution在1-16之间，表示分辨率为2^resolution 使用0-2^resolution调节亮度
    对于控制led灯freq一般在5000

## 串口相关：
    Serial.begin(int i) //设置串行通信速率
    Serial.print(char* a) //串行输出
    Serial.println(char* a)  // 同上，多换行
    Serial.available() //返回缓冲区数据长度 
    Serial.read() //读取一个字符  
    Serial.readBytes(char* buffer, int length)//读取length字节放到buffer
## WIFI 相关：<ESP8266WiFi.h>
    WiFi.begin(char* ssid,char* passwd);//连接WiFi
    
    
##  mqtt相关：[官方文档](https://pubsubclient.knolleary.net/api)
    类 PubSubClient(WiFiClient client )//创建一个pubsubclient对象，调用构造函数
    client.setServer(char* server,int port);//设置mqtt服务器
    bool client.connect(char* clientid,....);//用clientid连接服务器
    void client.disconnnect()//断开连接
    bool client.publish(const char[] topic,const char[] payload);//发布对应主题的消息
    bool client.subscribe(char* topic,[int qos]);//订阅某个主题
    bool unsubscribe(char* topic);
    bool client.loop()//定期调用以确保 与服务器的连接和允许客户端处理传入的消息
    bool client.connected()//检查是否与服务器连接
    PubSubclient* client.setcallback(function* callback);//设置回调函数，接受到消息会调用该函数
    void callback(const char[] topic, byte* payload, unsigned int length)//回调函数
    
## ESP8266 SPI踩坑
[链接](https://www.jianshu.com/p/c1d316ef6e8f)
esp8266的软件SPI的端口不能乱接，比如：
- GPIO2不能用作SDA
- GPIO0不能使用
- 丝印和实际GPIO不同  
贴个我自己的接线
```CPP
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS   5  // Chip select control pin
#define TFT_DC    4  // Data Command control pin
#define TFT_RST   12

```


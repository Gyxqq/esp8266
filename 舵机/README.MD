以20ms为周期的脉冲，其中高电平占比在0.6-2.5ms之间，控制舵机正负90°旋转。
    一些函数:
    delayMicroseconds(int i) //微秒级延时
    delay(int i) //毫秒级延迟
		
    Serial.begin(int i) //设置串行通信速率
    Serial.print() //串行输出
    Serial.println()  // 同上，多换行
    Serial.available() //返回缓冲区数据长度 
    Serial.read() //读取一个字符  
    Serial.readBytes(char* buffer, int length)//读取length字节放到buffer

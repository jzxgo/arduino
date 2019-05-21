#include <MsTimer2.h>
#include <SPI.h>
#include <Ethernet.h>
#include <MsTimer2.h>

volatile int body;
EthernetClient client;                  //声明点击变量
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};      //初始化以太网
void msTimer2_func() {                //定时器函数
  Serial.println("发送数据");
  //写入字符数据+换行到串口

  Serial.println(String("人体红外") + String(body));
 //在串口监视器中，分别打印出四个数值
  client.print("{\"M\":\"update\",\"ID\":\"10263\",\"V\":{\"10184\":\"");   //发送数据到自己智能设备的ID中，温度ID
  client.print("\",\"10184\":\"");
  client.print(body);
  
  client.print("\"}}\n");
}


void setup(){

  body = 0;
                  //初始化声明的变量
  Serial.begin(9600);               //设置波特率
  if (Ethernet.begin(mac)) {        //如果连接到以太网
    Serial.println(Ethernet.localIP());//打印出本地的ip

  }
  delay(5000);                    //延时5秒
  while (!client.connect("www.bigiot.net",8181)) {        //若是未连接到服务器，网址和端口
    Serial.println("未连接到服务器");
  }
  if (client.connected()) {
    Serial.println("已连接服务器");

  }
  delay(5000);
  if (client.available()) {           //如果客户端有数据可读
    while (client.available()) {          //持续读取数据，直到读取完
      Serial.print((char)client.read());
    }

  }
  client.print("{\"M\":\"checkin\",\"ID\":\"10263\",\"K\":\"f4103106c\"}\n");      //连接到自己API，ID
  delay(5000);
  if (client.available()) {
    while (client.available()) {
      Serial.print((char)client.read());
    }

  }
  MsTimer2::set(10000, msTimer2_func);    //定时函数
  MsTimer2::start();                      //定时启动
  pinMode(7, INPUT);                      //七号引脚定义为输入
}

void loop(){
 
  body = digitalRead(7); //获取传感器的值
  delay(5000);

}

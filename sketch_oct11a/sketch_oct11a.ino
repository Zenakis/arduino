#include "HX711.h"
#include <SPI.h>
#include <Ethernet.h>
#include "PubSubClient.h"

HX711 HX711_CH0(2, 3, 750); //SCK,DT,GapValue
//SCK引腳用於arduino和HX711模塊通訊的時序提供
//DT引腳用於從HX711讀取AD的數據
//GapValue用於校準輸出的重量值，如果數值偏大就加大該值，如果數據偏小就減小該值

int LED = 13;   //ARDUINO LED燈，亮起時代表初始化完成可以負重
long Weight = 0;    //定義一個變量用於存放承重的重量，單位為g

// Update these with values suitable for your network.
byte mac[] = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(172, 16, 0, 100);
const char* server = "192.168.1.101";

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

void setup()
{
  pinMode(LED, OUTPUT);       //設定LED是輸出引腳
  digitalWrite(LED, LOW);     //LED熄滅

  Serial.begin(9600);           //設定串口輸出波特率

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for (;;)
      ;
  }

  printIPAddress();

    // Allow the hardware to sort itself out
  delay(1500);
  mqttClient.setServer(server, 1883);
  
  if (mqttClient.connect("Arduino")) {
    // connection succeeded
    Serial.println("Succeeded");
  } else {
    // connection failed
    mqttClient.state();
    // will provide more information
    // on why it failed.
    Serial.println("Failed");
  }
  

  HX711_CH0.begin();          //讀取傳感器重量
  delay(3000);                //延時3s用於傳感器穩定
  HX711_CH0.begin();          //重新讀取傳感器支架毛重用於後續計算

  digitalWrite(LED, HIGH);    //板載LED點亮，說明可以承重
}

void loop()
{
    mqttClient.loop();
    Weight = HX711_CH0.Get_Weight();    //當前傳感器重量，該重量已經扣除支架重量
    Serial.print(Weight);     //串口輸出當前重量
    Serial.println(" g");     //單位為g
    char str[8];
    itoa( Weight, str, 10);
    mqttClient.publish("milvus/test",str);
    
    delay(1000);  

}

void printIPAddress()
{
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }

  Serial.println();
}

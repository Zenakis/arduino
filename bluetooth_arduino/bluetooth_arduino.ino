#include <TM1637Display.h>
#include <SoftwareSerial.h>

SoftwareSerial BT(8, 9); // 接收, 传送，程序中8为RX需要接hc-05的TXD,9为TX，需要接hc-05的RX。
String result;

const int CLK = 7; //Set the CLK pin connection to the display
const int DIO = 6; //Set the DIO pin connection to the display
int NumStep = 0;  //Variable to interate
TM1637Display display(CLK, DIO);  //set up the 4-Digit Display.

void setup() {
  Serial.begin(9600); 
  Serial.println("BT is ready!");
  pinMode(13, OUTPUT);  
  digitalWrite(13,HIGH);
  display.setBrightness(0x0a);  //set the diplay to maximum brightness
  
  BT.begin(9600);
}
void loop() {

//  for(NumStep = 0; NumStep < 9999; NumStep++)  //Interrate NumStep
//  {
//    display.showNumberDec(NumStep); //Display the Variable value;
//    delay(500);  //A half second delay between steps.
//  }

//接收PC端串口信息，如果收到就通过蓝牙发送出去
  if (Serial.available()) {
    result = Serial.readString();
    BT.println(result);
  }
//接收蓝牙无线信息，如果收到就通过串口发送到PC
  if (BT.available()) {
    result += BT.readString();
    Serial.println(result);
  } 

  if(result.length() > 0){ 
    if(result == "open"){
      digitalWrite(13,LOW);
    }if(result == "close"){
      digitalWrite(13,HIGH);
    }

    if(result.toInt()){
      display.showNumberDec(result.toInt());  
    }else{
      Serial.println("Not Number!");
    }
    
    result = "";
  }
}

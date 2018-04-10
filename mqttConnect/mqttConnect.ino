#include <SPI.h>
#include <Ethernet.h>
#include "PubSubClient.h"

// Update these with values suitable for your network.
byte mac[] = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(172, 16, 0, 100);
const char* server = "192.168.1.109";

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

void setup()
{
  Serial.begin(9600);

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // this check is only needed on the Leonardo:
  
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
  // print your local IP address:
  printIPAddress();
  
//  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
  mqttClient.setServer(server, 1883);
  
  if (mqttClient.connect("Arduino")) {
    // connection succeeded
    Serial.print("Succeeded");
  } else {
    // connection failed
    mqttClient.state();
    // will provide more information
    // on why it failed.
    Serial.print("Failed");
  }
}

void loop()
{
  mqttClient.loop();
  mqttClient.publish("arduino","hello world");
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

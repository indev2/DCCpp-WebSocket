//*** IMPORTANT *** Most of the libraries used are included in the ESP8266 Core package which you must install to the Arduino IDE for this sketch to compile.
//Please use the Git method here https://github.com/esp8266/Arduino and NOT the File > Preferences > Additional Boards Manager URL.
//WebSocketsServer.h appears to be broken using the later. And DO include the Python requirement (correct at Feb 15 2016) The author is aware of this issue.
//You will need to install Git to simplify that operation.
//If the sketch compiles OK but won't upload properly, then go Sketch > Export compiled Binary. A .bin file will be created in this sketch's folder.
//Then upload the binary with your favorite flashing tool.

//Please remember to fill in your home network SSID and PASSWORD. In this version when you reboot your esp8266 it will automatically log on to your WLAN.
//Make a note of the IP address assigned by your router by logging on to its home page. Your PC or Mobile also need to be on this same network. 
//With the "Webpage" open on your chosen device, type the IP and Port in the following format eg. 192.168.11.55:81 (port == @line 28) press Enter(desktop) or tap the screen outside the box (mobile).
//With luck you are now connected enjoy. A message will confirm in later versions.
//Console Debugging is available in Chrome windows desktop for the html wireless side. And at the other end (Arduino) a serial monitor will let you know whats happening there.
//You can also see serial out from esp8266 if you connect by usb and use a serial monitor there. (Debug from this sketch shows up there)

//In tests I hooked up 2 seperate clients at the same time so multi-client is confirmed. However as certain values are fixed assignment things start to go wrong quickly. We can work that out ;)
//There is room for many more functions on the page, this is just Proof of Concept.

//Special thanks to Rahul 27 who created the original sketch on which this sketch is based. And also Markus Sattler for the WebSocketsServer library.

//This software is Free. Enjoy. Steve Lowry(indev2)2016. 

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <EEPROM.h>
 
WebSocketsServer webSocket = WebSocketsServer(81);
int xxx;
String c;
String d;
int cInt;
int dInt;
const char* ssid     = "Your SSID";
const char* password = "Your Password";
//const int redPin = 15;  //Debug LED function not needed ***All debug lines can be deleted once you understand the code and you have it working, we like tidy code ;)***
//const int greenPin = 12; //Debug LED function not needed
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

  switch (type) {
    case WStype_DISCONNECTED:

      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);


      }
      break;
    case WStype_TEXT:
      {

        String text = String((char *) &payload[0]);
        if (text == "ON") {

          //digitalWrite(12, HIGH);//Visual Debug - Green LED will come on at Power ON
          //digitalWrite(15, LOW);//Debug - not needed
          Serial.println("<1>");
          int dvalue = EEPROM.read(0);
          int cvalue = EEPROM.read(8)*256;
          cvalue = cvalue + EEPROM.read(9);
          int xInt = EEPROM.read(2);
          Serial.print("<t1 ");
          Serial.print(cvalue);
          Serial.print(" ");
          Serial.print(xInt);
          Serial.print(" ");
          Serial.print(dvalue);
          Serial.println(">");
          dvalue = EEPROM.read(4);
          cvalue = EEPROM.read(12)*256;
          cvalue = cvalue + EEPROM.read(13);
          int yInt = EEPROM.read(5);
          Serial.print("<t2 ");
          Serial.print(cvalue);
          Serial.print(" ");
          Serial.print(yInt);
          Serial.print(" ");
          Serial.print(dvalue);
          Serial.println(">");

        }

        if (text == "OFF") {

          Serial.println("<0>");          
          
        }

        if (text == "STOP1") {

          int dvalue = EEPROM.read(0);
          int cvalue = EEPROM.read(8)*256;
          cvalue = cvalue + EEPROM.read(9);
          Serial.print("<t1 ");
          Serial.print(cvalue);
          Serial.print(" -1 ");
          Serial.print(dvalue);
          Serial.println(">");
          
        }

        if (text == "STOP2") {

          int dvalue = EEPROM.read(4);
          int cvalue = EEPROM.read(12)*256;
          cvalue = cvalue + EEPROM.read(13);
          Serial.print("<t2 ");
          Serial.print(cvalue);
          Serial.print(" -1 ");
          Serial.print(dvalue);
          Serial.println(">");
          
        }
        
        if (text.startsWith("CAB1")) {

          String c = (text.substring(text.indexOf("CAB1") + 4, text.length()));
          int cInt = c.toInt();
          xxx = cInt /256;
          EEPROM.write (8, xxx);
          xxx = cInt - (xxx * 256);
          EEPROM.write (9, xxx);
          EEPROM.commit();
          delay (10);
          //Serial.print("Cab# Entered t1 ");//Debug
          //Serial.println(cInt);//Debug

        }

        if (text.startsWith("CAB2")) {

          String c = (text.substring(text.indexOf("CAB2") + 4, text.length()));
          int cInt = c.toInt();
          xxx = cInt /256;
          EEPROM.write (12, xxx);
          xxx = cInt - (xxx * 256);
          EEPROM.write (13, xxx);
          EEPROM.commit();
          delay (10);
          //Serial.print("Cab# Entered t2 ");//Debug
          //Serial.println(cInt);//Debug

        }

        if (text == "FOR") {
          
          //dInt = (1);  
          EEPROM.write (0, 1);
          EEPROM.commit();
          int Cab1 = EEPROM.read(8)*256;
          Cab1 = Cab1 + EEPROM.read(9);
          int xInt = EEPROM.read(2);
          //Serial.print("Forward Entered t1 ");//Debug
          //Serial.println(dInt);//Debug
          Serial.print("<t1 ");
          Serial.print(Cab1);
          Serial.print(" ");
          Serial.print(xInt);
          Serial.println(" 1>"); 
          delay (10);

        }

        if (text == "FOR2") {

          //dInt = (1);
          EEPROM.write (4, 1);
          EEPROM.commit();
          int Cab2 = EEPROM.read(12)*256;
          Cab2 = Cab2 + EEPROM.read(13);
          int yInt = EEPROM.read(5);
          //Serial.print("Forward Entered t2 ");//Debug
          //Serial.println(dInt);//Debug
          Serial.print("<t2 ");
          Serial.print(Cab2);
          Serial.print(" ");
          Serial.print(yInt);
          Serial.println(" 1>"); 
          delay (10);

        }

        if (text == "REV") {

          //dInt = (0);
          EEPROM.write (0, 0);
          EEPROM.commit();
          int Cab1 = EEPROM.read(8)*256;
          Cab1 = Cab1 + EEPROM.read(9);
          int xInt = EEPROM.read(2);
          //Serial.print("Reverse Entered t1 ");//Debug
          //Serial.println(dInt);//Debug
          Serial.print("<t1 ");
          Serial.print(Cab1);
          Serial.print(" ");
          Serial.print(xInt);
          Serial.println(" 0>");
          delay (10);


        }

        if (text == "REV2") {

          //dInt = (0);
          EEPROM.write (4, 0);
          EEPROM.commit();
          int Cab2 = EEPROM.read(12)*256;
          Cab2 = Cab2 + EEPROM.read(13);
          int yInt = EEPROM.read(5);
          //Serial.print("Reverse Entered t2 ");//Debug
          //Serial.println(dInt);//Debug
          Serial.print("<t2 ");
          Serial.print(Cab2);
          Serial.print(" ");
          Serial.print(yInt);
          Serial.println(" 0>");
          delay (10);


        }

        if (text.startsWith("x")) {

          String xVal = (text.substring(text.indexOf("x") + 1, text.length()));
          int xInt = xVal.toInt();
          //analogWrite(greenPin, xInt); //Debug - not needed, Green "ON" LED will brighten/dim according to throttle up/down
          int dvalue = EEPROM.read(0);
          int cvalue = EEPROM.read(8)*256;
          cvalue = cvalue + EEPROM.read(9);
          Serial.print("<t1 ");
          Serial.print(cvalue);
          Serial.print(" ");
          Serial.print(xInt);
          Serial.print(" ");
          Serial.print(dvalue);
          Serial.println(">");
          EEPROM.write(2,xInt);
          EEPROM.commit();

        }

        if (text.startsWith("y")) {

          String yVal = (text.substring(text.indexOf("y") + 1, text.length()));
          int yInt = yVal.toInt();
          int dvalue = EEPROM.read(4);
          int cvalue = EEPROM.read(12)*256;
          cvalue = cvalue + EEPROM.read(13);
          Serial.print("<t2 ");
          Serial.print(cvalue);
          Serial.print(" ");
          Serial.print(yInt);
          Serial.print(" ");
          Serial.print(dvalue);
          Serial.println(">");
          EEPROM.write(5,yInt);
          EEPROM.commit();

        }

        if (text.startsWith("FnT1")) {

          String FnVal = (text.substring(text.indexOf("FnT1") + 4, text.length()));
          int FnInt = FnVal.toInt();
          int cvalue = EEPROM.read(8)*256;
          cvalue = cvalue + EEPROM.read(9);
          Serial.print("<f ");
          Serial.print(cvalue);
          Serial.print(" ");
          Serial.print(FnInt);
          Serial.println(">");
                    
        }

        if (text.startsWith("FnT2")) {

          String FnVal = (text.substring(text.indexOf("FnT2") + 4, text.length()));
          int FnInt = FnVal.toInt();
          int cvalue = EEPROM.read(12)*256;
          cvalue = cvalue + EEPROM.read(13);
          Serial.print("<f ");
          Serial.print(cvalue);
          Serial.print(" ");
          Serial.print(FnInt);
          Serial.println(">");
                    
        }

        if (text == "EXIT") {

          EEPROM.write (0, 1);
          EEPROM.write (2, 0);
          EEPROM.write (4, 1);
          EEPROM.write (5, 0);
          EEPROM.write (8, 0);
          EEPROM.write (9, 0);
          EEPROM.write (12, 0);
          EEPROM.write (13, 0);
          EEPROM.commit (); //All stored values reset
          Serial.println("<0>");
          delay (20);
          //digitalWrite(15, HIGH);//Visual Debug - not needed - Red LED will come on at Power OFF
          //analogWrite(greenPin, 0); //LED function - not needed          

        }
        
        break;

      }

      webSocket.sendTXT(num, payload, lenght);
      webSocket.broadcastTXT(payload, lenght);
      break;

    case WStype_BIN:

      hexdump(payload, lenght);

      // echo data back to browser
      webSocket.sendBIN(num, payload, lenght);
      break;
  }

}


void setup() {

  Serial.begin(115200);
  pinMode(15, OUTPUT);
  pinMode(12, OUTPUT);
  EEPROM.begin(512);
  EEPROM.write(0,1);
  EEPROM.write(4,1);
  EEPROM.commit();
  delay (20);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  Serial.println(WiFi.localIP());
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
}

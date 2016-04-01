//The MIT License (MIT)

//Copyright (c) 2016, Steve Lowry

//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

//*** IMPORTANT *** Most of the libraries used are included in the ESP8266 Core package which you must install to the Arduino IDE for this sketch to compile.
//Please use the Git method here https://github.com/esp8266/Arduino and NOT the File > Preferences > Additional Boards Manager URL.
//WebSocketsServer.h appears to be broken using the later. And DO include the Python requirement (correct at Feb 15 2016) The author is aware of this issue.
//You will need to install Git to simplify that operation.
//If the sketch compiles OK but won't upload properly, then go Sketch > Export compiled Binary. A .bin file will be created in this sketch's folder.
//Then upload the binary with your favorite flashing tool.

//Please remember to fill in your local network SSID and PASSWORD. In this version when you reboot your esp8266 it will automatically log on to your WLAN.
//Make a note of the IP address assigned by your router by logging on to its home page. Your PC or Mobile also need to be on this same network.
//With the "Webpage" open on your chosen device, type the IP and Port into the field in the following format eg. ws://192.168.11.55:81 (port is as defined in line 55) click or tap the Connect button.
//With luck you are now connected enjoy. A message will confirm in the serial display to the right of the input field.
//Console Debugging is available in Chrome windows desktop for the html wireless side. At the other end (ESP8266 & Arduino) the on-screen serial display will let you know whats happening there.

//In tests I hooked up 2 seperate clients at the same time so multi-client is confirmed. However as certain values are designed for a single client assignment, things start to go wrong quickly.

//Thanks to Rahul 27 for the original inspiration and some coding examples used this project. Markus Sattler for the WebSocketsServer library and other coding examples.
//Also thanks to https://github.com/tzapu for the serial monitor coding, partly used and modified for this project.

//This software is Free and still a Work in Progress. Enjoy. Steve Lowry(indev2)2016.

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <Hash.h>
//#include <WiFiManager.h>
#include <EEPROM.h>

//#define MAX_SRV_CLIENTS 1

//WiFiServer server(80);
//WiFiClient serverClients[MAX_SRV_CLIENTS];

WebSocketsServer webSocket = WebSocketsServer(81);

int xxx;
String c;
String d;
int cInt;
int dInt;
const char* ssid     = "Your SSID";
const char* password = "Your Password";

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

  switch (type) {
    case WStype_DISCONNECTED:

      break;
    case WStype_CONNECTED: {

        IPAddress ip = webSocket.remoteIP(num);
        webSocket.sendTXT(num, "Welcome, Server Ready");
      }
      break;
    case WStype_TEXT: {

        String text = String((char *) &payload[0]);
        if (text == "ON") {

          Serial.println("<1>");
          int dvalue = EEPROM.read(0);
          int cvalue = EEPROM.read(8) * 256;
          cvalue = cvalue + EEPROM.read(9);
          int aInt = EEPROM.read(2);
          Serial.print("<t1 ");
          Serial.print(cvalue);
          Serial.print(" ");
          Serial.print(aInt);
          Serial.print(" ");
          Serial.print(dvalue);
          Serial.println(">");
          dvalue = EEPROM.read(4);
          cvalue = EEPROM.read(12) * 256;
          cvalue = cvalue + EEPROM.read(13);
          int bInt = EEPROM.read(5);
          Serial.print("<t2 ");
          Serial.print(cvalue);
          Serial.print(" ");
          Serial.print(bInt);
          Serial.print(" ");
          Serial.print(dvalue);
          Serial.println(">");
          delay(10);
          webSocket.broadcastTXT("Power ON.");

        }
        if (text == "OFF") {

          Serial.println("<0>");
          delay(10);
          webSocket.broadcastTXT("Power OFF.");

        }
        if (text == "S") {

          Serial.println("<s>");
          delay(10);
          
        }
        if (text == "STOP1") {

          int dvalue = EEPROM.read(0);
          int cvalue = EEPROM.read(8) * 256;
          cvalue = cvalue + EEPROM.read(9);
          Serial.print("<t1 ");
          Serial.print(cvalue);
          Serial.print(" -1 ");
          Serial.print(dvalue);
          Serial.println(">");
          EEPROM.write(2,0);
          EEPROM.commit();
          delay(10);
          webSocket.broadcastTXT("Emergency Stop T1.");

        }
        if (text == "STOP2") {

          int dvalue = EEPROM.read(4);
          int cvalue = EEPROM.read(12) * 256;
          cvalue = cvalue + EEPROM.read(13);
          Serial.print("<t2 ");
          Serial.print(cvalue);
          Serial.print(" -1 ");
          Serial.print(dvalue);
          Serial.println(">");
          EEPROM.write(5,0);
          EEPROM.commit();
          delay(10);
          webSocket.broadcastTXT("Emergecy Stop T2.");

        }
        if (text.startsWith("CAB1")) {

          String c = (text.substring(text.indexOf("CAB1") + 5, text.length()));
          int cInt = c.toInt();
          xxx = cInt / 256;
          EEPROM.write (8, xxx);
          xxx = cInt - (xxx * 256);
          EEPROM.write (9, xxx);
          EEPROM.commit();
          delay(10);
          webSocket.broadcastTXT("New Cab# T1.");

        }
        if (text.startsWith("CAB2")) {

          String c = (text.substring(text.indexOf("CAB2") + 5, text.length()));
          int cInt = c.toInt();
          xxx = cInt / 256;
          EEPROM.write (12, xxx);
          xxx = cInt - (xxx * 256);
          EEPROM.write (13, xxx);
          EEPROM.commit();
          delay(10);
          webSocket.broadcastTXT("New Cab# T2.");

        }
        if (text == "FOR") {

          EEPROM.write (0, 1);
          EEPROM.commit();
          int Cab1 = EEPROM.read(8) * 256;
          Cab1 = Cab1 + EEPROM.read(9);
          int aInt = EEPROM.read(2);
          Serial.print("<t1 ");
          Serial.print(Cab1);
          Serial.print(" ");
          Serial.print(aInt);
          Serial.println(" 1>");
          delay(10);
          webSocket.broadcastTXT("Forward T1.");

        }
        if (text == "FOR2") {

          EEPROM.write (4, 1);
          EEPROM.commit();
          int Cab2 = EEPROM.read(12) * 256;
          Cab2 = Cab2 + EEPROM.read(13);
          int bInt = EEPROM.read(5);
          Serial.print("<t2 ");
          Serial.print(Cab2);
          Serial.print(" ");
          Serial.print(bInt);
          Serial.println(" 1>");
          delay (10);
          webSocket.broadcastTXT("Forward T2.");

        }
        if (text == "REV") {

          EEPROM.write (0, 0);
          EEPROM.commit();
          int Cab1 = EEPROM.read(8) * 256;
          Cab1 = Cab1 + EEPROM.read(9);
          int aInt = EEPROM.read(2);
          Serial.print("<t1 ");
          Serial.print(Cab1);
          Serial.print(" ");
          Serial.print(aInt);
          Serial.println(" 0>");
          delay (10);
          webSocket.broadcastTXT("Reverse T1.");


        }
        if (text == "REV2") {

          EEPROM.write (4, 0);
          EEPROM.commit();
          int Cab2 = EEPROM.read(12) * 256;
          Cab2 = Cab2 + EEPROM.read(13);
          int bInt = EEPROM.read(5);
          Serial.print("<t2 ");
          Serial.print(Cab2);
          Serial.print(" ");
          Serial.print(bInt);
          Serial.println(" 0>");
          delay (10);
          webSocket.broadcastTXT("Reverse T2.");


        }
        if (text.startsWith("T1")) {

          String aVal = (text.substring(text.indexOf("T1") + 3, text.length()));
          int aInt = aVal.toInt();
          int dvalue = EEPROM.read(0);
          int cvalue = EEPROM.read(8) * 256;
          cvalue = cvalue + EEPROM.read(9);
          Serial.print("<t1 ");
          Serial.print(cvalue);
          Serial.print(" ");
          Serial.print(aInt);
          Serial.print(" ");
          Serial.print(dvalue);
          Serial.println(">");
          EEPROM.write(2, aInt);
          EEPROM.commit();

        }
        if (text.startsWith("T2")) {

          String bVal = (text.substring(text.indexOf("T2") + 3, text.length()));
          int bInt = bVal.toInt();
          int dvalue = EEPROM.read(4);
          int cvalue = EEPROM.read(12) * 256;
          cvalue = cvalue + EEPROM.read(13);
          Serial.print("<t2 ");
          Serial.print(cvalue);
          Serial.print(" ");
          Serial.print(bInt);
          Serial.print(" ");
          Serial.print(dvalue);
          Serial.println(">");
          EEPROM.write(5, bInt);
          EEPROM.commit();

        }
        if (text.startsWith("FnT1")) {

          String FnVal = (text.substring(text.indexOf("FnT1") + 4, text.length()));
          int cvalue = EEPROM.read(8) * 256;
          cvalue = cvalue + EEPROM.read(9);
          Serial.print("<f ");
          Serial.print(cvalue);
          Serial.print(" ");
          Serial.print(FnVal);
          Serial.println(">");
          webSocket.broadcastTXT("Fn T1.");

        }
        if (text.startsWith("FnT2")) {

          String FnVal = (text.substring(text.indexOf("FnT2") + 4, text.length()));
          int cvalue = EEPROM.read(12) * 256;
          cvalue = cvalue + EEPROM.read(13);
          Serial.print("<f ");
          Serial.print(cvalue);
          Serial.print(" ");
          Serial.print(FnVal);
          Serial.println(">");
          webSocket.broadcastTXT("Fn T2.");

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
          EEPROM.commit (); //All stored values reset default
          Serial.println("<0>");
          delay (20);
          webSocket.broadcastTXT("System RESET default");

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

String inputString = "";         
boolean stringComplete = false;  

void setup() {

  Serial.begin(115200);
  EEPROM.begin(128);
  EEPROM.write(0, 1);
  EEPROM.write(4, 1);
  EEPROM.write(8, 0);
  EEPROM.write(9, 0);
  EEPROM.write(12, 0);
  EEPROM.write(13, 0);
  EEPROM.commit();
  delay (20);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  Serial.println(WiFi.localIP());
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  inputString.reserve(256);
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '>') {
      inputString += inChar;
      //inputString += '\n';
      stringComplete = true;
      return;
    } else {
      inputString += inChar;
    }
  }
}

void loop() {
  serialEvent();
  if (stringComplete) {

    String data = inputString;    
    inputString = "";
    stringComplete = false;
    webSocket.broadcastTXT(data);
    
    
  }
  webSocket.loop();
}

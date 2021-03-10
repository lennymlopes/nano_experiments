#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include "qrcode.h"
#include <WiFi.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>

// board: esp32 dev module, 

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

// 3 x 3 pixel cells, scaling the qr code by 3x
#define GRIDX 80
#define GRIDY 45
#define CELLXY 3

// user setup 
const char* ssid     = "your_ssid";
const char* password = "your_password";
#define ADDRESS "nano_yournanoaddress"

//-- init websocket ------------------
WebSocketsClient webSocket;

//-- json stuff ----------------------
StaticJsonDocument<200> doc;
StaticJsonDocument<1024> rx_doc;

//-- init qr code --------------------
QRCode qrcode;

void setup()   {
//-- display setup -------------------
  tft.init();
  tft.setRotation(24);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);

//-- serial setup --------------------
  Serial.begin(9600);
    
//-- wifi setup -----------------------
  tft.setCursor(5, 5);
  tft.setTextSize(2);
  tft.println(F("connecting to wifi"));
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    tft.setCursor(5, 5);
    tft.println(F("connecting to wifi.")); 
    delay(500);
    
    tft.setCursor(5, 5);
    tft.fillScreen(TFT_BLACK);
    tft.println(F("connecting to wifi ")); 
    delay(500);
  }

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(5, 5);
  tft.println(F("connected"));

//-- websocket setup ----------------

  // server address, port and URL
  webSocket.begin("yapraiwallet.space", 80, "/call");

  // event handler
  webSocket.onEvent(webSocketEvent);

  // retry every 5s if failed
  webSocket.setReconnectInterval(5000);
}



void loop() {
  webSocket.loop();
  
  // hacky display brightness, todo: ISR Timer PWM
  digitalWrite(TFT_BL, 0);
  delay(2);
  digitalWrite(TFT_BL, 1);
  delay(1);
  
}

//-- show qr code on display
void displayQR(){
  // generate qr
  uint8_t qrcodeData[qrcode_getBufferSize(5)]; 
  //qrcode_initText(&qrcode, qrcodeData, 5, 0, "nano://nano_15f3d5yo8xghqfqd8694wr57x551xfxid1ywwp1jreirrn33t8yiptoxoipb?amount=0x0D3C21BCECCEDA1000000");  // https://nanoo.tools/unit-converter
  qrcode_initText(&qrcode, qrcodeData, 5, 0, "nano://nano_15f3d5yo8xghqfqd8694wr57x551xfxid1ywwp1jreirrn33t8yiptoxoipb");
  
  // wipe screen
  tft.fillScreen(TFT_BLACK);
  
  // display qr on screen
  for(int y = 0; y < qrcode.size; y++){
    for(int x = 0; x < qrcode.size; x++){
      if(qrcode_getModule(&qrcode, x, y)){
        tft.fillRect((CELLXY * x), CELLXY * y, CELLXY, CELLXY, TFT_WHITE);    
      }  
    }
  }
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    
    case WStype_DISCONNECTED:
      displayQR();
      tft.setCursor(5, 220);
      tft.println(F("waiting"));
      break;
      
    case WStype_CONNECTED:
      displayQR();
      tft.setCursor(5, 220);
      tft.println(F("ready"));
      doc["address"] = ADDRESS;
      doc["api_key"] = "0";
      char output[512];
      serializeJson(doc, output);
      Serial.println(output);
      webSocket.sendTXT(output);
      break;
      
    case WStype_TEXT:
      deserializeJson(rx_doc, payload);
      String block_amount = rx_doc["amount"];

      Serial.println(block_amount);

      const int raw_length = 31;
      int diff = raw_length - block_amount.length();
      String nano_amount = "";
      
      if(diff>0) {
        // amounts smaller than 1 nano
        nano_amount = "0.";
        for(int i = 0; i< diff-1; i++){
          nano_amount += "0";
        }
        nano_amount += block_amount;
      } else if (diff == 0) {
        // 1 to 9 nano
        nano_amount = block_amount[0];
        nano_amount += ".";
        nano_amount += block_amount.substring(1);
      } else {
        // more than 9 nano
        Serial.println((-diff)-1);
        nano_amount = block_amount.substring(0, (-diff)+1);
        nano_amount += ".";
        nano_amount += block_amount.substring((-diff)+1);
      }

      Serial.println(nano_amount);
      
      tft.setCursor(5, 155);
      tft.println(F("received"));
      tft.setCursor(5, 175);

      // float, decimals, x, y, fontsize
      tft.drawFloat( nano_amount.toFloat(), 6, 5, 175, 1);

      delay(5000);
      tft.fillRect(0, 155, 135, 200, TFT_BLACK);
      break;

  }
}

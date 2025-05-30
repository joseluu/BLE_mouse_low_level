#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "BLE2902.h"
#include "BLEHIDDevice.h"
#include "HIDTypes.h"
#include "HIDKeyboardTypes.h"
#include <driver/adc.h>
#include "sdkconfig.h"

  

BLEHIDDevice* hid;
BLECharacteristic* inputMouse;


bool connected = false;

class MyCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer){
    connected = true;
    BLE2902* desc = (BLE2902*)inputMouse->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    desc->setNotifications(true);
  }

  void onDisconnect(BLEServer* pServer){
    connected = false;
    BLE2902* desc = (BLE2902*)inputMouse->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    desc->setNotifications(false);
  }
};



void taskServer(void*){


    BLEDevice::init("Flip-O-Matic");
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyCallbacks());

    hid = new BLEHIDDevice(pServer);
    inputMouse = hid->inputReport(1); // <-- input REPORTID from report map

    std::string name = "chegewara";
    hid->manufacturer()->setValue(name.c_str());

    hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
    hid->hidInfo(0x00,0x02);

  BLESecurity *pSecurity = new BLESecurity();

  pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);

// http://www.keil.com/forum/15671/usb-mouse-with-scroll-wheel/
// Wheel Mouse - simplified version - 5 button, vertical and horizontal wheel
//
// Input report - 5 bytes
//
//     Byte | D7      D6      D5      D4      D3      D2      D1      D0
//    ------+---------------------------------------------------------------------
//      0   |  0       0       0    Forward  Back    Middle  Right   Left (Buttons)
//      1   |                             X
//      2   |                             Y
//      3   |                       Vertical Wheel
//      4   |                    Horizontal (Tilt) Wheel
//
// Feature report - 1 byte
//
//     Byte | D7      D6      D5      D4   |  D3      D2  |   D1      D0
//    ------+------------------------------+--------------+----------------
//      0   |  0       0       0       0   |  Horizontal  |    Vertical
//                                             (Resolution multiplier)
//
// Reference
//    Wheel.docx in "Enhanced Wheel Support in Windows Vista" on MS WHDC
//    http://www.microsoft.com/whdc/device/input/wheel.mspx
//


const uint8_t reportMapMouse[] = {

USAGE_PAGE(1),       0x01,
      USAGE(1),         0x02,
       COLLECTION(1),     0x01,
       REPORT_ID(1),  0x01,
       USAGE(1),        0x01,
       COLLECTION(1),     0x00,
       USAGE_PAGE(1),     0x09,
       USAGE_MINIMUM(1),    0x1,
       USAGE_MAXIMUM(1),    0x3,
       LOGICAL_MINIMUM(1),  0x0,
       LOGICAL_MAXIMUM(1),  0x1,
       REPORT_COUNT(1),   0x3,
       REPORT_SIZE(1),    0x1,
       0x80|0x01,        0x2,    // (Data, Variable, Absolute), ;3 button bits
       REPORT_COUNT(1),   0x1,
       REPORT_SIZE(1),    0x5,
       0x80|0x01,        0x1,    //(Constant), ;5 bit padding
       USAGE_PAGE(1),     0x1,    //(Generic Desktop),
       USAGE(1),        0x30,
       USAGE(1),        0x31,
       LOGICAL_MINIMUM(1),  0x81,
       LOGICAL_MAXIMUM(1),  0x7f,
       REPORT_SIZE(1),    0x8,
       REPORT_COUNT(1),   0x2,
       0x80|0x01,        0x6,    //(Data, Variable, Relative), ;2 position bytes (X & Y)
       END_COLLECTION(0),
      END_COLLECTION(0)
};

    hid->reportMap((uint8_t*)reportMapMouse, sizeof(reportMapMouse));
    hid->startServices();

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->setAppearance(HID_MOUSE);
    pAdvertising->addServiceUUID(hid->hidService()->getUUID());
    pAdvertising->start();
    hid->setBatteryLevel(7);

    ESP_LOGD(LOG_TAG, "Advertising started!");
    delay(portMAX_DELAY);
  
};


const int button1Pin = 39;     // the number of the pushbutton pin
const int button2Pin = 38;     // the number of the pushbutton pin
const int ledPin =     5;      // the number of the LED pin

void setup() {
  
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  xTaskCreate(taskServer, "server", 20000, NULL, 5, NULL);
}

void loop() {
  
  if(connected){

    //vTaskDelay(5000);Serial.println("dormindo");

    if (true) {
          Serial.println("Move mouse pointer up");
          uint8_t msg[] = {  0x00, 0x00, 0xDF};
         
          inputMouse->setValue(msg,3);
          inputMouse->notify();
          delay(10);

    delay(5000);

    Serial.println("Move mouse pointer down");
          uint8_t msg2[] = {  0x00, 0x00, 0x20};
         
          inputMouse->setValue(msg2,3);
          inputMouse->notify();
    delay(5000);
    }
    else {
     while (digitalRead(button2Pin) ==  LOW ){ 

          Serial.println("mouse Scroll DOWN");
          //<button>, <x>, <y>, <wheel>
         
          uint8_t msg[] = {  0x00, 0x00, 0xA0};
         
          inputMouse->setValue(msg,3);
          inputMouse->notify();
          delay(10);

          
          
     }
    
    while (digitalRead(button1Pin) ==  LOW ){ 

          Serial.println("mouse Scroll UP");
          //<button>, <x>, <y>, <wheel>
         
          uint8_t msg[] = {  0x00, 0x00, 0x01};
         
          inputMouse->setValue(msg,3);
          inputMouse->notify();
          delay(10);

     }
    }
    
  }
  delay(50);
}
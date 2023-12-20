#include <Arduino.h>
#include <WiFi.h>
//needed for library
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <WebSerial.h>
#include <ElegantOTA.h>
#include "Effortless_SPIFFS.h"


#include "fishScheduler.h"
#include "fbdb.h"

AsyncWebServer server(80);
DNSServer dns;
unsigned long ota_progress_millis = 0;
eSPIFFS fileSystem;

FishSched *mySched;
Database *db;

int yr = 0;
String yrStr = "";
int mo = 0;
String moStr = "";
int da = 0;
String daStr = "";
bool aFlagWasSetInLoop = false;
bool blueDosed = false;
bool midnightDone = false;

// put function declarations here:
void configModeCallback (AsyncWiFiManager *myWiFiManager);
void checkAtoAwcSched(int i);

void onOTAStart() {
  // Log when OTA has started
  Serial.println("OTA update started!");
  // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final) {
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
    Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
  }
}

void onOTAEnd(bool success) {
  // Log when OTA has finished
  if (success) {
    Serial.println("OTA update finished successfully!");
  } else {
    Serial.println("There was an error during OTA update!");
  }
  // <Add your own code here>
}

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
    ///////////////////Start WiFi ///////////////////////////////////////
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  AsyncWiFiManager wifiManager(&server, &dns);
  //reset settings - for testing
  //wifiManager.resetSettings();
  //wifiManager.setSTAStaticIPConfig(IPAddress(192,168,1,175), IPAddress(192,168,1,1), IPAddress(255,255,255,0), IPAddress(192,168,1,1), IPAddress(192,168,1,1));
  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  //Serial.println("got here");
  if (!wifiManager.autoConnect("Template")) {
    Serial.println("failed to connect and hit timeout");
    Serial.println("restarting esp");
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(1000);
  }
  delay(50);
  //Serial.print("FreeHeap is :");
  //Serial.println(ESP.getFreeHeap());
  delay(50);
  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  Serial.println("local ip");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! This is ElegantOTA AsyncDemo.");
  });

  ElegantOTA.begin(&server);    // Start ElegantOTA
  // ElegantOTA callbacks
  ElegantOTA.onStart(onOTAStart);
  ElegantOTA.onProgress(onOTAProgress);
  ElegantOTA.onEnd(onOTAEnd);

  server.begin();
 WebSerial.begin(&server);
     // Check Flash Size - Always try to incorrperate a check when not debugging to know if you have set the SPIFFS correctly
    if (!fileSystem.checkFlashConfig()) {
      Serial.println("Flash size was not correct! Please check your SPIFFS config and try again");
      delay(100000);
      ESP.restart();
    }
  else{
    // Create fileSystem with debug output
    eSPIFFS fileSystem(&Serial);  // Optional - allow the methods to print debug
  }
 delay(100);
  db  = new Database();
  db->initDb();
 mySched = new FishSched();
 mySched->updateMyTime();
 //TODO need to finish adding all the scheduler stuff  tons
}

void loop() {
  // put your main code here, to run repeatedly:
  mySched->setNowTime(); //initializes time MUST DO THIS
  mySched->tick(); //sets hour
  mySched->tock();//sets minute
  //Serial.println("Just getting for flag for loop");

  for(int i= 0;i<37;i++){
      int flagSet = mySched->isFlagSet(i);
      
      if(flagSet == 1){
        aFlagWasSetInLoop = true;
        //Serial.print("event is: ");
        //Serial.println(i);
   mySched->printArray();
  checkAtoAwcSched(i);
  mySched->printArray();
      }

  }
  if(aFlagWasSetInLoop) {
    aFlagWasSetInLoop = false;
  }else {
    blueDosed = false;

  }

  ElegantOTA.loop();
  WebSerial.print(".");
  delay(5000);
}

// put function definitions here:
void configModeCallback (AsyncWiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  //myWiFiManager->startConfigPortal("ATOAWC");addDailyDoseAmt
  //myWiFiManager->autoConnect("DOSER");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());

}

void setDate(){
  mySched->syncTime();
  yr = mySched->getCurrentYear();
  yr = yr;
  yrStr = String(yr);
  Serial.print("Year is: ");
  Serial.println(yrStr);
  mo = mySched->getCurrentMonth();
  Serial.print("Month is: ");
  Serial.println(mo);

  da = mySched->getCurrentDay();
  daStr = String(da-1);  //i need the day for save to db be the day before since i save at midnight
  Serial.print("YesterDay is: ");
  Serial.println(daStr);
 
  fileSystem.saveToFile("/curDay.txt", daStr);

}


void checkAtoAwcSched(int i){
    if(db->isThisEventAtoAwcSet(i, 0)){
      //aFlagWasSetInLoop = 0;//TODO fix this to only run once
      //thisIsAPumpEvent = true;
      //doseBlue(i);
      //ato->doATO
      //if(ato->atoFinished) mysched->setFlage(i,0)

    }
    if (db->isThisEventAtoAwcSet(i, 1)){
      aFlagWasSetInLoop = 0;//TODO fix this to only run once
      //thisIsAPumpEvent = true;
      //doseGreen(i);
      //awt->doAwc 
      //if(awt->awcFinished) mysched-setFlag(i,0)
      //put the pedning stuff in*****
    }

/*if(notDosing() && !alreadyReset){
      mySched->setFlag(i,0);
    }*/

  if(i == 8 && !midnightDone){
    midnightDone = true;
    setDate();
    mySched->setFlag(i,0);
    mySched->updateMyTime();
    //writeDailyDosesToDb();
    WebSerial.print("saved to db");
  }
  /*if(!thisIsAPumpEvent){
    mySched->setFlag(i,0);
  }*/
}
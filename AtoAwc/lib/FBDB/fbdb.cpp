#include "fbdb.h"

  FirebaseData stream;
  FirebaseData fbdo;

  bool Database::dataChanged = false;
  
Database::Database(){

  
}

void  Database::streamCallback(StreamData data)
{
  //Serial.println("in callback");

  /*Serial.printf("sream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
                data.streamPath().c_str(),
                data.dataPath().c_str(),
                data.dataType().c_str(),
                data.eventType().c_str());*/
    //printResult(data); //see addons/RTDBHelper.h
    //Serial.print("Stream Path: ");
    //Serial.println(data.streamPath());
    //Serial.print("Data Path: ");
    //Serial.println(data.dataPath());
    //Serial.print("Data Type: ");
    //Serial.println(data.dataType());
    //Serial.print("Event Type: ");
    //Serial.println(data.eventType());
    //Serial.print("Data");
    //Serial.println(data.value());
  if (data.dataTypeEnum() == fb_esp_rtdb_data_type_integer)
      Serial.println(data.to<int>());
  else if (data.dataTypeEnum() == fb_esp_rtdb_data_type_float)
      Serial.println(data.to<float>(), 5);
  else if (data.dataTypeEnum() == fb_esp_rtdb_data_type_double)
      printf("%.9lf\n", data.to<double>());
  else if (data.dataTypeEnum() == fb_esp_rtdb_data_type_boolean)
      Serial.println(data.to<bool>()? "true" : "false");
  else if (data.dataTypeEnum() == fb_esp_rtdb_data_type_string)
      Serial.println(data.to<String>());
  else if (data.dataTypeEnum() == fb_esp_rtdb_data_type_json)
  {
      FirebaseJson *json = data.to<FirebaseJson *>();
      //Serial.println(json->raw());
  }
  else if (data.dataTypeEnum() == fb_esp_rtdb_data_type_array)
  {
      FirebaseJsonArray *arr = data.to<FirebaseJsonArray *>();
      //Serial.println(arr->raw());
      
  }


  Serial.println();
  dataChanged = true;
  //Database db;
  //db.setEvents();
  //setEvents();

  //This is the size of stream payload received (current and max value)
  //Max payload size is the payload size under the stream path since the stream connected
  //and read once and will not update until stream reconnection takes place.
  //This max value will be zero as no payload received in case of ESP8266 which
  //BearSSL reserved Rx buffer size is less than the actual stream payload.
  //Serial.printf("Received stream payload size: %d (Max. %d)\n\n", data.payloadLength(), data.maxPayloadLength());
}

void Database::streamTimeoutCallback(bool timeout)
{
  //FirebaseData stream;
  if (timeout)
    Serial.println("stream timed out, resuming...\n");

  if (!stream.httpConnected())
    Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
}

void Database::initDb(){
  //FirebaseData stream;
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
    /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  //config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  //Or use legacy authenticate method
  //config.database_url = DATABASE_URL;
  //config.signer.tokens.legacy_token = "<database secret>";

  //To connect without auth in Test Mode, see Authentications/TestMode/TestMode.ino

  Firebase.begin(&config, &auth);

  Firebase.reconnectWiFi(true);

//Recommend for ESP8266 stream, adjust the buffer size to match your stream data size
#if defined(ESP8266)
  stream.setBSSLBufferSize(2048 /* Rx in bytes, 512 - 16384 */, 512 /* Tx in bytes, 512 - 16384 */);
#endif

  if (!Firebase.beginStream(stream, "/Doser/event"))
    //Serial.printf("sream begin error, %s\n\n", stream.errorReason().c_str());

  Firebase.setStreamCallback(stream, this->streamCallback, this->streamTimeoutCallback);

  /** Timeout options, below is default config.

  //WiFi reconnect timeout (interval) in ms (10 sec - 5 min) when WiFi disconnected.
  config.timeout.wifiReconnect = 10 * 1000;

  //Socket begin connection timeout (ESP32) or data transfer timeout (ESP8266) in ms (1 sec - 1 min).
  config.timeout.socketConnection = 30 * 1000;

  //ESP32 SSL handshake in ms (1 sec - 2 min). This option doesn't allow in ESP8266 core library.
  config.timeout.sslHandshake = 2 * 60 * 1000;

  //Server response read timeout in ms (1 sec - 1 min).
  config.timeout.serverResponse = 10 * 1000;

  //RTDB Stream keep-alive timeout in ms (20 sec - 2 min) when no server's keep-alive event data received.
  config.timeout.rtdbKeepAlive = 45 * 1000;

  //RTDB Stream reconnect timeout (interval) in ms (1 sec - 1 min) when RTDB Stream closed and want to resume.
  config.timeout.rtdbStreamReconnect = 1 * 1000;

  //RTDB Stream error notification timeout (interval) in ms (3 sec - 30 sec). It determines how often the readStream
  //will return false (error) when it called repeatedly in loop.
  config.timeout.rtdbStreamError = 3 * 1000;

  */
    Serial.println("Just calling setEvent because detected db change");
   
}
  bool Database::databaseReady(){
    if (Firebase.ready()){
      return true;
    }else{
      return false;
    }
  }

/*bool Database::putData(String path, FirebaseJson json){
  bool retVal = false;
  retVal = Firebase.RTDB.setJSON(&fbdo, path.c_str(), &json);
    return retVal;
}*/

  FirebaseJson* Database::getJson(String path){
    bool retVal = false;
    retVal = Firebase.getJSON(fbdo, path);
      FirebaseJson *json = fbdo.jsonObjectPtr();

      //FirebaseJson *jsn = json.to<FirebaseJson *>();
      //Serial.println(json->raw());
      return json;
  }
  
    String Database::putFloat(String nam, float val){
      String retVal = "ok";
    // set value
      if (databaseReady()){
        Firebase.setFloat(fbdo, nam, val);
      // handle error
      }else{        
        String retVal = fbdo.errorReason().c_str();
      }
      return retVal;
    }

    String Database::putString(String nam, String val){
      String retVal = "ok";
    // set value
      if (databaseReady()){
        Firebase.setString(fbdo, nam, val);
      // handle error
      }else{        
        String retVal = fbdo.errorReason().c_str();
      }
      return retVal;
    }
    
    String Database::putInt(String nam, int val){
      String retVal = "ok";
      if (databaseReady() ){
        Firebase.setInt(fbdo, nam, val);
      // handle error
      }else{        
         String retVal = fbdo.errorReason().c_str();
      }
      return retVal;
    }
    
    String Database::putBool(String nam, bool val){
      String retVal = "ok";
      if (databaseReady()){
        Firebase.setBool(fbdo, nam, val);
      // handle error
      }else{        
        String retVal = fbdo.errorReason().c_str();
      }
      return retVal;
    }
    
    float Database::getFloat(String nam){
      float retVal = 0;
      if (databaseReady()){
        retVal = Firebase.getFloat(fbdo, nam);
        float f = fbdo.to<float>();
        retVal = f;
      // handle error
      }else{        
        Serial.println(fbdo.errorReason().c_str());
        retVal = 99;
      }
      return retVal;
    }
    
    String Database::getStr(String nam){
      String retVal = "";
      if (databaseReady()){
        retVal = Firebase.getString(fbdo, nam);
        String s = fbdo.to<String>();
        retVal = s;
      // handle error
      }else{        
        retVal = fbdo.errorReason().c_str();
      }
       return retVal;
    }
    
    int Database::getInt(String nam){
      int retVal;
      if (databaseReady()){
        //Serial.printf("Get int ref... %s\n", Firebase.getInt(fbdo, F(nam.c_str), &retVal) ? String(retVal).c_str() : fbdo.errorReason().c_str());
        retVal = Firebase.getInt(fbdo, nam);
        int i = fbdo.to<int>();
        retVal = i;
      }else{        
        Serial.println(fbdo.errorReason().c_str());
        retVal = 99;
      }
      return retVal;
    }
    
    bool Database::getBool(String nam){
      bool retVal = false;
      // set value
      if (databaseReady()){
        retVal = Firebase.getBool(fbdo, nam);
        bool b = fbdo.to<bool>();
        retVal = b;
      }else{        
        Serial.println(fbdo.errorReason().c_str());
        retVal = false;
      }
      return retVal;
    }

    bool Database::putDailyDoseData(String parent, String child, int amt){
      bool retVal = false;
      FirebaseJson json;
      json.set(child.c_str(), String(amt));
      //retVal = Firebase.RTDB.setJSON(&fbdo, parent.c_str(), &json);
      //Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parent.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
      return retVal;
    }

    bool Database::putColorMlSec(String parent, String child, int amt){
        bool retVal = false;
      FirebaseJson json;
      json.set(child.c_str(), String(amt));
      //retVal = Firebase.RTDB.setJSON(&fbdo, parent.c_str(), &json);
      //Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parent.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
      return retVal;
   
    }

    bool Database::putDailyAfillData(String parent, String child, int amt){
      bool retVal = false;
      FirebaseJson json;
      json.set(child.c_str(), String(amt));
      //retVal = Firebase.RTDB.setJSON(&fbdo, parent.c_str(), &json);
      //Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parent.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
      return retVal;

    }

    void Database::setEvents(int sys){
      String device = "";
      if(sys == 0){
        device = "Doser";
      }else if(sys == 1){
        device = "AtoAwc";
      }
       Serial.println("just ran an update");
       Serial.print(" on system ");
       Serial.println(device);
      QueryFilter query;
      String evt = "";
      //evtPumpArr = new int* [40];
      int* arr;
      for(int i=0;i<37;i++){
        if(i == 0){
          evt = "one_hour";
          //arr = one_hourArr;
        }else if (i == 1){
          evt = "fifteen";
          //arr = fifteenArr;
        }else if (i == 2){
          evt = "thirty";
          //arr = thirtyArr;
        }else if (i == 3){
          evt = "two_hour";
         // arr = two_hourArr;
        }else if (i == 4){
          evt = "three_hour";
         // arr = three_hourArr;
        }else if (i == 5){
          evt = "four_hour";
          //arr = four_hourArr;
        }else if (i == 6){
          evt = "eight_hour";
          //arr = eight_hourArr;
        }else if (i == 7){
          evt = "twelve_hour";
          //arr = twelve_hourArr;
        }else if (i == 8){
          evt = "midnight";
          //arr = one_amArr;
        }else if (i == 9){
          evt = "one_am";
          //arr = one_amArr;
        }else if (i == 10){
          evt = "two_am";
          //arr = two_amArr;
        }else if (i == 11){
          evt = "three_am";
         // arr = three_amArr;
        }else if (i == 12){
          evt = "four_am";
         // arr = four_amArr;
        }else if (i == 13){
          evt = "five_am";
          //arr = five_amArr;
        }else if (i == 14){
          evt = "six_am";
          //arr = six_amArr;
        }else if (i == 15){
          evt = "seven_am";
          //arr = seven_amArr;
        }else if (i == 16){
          evt = "eight_am";
          //arr = eight_amArr;
        }else if (i == 17){
          evt = "nine_am";
          //arr = nine_amArr;
        }else if (i == 18){
          evt = "ten_am";
          //arr = ten_amArr;
        }else if (i == 19){
          evt = "eleven_am";
          //arr = eleven_amArr;
        }else if (i == 20){
          evt = "noon";
        }else if (i == 21){
          evt = "one_pm";
          //arr = one_pmArr;
        }else if (i == 22){
          evt = "two_pm";
          //arr = two_pmArr;
        }else if (i == 23){
          evt = "three_pm";
          //arr = three_pmArr;
        }else if (i == 24){
          evt = "four_pm";
          //arr = four_pmArr;
        }else if (i == 25){
          evt = "five_pm";
          //arr = five_pmArr;
        }else if (i == 26){
          evt = "six_pm";
          //arr = six_pmArr;
        }else if (i == 27){
          evt = "seven_pm";
          //arr = seven_pmArr;
        }else if (i == 28){
          evt = "eight_pm";
          //arr = eight_pmArr;
        }else if (i == 29){
          evt = "nine_pm";
          //arr = nine_pmArr;
        }else if (i == 30){
          evt = "ten_pm";
          //arr = ten_pmArr;
        }else if (i == 31){
          evt = "eleven_pm";
          //arr = eleven_pmArr;
        }else if (i == 32){
          evt = "day";
          //arr = eleven_pmArr;
        }else if (i == 33){
          evt = "every_other_day";
          //arr = eleven_pmArr;
        }else if (i == 34){
          evt = "week";
          //arr = eleven_pmArr;
        }else if (i == 35){
          evt = "every_other_week";
          //arr = eleven_pmArr;
        }else if (i == 36){
          evt = "month";
          //arr = eleven_pmArr;
        }
        //Serial.print("evt in db query is: ");
        //Serial.println(evt);
        query.limitToLast(50);
        if (Firebase.getJSON(fbdo, "/"+device+"/event/"+evt, query)) // Doser or AtoAwc
        {
          // Success, then try to read the JSON payload value
          /////////////////////////////////////////////////////////
                  //FirebaseJson *json = data.to<FirebaseJson *>();
          //Print all object data
          //Serial.println((const char *)FPSTR("Pretty printed JSON data:"));
          FirebaseJson &json = fbdo.jsonObject();
          json.toString(Serial, true);
          //Serial.println();
          //Serial.println((const char *)FPSTR("Iterate JSON data:"));
          //Serial.println();
          size_t len = json.iteratorBegin();
          //Serial.print("!!!!!!!!!!!!! interator length iss: ");
          //Serial.println(len);
          FirebaseJson::IteratorValue value;
          if(sys == 0){
            for (size_t j = 0; j < len; j++)
            {
                value = json.valueAt(j);
              // Serial.printf((const char *)FPSTR("%d, Type: %s, Name: %s, Value: %s\n"), i, value.type == FirebaseJson::JSON_OBJECT ? (const char *)FPSTR("object") : (const char *)FPSTR("array"), value.key.c_str(), value.value.c_str());
              int val = value.value.toInt();
              int k = 0;
              k = j;
              if(k == 2){
                k = 3;   //these come back in alphabetical order blue, green purple, yellow
              }else if(k == 3){
                k = 2;
              }
              //Serial.println("^^^^^^^^^^^^^ made it here");
              evtPumpArr[i][k] = val;
              //Serial.print("value is: ");
              //Serial.println(val);
            //Serial.print("i is: ");
            // Serial.println(i);
              //Serial.print("j is: ");
              //Serial.println(j);
              delay(100);
            }
          }else if(sys == 1){
            for(size_t j = 0; j < len; j++){
              value = json.valueAt(j);
              // Serial.printf((const char *)FPSTR("%d, Type: %s, Name: %s, Value: %s\n"), i, value.type == FirebaseJson::JSON_OBJECT ? (const char *)FPSTR("object") : (const char *)FPSTR("array"), value.key.c_str(), value.value.c_str());
              int val = value.value.toInt();
              evtAtoAwcArr[i][j] = val;
            }
          }
          json.iteratorEnd();
          json.clear();
      
  ////////////////////////////////////////////////////////////////////////////////
          
          //Serial.println(fbdo.jsonString());
        }
        else
        {
          // Failed to get JSON data at defined database path, print out the error reason
          Serial.println(fbdo.errorReason());
        }

      /* FirebaseJsonData result;
        FirebaseJsonArray arr;
        arr.add("Doser/events", "midnight");
        arr.set("[1]/blue" , "0");
        arr.set("[1]/green" , "0");
        arr.set("[1]/yellow" , "0");
        arr.set("[1]/purple" , "0");

        ar.get(result, "[1]");
        FirebaseJson json;
        result.get<FirebaseJson>(json);
        size_t len = json.iteratorBegin();
        FirebaseJson::IteratorValue value;
        for(size_t i = 0;i<len;i++){
          value = json.valueAt(i);
          int va = value.value.toInt();
          
          Serial.print(va);
          Serial.println(",");
        } */
      }
    }

    //int** Database::getEvtPump(){
    //  return evtPumpArr;
   // }




bool Database::isThisEventPumpSet(int evt, int pump){

  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  //one_hour=0, fifteen=1, thirty=2, two_hour=3, three_hour=4, 
  //four_hour=5,eight_hour=6, twelve_hour=7,
  //midnight=8, one_am=9, two_am=10, three_am=11, four_am=12, 
  //five_am=13, six_am=14, severn_am=15,
  //eight_amm=16, nine_am=17 ten_am=18, eleven_am=19,
  // noon=20, one_pm=21, two_pm=22, three_pm=23, 
  //four_pm=24, five_pm=25, six_pm=26, severn_pm=27,
  //eight_pmm=28, nine_pm=29 ten_pm=30, eleven_pm=31,
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  bool retVal = false;
 
  retVal = evtPumpArr[evt][pump];
  if(retVal == 1){
    retVal = true;
    //Serial.print("ent is ");
    //Serial.print(evt);
    //Serial.print(" and pump is ");
    //Serial.println(pump);

  }else{
    retVal = false;
  }


  return retVal;
}

bool Database::isThisEventAtoAwcSet(int evt, int atoAwc){

  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  //one_hour=0, fifteen=1, thirty=2, two_hour=3, three_hour=4, 
  //four_hour=5,eight_hour=6, twelve_hour=7,
  //midnight=8, one_am=9, two_am=10, three_am=11, four_am=12, 
  //five_am=13, six_am=14, severn_am=15,
  //eight_amm=16, nine_am=17 ten_am=18, eleven_am=19,
  // noon=20, one_pm=21, two_pm=22, three_pm=23, 
  //four_pm=24, five_pm=25, six_pm=26, severn_pm=27,
  //eight_pmm=28, nine_pm=29 ten_pm=30, eleven_pm=31,
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  bool retVal = false;
 
  retVal = evtAtoAwcArr[evt][atoAwc];
  if(retVal == 1){
    retVal = true;
  }else{
    retVal = false;
  }
  return retVal;
}

void Database::callBack(){
  Firebase.setStreamCallback(stream, streamCallback, streamTimeoutCallback);
}
#ifndef FBDB___h
#define FBDB___h


#if defined(ESP32)
#include <WiFi.h>
#include "FirebaseESP32.h"
#include <Firebase.h>

#elif defined(ESP8266)
#endif


//Provide the token generation process info.
//#include <addons/TokenHelper.h>

//Provide the RTDB payload printing info and other helper functions.
//#include <addons/RTDBHelper.h>



//For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
//#define API_KEY "AIzaSyCq0NqOvq6DMjdW2wu3P0sEPLV-3PuIcKs"
#define API_KEY "AIzaSyA9Tb-xbeCJzh9pAVq64kFP_mcwA4sbp-g"  //DEV
/* 3. Define the RTDB URL */
//#define DATABASE_URL "https://esp32-firebase-example-852b0-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_URL "https://atoawcdev-default-rtdb.firebaseio.com/" //DEV

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "mmkkrood@aol.com"
#define USER_PASSWORD "Skippy"




class Database
{
  
  public:

    Database();
    void initDb();
    bool databaseReady();
    bool putData(String path, FirebaseJson json);
    static void streamCallback(StreamData data);
    static void streamTimeoutCallback(bool timeout);
    String putFloat(String nam, float val);
    String putString(String nam, String val);
    String putInt(String nam, int val);
    String putBool(String nam, bool val);
    float getFloat(String nam);
    String getStr(String nam);
    int getInt(String nam);
    bool getBool(String nam);
    FirebaseJson* getJson(String path);
    void callBack();

    static bool dataChanged;

    
    bool putDailyDoseData(String parent, String child, int amt);
    bool putDailyAfillData(String parent, String child, int amt);
    bool putColorMlSec(String parent, String child, int amt);

    void setEvents(int sys);
    bool isThisEventPumpSet();

    bool isThisEventPumpSet(int evt, int pump);
    bool isThisEventAtoAwcSet(int evt, int atoAwc);
    bool isAtoSet(int evt);
    bool isAwcSet(int evt);
        int evtPumpArr[37][4] {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, 
                          {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, 
                          {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, 
                          {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0},
                          {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, 
                          {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, 
                          {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, 
                          {0,0,0,0}, {0,0,0,0}
                         };


    int evtAtoAwcArr[37][2] {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, 
                          {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, 
                          {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, 
                          {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
                          {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, 
                          {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, 
                          {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, 
                          {0,0}, {0,0} 
                          };
    //int** getEvtPump();
/*    int one_hourArr[4];
    int fifteenArr[4];
    int thirtyArr[4];
    int two_hourArr[4];
    int three_hourArr[4];
    int four_hourArr[4];
    int eight_hourArr[4];
    int twelve_hourArr[4];
    int midnightArr[4];
    int one_amArr[4];
    int two_amArr[4];
    int three_amArr[4];
    int four_amArr[4];
    int five_amArr[4];
    int six_amArr[4];
    int seven_amArr[4];
    int eight_amArr[4];
    int nine_amArr[4];
    int ten_amArr[4];
    int eleven_amArr[4];
    int noonArr[4];
     int one_pmArr[4];
    int two_pmArr[4];
    int three_pmArr[4];
    int four_pmArr[4];
    int five_pmArr[4];
    int six_pmArr[4];
    int seven_pmArr[4];
    int eight_pmArr[4];
    int nine_pmArr[4];
    int ten_pmArr[4];
    int eleven_pmArr[4];
   */
    
  private:
    //Define Firebase Data object
    //Firebase *fb;

    
    FirebaseAuth auth;
    FirebaseConfig config;
    bool needToUpdate = false;
    //static FirebaseData stream;
    

};


#endif
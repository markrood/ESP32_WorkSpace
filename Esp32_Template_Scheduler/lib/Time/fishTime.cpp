/**********************************************************************/
//
//  Class:
//    FishTime
//
//  Description
//    Super class for FishSched.
//    This is just a time class uses Real Time Clock (RTC) built into the esp32 and
//    updates RTC to correct time form the internet NTP time.
//
//  ERROR CODES:
//    none yet
//
/**************************************************************************/


#include "fishTime.h"

RtcDS3231<TwoWire> Rtc(Wire);


FishTime::FishTime(int offset,bool dst){
    int daylightOffset_sec = 0;
    if(dst){
      daylightOffset_sec = 3600;
    }
    configTime(offset, daylightOffset_sec, ntpServer);
    numOfI2C = scanner();
    syncTime();
    //Serial.println(timeinfo.tm_mon);
  }

  void FishTime::syncTime(){
    bool timeUpdate = false;
    //while(!timeUpdate){
      if(!getLocalTime(&timeinfo,1000)){
        //rtc.setTimeStruct(timeinfo);
        Serial.println("Failed to obtain time");
        Serial.println("THIS IS AN ERROR WE ARE LOOPING UNTIL GET TIME");
        timeUpdate = false;
        delay(3000);
      }else{
        Serial.println("time is sync and Rtc time may set if hw there");
        setNowTime();
        Serial.println("111111111111");
        setRtcTime();
        //Rtc.setTimeStruct(timeinfo);
        Serial.println("22222222222");
        timeUpdate = true;
      }
    //}
    timeUpdate = false;
   }



void FishTime::setNowTime(){
  
  if(numOfI2C == 0){
        while(!timeUpdate){
      if(!getLocalTime(&timeinfo,1000)){
        //rtc.setTimeStruct(timeinfo);
        Serial.println("Failed to obtain time");
        Serial.println("THIS IS AN ERROR WE ARE LOOPING UNTIL GET TIME");
        timeUpdate = false;
        delay(30000);
      }else{
        RtcDateTime currentTime = RtcDateTime(timeinfo.tm_year+1900, timeinfo.tm_mon+1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, 0);  //define date and time object
        Rtc.SetDateTime(currentTime);
        timeUpdate = true;
      }
    }
    timeUpdate = false;
    currentYear = timeinfo.tm_year;
    //Serial.print("Year is: ");
    //Serial.print(currentYear);
    currentMonth = timeinfo.tm_mon+1;
    //Serial.print(", Month is: ");
    //Serial.print(currentMonth);
    currentMonthName = months[getCurrentMonth()-1];
    dayWeek = weekDays[timeinfo.tm_wday];
    currentDay = timeinfo.tm_mday;
    //Serial.print(", Day is: ");
    //Serial.print(currentDay);
    currentHour = timeinfo.tm_hour;
    //Serial.print(", Hour is: ");
    //Serial.print(currentHour);
    currentMinute = timeinfo.tm_min;
    //Serial.print(", Minute is: ");
    //Serial.print(currentMinute);
    currentSecond = timeinfo.tm_sec;
    //Serial.print(", Second is: ");
    //Serial.print(currentSecond);
  }else{
    RtcDateTime dt = Rtc.GetDateTime();
    currentMonth = dt.Month();
    //Serial.print("RTC month is: ");
    //Serial.print(currentMonth);
    currentDay = dt.Day();
    //Serial.print(", RTC day is: ");
    //Serial.print(currentDay);
    currentYear = dt.Year();
    //Serial.print(", RTC year is: ");
    //Serial.print(currentYear);
    currentHour = dt.Hour();
    //Serial.print(", RTC hour is: ");
    //Serial.print(currentHour);
    currentMinute = dt.Minute();
    //Serial.print(", RTC minute is: ");
    //Serial.print(currentMinute);
    currentSecond = dt.Second();
    //Serial.print(", RTC second is: ");
    //Serial.print(currentSecond);
  }
}

////////////////////////////////////////////////////////////////////////
//
//  This checks to see if there is an I2C device connected i.e. DS3231
//
//  return number of devices
//
///////////////////////////////////////////////////////////////////////////////
int FishTime::scanner ()
{
  Serial.println ();
 // Serial.println ("I2C scanner. Scanning ...");
  byte count = 0;

  Wire.begin();
  for (byte i = 8; i < 120; i++)
  {
    Wire.beginTransmission (i);          // Begin I2C transmission Address (i)
    if (Wire.endTransmission () == 0)  // Receive 0 = success (ACK response) 
    {
      //Serial.print ("Found address: ");
      //Serial.print (i, DEC);
      //Serial.print (" (0x");
      //Serial.print (i, HEX);     // PCF8574 7 bit address
      //Serial.println (")");
      count++;
    }
  }
  Serial.print ("Found ");      
  Serial.print (count, DEC);        // numbers of devices
  Serial.println (" device(s).");
  return count;
}

void FishTime::setRtcTime() {
  RtcDateTime currentTime = RtcDateTime(timeinfo.tm_year+1900, timeinfo.tm_mon+1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, 0);  //define date and time object
  
  int numOfI2CDevices = scanner();
  if(numOfI2CDevices > 0){
    Serial.println("Rtc hardware exist");
      Rtc.SetDateTime(currentTime); //configure the RTC with object
      printTimeDate();
    }                                                                               
}


    int FishTime::getMonthDay(){
      return monthDay;
    }


    int FishTime::getCurrentMonth(){
      return currentMonth;
    }

    String FishTime::getCurrentMonthName(){
      return currentMonthName;
    }

    
    int FishTime::getCurrentYear(){
      return currentYear;
    }

    String FishTime::getFormattedTime_(){
      return formattedTime;
    }

    int FishTime::getCurrentDay(){
      return currentDay;
    }


    int FishTime::getCurrentHour(){
      return currentHour;
    }

    int FishTime::getCurrentMinute(){
      return currentMinute;
    }

    int FishTime::getCurrentSecond(){
      return currentSecond;
    }

    String FishTime::getWeekDay(){
      return dayWeek;
    }

    String FishTime::getCurrentDate(){
      return currentDate;
    }

void FishTime::printTimeDate(){
  RtcDateTime dt = Rtc.GetDateTime();
    currentMonth = dt.Month();
    Serial.print("RTC month is: ");
    Serial.print(currentMonth);
    currentDay = dt.Day();
    Serial.print(", RTC day is: ");
    Serial.print(currentDay);
    currentYear = dt.Year();
    Serial.print(", RTC year is: ");
    Serial.print(currentYear);
    currentHour = dt.Hour();
    Serial.print(", RTC hour is: ");
    Serial.print(currentHour);
    currentMinute = dt.Minute();
    Serial.print(", RTC minute is: ");
    Serial.print(currentMinute);
    currentSecond = dt.Second();
    Serial.print(", RTC second is: ");
    Serial.print(currentSecond);
}
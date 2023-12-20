#ifndef FISHTIME_H_
#define FISHTIME_H_

#include <WiFi.h>
#include "time.h"
#include "Arduino.h"
#include <RtcDS3231.h>
#include "Wire.h"






class FishTime{
  public:
    FishTime(int offset,bool dst);
    void syncTime();
    void setNowTime();
    int scanner();
    void setRtcTime();
    //unsigned long getEpochTime();
    int getMonthDay();
    int getCurrentMonth();
    String getCurrentMonthName();
    int getCurrentYear();
    String getFormattedTime_();
    int getCurrentDay();
    int getCurrentHour();
    int getCurrentMinute();
    int getCurrentSecond();
    String getWeekDay();
    String getCurrentDate();
    void printTimeDate();
   // bool getLocalTime(struct tm * info, uint32_t ms);
    
  private:
    //NTPClient *_timeClient;
    //WiFiUDP ntpUDP;
    const char* ntpServer = "pool.ntp.org";
    //bool daylightSavingsTime = false;
    //Week Days
    String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    //Month names
    String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    struct tm *ptm = gmtime ((time_t *)&epochTime);
    unsigned long epochTime;
    //struct tm *ptm;
    struct tm timeinfo;
    int monthDay;
    int currentMonth;
    String currentMonthName;
    int currentYear;
    String formattedTime;
    int currentDay;
    int currentHour;
    int currentMinute;
    int currentSecond;
    String dayWeek;
    String currentDate;
    bool timeUpdate = false;
    int numOfI2C = 0;
    ///////////////////////////////////
    //bool getLocalTime(struct tm * info, uint32_t ms);
    
};
#endif
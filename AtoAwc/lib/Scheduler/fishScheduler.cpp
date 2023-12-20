/*************************************************set**********blowHourHorn******************************/
//
//  Class:
//    FishSched
//
//  Description
//    User super class FishTime for RTC and NTP update time from internet.
//    This class used the RTC to call the "crons" It simple set flags in an
//    bitset/enum (flags) when a certain time is hit (i.e. hour,15min,30min,2h,
//    3h,4h,8h,12h,1am-noon, 1pm-midnight).  A main.cpp class will use this scheduler 
//    to look for events and act on the event and reset the flag for that event when fired
//
//  ERROR_CODES
//    none yet
//
/********************************************************************************************/

#include "fishScheduler.h"
#include "fishTime.h"



  FishSched::FishSched():FishTime(-21600,true){  //TODO get offset and dst from database
  for(int i = 0;i<NumFlags;i++){
    if(isFlagSet(i)){
      //resetFlag(i);
    }
  }
  //printArray();
    syncTime();
    setNowTime();
  }
void FishSched::updateMyTime(){
    setNowTime();
}

void FishSched::tick(){
//Serial.println("1.11");
  int nowHour = getCurrentHour();
    if(nowHour != currHour ){
      if(firstHour){
        firstHour = false;
        currHour = nowHour;
        return;
      }
    currHour = nowHour;
    //Serial.print("New Hour is: ");
    //Serial.println(currHour);
    blowHourHorn(currHour);
    }
}

void FishSched::tock(){
//Serial.println("1.22");
  int nowMinute = getCurrentMinute();
  //Serial.println("1.23");
  if(nowMinute != currentMinute){
    //Serial.println("1.24");
    currentMinute = nowMinute;
    ////Serial.println(currentMinute);
    blow15MinuteHorn(currentMinute);
    //Serial.println("1.25");
    blow30MinuteHorn(currentMinute);
  }
}

void FishSched::blowHourHorn(int hr){
  //Serial.println("0.1111");
  addHour();
  if(hr == 1){
    //Serial.println("the hour is 1");
    flags[OneAm] = 1;
  }else if(hr == 2){
    //Serial.println("the hour is 2"); 
    flags[TwoAm] = 1;
  }else if(hr == 3){
    //Serial.println("the hour is 3");  
    flags[ThreeAm] =1;
  }else if(hr == 4){
    //Serial.println("the hour is 4");
    flags[FourAm] = 1;  
  }else if(hr == 5){
    //Serial.println("the hour is 5"); 
    flags[FiveAm] = 1; 
  }else if(hr == 6){
    //Serial.println("the hour is 6"); 
    flags[SixAm] = 1; 
  }else if(hr == 7){
    //Serial.println("the hour is 7"); 
    flags[SevenAm] = 1; 
  }else if(hr == 8){
    //Serial.println("the hour is 8"); 
    flags[EightAm] = 1; 
  }else if(hr == 9){
    //Serial.println("the hour is 9");  
    flags[NineAm] = 1;
  }else if(hr == 10){
    //Serial.println("the hour is 10");
    flags[TenAm] =1;  
  }else if(hr == 11){
    //Serial.println("the hour is 11");
    flags[ElevenAm] = 1;  
  }else if(hr == 12){
    //Serial.println("the hour is 12"); 
    flags[Noon] = 1; 
  }else if(hr == 13){
    //Serial.println("the hour is 13"); 
    flags[OnePm] = 1;
  }else if(hr == 14){
    //Serial.println("the hour is 14"); 
    flags[TwoPm] = 1; 
  }else if(hr == 15){
    //Serial.println("the hour is 15"); 
    flags[ThreePm] =1; 
  }else if(hr == 16){
    //Serial.println("the hour is 16");  
    flags[FourPm] = 1;
  }else if(hr == 17){
    //Serial.println("the hour is 17"); 
    flags[FivePm] = 1;
  }else if(hr == 18){
    //Serial.println("the hour is 18");
    flags[SixPm] =1;  
  }else if(hr == 19){
    //Serial.println("the hour is 19");
    flags[SevenPm] =1;  
  }else if(hr == 20){
    //Serial.println("the hour is 20"); 
    flags[EightPm] =1; 
  }else if(hr == 21){
    //Serial.println("the hour is 21");
    flags[NinePm] =1;
  }else if(hr == 22){
    //Serial.println("the hour is 22"); 
    flags[TenPm] = 1; 
  }else if(hr == 23){
    //Serial.println("the hour is 23");  
    flags[ElevenPm] = 1;
  }else if(hr == 0){
    //Serial.println("the hour is 0");  
    flags[Midnight] = 1;
    addDay();
  }
  //Serial.println("doneeeeeeeee with flags.set");

}

void FishSched::blow15MinuteHorn(int m){
  if(m == 15 || m == 30 || m == 45 || m == 0){
    //Serial.println("Fifteen Minute Alarm");
    flags[Fifteen] = 1;
    //Serial.println("Fifteen Minute Alarm");
  }
}

void FishSched::blow30MinuteHorn(int m){
  if(m == 30 || m == 0){
    //Serial.println("Thirty Minute Alarm");
    flags[Thirty] = 1;
    //Serial.println("Thirty Minute Alarm");
  }
}

void FishSched::addHour(){
  oneHourAlarm++;
  twoHourAlarm++;
  threeHourAlarm++;
  fourHourAlarm++;
  eightHourAlarm++;
  twelveHourAlarm++;
//Serial.println("3.0");

  if(oneHourAlarm == 1){
    oneHourAlarm = 0;
    //blow2HourHorn();
    flags[Hour] = 1;
    //Serial.println("One Hour Alarm");
  }
  if(twoHourAlarm == 2){
    twoHourAlarm = 0;
    //blow2HourHorn();
    flags[TwoHour] = 1;
    //Serial.println("Two Hour Alarm");
  }
  if(threeHourAlarm == 3){
    threeHourAlarm = 0;
    //blow3HourHorn();
    flags[ThreeHour] = 1;
    //Serial.println("Three Hour Alarm");
  }

  if(fourHourAlarm == 4){
    fourHourAlarm = 0;
    //blow4HourHorn();
    //Serial.println("Four Hour Alarm");
    flags[FourHour] = 1;
  }

  if(eightHourAlarm == 8){
    eightHourAlarm = 0;
    //blow8HourHorn();
    flags[EightHour] = 1;
    //Serial.println("Eight Hour Alarm");
  }

  if(twelveHourAlarm == 12){
    twelveHourAlarm = 0;
    //blow12HourHorn();
    flags[TwelveHour] = 1;
    //Serial.println("Twelve Hour Alarm");
  }

}

void FishSched::addDay(){
  oneDayAlarm++;
  twoDayAlarm++;
  sevenDayAlarm++;
  fourteenDayAlarm++;
  thirtyDayAlarm++;


  if(oneDayAlarm == 1){
    oneDayAlarm = 0;
    //blow2HourHorn();
    flags[Day] = 1;
    //Serial.println("One Day Alarm");
  }
  if(twoDayAlarm == 2){
    twoDayAlarm = 0;
    //blow2HourHorn();
    flags[EveryOtherDay] = 1;
    //Serial.println("EveryOther Day Alarm");
  }
  if(sevenDayAlarm == 7){
    sevenDayAlarm = 0;
    //blow3HourHorn();
    flags[Week] = 1;
    //Serial.println("Week Alarm");
  }

  if(fourteenDayAlarm == 14){
    fourteenDayAlarm = 0;
    //blow4HourHorn();
    //Serial.println("EveryOtherWeek Alarm");
    flags[EveryOtherWeek] = 1;
  }

  if(thirtyDayAlarm == 30){
    thirtyDayAlarm = 0;
    //blow8HourHorn();
    flags[Month] = 1;
    //Serial.println("Month Alarm");
  }
}
/*template<size_t N>
std::bitset<N> FishSched::getFlags(){
  return flags;
}*/
bool FishSched::isFlagSet(int flag){
  //bool retVal = flags.test(flag);
bool retVal = false;
  if(flags[flag] == 1){
    retVal = true;
  }
  //flags.flip(Fifteen);
  return retVal;
}

void FishSched::resetFlag(int flag){
 // Serial. ,2,2");
  //flags.flip(flag);
  if(flags[flag] == 1){
    flags[flag] = 0;
  }else{
    flags[flag] = 1;
  //Serial.println("2.2.4");
}
}

void FishSched::setFlag(int evt,int val){
  flags[evt] = val;
}

void FishSched::printArray(){
  for(int i=0;i>37;i++){
    Serial.print(flags[i]);
    Serial.println(" ");
  }

}
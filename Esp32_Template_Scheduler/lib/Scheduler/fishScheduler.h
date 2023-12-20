#ifndef FISHSCHED_H_
#define FISHSCHED_H_


#include "fishTime.h"
#include <bitset>


class FishSched: public FishTime{  
  private:
    FishTime *fishTime;
    int myHour;
    int adjHour;
    int myMin;
    int myMo;
    int myDay;
    int myYear;
    int curTime;
    int currHour;
    int currMinute;
    int curHour;
    int currentMinute;
    int hourCount = 0;
    int oneHourAlarm = 0;
    int twoHourAlarm = 0;
    int threeHourAlarm = 0;
    int fourHourAlarm = 0;
    int eightHourAlarm = 0;
    int twelveHourAlarm = 0;
    int oneDayAlarm = 0;
    int twoDayAlarm = 0;
    int sevenDayAlarm = 0;
    int fourteenDayAlarm = 0;
    int thirtyDayAlarm = 0;
    bool firstHour = true;
    enum{Hour,Fifteen,Thirty,TwoHour,ThreeHour,FourHour,EightHour,TwelveHour,
        Midnight,OneAm,TwoAm,ThreeAm,FourAm,FiveAm,SixAm,SevenAm,EightAm,NineAm,TenAm,ElevenAm,
        Noon,OnePm,TwoPm,ThreePm,FourPm,FivePm,SixPm,SevenPm,EightPm,NinePm,TenPm,ElevenPm,
        Day,EveryOtherDay,Week,EveryOtherWeek,Month,NumFlags
        };
    //std::bitset<NumFlags> flags;
    int flags[NumFlags] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int nf = NumFlags;


typedef uint64_t flags_t;

  public:
    FishSched();
    void updateMyTime();
        void tick(); //sets hour
        void tock(); //sets minute   
        void blowHourHorn(int hr);
        void blow15MinuteHorn(int m);
        void blow30MinuteHorn(int m);
        void blow2HourHorn();
        void blow3HourHorn();
        void blow4HourHorn();
        void blow8HourHorn();
        void blow12HourHorn();
        void addHour();
        void addDay();
        //template<size_t N>
        //std::bitset<N> getFlags();
        bool isFlagSet(int flag);
        void resetFlag(int flag);
        void setFlag(int evt,int val);

        void printArray();

       

};

#endif
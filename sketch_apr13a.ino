#include <dht.h>
#include<LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
dht DHT;
#define DHT11_PIN A0
int in = 8;
int Reset=6;
int start=7;
int steps=9;
int coordination=10;
int count=0,i=0,k=0,rate=0,sp=0,dp=0;

int LAS=-1;    //less amount of steps
int LCAL=-1;   //less coordination in the arm or legs
int HBP=0;    //high blood pressure
int IBT=0;    //increase body temperature
int HHR=0;    //High heart rate
int LHR=0;    //low heart rate
int LADS=0;   //less amount of deep sleep
int INS=0;    //insomnia
int LAAT=0;   //large amount of awake time in between sleep

int HA=0;     //Headache
int VN=0;     //Vomiting or Nausea
int VC=0;     //Vision changes
int SZ=0;     //Seizures
int WP=0;     //Walking Problem
int DS=0;     //Drawsiness or Sleeping problem
int FG=0;     //Fatigue

int hat=0,vnt=0,vct=0,szt=0,wpt=0,dst=0,fgt=0;    //sum of CS for measuring SS

int tot=0;
float bt=0;

unsigned long time2,time1,sleep,isleep,asleep;
unsigned long time;
byte heart[8] = 
{
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};
void setup()
{
  lcd.createChar(1, heart);
  lcd.begin(16,2);
  
  lcd.print("Detection of ");
  //lcd.write(1);
  lcd.setCursor(0,1);
  lcd.print("Brain Tumor");
  pinMode(in, INPUT);
  pinMode(Reset, INPUT);
  pinMode(start, INPUT);
  pinMode(steps, INPUT);
  pinMode(coordination, INPUT);
  digitalWrite(Reset, HIGH);
  digitalWrite(start, HIGH);
  digitalWrite(steps, HIGH);
  digitalWrite(coordination, HIGH);
  delay(1000);
}
void loop()
{
  //************less amount of steps*************
  if(!(digitalRead(steps)))
      {
        LAS=1;
      }
  /*if(digitalRead(steps))
      {
        LAS=-1;
      }*/
  if(!(digitalRead(coordination)))
      {
        LCAL=1;
      }
  /*if(digitalRead(coordination))
      {
        LCAL=-1;
      }*/
  //******end of lack coordination in the arm or legs*******
  
  //**********Heart rate measurement****************
  if(!(digitalRead(start)))
  {
    k=0;
    lcd.clear();
    lcd.print("Please wait.......");
    while(k<5)
    {
     if(digitalRead(in))
     {
      if(k==0)
      time1=millis();
      k++;
      int chk = DHT.read11(DHT11_PIN);
      while(digitalRead(in));
     }
    }
    
      time2=millis();
      rate=time2-time1;
      rate=rate/5;
      rate=60000/rate;
      
      lcd.clear();
      lcd.print("HR:");
      //lcd.setCursor(0,1);
      lcd.print(rate);
      lcd.write(1); 
      lcd.print(",");   
    //*************end or HR measurement*****************
      
    //***************temperature measurement****************
    DHT.temperature=(((DHT.temperature*9)/5)+32+19);
    lcd.print("T:");
    lcd.print(DHT.temperature);
    lcd.print((char)223);
    //lcd.print("F");   
    //**************end of temperature measurement**********

      //*************blood pressure measurement **************   
      lcd.setCursor(0,1);
       if(rate>104 &&  DHT.temperature>98)
      {
        sp=(0.383*rate)+108.222;
        dp=(0.03021*rate)+89.1288;
      }
      else if (rate>75)
      {
        sp=(0.2411*rate)+106.27;
        dp=(0.0547*rate)+82.25;
      }
      else
      {
        sp=(0.2481*rate)+98.0867;
        dp=(0.1231*rate)+69.23;
      }
      lcd.print("SBP:");
      lcd.print(sp);
      lcd.print(",DBP:");
      lcd.print(dp);
      //************end of blood pressure measurement***********

      //*****************High blood pressure********************
      if(sp>=140 && dp>=90)
      {
        HBP=1;
      }
      else
      {
        HBP=-1;
      }
      //***************end of high blood pressure***************

      //*************Increase body temperature********************
      if(DHT.temperature>=98)
      {
        IBT=1;
      }
      else
      {
        IBT=-1;
      }
      //*************end of increase body temperature***********

      //*****************High Heart Rate********************
      if(rate>=101)
      {
        HHR=1;
      }
      else
      {
        HHR=-1;
      }
      //***************end of hign heart rate***************

      //*****************Low Heart Rate********************
      if(rate<60)
      {
        LHR=1;
      }
      else
      {
        LHR=-1;
      }
      //***************end of low heart rate***************

      //*************Less amount of deep sleep*************
      if(rate<60 && DHT.temperature<97)
      {
        sleep=millis();
        sleep=sleep/60;
        if(sleep<40)
        {
          LADS=1;
        }
        else
        {
          LADS=-1;
        }
      }
      
      //************end of less amount of deep sleep**********
      
      //**********************Insomnia************************
      if(rate<80 && DHT.temperature>=97)
      {
        isleep=millis();
        isleep=2*isleep/60;
        if(isleep<240)
        {
          INS=1;
        }
        else
        {
          INS=-1;
        }
      }
      
      //*******************end of insomnia********************
      
      //**************large amount of awake time**************
      if(rate<40 && DHT.temperature>=97)
      {
        asleep=millis();
        asleep=asleep/60;
        if(asleep>=35)
        {
          LAAT=1;
        }
        else
        {
          LAAT=-1;
        }
      }
      
      //***********end of large amount of awake time**********

      //==================Vomiting or Nausea=================
      vnt=IBT+HHR+HBP;
      if(vnt>=0)
      {
        VN=1;
      }
      else
      {
        VN=-1;
      }
      //================end of vomiting or nausea============
      //=====================Headache========================
      hat=HBP+IBT+VN;
      if(hat>=0)
      {
        HA=1;
      }
      else
      {
        HA=-1;
      }
      //==================end of headache====================
      //=================Vision changes======================
      vct=LHR+HBP+HA+VN;
      if(vct>=0)
      {
        VC=1;
      }
      else
      {
        VC=-1;
      }
      //==============end of vision changes==================
      //=====================seizures========================
      szt=HHR+HBP;
      if(szt>=0)
      {
        SZ=1;
      }
      else
      {
        SZ=-1;
      }
      //=================end of seizures=====================
      //=================walking problem=====================
      wpt=LAS+LCAL;
      if(wpt>=0)
      {
        WP=1;
      }
      else
      {
        WP=-1;
      }
      //===============end of walking problem================
      //===================Drowsiness========================
      dst=INS+LADS;
      if(dst>=0)
      {
        DS=1;
      }
      else
      {
        DS=-1;
      }
      //=================end of drowsiness===================
      //======================fatigue========================
      fgt=INS+HA+LAAT+VC;
      if(fgt>=0)
      {
        FG=1;
      }
      else
      {
        FG=-1;
      }
      //===================end of fatigue====================
      //*=*=*=*=*=*=*=*=*=Show SS value*=*=*=*=*=*=*=*=*=*=*=
      delay(5000);
      lcd.clear();
      lcd.print("HA:");
      lcd.print(HA);
      lcd.print(", VN:");
      lcd.print(VN);
      lcd.setCursor(0,1);
      lcd.print("VC:");
      lcd.print(VC);
      lcd.print(", SZ:");
      lcd.print(SZ);
      delay(2000);
      lcd.clear();
      lcd.print("WP:");
      lcd.print(WP);
      lcd.print(", DS:");
      lcd.print(DS);
      lcd.setCursor(0,1);
      lcd.print("FG:");
      lcd.print(FG);
      //*=*=*=*=*=*=*=*=*=*=*=*END=*=*=*=*=*=*=**=*=*=*=*=*=*
      //-----------------------------------------------------
      //-----------------Total Brain Tumor-------------------
      tot=(HA+VN+VC+SZ+WP+DS+FG);
      if(tot==7)
      {
        bt=99.99;
      }
      else if(tot==6)
      {
        bt=99.75;
      }
      else if(tot==5)
      {
        bt=99.33;
      }
      else if(tot==4)
      {
        bt=98.20;
      }
      else if(tot==3)
      {
        bt=95.25;
      }
      else if(tot==2)
      {
        bt=88.08;
      }
      else if(tot==1)
      {
        bt=73.11;
      }
      else if(tot==0)
      {
        bt=50.00;
      }
      else if(tot==-1)
      {
        bt=26.89;
      }
      else if(tot==-2)
      {
        bt=11.92;
      }
      else if(tot==-3)
      {
        bt=4.74;
      }
      else if(tot==-4)
      {
        bt=1.79;
      }
      else if(tot==-5)
      {
        bt=0.67;
      }
      else if(tot==-6)
      {
        bt=0.24;
      }
      else if(tot==-7)
      {
        bt=0.09;
      }
      //-------------------------END-------------------------
      k=0;
      rate=0;
      DHT.temperature=0;
      delay(5000);
      lcd.clear();
      lcd.print("Brain Tumor Poss");
      lcd.setCursor(0,1);
      lcd.print("ibility:");
      lcd.print(bt);
      lcd.print("%");
      asleep=0;
      isleep=0;
      sleep=0;
      LCAL=-1;
      LAS=-1;
      LADS=0;
      HBP=0;
      IBT=0;
      HHR=0;
      LHR=0;
      LAAT=0;
      INS=0;
      hat=0;
      vnt=0;
      vct=0;
      szt=0;
      wpt=0;
      dst=0;
      fgt=0;
      tot=0;
      HA=0;
      VN=0;
      VC=0;
      SZ=0;
      WP=0;
      DS=0;
      FG=0;
    }
  if(!digitalRead(Reset))
  {
      rate=0;
      sp=0;
      dp=0;
      bt=0;
      DHT.temperature=0;
      lcd.clear();
      lcd.print("HR:");
      lcd.print(rate);
      lcd.write(1);
      lcd.print(",");
      lcd.print("T:");
      lcd.print(DHT.temperature);
      lcd.print((char)223);
      lcd.setCursor(0,1);
      lcd.print("SBP:");
      lcd.print(sp);
      lcd.print(",DBP:");
      lcd.print(dp);
      k=0;
      delay(5000);
      lcd.clear();
      lcd.print("Brain Tumor ");
      lcd.setCursor(0,1);
      lcd.print("Possibility:");
      lcd.print(bt);
      lcd.print("%");
      
  }
}



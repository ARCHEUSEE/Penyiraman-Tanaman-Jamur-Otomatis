#define BLYNK_PRINT Serial    
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#define BLYNK_TEMPLATE_ID "TMPLRcpSQ3TT"
#define BLYNK_DEVICE_NAME "Smart Plant"
#define BLYNK_AUTH_TOKEN "ADaF3dCWVTwFRcgd6hB1_-XhibWvo8Qc"
#define DHTPIN D5
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C LCD(0x27,16,2);  //lcd board
float humi, temp;
int pump = D6;   //pin 0 pada nodemcupin 3

char auth[] = BLYNK_AUTH_TOKEN;                         //Enter your Auth token
char ssid[] = "ARCHEUSE";                                   //Enter your WIFI name
char pass[] = "archeuse";                           //Enter your WIFI password 

SimpleTimer timer;
WidgetLCD lcd(V1); //lcd android

void sendSensor(){
  //lcd to android && LCD to board lcd 
  humi = dht.readHumidity();
  temp = dht.readTemperature();
  float h = humi;
  float t = temp;
   
  Serial.print(h);
  lcd.print(0,0,"KEADAAN");   
  LCD.setCursor(0,0);
  LCD.print("HUMI");
  LCD.setCursor(5,0);
  LCD.print(h);
  LCD.print(" ");
  lcd.print(0,1,"PUMP");      
  LCD.setCursor(0,1);
  LCD.print("PUMP");
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V0, t);
  
  if (h<70){
    Serial.println("KERING");//ke serial monitor
    lcd.print(8,0," KERING");      
    LCD.setCursor(9,0);
    LCD.print(" KERING");
    lcd.print(5,1,"ON ");         
    LCD.setCursor(5,1);
    LCD.print("ON ");
    digitalWrite(pump,HIGH);
    for(int x=0; x<=10; x++){     
      LCD.setCursor(9,1);
      LCD.print(x);
      lcd.print(9,1,x);
      delay(500);}
      lcd.clear();                  
      LCD.clear();
      digitalWrite(pump,HIGH);
      lcd.print(0,0,"MENYIRAM"); 
      LCD.setCursor(0,0);
      LCD.print("MENYIRAM");
      lcd.print(0,1,"    WAIT");    
      LCD.setCursor(0,1);
      LCD.print("    WAIT");
    for(int x=9; x>0; x--){       
      LCD.setCursor(9,1);
      LCD.print(x);
      lcd.print(9,1,x);
      delay(500);}
      lcd.clear();                  
      LCD.clear();
    }
  
  else if (h>=70&&h<81){
  Serial.println("NORMAL");
  lcd.print(8,0," NORMAL");      
  LCD.setCursor(9,0);
  LCD.print(" NORMAL");
  lcd.print(5,1,"OFF");         
  LCD.setCursor(5,1);
  LCD.print("OFF");
  digitalWrite(pump,LOW);
  }
  
  else if (h>80){
  Serial.println(" BASAH");
  lcd.print(8,0," BASAH ");      
  LCD.setCursor(9,0);
  LCD.print(" BASAH ");
  lcd.print(5,1,"OFF");         
  LCD.setCursor(5,1);
  LCD.print("OFF");
  digitalWrite(pump,LOW);
  }
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  timer.setInterval(100L, sendSensor);
  pinMode(pump,OUTPUT);
  lcd.clear();
  LCD.begin();       
  LCD.backlight();
}

void loop()
{
  Blynk.run();
  timer.run();
  delay(100);
}

#define SW_VERSION " ThinkSpeak.com" // SW version will appears at innitial LCD Display
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
WiFiClient client;
const char* MY_SSID = "Mythili5g";
const char* MY_PWD = "1029384756";
const char* TS_SERVER = "api.thingspeak.com";
String TS_API_KEY = "EWXXMTOZHUYRSZHW";
int gas=A0;
int ldr=D7;
int b=D5;
int r=D3;
#include <DHT.h>
#define DHTPIN D4   // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321)
DHT dht(DHTPIN, DHTTYPE);
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
void connectWifi()
{
  Serial.print("Connecting to " + *MY_SSID);
  WiFi.begin(MY_SSID, MY_PWD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("");
}
void setup()
{
  pinMode(gas,INPUT);
  pinMode(ldr,OUTPUT);
  pinMode(b,OUTPUT);
  pinMode(r,OUTPUT);
  dht.begin();
  lcd.begin();
  lcd.backlight();
  lcd.print("Hello, world!");
   connectWifi();
  Serial.begin(9600);  
}
void loop()
{ 
  int x=analogRead(gas);
  Serial.print("OXIGEN RATE:");
  Serial.print(x);
   delay(1000);
 float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print(F("Humidity: "));
  Serial.println(h);
  delay(1000);
  Serial.print("temp:");
   Serial.print(t);
    delay(1000);
  Serial.print("OXIGEN:");
  Serial.print(x);
    delay(1000);
  lcd.clear();
  lcd.print("Humidity:");
    lcd.print(h);
    delay(1000);
     lcd.clear();
  lcd.print("temp:");
    lcd.print(t);
    delay(1000);
     lcd.clear();
  lcd.print("TEMPRAT:");
    lcd.print(f);
    delay(1000); 
     lcd.clear();
  lcd.print("NH3,SO4:");
    lcd.print(x);
    delay(1000);
if(h>=70)
{
  digitalWrite(b,HIGH);
  digitalWrite(ldr,HIGH);
  digitalWrite(r,LOW);
  delay(4000);
}
else
{
  digitalWrite(b,LOW);
  digitalWrite(ldr,LOW);
  digitalWrite(r,HIGH);
  delay(1000);
}
if(x<=500)
{
  digitalWrite(b,HIGH);
  digitalWrite(ldr,HIGH);
  digitalWrite(r,LOW);
  delay(4000);
}
else
{
  digitalWrite(b,LOW);
  digitalWrite(ldr,LOW);
  digitalWrite(r,HIGH);
  delay(1000);
}
  if (client.connect(TS_SERVER, 80))
  {
    String postStr = TS_API_KEY;
    postStr += "&field1=";
    postStr += String(h);
       postStr += "&field2=";
    postStr += String(t);
     postStr += "&field3=";
    postStr += String(f);
     postStr += "&field4=";
    postStr += String(x);
    postStr += "\r\n\r\n";
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + TS_API_KEY + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    delay(1000);
  }
}

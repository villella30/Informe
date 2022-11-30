#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Wifi y Firebase
#define FIREBASE_HOST ""  //host name of the firebase
#define FIREBASE_AUTH ""   //token for u to connect this particular with the firebase
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

//Pantalla OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  connectToWifi();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status() != WL_CONNECTED){
    connectToWifi();
  }

  String shour = Firebase.getString("Hora");
  String sminute = Firebase.getString("Minuto");
  String scall = Firebase.getString("Llamada");

  int hour = shour.toInt();
  int minute = sminute.toInt();
  int call = scall.toInt();

  display.clearDisplay();

  if(call != 0){
    incomingCall();
    display.display();
  }

  else if(date==false){
    drawTheTime(hour, minute);
  }

  else if(date==true){
     drawDate();
  }
  display.display();
  delay(100);
}

void connectToWifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}

void drawTheTime(int hour, int minute){
  display.setTextSize(4);
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(5,20);
  if(hour<10) display.print("0");
  display.print(hour);
  display.print(":");
  if(minute<10) display.print("0");
  display.print(minute);
}

void incomingCall(){
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(26, 10);
  display.print("Llamada");
  display.setCursor(19, 30);
  display.print("entrante");
}

void drawDate(){
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,20);
  display.print(20);
  display.print("/");
  display.print(03);
  display.print("/");
  display.print(03);
}

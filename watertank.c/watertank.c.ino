#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
//#include "DHT.h"
#include <ESP8266WebServer.h>
#include <BlynkSimpleEsp8266.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
int TRIGGER = D3;
int ECHO = D2;
int led = D4;
int buzz = D8;
int fan_1 = D7;
int fan_2 = D6;
int MOTOR = D5;


const char* ssid = "TP-Link_3flr";
const char* password = "camtonobaby";
const char* host = "roushanhomeautomation.000webhostapp.com";

const char* auth = "THYPVgMzwcF0TKO_Nt2vglvWUJonCzhE";
ESP8266WebServer server(80); //instantiate server at port 80 (http port)
String page = "";

int data;

void buzzerOn(){
  
  digitalWrite(buzz, LOW);
  }
void buzzerOff(){
  digitalWrite(buzz, HIGH);
  }
void motorOn(){
   digitalWrite(MOTOR, LOW);
   }

  void motorOff(){
   digitalWrite(MOTOR, HIGH);
  }

  void fan_1_On(){

    digitalWrite(fan_1, LOW);
    }
    void fan_1_Off(){

    digitalWrite(fan_1, HIGH);
    }

  void fan_2_On(){
    
    digitalWrite(fan_2, LOW);
    }
    
     void fan_2_Off(){
    
    digitalWrite(fan_2, HIGH);
    }

    void ledOn(){

      digitalWrite(led, LOW);
      }

      
    void ledOff(){

      digitalWrite(led, HIGH);
      }


void setup(void){
  Wire.begin(2, 0);
  lcd.init();
  lcd.begin(16, 2);
  //lcd.home();
  lcd.print("Hello IOT");
  
  pinMode(TRIGGER, OUTPUT); 
  pinMode(ECHO, INPUT); 
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Netmask : ");
  Serial.println(WiFi.subnetMask());
  Serial.println("Gateway : ");
  Serial.println(WiFi.gatewayIP());
  
  server.on("/", [](){
    page = "<head><meta http-equiv=\"refresh\" content=\"3\"></head><center><h1>Home Automation System.</h1><h2>Web based Water Level monitor</h2><h3>Current water level is :-</h3> <h4>"+String(data)+"</h4></center>";
    server.send(10, "text/html", page);
  });
  server.begin();
  Serial.println("Web server started!");
  Blynk.begin(auth,ssid, password);
   //pinMode(D0, OUTPUT);
   //pinMode(D1, OUTPUT);  
   pinMode(led, OUTPUT);
   pinMode(MOTOR,OUTPUT);
   pinMode(fan_2, OUTPUT);
   pinMode(fan_1, OUTPUT);
   pinMode(buzz, OUTPUT);
   
  Serial.print(data);
  Serial.print("   hghg ");
  Serial.print(String(data));
   
}

void loop(void){
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(2); 
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIGGER, LOW);
  long duration = pulseIn(ECHO, HIGH);
  data = (duration/2) / 29.09;
  server.handleClient();
  Blynk.run();


  Serial.println("Connecting to : ");
  Serial.println("host");

  WiFiClient client;
  const int httpPort = 80;
  if(!client.connect(host, httpPort)){

    Serial.println("Connection failed.");
    return;  
    
    }

    String url = "/api/superSonicSensorData/insert.php?distance=" + String(data);
    Serial.print("Requesting URL : ");
    Serial.println(url);


   client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
    delay(500);

     
     
     
     while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
  delay(300);

  


  Serial.print(data);
  Serial.print("   inside loop ");
  Serial.print(String(data));

 // digitalWrite(fan_1, LOW);
  //digitalWrite(fan_2, LOW);

  if(data > 20){
    Serial.print("\nDistance ..\n");
    Serial.print(data);
    //Serial.print(" hsfsf ");
    motorOn();
    
    //if(data > 150){
      //  buzzerOn();
        //Serial.println("Tank is empty.");
        //} 
        buzzerOff();
    }else{
     
      Serial.print("\nDistance ...\n");
       Serial.print(data);
      
      motorOff();
      if(data < 20){
        buzzerOn();
        Serial.println("Tank is full..");
        }
      }
   //motorOn();   
  //delay(10000);
  //motorOff();  
 // delay(1000);
}

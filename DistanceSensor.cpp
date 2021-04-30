#include "DistanceSensor.h"
#include <WiFi.h>
#include <WebServer.h>


const char* ssid1 = "EE162-2.4-g";
const char* password1 = "";

const int trigP = 32;
const int echoP = 35;

long duration;
int distance;

unsigned long startMillis;  
unsigned long currentMillis;
const unsigned long period = 500;  

const int statusClosedLower = 200;     // arbitrary low distance
const int statusClosedHigher = 225;    // arbitrary high distance
const int timeSec = 20;
unsigned long currentTime;
unsigned long lastTrig = 0;
// Prompt user for distance to door: lower = dist - 5, higher = dist + 5

int doorStatus = 0;
int count = 0;
int statusChange = 0;

WebServer distServer(90);
String webSite,JSONtxt;


void WebSite(){
  String message = "<html><head>\n";
  message += "<meta http-equiv=\"refresh\" content=\"5\">\n";
  message += "</head><body>\n";
  message += "<h1>Door Status: CLOSED</h1>\n";
  message += "</body></html>\n";
  distServer.send(200,"text/html",message);
  
}

void setupDistanceSensor() {
  //Serial.begin(9600);
//IPAddress ip(192,168,3,154);
  WiFi.config(IPAddress(192,168,3,156), IPAddress(192,168,0,3), IPAddress(255,255,255,0));
  WiFi.begin(ssid1,password1);
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.println(".");
    delay(500);  
  }
  WiFi.mode(WIFI_STA);
  Serial.println(" Start Dist ");
  Serial.println(WiFi.localIP());
  distServer.on("/",WebSite);
  distServer.begin();
  //webSocket.begin();

  pinMode(trigP, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoP, INPUT);   // Sets the echoPin as an Input
  startMillis = millis();  //initial start time 
  const int timeSec = 5;
  unsigned long currentTime;// = millis();
  unsigned long lastTrig = 0;
  currentTime = millis();
  if(currentTime - lastTrig > (timeSec*1000)) {
    attachInterrupt(digitalPinToInterrupt(echoP), pollDistanceSensor, CHANGE);
  }

}

int dist = 0;


void pollDistanceSensor() 
{
  //webSocket.loop();
  distServer.handleClient();
  String doorStatusString;
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    digitalWrite(trigP, LOW);   // Makes trigPin low
    delayMicroseconds(2);       // 2 micro second delay 
    
    digitalWrite(trigP, HIGH);  // tigPin high
    delayMicroseconds(10);      // trigPin high for 10 micro seconds
    digitalWrite(trigP, LOW);   // trigPin low
    
    duration = pulseIn(echoP, HIGH);   //Read echo pin, time in microseconds
    distance= duration*0.034/2;        //Calculating actual/real distance
    
    //Serial.print("Distance = ");        //Output distance on arduino serial monitor 
    //Serial.println(distance);
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.

  }

  if (distance >= statusClosedHigher){
        count++;
        if(doorStatus != 1) {
            delayMicroseconds(100);
            doorStatus = 1;
            Serial.print("Door Status: OPENED\n");
            doorStatusString = "Door Status: OPENED\n";
            statusChange = 1;
        }
  }
    else if(distance <= statusClosedLower){
        count++;
        if(doorStatus != 2) {
            delayMicroseconds(100);
            doorStatus = 2;
            Serial.print("Door Status: OBSTRUCTION\n");
            doorStatusString = "Door Status: OBSTRUCTION\n";
            statusChange = 1;
        }
    }
    else{
        count++;
        if(doorStatus != 3) {
            delayMicroseconds(100);
            doorStatus = 3;
            Serial.print("Door Status: CLOSED\n");
            doorStatusString = "Door Status: CLOSED\n";
            statusChange = 1;
        }
    }
  String message = "<html><head>\n";
  //message += "<meta http-equiv=\"refresh\" content=\"1\">\n";
  message += "</head><body>\n";
  message += "<h1>" + doorStatusString + "</h1>\n";
  message += "</body></html>\n";
  if(statusChange == 1){
        distServer.send(200,"text/html",message);
        //delay(5000);
        count = 0;
        statusChange = 0;
    }

}

#include "webApp.h"
#include "DoorControls.h"

int fixedCode = 256;
// Replace with your network credentials
const char* ssid = "EE162-2.4-g";
const char* password = "";

// Set LED GPIO
const int ledPin = 2;
// Stores LED state
String ledState;

const char* PARAM_INPUT_1 = "userDoorRequest";

// Create AsyncWebServer object on port 80
AsyncWebServer ayncServer(80);


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<body>

<h3>Garage Door Control</h3>

<p>Enter a fixed code if you have one.</p>

<input type="number" id="myNumber" value="256">

<p>Click the button to OPEN/CLOSE the Garage Door</p>

<button onclick="myFunction()">OPEN/CLOSE</button>

<p id="demo"></p>

<script>

function myFunction() {
  var xhr = new XMLHttpRequest();
  var x = document.getElementById("myNumber").value;
  document.getElementById("demo").innerHTML = x;
  xhr.open("GET", "/update?userDoorRequest="+x, true);
  xhr.send();
}
</script>

</body>
</html>
)rawliteral";



// Replaces placeholder with LED state value
String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(ledPin)){
      ledState = "ON";
    }
    else{
      ledState = "OFF";
    }
    Serial.print(ledState);
    return ledState;
  }
  return String();
}

String doorCode(const String& var) {
  Serial.println(var);
  //if(var == "BUTTONPLACEHOLDER"){
  String buttons = "";
  return String();
}
 
void setupWeb(){
  // Serial port for debugging purposes
  //Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  //IPAddress ip(192,168,3,154);
  WiFi.config(IPAddress(192,168,3,154), IPAddress(192,168,0,2), IPAddress(255,255,255,0));
  WiFi.begin(ssid, password);
  
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  ayncServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/home.html", String(), false, processor);
  });
  
  // Route to go to login page
  ayncServer.on("/login", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/login.html", String(), false, processor);
  });
  
  // Route to go to create account
  ayncServer.on("/create_account", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/createAccount.html", String(), false, processor);
  });

  /// Route to go to main menu
  ayncServer.on("/menu", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/menu.html", String(), false, processor);
  });

  ayncServer.on("/doorControl", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, doorCode);
  });
  
  ayncServer.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      //userDoorRequest = true;
      fixedCode = inputMessage1.toInt();
      changeDoorState(fixedCode);
      Serial.print("Fixed Code: ");
      Serial.println(fixedCode);
    }
    else {
      inputMessage1 = "No message sent";
    }
    Serial.print("Input: ");
    Serial.println(inputMessage1);
    request->send(200, "text/plain", "OK");
    
  });
  

  /*// Route to sign out
  ayncServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/menu.html", String(), false, processor);
  });*/

  
  // Start server
  ayncServer.begin();
}
 
void loopApp(){
 /* if(userDoorRequest){
    changeDoorState(fixedCode);
    userDoorRequest = false;
  }
  delay(5000);
  userDoorRequest = true;*/
}

//include ESP8266 library
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

int button = D1; //D2(gpio4)
int buttonState=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);                 //Serial connection
  WiFi.begin("Dillon", "stingray");   //WiFi connection


  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(5000);
    Serial.println("Waiting for connection");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected"); 
  }
  
}

void loop() {

  buttonState=digitalRead(button); // put your main code here, to run repeatedly:
  if (buttonState == 1) {
    // put your main code here, to run repeatedly:
    if (WiFi.status()== WL_CONNECTED){   //Check WiFi connection status

      StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
      JsonObject& JSONencoder = JSONbuffer.createObject(); 

      //photoresistor sensor
      int sensorValue = analogRead(A0);   // read the input on analog pin 0
  
      Serial.println(sensorValue);   // print out the value you read

      if (sensorValue > 1000) {
        JSONencoder["lights"] = "on";
      }

      else if (sensorValue < 1000) {
        JSONencoder["lights"] = "off";
      }


      char JSONmessageBuffer[300];
      JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
      Serial.println(JSONmessageBuffer);
 
      HTTPClient http;    //Declare object of class HTTPClient
 
      http.begin("http://enjiela.pythonanywhere.com/api/add");      //Specify request destination
      http.addHeader("Content-Type", "application/json");  //Specify content-type header
 
      int httpCode = http.POST(JSONmessageBuffer);//Send the request
    
      String payload = http.getString();                  //Get the response payload

      Serial.println("Code:");
      Serial.println(httpCode);   //Print HTTP return code
      Serial.println("Payload:");
      Serial.println(payload);    //Print request response payload
 
      http.end();  //Close connection
 
    }
    else {
 
      Serial.println("Error in WiFi connection");   
 
    }

 
    delay(2000);  //Send a request every 30 seconds
  }
  else if (buttonState == 0) {
    
    delay(200);
  }
  
 
}

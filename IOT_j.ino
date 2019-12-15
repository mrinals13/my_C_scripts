#include "arduino.h"
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define DHTTYPE DHT11
#define dht_dpin D4
DHT dht(dht_dpin, DHTTYPE); 

float hum;
float temp;
const int lightPin = A0 ;
const char* ssid = "MotoG";
const char* password = "11001100";
int ldr ;

void setup()
{
    Serial.begin(115200);
    dht.begin();
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print("\nConnecting...");
    }
    pinMode(lightPin,INPUT) ;
}

void loop()
{
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    ldr = analogRead(lightPin) ;
    //Displaying the data in serial monitor
    Serial.print("\n\nHumidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");
    Serial.println("LDR: ");
    Serial.println(ldr);

    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
  //Replace ##YourNodeID## with the Node ID generated in the website
        String url = "http://overwatch.pulseflare.com/api/weather/insert.php?nid=5a97dd6e58baa&temp=" + String(temp) + "&hum="+ String(hum);
        http.begin(url);
        int httpCode = http.GET();
        if (httpCode > 0)
        {
      //Check return JSON payload
            String payload = http.getString();
            Serial.println(payload);
        }
        http.end();
    }
    //Don't remove or decrease this delay, sometimes ESP8266 needs 2 seconds delay to re-initiate http requests
    delay(2000);
}

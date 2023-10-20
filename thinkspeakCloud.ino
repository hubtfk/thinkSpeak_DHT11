#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

#define DHTPIN D4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Your-SSID";
const char* password = "Password";

const char* apiKey = "160G9E3MTRIU69ZD";
const char* serverAddress = "http://api.thingspeak.com/update";

void setup() {
  dht.begin();
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
}

void loop() {

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read DHT sensor data");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("%\tTemperature: ");
    Serial.print(temperature);
    Serial.println("°C");
  }
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverAddress);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  String postData = "api_key=" + String(apiKey) + "&field1=" + String(temperature) + "&field2=" + String(humidity);

  int httpCode = http.POST(postData);

  if (httpCode > 0) {
    Serial.print("HTTP Response Code: ");
    Serial.println(httpCode);
  } else {
    Serial.println("HTTP POST failed.");
  }
  http.end();

 delay(3000);
}

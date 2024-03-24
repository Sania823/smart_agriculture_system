#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <DHT.h>

#define MOISTURE_SENSOR_PIN A0
#define DHTPIN 2
#define DHTTYPE DHT11
#define MOTOR_PIN 5
#define THRESHOLD_MOISTURE 700

#define WIFI_SSID "Home"
#define WIFI_PASS "88888888"
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "dhanu2703"
#define AIO_KEY "aio_XFfb82W8beBzpPt2UGPj2766N5wC"

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

void setup() {
  pinMode(MOISTURE_SENSOR_PIN, INPUT);
  pinMode(MOTOR_PIN, OUTPUT);

  Serial.begin(115200);
  delay(10);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("WiFi connected");
  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int moisture = analogRead(MOISTURE_SENSOR_PIN); 

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(1000);
    return;
  }

  publishToAdafruitIO(humidity, temperature);
  delay(2000);

  Serial.print("Moisture Level: ");
  Serial.println(moisture);
  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);

  if (moisture > THRESHOLD_MOISTURE) {
    digitalWrite(MOTOR_PIN, HIGH);
    Serial.println("Moisture above threshold!");
  } else {
    digitalWrite(MOTOR_PIN, LOW);
    Serial.println("Moisture below threshold.");
  }
}

void publishToAdafruitIO(float humidity, float temperature) {
  if (!mqtt.connected()) {
    int8_t ret;
    if ((ret = mqtt.connect()) != 0) {
      Serial.println("Error: Failed to connect to Adafruit IO");
      Serial.println(mqtt.connectErrorString(ret));
      return;
    }
  }

  Adafruit_MQTT_Publish humidityFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");
  Adafruit_MQTT_Publish temperatureFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
  
  if (!humidityFeed.publish(humidity)) 
  {
    Serial.println("Error: Failed to publish humidity to Adafruit IO");
  }
  if (!temperatureFeed.publish(temperature)) 
  {
    Serial.println("Error: Failed to publish temperature to Adafruit IO");
  }
  mqtt.disconnect();
}
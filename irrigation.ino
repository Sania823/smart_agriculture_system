#include <DHT.h>
#define MOISTURE_SENSOR_PIN A0
#define DHTPIN 2
#define DHTTYPE DHT11
#define MOTOR_PIN 5
#define THRESHOLD_MOISTURE 1000


DHT dht(DHTPIN, DHTTYPE);
void setup()
{
   pinMode(MOISTURE_SENSOR_PIN, INPUT);
  Serial.begin(9600);
  dht.begin();
  pinMode(MOTOR_PIN, OUTPUT);
}

void loop()
{
  delay(2000); 
  int moistureLevel = analogRead(MOISTURE_SENSOR_PIN);
  float temperature = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(temperature)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
   Serial.print("Moisture Level: ");
  Serial.println(moistureLevel);
  Serial.print("Temperature: ");
  Serial.println(temperature);
   Serial.print("Humidity: ");
   Serial.print(h);
  

  
  if (moistureLevel > THRESHOLD_MOISTURE )
  {
    digitalWrite(MOTOR_PIN, HIGH);
    Serial.println("\nMOISTURE above threshold! .");
  } 
  else 
  {

    digitalWrite(MOTOR_PIN, LOW);
    Serial.println("\nMOISTURE below threshold. .");
  }
}

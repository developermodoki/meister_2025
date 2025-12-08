#include <DHT.h>

#define DHT_INPUT_PIN 33
#define DHT_TYPE DHT11

DHT dht(DHT_INPUT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(9600);
  //pinMode(21, INPUT);
  dht.begin();

}

void loop() {
  delay(2000);
  Serial.println("Reading Temperature.");

  float humid = dht.readHumidity();
  float temp = dht.readTemperature();

  
  if (isnan(humid) || isnan(temp)) {
    Serial.println("Failed to reading");
    return;
  }

  Serial.printf("Temp: %.2f\n", temp);
  Serial.printf("Humid: %.2f\n", humid);
  
}

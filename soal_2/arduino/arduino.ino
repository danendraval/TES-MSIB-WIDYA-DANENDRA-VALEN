#include <DHT.h>
#include <ESP8266WiFi.h> // Modul WiFI ESP8266
#include <PubSubClient.h>

// Konfigurasi DHT
#define DHTPIN D4 
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

// Konfigurasi WiFi
const char* ssid = "SSID_WIFI"; 
const char* password = "PASSWORD_WIFI"; 

// Konfigurasi MQTT
const char* mqtt_server = "broker.hivemq.com";
const char* suhu_topic = "daq1/suhu";
const char* kelembapan_topic = "daq1/kelembapan";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Setup WiFi
  setup_wifi();
  
  // Setup MQTT
  client.setServer(mqtt_server, 1883);
}

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Mengambil data dari sensor
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();

  if (isnan(suhu) || isnan(kelembapan)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Mengirim data ke MQTT Broker
  char tempString[8];
  dtostrf(suhu, 1, 2, tempString);
  client.publish(suhu_topic, tempString);

  char humString[8];
  dtostrf(kelembapan, 1, 2, humString);
  client.publish(kelembapan_topic, humString);

  delay(2000);
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      delay(5000);
    }
  }
}

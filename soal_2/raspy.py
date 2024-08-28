import paho.mqtt.client as mqtt
import Adafruit_DHT
import time

# Konfigurasi DHT
DHT_SENSOR = Adafruit_DHT.DHT11
DHT_PIN = 4  # GPIO Pin

# Konfigurasi MQTT
mqtt_server = "broker.hivemq.com"
suhu_topic = "daq1/suhu"
kelembapan_topic = "daq1/kelembapan"

client = mqtt.Client()
client.connect(mqtt_server, 1883, 60)

while True:
    humidity, temperature = Adafruit_DHT.read(DHT_SENSOR, DHT_PIN)
    
    if humidity is not None and temperature is not None:
        # Publish data ke MQTT Broker
        client.publish(suhu_topic, "{:.2f}".format(temperature))
        client.publish(kelembapan_topic, "{:.2f}".format(humidity))
    else:
        print("Failed to retrieve data from sensor")
    
    time.sleep(2)

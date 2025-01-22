#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include "config.h"

#define DHTPIN 5
#define DHTTYPE DHT22


WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

void setupWiFi() {
  Serial.print("Подключаюсь к Wi-Fi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi подключён!");
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Подключаюсь к EMQX... ");
    String clientId = "ESP8266Client-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("Подключено к EMQX!");
    } else {
      Serial.print("Ошибка подключения: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setupWiFi();

  client.setServer(mqtt_broker, mqtt_port);

  dht.begin();

  reconnectMQTT();
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  String payload = "temp:" + String(t) + ", hum:" + String(h);

  if (client.publish(mqtt_topic, payload.c_str())) {
    Serial.println("Данные отправлены: " + payload);
  } else {
    Serial.print("Ошибка отправки, код: ");
    Serial.println(client.state());
  }

  delay(5000);
}

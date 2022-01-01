#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "canvas.h"
#include "effects.h"
#include "WindowLed.h"

//#define TEST

#ifdef TEST

#define NUM_LEDS 50
#define DATA_PIN 3
#define TEST_TIME 200
const uint8_t width = 6;
const uint8_t height = 8;
const uint8_t startPoint = START_AT_BOTTOMLEFT;
const uint8_t linesDirection = LINES_HORIZONTAL;
SKIP_INFO skip[1] = {{42, 1}};
const effectInfo effectList[] = {
  { squareSnakeRandom, 10},
  { fill, -5 },
  { towards, -5 },
  { fadeer, 1 },
  { drop, 1 },
  { rainbow, -3 }
};

#else

#define NUM_LEDS 50 * 3
#define TEST_TIME 1000
const uint8_t width = 10;
const uint8_t height = 14;
const uint8_t startPoint = START_AT_BOTTOMLEFT;
const uint8_t linesDirection = LINES_VERTICAL;
Nezumikun::WL::SKIP_INFO skip[3] = {{ 0, 5 } , { 5 + 14, 3 }, { 5 + 14 + 3 + 14 * 2, 2 }};
#endif

#define DELAY 40
#define WIFI_CHECK_PERIOD 500

Nezumikun::WindowLed::Canvas canvas(width, height);
Nezumikun::WindowLed::Effect* effect;

Nezumikun::WL::WindowLed wl(NUM_LEDS, 3, skip, 3, width, height, startPoint, linesDirection);
CRGB leds[NUM_LEDS];
uint8_t ledsHue[NUM_LEDS];
uint8_t hue = 0;

unsigned long prevLeds = 0;
unsigned long prevWifi = 0;

const char * WiFi_SSID = "XiaomiDev";
const char * WiFi_PASS = "XiaoMiDev";
const char * mqtt_server = "192.168.2.202";
const char * mqtt_auth = "mqtt";
#define MSG_BUFFER_SIZE	(500)
char msg[MSG_BUFFER_SIZE];

WiFiClient wifi;
PubSubClient mqtt(wifi);

byte mac[6];
String deviceId;
String mqtt_topicConfig;
String mqtt_topicSet;
String mqtt_topicState;

void mqtt_publish_state() {
  mqtt.publish(mqtt_topicState.c_str(), wl.getState() ? "ON" : "OFF");
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if ((length == 3) && (memcmp(payload, "OFF", 3) == 0)) {
    wl.off();
    mqtt_publish_state();
  }
  else if ((length == 2) && (memcmp(payload, "ON", 2) == 0)) {
    wl.on();
    mqtt_publish_state();
  }
}

void setup() {
  prevLeds = millis();
  Serial.begin(115200);
  delay(1000);
  Serial.println("Intialized");
  wl.begin();
  mqtt.setServer(mqtt_server, 1883);
  mqtt.setCallback(mqtt_callback);
  effect = new Nezumikun::WindowLed::EffectRise(canvas, 25);
  do {
    effect->loop();
    delay(1);
  } while (!effect->endOfEffect());

}

bool check_wifi() {
  uint8_t wifi_status = WiFi.status();
  static bool wifi_connecting = false;
  if ((wifi_status != WL_CONNECTED) && !wifi_connecting) {
    Serial.println();
    Serial.print("Connecting to ");
    Serial.print(WiFi_SSID);
    Serial.println("...");
    WiFi.begin(WiFi_SSID, WiFi_PASS);
    wifi_connecting = true;
  } else if (wifi_connecting && (wifi_status != 7)) {
    wifi_connecting = false;
    if (wifi_status == WL_CONNECTED) {
      Serial.println();
      Serial.print("WiFi connected to ");
      Serial.print(WiFi_SSID);
      Serial.print(" IP: ");
      Serial.println(WiFi.localIP());
      WiFi.macAddress(mac);
      deviceId = String("0x");
      for (int i = 0; i < 6; i++) {
        deviceId += String(mac[i], HEX);
      }
      Serial.println(String("DeviceID = ") + deviceId);
    }
  }
  return wifi_status == WL_CONNECTED;
}

void check_mqtt() {
  static bool mqtt_connecting = false;
  static bool reconnect_pause = false;
  static uint8_t reconnect_wait = 0;
  if (reconnect_pause && (reconnect_wait-- > 0)) {
    return;
  }
  while (!mqtt_connecting && !mqtt.connected()) {
    mqtt_connecting = true;
    Serial.println();
    Serial.print("Attempting MQTT connection (Current state = ");
    Serial.print(mqtt.state());
    Serial.print(")... ");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqtt.connect(clientId.c_str(), mqtt_auth, mqtt_auth)) {
      Serial.println("connected");
      reconnect_pause = false;
      mqtt_topicConfig = String("homeassistant/switch/window_led/") + deviceId + "/config";
      mqtt_topicSet = String("homeassistant/switch/window_led/") + deviceId + "/set";
      mqtt_topicState = String("homeassistant/switch/window_led/") + deviceId + "/state";
      String payload = String("{\"name\": \"Window.Led\", \"command_topic\": \"") + mqtt_topicSet
        + String("\", \"state_topic\": \"") + mqtt_topicState + String("\"}");
      mqtt.publish(mqtt_topicConfig.c_str(), payload.c_str());
      mqtt_publish_state();
      if (mqtt.subscribe(mqtt_topicSet.c_str())) {
        Serial.println(String("Subscribed to ") + mqtt_topicSet);
      }
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(" try again in 5 seconds");
      reconnect_pause = true;
      reconnect_wait = 20;
    }
    mqtt_connecting = false;
  }
}

void loop() {
  delay(1);
  return;
  unsigned long now = millis();
  if (now - prevLeds >= DELAY) {
    prevLeds = now;
    wl.update(now);
  }
  if (now - prevWifi >= WIFI_CHECK_PERIOD) {
    prevWifi = now;
    if (check_wifi()) {
      check_mqtt();
      if (mqtt.connected()) {
        mqtt.loop();
      }
    }
  }
}
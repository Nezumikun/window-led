#include <Arduino.h>
#ifndef BOARD_ARDUINO_NANO
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 1
#endif
#include "lights.h"
#include "uptime.h"
#include "LED.h"

#define FRAME_PER_SECOND 50
#define DELAY 1000 / FRAME_PER_SECOND
#define WIFI_CHECK_PERIOD 5000
#define NUM_LEDS 50 * 3

#ifndef WIDTH
#define WIDTH 10
#endif
#ifndef HEIGHT
#define HEIGHT 14
#endif
Nezumikun::WindowLed::SkipInfo _skip[3] = {{ 0, 5 } , { 5 + 14, 3 }, { 5 + 14 + 3 + 14 * 2, 2 }};

CRGB leds[NUM_LEDS];
Nezumikun::WindowLed::Lights lights(&leds[0], NUM_LEDS, WIDTH, HEIGHT, FRAME_PER_SECOND);
Nezumikun::Uptime uptime;

#ifndef WITHOUT_WIFI
#include <ESP8266WiFi.h>
//#include <PubSubClient.h>

byte mac[6];
String deviceId;
String mqtt_topicConfig;
String mqtt_topicSet;
String mqtt_topicState;
String mqtt_topicUptime;

Nezumikun::LED led_wifi(D1);

unsigned long prevWifi = 0;
const char * WiFi_SSID = "XiaomiDev";
const char * WiFi_PASS = "XiaoMiDev";
const char * mqtt_server = "192.168.2.202";
const char * mqtt_auth = "mqtt";
#define MSG_BUFFER_SIZE	(500)
char msg[MSG_BUFFER_SIZE];

WiFiClient wifi;
//PubSubClient mqtt(wifi);
/*
void mqtt_publish_state() {
  mqtt.publish(mqtt_topicState.c_str(), lights.isOn() ? "ON" : "OFF");
}

void mqtt_publish_uptime() {
  String strUptime = uptime.toString();
  Serial.println(String("Uptime: ") + strUptime);
  mqtt.publish(mqtt_topicUptime.c_str(), strUptime.c_str());
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print(F("Message arrived ["));
  Serial.print(topic);
  Serial.print(F("] "));
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if ((length == 3) && (memcmp(payload, "OFF", 3) == 0)) {
    lights.off();
    mqtt_publish_state();
    Serial.println(F("Lights OFF"));
  }
  else if ((length == 2) && (memcmp(payload, "ON", 2) == 0)) {
    lights.on();
    mqtt_publish_state();
    Serial.println(F("Lights ON"));
  }
}
*/
bool check_wifi() {
  uint8_t wifi_status = WiFi.status();
  static bool wifi_connecting = false;
  if ((wifi_status != WL_CONNECTED) && !wifi_connecting) {
    led_wifi.off();
    Serial.print(F("Connecting to "));
    Serial.print(WiFi_SSID);
    Serial.println("...");
    WiFi.begin(WiFi_SSID, WiFi_PASS);
    wifi_connecting = true;
  } else if (wifi_connecting && (wifi_status != 7)) {
    wifi_connecting = false;
    if (wifi_status == WL_CONNECTED) {
      led_wifi.blink(500);
      Serial.print(F("WiFi connected to "));
      Serial.println(WiFi_SSID);
      Serial.print(F("IP: "));
      Serial.println(WiFi.localIP());
      WiFi.macAddress(mac);
      deviceId = String("0x");
      for (int i = 0; i < 6; i++) {
        deviceId += String(mac[i], HEX);
      }
      Serial.println(String("DeviceID = ") + deviceId);
    } else {
      Serial.println(String("Status = ") + wifi_status);
    }
  }
  return wifi_status == WL_CONNECTED;
}
/*
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
    Serial.print(F("Attempting MQTT connection (Current state = "));
    Serial.print(mqtt.state());
    Serial.print(F(")... "));
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqtt.connect(clientId.c_str(), mqtt_auth, mqtt_auth)) {
      Serial.println("connected");
      reconnect_pause = false;
      mqtt_topicConfig = String("homeassistant/switch/window_led/") + deviceId + "/config";
      mqtt_topicSet = String("homeassistant/switch/window_led/") + deviceId + "/set";
      mqtt_topicState = String("homeassistant/switch/window_led/") + deviceId + "/state";
      mqtt_topicUptime = String("homeassistant/switch/window_led/") + deviceId + "/uptime";
      String payload = String("{\"name\": \"Window.Led\", \"command_topic\": \"") + mqtt_topicSet
        + String("\", \"state_topic\": \"") + mqtt_topicState + String("\"}");
      mqtt.publish(mqtt_topicConfig.c_str(), payload.c_str());
      mqtt_publish_state();
      if (mqtt.subscribe(mqtt_topicSet.c_str())) {
        Serial.println(String("Subscribed to ") + mqtt_topicSet);
      }
    } else {
      Serial.print(F("failed, rc="));
      Serial.print(mqtt.state());
      Serial.println(" try again in 5 seconds");
      reconnect_pause = true;
      reconnect_wait = 20;
    }
    mqtt_connecting = false;
  }
} */
#else
Nezumikun::LED led_wifi(13);
#endif

unsigned long prevLeds = 0;
unsigned char prevMinutes = -1;

void setup() {
  prevLeds = 500;
  Serial.begin(115200);
  led_wifi.blink(500);
  delay(500);
  Serial.print("Intialized ");
  Serial.print(WIDTH);
  Serial.print("x");
  Serial.print(HEIGHT);
  Serial.println();
#ifdef BOARD_ARDUINO_NANO
  FastLED.addLeds<WS2812B, 2, GRB>(leds, NUM_LEDS);
#else
  FastLED.addLeds<WS2812B, D2, GRB>(leds, NUM_LEDS);
#endif
  //Nezumikun::WindowLed::Settings::debugLevel = Nezumikun::WindowLed::DebugLevel::Debug;
#ifndef DEBUG
  lights.setSkipInfo(_skip, 3);
#else
  lights.setStartAt(Nezumikun::WindowLed::Lights::StartAt::BottomLeft);
  lights.setLinesDirectoin(Nezumikun::WindowLed::Lights::LinesDirection::Horizontal);
#endif
  uptime.reset();
#ifndef WITHOUT_WIFI
  //mqtt.setServer(mqtt_server, 1883);
  //mqtt.setCallback(mqtt_callback);
#endif
}

void loop() {
  unsigned long now = millis();
  if (now - prevLeds >= DELAY) {
    prevLeds = prevLeds + DELAY;
    lights.loop();
  }
  uptime.loop();
#ifdef DEBUG
  //delay(50);
#endif
  if (prevMinutes != uptime.info.minutes) {
    String strUptime = uptime.toString();
    Serial.println(String("Uptime: ") + strUptime);
    prevMinutes = uptime.info.minutes;
  }
#ifndef WITHOUT_WIFI
  if (now - prevWifi >= WIFI_CHECK_PERIOD) {
    prevWifi = now;
    if (check_wifi()) {
      /*check_mqtt();
      if (mqtt.connected()) {
        if (prevMinutes != uptime.info.minutes) {
          mqtt_publish_uptime();
          prevMinutes = uptime.info.minutes;
        }
        mqtt.loop();
      }*/
    }
  }
#else
#endif
  led_wifi.touch();
}
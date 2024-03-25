#define WIFI_SSID "MAKHOME"
#define WIFI_PASS "dUjochoO"
#define BOT_TOKEN "6440579019:AAH0gd0oeaCKRaG0FgAXqB2234irdnC1VfI"

#include <WiFi.h>
#include <FastBot.h>


FastBot bot(BOT_TOKEN);
void setup() {
  connectWiFi();
  bot.attach(newMsg);
}
// обработчик сообщений
void newMsg(FB_msg& msg) {
  // выводим ID чата, имя юзера и текст сообщения
  Serial.print(msg.chatID);     // ID чата 
  Serial.print(", ");
  Serial.print(msg.username);   // логин
  Serial.print(", ");
  Serial.println(msg.text);     // текст
}
void loop() {
  bot.tick();
}
void connectWiFi() {
  delay(2000);
  Serial.begin(115200);
  Serial.println();
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() > 15000) ESP.restart();
  }
  Serial.println("Connected");
}
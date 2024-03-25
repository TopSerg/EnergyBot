#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h> 
#include <ArduinoJson.h>

const char* ssid = "MAKHOME";      //RMS                      TODO Смена вай фая
const char* password = "dUjochoO"; //456852123                TODO подготовка кода
                                                            //TODO Презентация
#define BOTtoken "6440579019:AAH0gd0oeaCKRaG0FgAXqB2234irdnC1VfI" 
 
#define CHAT_ID "-1001947930912"                            //TODO Придумать как сделать GetId
 
#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif
 
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
 

int botRequestDelay = 1000;
unsigned long lastTimeBotRan;
 
const int ledPin = 2;
const int inPin = 23;
bool ledState = LOW;
bool a = false;
 

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
 
  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    String text = bot.messages[i].text;
    Serial.println(text);
 
    String from_name = bot.messages[i].from_name;
    
    if (text == "/status") {
      if(!a){
        bot.sendMessage(CHAT_ID, "Электричество есть.", "");
      }
      else{
        bot.sendMessage(CHAT_ID, "Электричества нет.", "");
      }
    }
  }
}
 
void setup() {
  Serial.begin(115200);
 
  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");  
    client.setTrustAnchors(&cert); 
  #endif
 
  pinMode(ledPin, OUTPUT);
  pinMode(inPin, INPUT);
  digitalWrite(ledPin, ledState);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); 
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println(WiFi.localIP());
 
  bot.sendMessage(CHAT_ID, "Bot Started", "");
}
 
void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
 
    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
  if(digitalRead(inPin) == 0 && !a){
    a = true;
    bot.sendMessage(CHAT_ID, "ВНИМАНИЕ! ОТКЛЮЧЕНИЕ ЭЛЕКТРИЧЕСТВА!", "");
  }
  else if(digitalRead(inPin) == 1 && a){
    a = false;
    bot.sendMessage(CHAT_ID, "УРА! МЫ СНОВА В СЕТИ!", "");
  }
}
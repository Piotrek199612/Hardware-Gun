#include "communication.hpp"
#include <ESP8266WiFi.h>
#include <espnow.h>

uint8_t Communication::remoteMac[] = {0x2C, 0x3A, 0xE8, 0x2E, 0xF1, 0x65};

void Communication::init(){
    WiFi.mode(WIFI_STA); // Station mode for esp-now sensor node
    WiFi.disconnect();

    Serial.printf("This mac: %s, ", WiFi.macAddress().c_str()); 
    Serial.printf("target mac: %02x%02x%02x%02x%02x%02x", Communication::remoteMac[0], Communication::remoteMac[1], Communication::remoteMac[2], Communication::remoteMac[3], Communication::remoteMac[4], Communication::remoteMac[5]); 
    Serial.printf(", channel: %i\n", Communication::WIFI_CHANNEL); 
  
    if (esp_now_init() != 0) {
      Serial.println("*** ESP_Now init failed");
    }
  
    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_add_peer(Communication::remoteMac, ESP_NOW_ROLE_SLAVE, Communication::WIFI_CHANNEL, NULL, 0);
    
    esp_now_register_send_cb([](uint8_t* mac, uint8_t sendStatus) {
      //Serial.printf("send_cb, send done, status = %i\n", sendStatus);
    });
}


void Communication::Send(uint8_t data[],uint8_t data_length){
  esp_now_send(NULL, data, data_length); // NULL means send to all peers
}

void Communication::Send(uint8_t id,uint8_t decimal,uint8_t fraction,uint8_t data_length){
  uint8_t tmp[3] {id,decimal,fraction};
  esp_now_send(NULL, tmp, data_length); // NULL means send to all peers
}

void Communication::Send_Number(uint8_t id,float number){
  uint8_t data_length = 3;
  uint8_t decimal = int(number);
  uint8_t fraction = (number - int(number))*100;
  uint8_t tmp[3] {id,decimal,fraction};
  esp_now_send(NULL, tmp, data_length); // NULL means send to all peers
  delay(10);
}


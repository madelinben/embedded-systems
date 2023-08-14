#include "WiFiSetup.h"
#include "WiFiConfig.h"
#include <WiFi.h>
#include <ESP32Ping.h>

void getInternetConfiguration() {
    Serial.print("SSID: "); Serial.println(WiFi.SSID());
    Serial.print("RSSI: "); Serial.println(WiFi.RSSI());
    Serial.print("MAC: "); Serial.println(WiFi.macAddress());
    Serial.print("IP: "); Serial.println(WiFi.localIP());
    Serial.print("GATEWAY: "); Serial.println(WiFi.gatewayIP());
    Serial.print("DNS: "); Serial.println(WiFi.dnsIP());
}

void testInternetConnection() {
    const char* url = "www.google.com";
    if(Ping.ping(url)) {
        Serial.println("Successful ping to test domain.");
    } else {
        Serial.println("Failed to ping test domain.");
    }
}

void listAvailableNetworks() {
    int nodes = WiFi.scanNetworks();
    Serial.println("Network scan complete.");
    if (nodes == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(nodes); Serial.println(" networks found.");
        for (int i=0; i<nodes; ++i) {
            Serial.println(WiFi.SSID(i));
        }
    }
}

void setupWiFi(){
    listAvailableNetworks();
    Serial.println(WiFi.macAddress());

    long lastChange = millis();
    // WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while ((WiFi.status() != WL_CONNECTED) && ((millis() - lastChange) >= BOUNCE_DELAY_MS)) {
        Serial.println("Still Connecting to WiFi.");
    }
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi failed to connect.");
    } else {
        Serial.println("WiFi Connected.");
        getInternetConfiguration();
        testInternetConnection();
    }
}

/* void postHttp(char* url, char* payload) {
    // parse json payload
    String jsonString;
    serializeJson(payload, jsonString);

    // format http request
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    http.POST(jsonString);
    
    // response
    Serial.println(http.getString());
} */

/* void getHttp(char* url) {
    // format http request
    http.begin(url);
    http.GET();

    String jsonResponse = http.getString();
    Serial.println(jsonResponse);

    DynamicJsonDocument<1024> jsonDocument;
    deserializeJson(jsonDocument, jsonResponse);
    JsonObject jsonObject = jsonDocument.as<JsonObject>();
} */
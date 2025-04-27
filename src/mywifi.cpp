#include <ESPAsyncWebServer.h>
#include "mywifi.h"
#include "config.h"
#include <WiFi.h>
#include <statusled.h>


AsyncWebServer server(80); // Serwer HTTP

void startAPMode() {
    Serial.println("Starting Access Point mode...");
    WiFi.softAP("SmartMeter", "smartmeter123"); 
    Serial.println("Access Point started. Connect to 'SmartMeter' with password 'smartmeter123'.");
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", "<h1>SmartMeter Configuration</h1><form action='/save' method='POST'>"
                                        "SSID: <input type='text' name='ssid'><br>"
                                        "Password: <input type='password' name='password'><br>"
                                        "MQTT Server: <input type='text' name='mqtt_server'><br>"
                                        "MQTT Port: <input type='number' name='mqtt_port'><br>"
                                        "<input type='submit' value='Save'></form>");
    });

    server.on("/save", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("ssid", true) && request->hasParam("password", true) &&
            request->hasParam("mqtt_server", true) && request->hasParam("mqtt_port", true)) {
            String ssid = request->getParam("ssid", true)->value();
            String password = request->getParam("password", true)->value();
            String mqtt_server = request->getParam("mqtt_server", true)->value();
            int mqtt_port = request->getParam("mqtt_port", true)->value().toInt();

            strncpy(config.ssid, ssid.c_str(), sizeof(config.ssid));
            strncpy(config.password, password.c_str(), sizeof(config.password));
            strncpy(config.mqtt_server, mqtt_server.c_str(), sizeof(config.mqtt_server));
            config.mqtt_port = mqtt_port;

            saveConfig();
            request->send(200, "text/html", "<h1>Configuration Saved!</h1><p>Restarting...</p>");
            delay(2000);
            ESP.restart();
        } else {
            request->send(400, "text/html", "<h1>Invalid Input</h1>");
        }
    });

    server.begin();
}


void setupWebServer() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        String html = "<h1>Device Configuration</h1>"
                      "<form action='/save' method='POST'>"
                      "WiFi SSID: <input type='text' name='ssid' value='" + String(config.ssid) + "'><br>"
                      "WiFi Password: <input type='password' name='password'><br>"
                      "MQTT Server: <input type='text' name='mqtt_server' value='" + String(config.mqtt_server) + "'><br>"
                      "MQTT Port: <input type='number' name='mqtt_port' value='" + String(config.mqtt_port) + "'><br>"
                      "Measurement Interval (ms): <input type='number' name='meas_itv' value='" + String(config.meas_itv) + "'><br>"
                      "LED Brightness (0-255): <input type='number' name='led_brightness' value='" + String(config.led_brightness) + "'><br>"
                      "<input type='submit' value='Save'>"
                      "</form>";
        request->send(200, "text/html", html);
    });

    server.on("/save", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("ssid", true) && request->hasParam("password", true) &&
            request->hasParam("mqtt_server", true) && request->hasParam("mqtt_port", true) &&
            request->hasParam("meas_itv", true) && request->hasParam("led_brightness", true)) {
            
            String ssid = request->getParam("ssid", true)->value();
            String password = request->getParam("password", true)->value();
            String mqtt_server = request->getParam("mqtt_server", true)->value();
            int mqtt_port = request->getParam("mqtt_port", true)->value().toInt();
            int meas_itv = request->getParam("meas_itv", true)->value().toInt();
            int led_brightness = request->getParam("led_brightness", true)->value().toInt();

            strncpy(config.ssid, ssid.c_str(), sizeof(config.ssid));
            strncpy(config.password, password.c_str(), sizeof(config.password));
            strncpy(config.mqtt_server, mqtt_server.c_str(), sizeof(config.mqtt_server));
            config.mqtt_port = mqtt_port;
            config.meas_itv = meas_itv;
            config.led_brightness = led_brightness;

            saveConfig();
            setLEDBrightness(config.led_brightness); // Apply brightness immediately

            request->send(200, "text/html", "<h1>Configuration Saved!</h1><p>Device will restart...</p>");
            delay(2000);
            ESP.restart();
        } else {
            request->send(400, "text/html", "<h1>Invalid Input</h1><p>Please fill all fields correctly.</p>");
        }
    });

    server.begin();
    Serial.println("Web server started.");
}

void setupMyWiFi() {
    if (strlen(config.ssid) == 0 || strlen(config.password) == 0) {
        Serial.println("WiFi credentials not set. Please configure via serial.");
        configureWiFi();
        return;
    }

    Serial.print("Connecting to WiFi: ");
    Serial.println(config.ssid);

    WiFi.begin(config.ssid, config.password);
    int retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < 20) {
        delay(500);
        Serial.print(".");
        retries++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        setupWebServer();
    } else {
        Serial.println("\nFailed to connect to WiFi.");
        startAPMode();
        setupWebServer();
    }
}

void updateMyWiFiStatus() {
    // TODO : status polaczenia wifi na led rgb
}

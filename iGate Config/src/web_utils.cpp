#include <ArduinoJson.h>
#include "configuration.h"
#include "ota_utils.h"
#include "web_utils.h"
#include "utils.h"

extern Configuration               Config;
extern uint32_t                    lastBeaconTx;
extern std::vector<ReceivedPacket> receivedPackets;

extern const char web_index_html[] asm("_binary_data_embed_index_html_gz_start");
extern const char web_index_html_end[] asm("_binary_data_embed_index_html_gz_end");
extern const size_t web_index_html_len = web_index_html_end - web_index_html;

extern const char web_style_css[] asm("_binary_data_embed_style_css_gz_start");
extern const char web_style_css_end[] asm("_binary_data_embed_style_css_gz_end");
extern const size_t web_style_css_len = web_style_css_end - web_style_css;

extern const char web_script_js[] asm("_binary_data_embed_script_js_gz_start");
extern const char web_script_js_end[] asm("_binary_data_embed_script_js_gz_end");
extern const size_t web_script_js_len = web_script_js_end - web_script_js;

extern const char web_bootstrap_css[] asm("_binary_data_embed_bootstrap_css_gz_start");
extern const char web_bootstrap_css_end[] asm("_binary_data_embed_bootstrap_css_gz_end");
extern const size_t web_bootstrap_css_len = web_bootstrap_css_end - web_bootstrap_css;

extern const char web_bootstrap_js[] asm("_binary_data_embed_bootstrap_js_gz_start");
extern const char web_bootstrap_js_end[] asm("_binary_data_embed_bootstrap_js_gz_end");
extern const size_t web_bootstrap_js_len = web_bootstrap_js_end - web_bootstrap_js;


namespace WEB_Utils {

    AsyncWebServer server(80);

    void handleNotFound(AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(404, "text/plain", "Not found");
        response->addHeader("Cache-Control", "max-age=3600");
        request->send(response);
    }

    void handleStatus(AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "OK");
    }

    void handleHome(AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", (const uint8_t*)web_index_html, web_index_html_len);
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    }

    void handleReadConfiguration(AsyncWebServerRequest *request) {
        File file = SPIFFS.open("/igate_conf.json");
        
        String fileContent;
        while(file.available()){
            fileContent += String((char)file.read());
        }

        request->send(200, "application/json", fileContent);
    }

    void handleReceivedPackets(AsyncWebServerRequest *request) {
        StaticJsonDocument<1536> data;

        for (int i = 0; i < receivedPackets.size(); i++) {
            data[i]["millis"] = receivedPackets[i].millis;
            data[i]["packet"] = receivedPackets[i].packet;
            data[i]["RSSI"] = receivedPackets[i].RSSI;
            data[i]["SNR"] = receivedPackets[i].SNR;
        }

        String buffer;

        serializeJson(data, buffer);

        request->send(200, "application/json", buffer);
    }

    void handleWriteConfiguration(AsyncWebServerRequest *request) {
        Serial.println("Got new config from www");

        int networks = request->getParam("wifi.APs", true)->value().toInt();

        Config.wifiAPs = {};

        for (int i=0; i<networks; i++) {
            WiFi_AP wifiap;
            wifiap.ssid                   = request->getParam("wifi.AP." + String(i) + ".ssid", true)->value();
            wifiap.password               = request->getParam("wifi.AP." + String(i) + ".password", true)->value();
            // wifiap.latitude               = request->getParam("wifi.AP." + String(i) + ".latitude", true)->value().toDouble();
            // wifiap.longitude              = request->getParam("wifi.AP." + String(i) + ".longitude", true)->value().toDouble();

            Config.wifiAPs.push_back(wifiap);
        }

        Config.callsign = request->getParam("callsign", true)->value();
        // Config.iGateComment = request->getParam("iGateComment", true)->value();
        
        Config.wifiAutoAP.password = request->getParam("wifi.autoAP.password", true)->value();
        Config.wifiAutoAP.powerOff = request->getParam("wifi.autoAP.powerOff", true)->value().toInt();

        Config.digi.mode = request->getParam("digi.mode", true)->value().toInt();
        // Config.digi.comment = request->getParam("digi.comment", true)->value();
        // Config.digi.latitude = request->getParam("digi.latitude", true)->value().toDouble();
        // Config.digi.longitude = request->getParam("digi.longitude", true)->value().toDouble();

        Config.tnc.enableServer = request->hasParam("tnc.enableServer", true);
        Config.tnc.enableSerial = request->hasParam("tnc.enableSerial", true);
        Config.tnc.acceptOwn = request->hasParam("tnc.acceptOwn", true);

        Config.aprs_is.active = request->hasParam("aprs_is.active", true);
        Config.aprs_is.passcode = request->getParam("aprs_is.passcode", true)->value();
        Config.aprs_is.server = request->getParam("aprs_is.server", true)->value();
        Config.aprs_is.port = request->getParam("aprs_is.port", true)->value().toInt();
        // Config.aprs_is.reportingDistance = request->getParam("aprs_is.reportingDistance", true)->value().toInt();
        Config.aprs_is.filter = request->getParam("aprs_is.filter", true)->value();
        Config.aprs_is.toRF = request->hasParam("aprs_is.toRF", true);
        
        // Config.loramodule.iGateFreq = request->getParam("lora.iGateFreq", true)->value().toInt();
        // if (request->hasParam("lora.digirepeaterTxFreq", true)) {
        //     Config.loramodule.digirepeaterTxFreq = request->getParam("lora.digirepeaterTxFreq", true)->value().toInt();
        // }
        // if (request->hasParam("lora.digirepeaterRxFreq", true)) {
        //     Config.loramodule.digirepeaterRxFreq = request->getParam("lora.digirepeaterRxFreq", true)->value().toInt();
        // }

        Config.loramodule.txFreq = request->getParam("lora.txFreq", true)->value().toInt();
        Config.loramodule.rxFreq = request->getParam("lora.rxFreq", true)->value().toInt();
        Config.loramodule.spreadingFactor = request->getParam("lora.spreadingFactor", true)->value().toInt();
        Config.loramodule.signalBandwidth = request->getParam("lora.signalBandwidth", true)->value().toInt();
        Config.loramodule.codingRate4 = request->getParam("lora.codingRate4", true)->value().toInt();
        Config.loramodule.power = request->getParam("lora.power", true)->value().toInt();
        Config.loramodule.txActive = request->hasParam("lora.txActive", true);
        Config.loramodule.rxActive = request->hasParam("lora.rxActive", true);

        Config.display.alwaysOn = request->hasParam("display.alwaysOn", true);

        if (!Config.display.alwaysOn) {
            Config.display.timeout = request->getParam("display.timeout", true)->value().toInt();
        }

        Config.display.turn180 = request->hasParam("display.turn180", true);

        Config.syslog.active = request->hasParam("syslog.active", true);

        if (Config.syslog.active) {
            Config.syslog.server = request->getParam("syslog.server", true)->value();
            Config.syslog.port = request->getParam("syslog.port", true)->value().toInt();
        }

        Config.bme.active = request->hasParam("bme.active", true);
        Config.bme.heightCorrection = request->getParam("bme.heightCorrection", true)->value().toInt();
        Config.bme.temperatureCorrection = request->getParam("bme.temperatureCorrection", true)->value().toFloat();

        Config.ota.username = request->getParam("ota.username", true)->value();
        Config.ota.password = request->getParam("ota.password", true)->value();

        Config.beacon.interval = request->getParam("beacon.interval", true)->value().toInt();
        Config.beacon.sendViaAPRSIS = request->hasParam("beacon.sendViaAPRSIS", true);
        Config.beacon.sendViaRF = request->hasParam("beacon.sendViaRF", true);
        Config.beacon.latitude = request->getParam("beacon.latitude", true)->value().toDouble();
        Config.beacon.longitude = request->getParam("beacon.longitude", true)->value().toDouble();
        Config.beacon.comment = request->getParam("beacon.comment", true)->value();
        Config.beacon.overlay = request->getParam("beacon.overlay", true)->value();
        Config.beacon.symbol = request->getParam("beacon.symbol", true)->value();
        Config.beacon.path = request->getParam("beacon.path", true)->value();

        // Config.beaconInterval = request->getParam("other.beaconInterval", true)->value().toInt();
        // Config.igateSendsLoRaBeacons = request->hasParam("other.igateSendsLoRaBeacons", true);
        // Config.igateRepeatsLoRaPackets = request->hasParam("other.igateRepeatsLoRaPackets", true);
        Config.rememberStationTime = request->getParam("other.rememberStationTime", true)->value().toInt();
        Config.sendBatteryVoltage = request->hasParam("other.sendBatteryVoltage", true);
        Config.externalVoltageMeasurement = request->hasParam("other.externalVoltageMeasurement", true);

        if (Config.externalVoltageMeasurement) {
            Config.externalVoltagePin = request->getParam("other.externalVoltagePin", true)->value().toInt();
        }

        Config.lowPowerMode = request->hasParam("other.lowPowerMode", true);
        Config.lowVoltageCutOff = request->getParam("other.lowVoltageCutOff", true)->value().toDouble();

        if (Config.bme.active) {
            Config.beacon.symbol = "_";
        }

        Config.writeFile();

        AsyncWebServerResponse *response = request->beginResponse(302, "text/html", "");
        response->addHeader("Location", "/");
        request->send(response);

        ESP.restart();
    }

    void handleAction(AsyncWebServerRequest *request) {
        String type = request->getParam("type", false)->value();

        if (type == "send-beacon") {
            lastBeaconTx = 0;

            request->send(200, "text/plain", "Beacon will be sent in a while");
        } else if (type == "reboot") {
            ESP.restart();
        } else {
            request->send(404, "text/plain", "Not Found");
        }
    }

    void handleStyle(AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/css", (const uint8_t*)web_style_css, web_style_css_len);
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    }

    void handleScript(AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/javascript", (const uint8_t*)web_script_js, web_script_js_len);
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    }

    void handleBootstrapStyle(AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/css", (const uint8_t*)web_bootstrap_css, web_bootstrap_css_len);
        response->addHeader("Content-Encoding", "gzip");
        response->addHeader("Cache-Control", "max-age=3600");
        request->send(response);
    }

    void handleBootstrapScript(AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/javascript", (const uint8_t*)web_bootstrap_js, web_bootstrap_js_len);
        response->addHeader("Content-Encoding", "gzip");
        response->addHeader("Cache-Control", "max-age=3600");
        request->send(response);
    }

    void setup() {
        server.on("/", HTTP_GET, handleHome);
        server.on("/status", HTTP_GET, handleStatus);
        server.on("/received-packets.json", HTTP_GET, handleReceivedPackets);
        server.on("/configuration.json", HTTP_GET, handleReadConfiguration);
        server.on("/configuration.json", HTTP_POST, handleWriteConfiguration);
        server.on("/action", HTTP_POST, handleAction);
        server.on("/style.css", HTTP_GET, handleStyle);
        server.on("/script.js", HTTP_GET, handleScript);
        server.on("/bootstrap.css", HTTP_GET, handleBootstrapStyle);
        server.on("/bootstrap.js", HTTP_GET, handleBootstrapScript);

        OTA_Utils::setup(&server); // Include OTA Updater for WebServer

        server.onNotFound(handleNotFound);

        server.begin();
    }

}
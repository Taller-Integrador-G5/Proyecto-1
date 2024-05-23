/*______________________________________________________________________________________________________________

██╗      ██████╗ ██████╗  █████╗      █████╗ ██████╗ ██████╗ ███████╗    ██╗ ██████╗  █████╗ ████████╗███████╗
██║     ██╔═══██╗██╔══██╗██╔══██╗    ██╔══██╗██╔══██╗██╔══██╗██╔════╝    ██║██╔════╝ ██╔══██╗╚══██╔══╝██╔════╝
██║     ██║   ██║██████╔╝███████║    ███████║██████╔╝██████╔╝███████╗    ██║██║  ███╗███████║   ██║   █████╗  
██║     ██║   ██║██╔══██╗██╔══██║    ██╔══██║██╔═══╝ ██╔══██╗╚════██║    ██║██║   ██║██╔══██║   ██║   ██╔══╝  
███████╗╚██████╔╝██║  ██║██║  ██║    ██║  ██║██║     ██║  ██║███████║    ██║╚██████╔╝██║  ██║   ██║   ███████╗
╚══════╝ ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝    ╚═╝  ╚═╝╚═╝     ╚═╝  ╚═╝╚══════╝    ╚═╝ ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚══════╝

                                          Ricardo Guzman - CA2RXU 
                              https://github.com/richonguzman/LoRa_APRS_Tracker
                                 (donations : http://paypal.me/richonguzman)                                                                       
______________________________________________________________________________________________________________*/

#include <ElegantOTA.h>
#include <Arduino.h>
#include <WiFi.h>
#include <vector>
#include "configuration.h"
#include "battery_utils.h"
#include "aprs_is_utils.h"
#include "station_utils.h"
#include "boards_pinout.h"
#include "syslog_utils.h"
#include "query_utils.h"
#include "power_utils.h"
#include "lora_utils.h"
#include "wifi_utils.h"
#include "digi_utils.h"
#include "gps_utils.h"
#include "bme_utils.h"
#include "web_utils.h"
#include "tnc_utils.h"
#include "display.h"
#include "utils.h"
#ifdef ESP32_DIY_LoRa_A7670
    #include "A7670_utils.h"
#endif

String          versionDate             = "2024.05.17";
Configuration   Config;
WiFiClient      espClient;
uint8_t         myWiFiAPIndex           = 0;
int             myWiFiAPSize            = Config.wifiAPs.size();
WiFi_AP         *currentWiFi            = &Config.wifiAPs[myWiFiAPIndex];

bool            isUpdatingOTA           = false;
uint32_t        previousWiFiMillis      = 0;
uint32_t        lastScreenOn            = millis();

uint32_t        lastWiFiCheck           = 0;
bool            WiFiConnect             = true;
bool            WiFiConnected           = false;
bool            WiFiAutoAPStarted       = false;
long            WiFiAutoAPTime          = false;

uint32_t        lastBatteryCheck        = 0;
String          batteryVoltage;

std::vector<ReceivedPacket> receivedPackets;

bool            modemLoggedToAPRSIS     = false;

String firstLine, secondLine, thirdLine, fourthLine, fifthLine, sixthLine, seventhLine;

//#define STARTUP_DELAY 5 //min

void setup() {
    Serial.begin(115200);
    POWER_Utils::setup();
    Utils::setupDisplay();
    Config.check();
    LoRa_Utils::setup();
    Utils::validateFreqs();
    GPS_Utils::generateBeacons();

    #ifdef STARTUP_DELAY    // (TEST) just to wait for WiFi init of Routers
    show_display("", "  STARTUP DELAY ...", "", "", 0);
    delay(STARTUP_DELAY * 60 * 1000);
    #endif

    #ifdef HELTEC_HTCT62
        if (Config.lowPowerMode) {
            gpio_wakeup_enable(GPIO_NUM_3, GPIO_INTR_HIGH_LEVEL);
            esp_deep_sleep_enable_gpio_wakeup(GPIO_NUM_3, ESP_GPIO_WAKEUP_GPIO_HIGH);
            long lastBeacon = 0;
            LoRa_Utils::startReceive();
            while (true) {
                auto wakeup_reason = esp_sleep_get_wakeup_cause();
                if (wakeup_reason == 7) { // packet received
                    Serial.println("Received packet");
                    String packet = LoRa_Utils::receivePacket();
                    Serial.println(packet);
                    if (Config.digi.mode == 2) DIGI_Utils::processLoRaPacket(packet);

                    if (packet.indexOf(Config.callsign + ":?APRSELP{") != -1) { // Send `?APRSELP` to exit low power
                        Serial.println("Got ?APRSELP message, exiting from low power mode");
                        break;
                    };
                }
                long time = esp_timer_get_time() / 1000000;
                if (lastBeacon == 0 || time - lastBeacon >= Config.beacon.interval * 60) {
                    Serial.println("Sending beacon");
                    String comment = Config.beacon.comment;
                    if (Config.sendBatteryVoltage) comment += " Batt=" + String(BATTERY_Utils::checkBattery(),2) + "V";
                    if (Config.externalVoltageMeasurement) comment += " Ext=" + String(BATTERY_Utils::checkExternalVoltage(),2) + "V";
                    STATION_Utils::addToOutputPacketBuffer(GPS_Utils::getiGateLoRaBeaconPacket() + comment);                
                    lastBeacon = time;
                }

                LoRa_Utils::startReceive();
                Serial.println("Sleeping");
                long sleep = (Config.beacon.interval * 60) - (time - lastBeacon);
                Serial.flush();
                esp_sleep_enable_timer_wakeup(sleep * 1000000);
                esp_light_sleep_start();
                Serial.println("Waked up");
            }
            Config.loramodule.rxActive = false;
        }
    #endif

    WIFI_Utils::setup();
    SYSLOG_Utils::setup();
    BME_Utils::setup();
    WEB_Utils::setup();
    TNC_Utils::setup();
    #ifdef ESP32_DIY_LoRa_A7670
        A7670_Utils::setup();
    #endif
}

void loop() {
    WIFI_Utils::checkIfAutoAPShouldPowerOff();

    if (isUpdatingOTA) {
        ElegantOTA.loop();
        return; // Don't process IGate and Digi during OTA update
    }

    if (Config.lowVoltageCutOff > 0) {
        BATTERY_Utils::checkIfShouldSleep();
    }

    thirdLine = Utils::getLocalIP();

    WIFI_Utils::checkWiFi(); // Always use WiFi, not related to IGate/Digi mode
    // Utils::checkWiFiInterval();

    #ifdef ESP32_DIY_LoRa_A7670
        if (Config.aprs_is.active && !modemLoggedToAPRSIS) A7670_Utils::APRS_IS_connect();
    #else
        if (Config.aprs_is.active && !espClient.connected()) APRS_IS_Utils::connect();
    #endif

    TNC_Utils::loop();

    Utils::checkDisplayInterval();
    Utils::checkBeaconInterval();
    
    APRS_IS_Utils::checkStatus(); // Need that to update display, maybe split this and send APRSIS status to display func?

    String packet = "";
    if (Config.loramodule.rxActive) {
        packet = LoRa_Utils::receivePacket(); // We need to fetch LoRa packet above APRSIS and Digi
    } 

    if (packet != "") {
        if (Config.aprs_is.active) { // If APRSIS enabled
            APRS_IS_Utils::processLoRaPacket(packet); // Send received packet to APRSIS
        }

        if (Config.digi.mode == 2) { // If Digi enabled
            DIGI_Utils::processLoRaPacket(packet); // Send received packet to Digi
        }

        if (Config.tnc.enableServer) { // If TNC server enabled
            TNC_Utils::sendToClients(packet); // Send received packet to TNC KISS
        }
        if (Config.tnc.enableSerial) { // If Serial KISS enabled
            TNC_Utils::sendToSerial(packet); // Send received packet to Serial KISS
        }
    }

    if (Config.aprs_is.active) { // If APRSIS enabled
        APRS_IS_Utils::listenAPRSIS(); // listen received packet from APRSIS
    }

    STATION_Utils::processOutputPacketBuffer();
    STATION_Utils::clean25SegBuffer();

    show_display(firstLine, secondLine, thirdLine, fourthLine, fifthLine, sixthLine, seventhLine, 0);
}
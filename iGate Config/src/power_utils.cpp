#include "configuration.h"
#include "boards_pinout.h"
#include "power_utils.h"

#if defined(HAS_AXP192) || defined(HAS_AXP2101)
    #define I2C_SDA 21
    #define I2C_SCL 22
    #define IRQ_PIN 35
#endif

#ifdef HAS_AXP192
    XPowersAXP192 PMU;
#endif
#ifdef HAS_AXP2101
    XPowersAXP2101 PMU;
#endif

extern Configuration    Config;


namespace POWER_Utils {

    void activateMeasurement() {
        #if defined(HAS_AXP192) || defined(HAS_AXP2101)
            PMU.disableTSPinMeasure();
            PMU.enableBattDetection();
            PMU.enableVbusVoltageMeasure();
            PMU.enableBattVoltageMeasure();
            PMU.enableSystemVoltageMeasure();
        #endif
    }

    void activateLoRa() {
        #ifdef HAS_AXP192
            PMU.setLDO2Voltage(3300);
            PMU.enableLDO2();
        #endif
        #ifdef HAS_AXP2101
            PMU.setALDO2Voltage(3300);
            PMU.enableALDO2();
        #endif
    }

    void deactivateLoRa() {
        #ifdef HAS_AXP192
            PMU.disableLDO2();
        #endif
        #ifdef HAS_AXP2101
            PMU.disableALDO2();
        #endif
    }

    bool begin(TwoWire &port) {
        #if defined(HAS_AXP192)
            bool result = PMU.begin(Wire, AXP192_SLAVE_ADDRESS, I2C_SDA, I2C_SCL);
            if (result) {
                PMU.disableDC2();
                PMU.disableLDO2();
                PMU.disableLDO3();
                PMU.setDC1Voltage(3300);
                PMU.enableDC1();
                PMU.setProtectedChannel(XPOWERS_DCDC3);
                PMU.disableIRQ(XPOWERS_AXP192_ALL_IRQ);
            }
            return result;
        #elif defined(HAS_AXP2101)
            bool result = PMU.begin(Wire, AXP2101_SLAVE_ADDRESS, I2C_SDA, I2C_SCL);
            if (result) {
                PMU.disableDC2();
                PMU.disableDC3();
                PMU.disableDC4();
                PMU.disableDC5();
                PMU.disableALDO1();
                PMU.disableALDO4();
                PMU.disableBLDO1();
                PMU.disableBLDO2();
                PMU.disableDLDO1();
                PMU.disableDLDO2();
                PMU.setDC1Voltage(3300);
                PMU.enableDC1();
                PMU.setButtonBatteryChargeVoltage(3300);
                PMU.enableButtonBatteryCharge();
                PMU.disableIRQ(XPOWERS_AXP2101_ALL_IRQ);
            }
            return result;
        #else
            return true;
        #endif
    }

    void setup() {
        #ifdef HAS_AXP192
            Wire.begin(SDA, SCL);
            if (begin(Wire)) {
                Serial.println("AXP192 init done!");
            } else {
                Serial.println("AXP192 init failed!");
            }
            activateLoRa();
            activateMeasurement();
            PMU.setChargerTerminationCurr(XPOWERS_AXP192_CHG_ITERM_LESS_10_PERCENT);
            PMU.setChargeTargetVoltage(XPOWERS_AXP192_CHG_VOL_4V2);
            PMU.setChargerConstantCurr(XPOWERS_AXP192_CHG_CUR_780MA);
            PMU.setSysPowerDownVoltage(2600);
        #endif

        #ifdef HAS_AXP2101
            Wire.begin(SDA, SCL);
            if (begin(Wire)) {
                Serial.println("AXP2101 init done!");
            } else {
                Serial.println("AXP2101 init failed!");
            }
            activateLoRa();
            activateMeasurement();
            PMU.setPrechargeCurr(XPOWERS_AXP2101_PRECHARGE_200MA);
            PMU.setChargerTerminationCurr(XPOWERS_AXP2101_CHG_ITERM_25MA);
            PMU.setChargeTargetVoltage(XPOWERS_AXP2101_CHG_VOL_4V2);
            PMU.setChargerConstantCurr(XPOWERS_AXP2101_CHG_CUR_800MA);
            PMU.setSysPowerDownVoltage(2600);
        #endif

        #ifdef BATTERY_PIN
            pinMode(BATTERY_PIN, INPUT);
        #endif

        #ifdef INTERNAL_LED_PIN
            pinMode(INTERNAL_LED_PIN, OUTPUT);
        #endif

        if (Config.externalVoltageMeasurement) {
            pinMode(Config.externalVoltagePin, INPUT);
        }

        #ifdef VEXT_CTRL
            pinMode(VEXT_CTRL,OUTPUT); // GPS + TFT on HELTEC Wireless_Tracker and only for Oled in HELTEC V3
            #ifndef HELTEC_WSL_V3
                digitalWrite(VEXT_CTRL, HIGH);
            #endif
        #endif
        
        #ifdef ADC_CTRL
            pinMode(ADC_CTRL, OUTPUT);
        #endif

        #ifdef HELTEC_WIRELESS_TRACKER
            Wire.begin(BOARD_I2C_SDA, BOARD_I2C_SCL);
        #endif

        #ifdef HELTEC_V3
            Wire1.begin(BOARD_I2C_SDA, BOARD_I2C_SCL);
        #endif
        
        delay(1000);
    }

}
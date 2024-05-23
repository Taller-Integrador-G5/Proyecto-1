#ifndef PINS_CONFIG_H_
#define PINS_CONFIG_H_

#include <Arduino.h>

#undef OLED_SDA
#undef OLED_SCL
#undef OLED_RST


// LORA MODULES
#if defined(TTGO_T_LORA32_V2_1) || defined(HELTEC_V2) || defined(ESP32_DIY_LoRa) || defined(TTGO_T_Beam_V1_2) || defined(TTGO_T_Beam_V1_0) || defined(TTGO_T_LORA32_V2_1_915)
    #define RADIO_SCLK_PIN  5      // GPIO5    - SX1278 SCK
    #define RADIO_MISO_PIN  19      // GPIO19   - SX1278 MISO
    #define RADIO_MOSI_PIN  27      // GPIO27   - SX1278 MOSI
    #define RADIO_CS_PIN    18      // GPIO18   - SX1278 CS ---> NSS
    #define RADIO_RST_PIN   14      // GPIO14   - SX1278 RST
    #define RADIO_BUSY_PIN  26      // GPIO26   - SX1278 IRQ ---->DIO0
#endif

#if defined(HELTEC_V3) || defined(HELTEC_WSL_V3) || defined(HELTEC_WIRELESS_TRACKER) || defined(HELTEC_WS)
    #define RADIO_SCLK_PIN  9   // SX1262 SCK
    #define RADIO_MISO_PIN  11  // SX1262 MISO
    #define RADIO_MOSI_PIN  10  // SX1262 MOSI
    #define RADIO_CS_PIN    8   // SX1262 NSS
    #define RADIO_RST_PIN   12  // SX1262 RST
    #define RADIO_DIO1_PIN  14  // SX1262 DIO1
    #define RADIO_BUSY_PIN  13  // SX1262 BUSY
#endif

#ifdef ESP32_DIY_1W_LoRa    // Ebyte E22 400M30S / SX1268
    #define RADIO_SCLK_PIN  18
    #define RADIO_MISO_PIN  19
    #define RADIO_MOSI_PIN  23
    #define RADIO_CS_PIN    5
    #define RADIO_RST_PIN   27
    #define RADIO_DIO1_PIN  12
    #define RADIO_BUSY_PIN  14
    #define RADIO_RXEN      32
    #define RADIO_TXEN      25
#endif

#ifdef WEMOS_LOLIN32_OLED_DIY_LoRa
    #define RADIO_SCLK_PIN  15
    #define RADIO_MISO_PIN  13
    #define RADIO_MOSI_PIN  12
    #define RADIO_CS_PIN    14
    #define RADIO_RST_PIN   2
    #define RADIO_BUSY_PIN  25
#endif

#if defined(TTGO_T_Beam_V1_0_SX1268) || defined(TTGO_T_Beam_V1_2_SX1262)
    #define RADIO_SCLK_PIN  5
    #define RADIO_MISO_PIN  19
    #define RADIO_MOSI_PIN  27
    #define RADIO_CS_PIN    18
    #define RADIO_DIO0_PIN  26
    #define RADIO_RST_PIN   23
    #define RADIO_DIO1_PIN  33
    #define RADIO_BUSY_PIN  32
#endif

#if defined(OE5HWN_MeshCom)
    #define RADIO_SCLK_PIN      18
    #define RADIO_MISO_PIN      19
    #define RADIO_MOSI_PIN      23
    #define RADIO_CS_PIN        5
    #define RADIO_RST_PIN       27
    #define RADIO_DIO1_PIN      33
    #define RADIO_BUSY_PIN      26
    #define RADIO_RXEN          14
    #define RADIO_TXEN          13
#endif

#ifdef HELTEC_HTCT62
    #define RADIO_SCLK_PIN  10   // SX1262 SCK
    #define RADIO_MISO_PIN  6    // SX1262 MISO
    #define RADIO_MOSI_PIN  7    // SX1262 MOSI
    #define RADIO_CS_PIN    8    // SX1262 NSS
    #define RADIO_RST_PIN   5    // SX1262 RST
    #define RADIO_DIO1_PIN  3    // SX1262 DIO1
    #define RADIO_BUSY_PIN  4    // SX1262 BUSY
#endif

#ifdef ESP32_DIY_LoRa_A7670
    #define RADIO_SCLK_PIN  18
    #define RADIO_MISO_PIN  19
    #define RADIO_MOSI_PIN  23
    #define RADIO_CS_PIN    2
    #define RADIO_RST_PIN   0
    #define RADIO_BUSY_PIN  32
    #define A7670_PWR_PIN   4
    #define A7670_ResetPin  5
    #define A7670_TX_PIN    26
    #define A7670_RX_PIN    27
#endif

#ifdef WEMOS_D1_R32_RA02
    #define RADIO_SCLK_PIN  18
    #define RADIO_MISO_PIN  19
    #define RADIO_MOSI_PIN  23
    #define RADIO_CS_PIN    5
    #define RADIO_BUSY_PIN  12
    #define RADIO_RST_PIN   13
    #define RADIO_DIO1_PIN  14
    #define OLED_SDA        21
    #define OLED_SCL        22
    #define OLED_RST        36
#endif   

// OLED 
#if defined(TTGO_T_LORA32_V2_1) || defined(ESP32_DIY_LoRa) || defined(ESP32_DIY_1W_LoRa) || defined(TTGO_T_Beam_V1_0) || defined(TTGO_T_Beam_V1_2) || defined(TTGO_T_Beam_V1_0_SX1268) || defined(TTGO_T_Beam_V1_2_SX1262) || defined(OE5HWN_MeshCom) || defined(ESP32_DIY_LoRa_A7670) || defined(TTGO_T_LORA32_V2_1_915)
    #define OLED_SDA    21
    #define OLED_SCL    22
    #define OLED_RST    -1      // Reset pin # (or -1 if sharing Arduino reset pin)
#endif

#ifdef HELTEC_V2
    #define OLED_SDA    4
    #define OLED_SCL    15
    #define OLED_RST    16
#endif

#if defined(HELTEC_V3) || defined(HELTEC_WS)
    #define OLED_SDA    17
    #define OLED_SCL    18
    #define OLED_RST    21
#endif

#ifdef WEMOS_LOLIN32_OLED_DIY_LoRa
    #define OLED_SDA    5
    #define OLED_SCL    4
    #define OLED_RST    -1
#endif

#if !defined(HELTEC_HTCT62) && !defined(HELTEC_WSL_V3)
        #define HAS_DISPLAY
#endif

// Leds and other stuff
#ifdef HELTEC_HTCT62
    #define BATTERY_PIN         1
#endif
#if defined(TTGO_T_LORA32_V2_1) || defined(HELTEC_V2) || defined(TTGO_T_LORA32_V2_1_915)
    #define INTERNAL_LED_PIN    25      // Green Led
    #define BATTERY_PIN         35      // es 35 el led y 1 bateria?
#endif
#if defined(HELTEC_V3) || defined(HELTEC_WSL_V3) || defined(HELTEC_WS)
    #define INTERNAL_LED_PIN    35
    #define BATTERY_PIN         1
    #define VEXT_CTRL           36
    #define ADC_CTRL            37 // Heltec WSL_V3 just like Heltec WT
    #define BOARD_I2C_SDA       41
    #define BOARD_I2C_SCL       42
#endif

#if defined(ESP32_DIY_LoRa) || defined(ESP32_DIY_1W_LoRa)
    #define INTERNAL_LED_PIN    2
#endif
#if defined(ESP32_DIY_LoRa_A7670)
    #define INTERNAL_LED_PIN    13      // 13 for V1.1 and 12 for V1.0
    #define BATTERY_PIN         35
#endif

#ifdef HELTEC_WIRELESS_TRACKER
    #define INTERNAL_LED_PIN    18
    #define BATTERY_PIN         1
    #define ADC_CTRL            2   // HELTEC Wireless Tracker ADC_CTRL = HIGH powers the voltage divider to read BatteryPin. Only on V05 = V1.1
    #define VEXT_CTRL           3   // To turn on GPS and TFT
    #define BOARD_I2C_SDA       7
    #define BOARD_I2C_SCL       6
#endif

#ifdef ESP32_C3_DIY_LoRa
    #define OLED_SDA         8
    #define OLED_SCL         9
    #define OLED_RST         10
    #define RADIO_SCLK_PIN   4
    #define RADIO_MISO_PIN   5
    #define RADIO_MOSI_PIN   6
    #define RADIO_CS         7
    #define RADIO_RST_PIN    3
    #define RADIO_IRQ_PIN    2
#endif

/* (Same pins for LILYGO LoRa32 and ESP32 Wroom Dev )
SX1278-------------------> ESP32 ttgo-lora32-v21 and ESP32 WROOM Dev
GND                         GND
DIO1                        -
DIO2                        -
DIO3                        -
VCC                         3.3V
MISO                        19
MOSI                        27
SCLK                        5
NSS                         18
DIO0                        26
REST                        14
GND                         -  */

#endif
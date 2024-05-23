#include <Wire.h>
#include "configuration.h"
#include "boards_pinout.h"
#include "display.h"


#ifdef HAS_DISPLAY
    #ifdef HAS_TFT
        #include <TFT_eSPI.h>

        TFT_eSPI tft = TFT_eSPI(); 

        #ifdef HELTEC_WIRELESS_TRACKER
            #define bigSizeFont     2
            #define smallSizeFont   1
            #define lineSpacing     9
        #endif
    #else
        #include <Adafruit_GFX.h>
        #include <Adafruit_SSD1306.h>
        #if defined(HELTEC_V3)
            #define OLED_DISPLAY_HAS_RST_PIN
        #endif
        Adafruit_SSD1306 display(128, 64, &Wire, OLED_RST);
    #endif
#endif

extern Configuration Config;

String oldHeader, oldFirstLine, oldSecondLine, oldThirdLine, oldFourthLine, oldFifthLine, oldSixthLine;

void cleanTFT() {
    #ifdef HAS_TFT
        tft.fillScreen(TFT_BLACK);
    #endif
}

void setup_display() {
    #ifdef HAS_DISPLAY
        delay(500);
        #ifdef HAS_TFT
            tft.init();
            tft.begin();
            if (Config.display.turn180) {
                    tft.setRotation(3);
            } else {
                tft.setRotation(1);
            }
            tft.setTextFont(0);
            tft.fillScreen(TFT_BLACK);
        #else    
            #ifdef OLED_DISPLAY_HAS_RST_PIN
                pinMode(OLED_RST, OUTPUT);
                digitalWrite(OLED_RST, LOW);
                delay(20);
                digitalWrite(OLED_RST, HIGH);
            #endif
            Wire.begin(OLED_SDA, OLED_SCL);

            if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { 
                Serial.println(F("SSD1306 allocation failed"));
                for(;;); // Don't proceed, loop forever
            }
            if (Config.display.turn180) {
                display.setRotation(2);
            }
            display.clearDisplay();
            display.setTextColor(WHITE);
            display.setTextSize(1);
            display.setCursor(0, 0);
            display.ssd1306_command(SSD1306_SETCONTRAST);
            display.ssd1306_command(1);
            display.display();
        #endif
        delay(1000);
    #endif
}

void display_toggle(bool toggle) {
    #ifdef HAS_DISPLAY
        if (toggle) {
            #ifdef HAS_TFT
                digitalWrite(TFT_BL, HIGH);
            #else
                display.ssd1306_command(SSD1306_DISPLAYON);
            #endif
        } else {
            #ifdef HAS_TFT
                digitalWrite(TFT_BL, LOW);
            #else
                display.ssd1306_command(SSD1306_DISPLAYOFF);
            #endif
        }
    #endif
}

bool shouldCleanTFT(const String& header, const String& line1, const String& line2, const String& line3) {
    if (oldHeader != header || oldFirstLine != line1 || oldSecondLine != line2 || oldThirdLine != line3) {
        oldHeader       = header;
        oldFirstLine    = line1;
        oldSecondLine   = line2;
        oldThirdLine    = line3;
        return true;
    } else {
        return false;
    }
}

bool shouldCleanTFT(const String& header, const String& line1, const String& line2, const String& line3, const String& line4, const String& line5, const String& line6) {
    if (oldHeader != header || oldFirstLine != line1 || oldSecondLine != line2 || oldThirdLine != line3 || oldFourthLine != line4 || oldFifthLine != line5 || oldSixthLine != line6) {
        oldHeader       = header;
        oldFirstLine    = line1;
        oldSecondLine   = line2;
        oldThirdLine    = line3;
        oldFourthLine   = line4;
        oldFifthLine    = line5;
        oldSixthLine    = line6;
        return true;
    } else {
        return false;
    }
}

void show_display(const String& header, const String& line1, const String& line2, const String& line3, int wait) {
    #ifdef HAS_DISPLAY
        #ifdef HAS_TFT
            if (shouldCleanTFT(header, line1, line2, line3)) {
                cleanTFT();
            }
            tft.setTextColor(TFT_WHITE,TFT_BLACK);
            tft.setTextSize(bigSizeFont);
            tft.setCursor(0, 0);
            tft.print(header);
            tft.setTextSize(smallSizeFont);
            tft.setCursor(0, ((lineSpacing * 2) - 2));
            tft.print(line1);
            tft.setCursor(0, ((lineSpacing * 3) - 2));
            tft.print(line2);
            tft.setCursor(0, ((lineSpacing * 4) - 2));
            tft.print(line3);
        #else
            display.clearDisplay();
            display.setTextColor(WHITE);
            display.setTextSize(1);
            display.setCursor(0, 0);
            display.println(header);
            display.setCursor(0, 8);
            display.println(line1);
            display.setCursor(0, 16);
            display.println(line2);
            display.setCursor(0, 24);
            display.println(line3);
            display.ssd1306_command(SSD1306_SETCONTRAST);
            display.ssd1306_command(1);
            display.display();
        #endif
        delay(wait);
    #endif
}

void show_display(const String& header, const String& line1, const String& line2, const String& line3, const String& line4, const String& line5, const String& line6, int wait) {
    #ifdef HAS_DISPLAY
        #ifdef HAS_TFT
            if (shouldCleanTFT(header, line1, line2, line3, line4, line5, line6)) {
                cleanTFT();
            }
            tft.setTextColor(TFT_WHITE,TFT_BLACK);
            tft.setTextSize(bigSizeFont);
            tft.setCursor(0, 0);
            tft.print(header);
            tft.setTextSize(smallSizeFont);
            tft.setCursor(0, ((lineSpacing * 2) - 2));
            tft.print(line1);
            tft.setCursor(0, ((lineSpacing * 3) - 2));
            tft.print(line2);
            tft.setCursor(0, ((lineSpacing * 4) - 2));
            tft.print(line3);
            tft.setCursor(0, ((lineSpacing * 5) - 2));
            tft.print(line4);
            tft.setCursor(0, ((lineSpacing * 6) - 2));
            tft.print(line5);
            tft.setCursor(0, ((lineSpacing * 7) - 2));
            tft.print(line6);
        #else
            display.clearDisplay();
            display.setTextColor(WHITE);
            display.setTextSize(2);
            display.setCursor(0, 0);
            display.println(header);
            display.setTextSize(1);
            display.setCursor(0, 16);
            display.println(line1);
            display.setCursor(0, 24);
            display.println(line2);
            display.setCursor(0, 32);
            display.println(line3);
            display.setCursor(0, 40);
            display.println(line4);
            display.setCursor(0, 48);
            display.println(line5);
            display.setCursor(0, 56);
            display.println(line6);
            display.ssd1306_command(SSD1306_SETCONTRAST);
            display.ssd1306_command(1);
            display.display();
        #endif
        delay(wait);
    #endif
}
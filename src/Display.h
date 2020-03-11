#include <Arduino.h>
#include <U8g2lib.h>

class Display {
    bool encActive = false;
    U8G2_SSD1306_128X64_NONAME_F_SW_I2C driver;
    
    void render(String value) {
        driver.setFontMode(0);
        driver.setFont(u8g2_font_logisoso26_tn);		
        driver.clearBuffer();
        if(encActive) {
            driver.setCursor(0,60);
            driver.print(F(":"));
        }
        driver.setCursor(7,60);
        driver.print(value);
        driver.sendBuffer();
    };
    
    public:
    Display(uint8_t _clock,uint8_t _data): driver(U8G2_R0, /* clock=*/ _clock, /* data=*/ _data, /* reset=*/ U8X8_PIN_NONE){};
    void setValue(signed int value) {
        render(value);
    }
    void setup(){
        driver.begin();
    }
};
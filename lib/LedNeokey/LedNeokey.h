#ifdef NEOKEY
#ifndef LedNeokey_h
#define LedNeokey_h
#include "Arduino.h"
#include <Led.h>
#include <Adafruit_NeoPixel.h>

class LedNeokey {
    private:
        LedColor color;
        LedEffect effect;
        void brighten();
        void darken();
        uint32_t actual_color;
        uint8_t fade(uint32_t color, uint8_t fade_type, uint16_t time);
        uint8_t pixels0_loop0(uint32_t color, uint16_t time);
        void pixels_loop(uint32_t color, uint16_t time);
    public:
        void initPixels();
        void debugColors();
        void update();
        void setEffect(LedEffect effect);
        void setColor(LedColor color);
        class Pixels {
            public:
                uint8_t effect_np;
                uint8_t effects_np;
                uint16_t effStep;
                unsigned long effStart;
                Adafruit_NeoPixel neopixel;
                Pixels(uint16_t leds, uint8_t pin, uint16_t striptype) : neopixel(leds, pin, striptype)
                {
                    effect_np = -1;
                    // effects_np = toteffects;
                    reset();
                }
                void reset()
                {
                    effStep = 0;
                    effect_np = (effect_np + 1) % effects_np;
                    effStart = millis();
                }
        };
};

struct loop
{
    uint8_t currentChild;
    uint8_t childs;
    bool timeBased;
    uint16_t cycles;
    uint16_t currentTime;
    loop(uint8_t totchilds, bool timebased, uint16_t tottime)
    {
        currentTime = 0;
        currentChild = 0;
        childs = totchilds;
        timeBased = timebased;
        cycles = tottime;
    }
};

#endif
#endif
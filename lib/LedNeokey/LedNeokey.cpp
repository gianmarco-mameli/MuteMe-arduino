#ifdef NEOKEY
#include <LedNeokey.h>

#define PIN PIN_NEOPIXEL
#define NUMPIXELS NUM_NEOPIXEL
#define DELAYVAL 300

boolean fadeToColor = false;
const uint16_t colorSaturation=128;
const uint16_t dimFactor = 70;

uint16_t fast = 500;
uint16_t slow = 2500;
uint16_t actual_time = 0;

LedNeokey::Pixels pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
struct loop pixels0loop0(2, false, 1);

uint32_t actual_color = pixels.neopixel.Color(0,0,0);

void LedNeokey::initPixels()
{
    pixels.neopixel.begin();
    pixels.neopixel.clear();
}

void LedNeokey::debugColors() {
    LedColor colors[4] = {red, green, blue, no_color};
    for (size_t i = 0; i < 4; i++)
    {
        setColor(colors[i]);
        delay(400);
    }
}

uint8_t LedNeokey::fade(uint32_t color, uint8_t fade_type, uint16_t time)
{
    if (millis() - pixels.effStart < 5 * (pixels.effStep))
        return 0x00;
    uint8_t r, g, b;
    uint8_t u_red = (color >> 16) & 0xff;
    uint8_t u_green = (color >> 8) & 0xff;
    uint8_t u_blue = color & 0xff;
    double e;
    e = (pixels.effStep * 5) / (double)time;

    if (fade_type == 0)
    {
        r = (e)*u_red + 0 * (1.0 - e);
        g = (e)*u_green + 0 * (1.0 - e);
        b = (e)*u_blue + 0 * (1.0 - e);
    }
    else
    {
        r = (e)*0 + u_red * (1.0 - e);
        g = (e)*0 + u_green * (1.0 - e);
        b = (e)*0 + u_blue * (1.0 - e);
    }
    for (uint16_t j = 0; j < 4; j++)
    {
        if ((j % 1) == 0)
            pixels.neopixel.setPixelColor(j, r, g, b);
        else
            pixels.neopixel.setPixelColor(j, 0, 0, 0);
    }
    if (pixels.effStep >= (time / 5))
    {
        pixels.reset();
        return 0x03;
    }
    else
        pixels.effStep++;
    return 0x01;
}

uint8_t LedNeokey::pixels0_loop0(uint32_t color, uint16_t time)
{
    uint8_t ret = 0x00;
    switch (pixels0loop0.currentChild)
    {
    case 0:
        ret = fade(color, 0, time);
        break;
    case 1:
        ret = fade(color, 1, time);
        break;
    }
    if (ret & 0x02)
    {
        ret &= 0xfd;
        if (pixels0loop0.currentChild + 1 >= pixels0loop0.childs)
        {
            pixels0loop0.currentChild = 0;
            if (++pixels0loop0.currentTime >= pixels0loop0.cycles)
            {
                pixels0loop0.currentTime = 0;
                ret |= 0x02;
            }
        }
        else
        {
            pixels0loop0.currentChild++;
        }
    };
    return ret;
}

void LedNeokey::pixels_loop(uint32_t color, uint16_t time)
{
    if (pixels0_loop0(color, time) & 0x01)
        pixels.neopixel.show();
}

void LedNeokey::setColor(LedColor color)
{
    switch (color) {
        case 0:
            actual_color = (0);
            break;
        case 1:
            actual_color = pixels.neopixel.Color(colorSaturation, 0, 0);
            break;
        case 2:
            actual_color = pixels.neopixel.Color(0, colorSaturation, 0);
            break;
        case 3:
            actual_color = pixels.neopixel.Color(colorSaturation, colorSaturation, 0);
            break;
        case 4:
            actual_color = pixels.neopixel.Color(0, 0, colorSaturation);
            break;
        case 5:
            actual_color = pixels.neopixel.Color(colorSaturation, 0, colorSaturation);
            break;
        case 6:
            actual_color = pixels.neopixel.Color(0, colorSaturation, colorSaturation);
            break;
        case 7:
            actual_color = pixels.neopixel.Color(colorSaturation,colorSaturation,colorSaturation);
            break;
    }
    for(uint8_t i=0; i<NUMPIXELS; i++) {

        pixels.neopixel.setPixelColor(i, actual_color);

        pixels.neopixel.show();

        // delay(DELAYVAL); // Pause before next pass through loop
    }
}


void LedNeokey::setEffect(LedEffect effect) {
    this->effect = effect;
}

void LedNeokey::brighten() {
    pixels.neopixel.setBrightness(255);
}

void LedNeokey::darken() {
    pixels.neopixel.setBrightness(dimFactor);
}

// void LedNeoPixel::AnimationStart(RgbColor color, uint16_t time) {
//     FadeInFadeOutRinseRepeat(0.25, color, time);
// }

void LedNeokey::update() {
    // if (animations.IsAnimating())
    // {
    //     animations.UpdateAnimations();
    //     strip.Show();
    // }
    switch (this->effect) {
        case LedEffect::dim:
            // animations.StopAll();
            darken();
            // strip.Show();
            pixels.neopixel.show();
            break;
        case LedEffect::bright:
            // animations.StopAll();
            brighten();
            pixels.neopixel.show();
            break;
        case LedEffect::fast_pulse:
            actual_time = fast;
            pixels_loop(actual_color, actual_time);
            break;
        case LedEffect::slow_pulse:
            actual_time = slow;
            pixels_loop(actual_color, actual_time);
            break;
    }
}
#endif
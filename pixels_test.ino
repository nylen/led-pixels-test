#include <FastLED.h>

#define PIN_LEDS 2

#define NUM_LEDS 50

CRGB leds[NUM_LEDS];

void setAll(CRGB color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
}

void frame(int delayMs) {
static unsigned long lastFrame = 0;
  if (delayMs < 0) {
    return;
  }
  unsigned long now = millis();
  if (now - lastFrame < delayMs) {
    delay(delayMs - (now - lastFrame));
  }
  FastLED.show();
  lastFrame = millis();
}

template<uint8_t DATA_PIN> class ALMOST_NEOPIXEL : public WS2812Controller800Khz<DATA_PIN, RGB> {};

#define NUM_MODES 2
#define MS_PER_MODE_STEP 20000
u8 modeOffset = 0;

void setup() {
  FastLED.addLeds<ALMOST_NEOPIXEL, PIN_LEDS>(leds, NUM_LEDS);
  modeOffset = analogRead(A0) % NUM_MODES;
}

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

void blinkyRainbow() {
static u8 hue = 0;
static u8 index = 0;
  hue += 7;
  setAll(CRGB::Black);
  u8 tempIndex = index;
  for (u8 i = 0; i < 8; i++) {
      tempIndex = (tempIndex + 4) % NUM_LEDS;
      leds[tempIndex] = CHSV(20 * i + hue, 255, 128);
  }
  index++;
  frame(70);
}

void whiteDissolve() {
static u8 whitePos = NUM_LEDS;
static u8 rainbowPos = NUM_LEDS;
static u8 rainbowHue = 0;
  setAll(CRGB::Black);

  // white
  if (whitePos == NUM_LEDS && random8() < 20) {
    whitePos = 0;
  }
  for (u8 i = whitePos; i < whitePos + 20 && i < NUM_LEDS; i++) {
    u8 val = (i - whitePos) * 10;
    leds[i] = CRGB(val, val / 2, val / 4);
  }
  if (whitePos < NUM_LEDS) {
    whitePos++;
  }

  // rainbow
  if (rainbowPos == NUM_LEDS && random8() < 10) {
    rainbowPos = 0;
  }
  for (u8 i = rainbowPos; i < rainbowPos + 30 && i < NUM_LEDS; i++) {
    u8 val;
    if (i - rainbowPos < 10) {
      val = (i - rainbowPos) * 12;
    } else if (i - rainbowPos > 20) {
      val = 80 - (i - rainbowPos) * 12;
    } else {
      val = 240;
    }
    leds[i] = CHSV(rainbowHue + i * 8, 255, val);
  }
  if (rainbowPos < NUM_LEDS) {
    rainbowPos = min(rainbowPos + 2, NUM_LEDS);
  }

  frame(70);
}

void loop() {
  switch ((millis() / MS_PER_MODE_STEP + modeOffset) % NUM_MODES) {
    case 0:
      blinkyRainbow();
      break;
    case 1:
      whiteDissolve();
      break;
  }
}

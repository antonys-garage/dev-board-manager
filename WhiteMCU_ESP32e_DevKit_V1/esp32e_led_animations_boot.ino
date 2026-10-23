/*
  Onboard RGB LED animations with BOOT button control
  WhiteMCU ESP32e DevKit V1 (RGB LED on GPIO5, BOOT button on GPIO0)

  BOOT button:
    single click  -> next animation
    double click  -> previous animation
    long press    -> cycle brightness (10% / 25% / 50% / 100%)

  Animations:
    0 Rainbow       1 Breathing     2 Police
    3 Heartbeat     4 Fire          5 Party strobe
    6 Off

  Open the Serial Monitor at 115200 to see the current mode.
  Board: ESP32 Dev Module
*/

#include <Arduino.h>
#include <math.h>

#define RGB_PIN 5    // onboard RGB LED data pin
#define BTN_PIN 0    // BOOT button

const uint8_t MODE_COUNT = 7;
const char* MODE_NAMES[MODE_COUNT] = {
  "Rainbow", "Breathing", "Police", "Heartbeat", "Fire", "Party strobe", "Off"
};

const uint8_t BRIGHT_LEVELS[4] = {10, 25, 50, 100};   // percent
uint8_t brightIdx = 1;                                // start at 25%
uint8_t mode = 0;

// Button timing (ms)
const uint32_t LONG_MS   = 700;
const uint32_t DOUBLE_MS = 350;
const uint32_t DEBOUNCE  = 30;

bool     btnDown = false;
bool     longFired = false;
uint32_t downAt = 0;
uint8_t  clicks = 0;
uint32_t lastRelease = 0;

/* ---------- LED helpers ---------- */

// If red and green look swapped on your LED, swap r and g in this function.
void setLed(uint8_t r, uint8_t g, uint8_t b) {
  uint8_t pct = BRIGHT_LEVELS[brightIdx];
  neopixelWrite(RGB_PIN, (r * pct) / 100, (g * pct) / 100, (b * pct) / 100);
}

// h: 0-359, v: 0.0-1.0 (full saturation)
void showHSV(uint16_t h, float v) {
  v = constrain(v, 0.0f, 1.0f);
  float hh = (h % 360) / 60.0f;
  int   i  = (int)hh;
  float f  = hh - i;
  float q  = 1.0f - f;
  float r = 0, g = 0, b = 0;
  switch (i) {
    case 0: r = 1; g = f; b = 0; break;
    case 1: r = q; g = 1; b = 0; break;
    case 2: r = 0; g = 1; b = f; break;
    case 3: r = 0; g = q; b = 1; break;
    case 4: r = f; g = 0; b = 1; break;
    default: r = 1; g = 0; b = q; break;
  }
  setLed((uint8_t)(r * v * 255), (uint8_t)(g * v * 255), (uint8_t)(b * v * 255));
}

float pulse(float t, float center, float width) {
  float d = (t - center) / width;
  return expf(-d * d);
}

/* ---------- Animations ---------- */

void animRainbow() {
  showHSV((millis() / 15) % 360, 1.0f);
}

void animBreathing() {
  const uint32_t period = 3500;
  uint32_t t = millis();
  float ph = (t % period) / (float)period;
  float v = (1.0f - cosf(2.0f * PI * ph)) / 2.0f;
  uint16_t hue = ((t / period) * 70) % 360;   // new color each breath
  showHSV(hue, v);
}

void animPolice() {
  uint32_t t = millis() % 800;
  if (t < 80 || (t >= 160 && t < 240))             setLed(255, 0, 0);
  else if ((t >= 400 && t < 480) || (t >= 560 && t < 640)) setLed(0, 0, 255);
  else                                              setLed(0, 0, 0);
}

void animHeartbeat() {
  float t = millis() % 1100;
  float v = pulse(t, 120, 45) + 0.65f * pulse(t, 330, 55);
  v = constrain(v, 0.0f, 1.0f);
  setLed((uint8_t)(255 * v), 0, (uint8_t)(10 * v));
}

void animFire() {
  static float v = 0.5f, target = 0.5f;
  static uint32_t nextChange = 0;
  uint32_t now = millis();
  if (now > nextChange) {
    target = random(20, 101) / 100.0f;
    nextChange = now + random(40, 140);
  }
  v += (target - v) * 0.2f;
  setLed((uint8_t)(255 * v), (uint8_t)(70 * v), 0);
}

void animParty() {
  static uint32_t last = 0, nextChange = 0;
  static uint16_t hue = 0;
  uint32_t now = millis();
  if (now >= nextChange) {
    hue = random(360);
    last = now;
    nextChange = now + random(100, 250);
  }
  if (now - last < 70) showHSV(hue, 1.0f);
  else                 setLed(0, 0, 0);
}

void runMode() {
  switch (mode) {
    case 0: animRainbow();   break;
    case 1: animBreathing(); break;
    case 2: animPolice();    break;
    case 3: animHeartbeat(); break;
    case 4: animFire();      break;
    case 5: animParty();     break;
    default: setLed(0, 0, 0); break;
  }
}

/* ---------- Button handling ---------- */

void changeMode(int dir) {
  mode = (mode + MODE_COUNT + dir) % MODE_COUNT;
  Serial.printf("Mode: %s\n", MODE_NAMES[mode]);
}

void handleButton() {
  bool pressed = (digitalRead(BTN_PIN) == LOW);
  uint32_t now = millis();

  if (pressed && !btnDown) {                 // press started
    btnDown = true;
    downAt = now;
    longFired = false;
  }

  if (pressed && btnDown && !longFired && now - downAt > LONG_MS) {
    longFired = true;                        // long press: brightness
    clicks = 0;
    brightIdx = (brightIdx + 1) % 4;
    Serial.printf("Brightness: %u%%\n", BRIGHT_LEVELS[brightIdx]);
  }

  if (!pressed && btnDown) {                 // released
    btnDown = false;
    if (now - downAt >= DEBOUNCE && !longFired) {
      clicks++;
      lastRelease = now;
    }
  }

  if (!btnDown && clicks && now - lastRelease > DOUBLE_MS) {
    if (clicks == 1) changeMode(+1);         // single click: next
    else             changeMode(-1);         // double click: previous
    clicks = 0;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BTN_PIN, INPUT_PULLUP);
  randomSeed(esp_random());
  Serial.printf("Mode: %s\n", MODE_NAMES[mode]);
}

void loop() {
  handleButton();
  runMode();
  delay(8);
}

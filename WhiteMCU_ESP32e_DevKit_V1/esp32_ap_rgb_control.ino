// ESP32 open WiFi AP + onboard WS2812 RGB LED control (GPIO5)
// Arduino-ESP32 core 3.x (neopixelWrite is built in). Open http://192.168.4.1
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

#define RGB_PIN 5
const char* AP_SSID = "ESP32-RGB";   // open network, no password

WebServer server(80);
DNSServer dns;

uint8_t R = 255, G = 255, B = 255, bright = 40;   // bright 0-255
enum Mode { STATIC, BLINK, RAINBOW } mode = STATIC;
bool on = true, blinkOn = true;
uint32_t last = 0, hue = 0;

void show(uint8_t r, uint8_t g, uint8_t b) {
  neopixelWrite(RGB_PIN, r * bright / 255, g * bright / 255, b * bright / 255);
}

void hsv(uint16_t h, uint8_t &r, uint8_t &g, uint8_t &b) {   // h 0-1535
  uint8_t s = h / 256, f = h % 256;
  switch (s) {
    case 0: r = 255; g = f; b = 0; break;
    case 1: r = 255 - f; g = 255; b = 0; break;
    case 2: r = 0; g = 255; b = f; break;
    case 3: r = 0; g = 255 - f; b = 255; break;
    case 4: r = f; g = 0; b = 255; break;
    default: r = 255; g = 0; b = 255 - f;
  }
}

void refresh() { if (mode == STATIC) on ? show(R, G, B) : show(0, 0, 0); }

const char PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta name="viewport" content="width=device-width,initial-scale=1">
<title>ESP32 RGB</title><style>
body{font-family:sans-serif;text-align:center;background:#111;color:#eee;padding:20px}
button{font-size:1.1em;padding:12px 22px;margin:5px;border:0;border-radius:10px;background:#2196f3;color:#fff}
button.o{background:#555}input[type=color]{width:140px;height:140px;border:0;background:none}
input[type=range]{width:80%}</style></head><body>
<h2>ESP32 RGB LED</h2>
<input type="color" id="c" value="#ffffff" oninput="col(this.value)"><br>
<button onclick="m('on')">ON</button><button class="o" onclick="m('off')">OFF</button>
<button onclick="m('blink')">BLINK</button><button onclick="m('rainbow')">RAINBOW</button>
<p>Brightness: <span id="bv">40</span></p>
<input type="range" min="0" max="255" value="40" oninput="br(this.value)">
<script>
function col(h){fetch('/color?r='+parseInt(h.substr(1,2),16)+'&g='+parseInt(h.substr(3,2),16)+'&b='+parseInt(h.substr(5,2),16));}
function m(x){fetch('/mode?m='+x);}
function br(v){document.getElementById('bv').textContent=v;fetch('/bright?v='+v);}
</script></body></html>
)rawliteral";

void ok() { server.send(200, "text/plain", "ok"); }

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID);                       // open AP
  dns.start(53, "*", WiFi.softAPIP());
  Serial.printf("AP '%s' open, http://%s\n", AP_SSID, WiFi.softAPIP().toString().c_str());

  server.on("/", []() { server.send_P(200, "text/html", PAGE); });
  server.on("/color", []() {
    R = server.arg("r").toInt(); G = server.arg("g").toInt(); B = server.arg("b").toInt();
    mode = STATIC; on = true; refresh(); ok();
  });
  server.on("/bright", []() { bright = server.arg("v").toInt(); refresh(); ok(); });
  server.on("/mode", []() {
    String x = server.arg("m");
    if (x == "on") { mode = STATIC; on = true; }
    else if (x == "off") { mode = STATIC; on = false; }
    else if (x == "blink") mode = BLINK;
    else if (x == "rainbow") mode = RAINBOW;
    refresh(); ok();
  });
  server.onNotFound([]() { server.send_P(200, "text/html", PAGE); });
  server.begin();
  refresh();
}

void loop() {
  dns.processNextRequest();
  server.handleClient();

  if (mode == BLINK && millis() - last >= 400) {
    last = millis(); blinkOn = !blinkOn;
    blinkOn ? show(R, G, B) : show(0, 0, 0);
  } else if (mode == RAINBOW && millis() - last >= 20) {
    last = millis(); hue = (hue + 6) % 1536;
    uint8_t r, g, b; hsv(hue, r, g, b); show(r, g, b);
  }
}

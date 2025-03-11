#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define PIN1 D4  // NeoPixel Pin
#define NUM_LEDS 16 // LED Count

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUM_LEDS, PIN1, NEO_GRB + NEO_KHZ800);
ESP8266WebServer server(80);

// Enter your Wi-Fi information here
const char* ssid = "Your ssid"; // Your ssid
const char* password = "Your password"; // Your password

int brightness1 = 255;
int effectSpeed1 = 100;
int currentAnimation1 = 11; // Animation on startup


unsigned long previousMillis = 0;
unsigned long lastAnimationUpdate = 0;


unsigned long lastPoliceEffectChange = 0;
unsigned long policeEffectDelay = 100;
unsigned long lastRainbowUpdate = 0;
unsigned long rainbowTransitionSpeed = 50;
int setSpeed1 = 100;

void setup() {
  Serial.begin(115200);
  strip1.begin();
  strip1.show();


  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/setColor1", handleSetColor1);
  server.on("/setBrightness1", handleSetBrightness1);
  server.on("/setSpeed1", handleSetSpeed1);
  server.on("/animation1", handleAnimation1);
  server.on("/closeLights", handleCloseLights);
  server.begin();

  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  

  if (currentAnimation1 != -1) {
    unsigned long currentMillis = millis();
    

    if (currentMillis - lastAnimationUpdate >= effectSpeed1) {
      lastAnimationUpdate = currentMillis;
      updateAnimation(strip1, currentAnimation1);
    }
  }
}

void updateAnimation(Adafruit_NeoPixel &strip, int animation) {
  switch (animation) {
    case 0: redBreathing(strip); break;
    case 1: greenBreathing(strip); break;
    case 2: blueBreathing(strip); break;
    case 3: yellowBreathing(strip); break;
    case 4: purpleBreathing(strip); break;
    case 5: whiteBreathing(strip); break;
    case 6: turquoiseBreathing(strip); break;
    case 7: favorite(strip); break;
    case 8: policeEffect(strip); break;
    case 9: dualRainbow(strip); break;
    case 10: rainbowRain(strip); break;
    case 11: multiColor(strip); break;
    case 12: rainbow(strip); break;
    default: break;
  }
}

void handleRoot() { 
  String html = "<html><head><style>"
                "body { background-color: #121212; color: #FFFFFF; font-family: Arial, sans-serif; text-align: center; }"
                "h1, h2, h3, button, select, input { color: #FFFFFF; }"
                "h1 { font-size: 48px; font-family: Arial, sans-serif; }"
                "h2 { font-size: 36px; font-family: Arial, sans-serif; }"
                "h3 { font-size: 24px; font-family: Arial, sans-serif; }"
                "button { margin: 5px; padding: 10px 18px; font-size: 18px; border-radius: 4px; border: 2px solid transparent; background-color: #333; }"
                "select { margin: 5px; padding: 10px 18px; font-size: 18px; border-radius: 4px; border: 2px solid transparent; background-color: #333; }"
                "input[type='color'] { width: 80px; height: 40px; border-radius: 4px; border: 2px solid transparent; background-color: #333; }"
                "input[type='number'] { padding: 10px; font-size: 18px; border-radius: 4px; border: 2px solid transparent; background-color: #333; }"
                "</style></head><body>";
  html += "<h1>MRT</h1>";
  html += "<h2>NeoPixel Control</h2>";


  html += "<h3>Set Brightness (1-255)</h3>";
  html += "<input type=\"number\" min=\"1\" max=\"255\" value=\"" + String(brightness1) + "\">";
  html += "<button onclick=\"location.href='/setBrightness1?brightness=' + this.previousElementSibling.value\">Set Brightness</button>";


  html += "<h3>Set Speed (1-1000 ms)</h3>";
  html += "<input type=\"number\" min=\"1\" max=\"1000\" value=\"" + String(effectSpeed1) + "\">";
  html += "<button onclick=\"location.href='/setSpeed1?speed=' + this.previousElementSibling.value\">Set Speed</button>";


  html += "<h3>Set Color </h3>";
  html += "<input type=\"color\" id=\"colorPicker\" value=\"#ff0000\">";
  html += "<button onclick=\"setColor()\">Set Color</button>";
  html += "<script>"
          "function setColor() {"
          "var color = document.getElementById('colorPicker').value;"
          "location.href='/setColor1?r=' + parseInt(color.slice(1, 3), 16) + '&g=' + parseInt(color.slice(3, 5), 16) + '&b=' + parseInt(color.slice(5, 7), 16);"
          "} "
          "</script>";


  html += "<h3>Select Animation</h3>";
  html += "<form action=\"/animation1\" method=\"get\">";
  html += "<select name=\"num\">";
  html += "<option value=\"0\">Red Breathing</option>";
  html += "<option value=\"1\">Green Breathing</option>";
  html += "<option value=\"2\">Blue Breathing</option>";
  html += "<option value=\"3\">Yellow Breathing</option>";
  html += "<option value=\"4\">Purple Breathing</option>";
  html += "<option value=\"5\">White Breathing</option>";
  html += "<option value=\"6\">Turquoise Breathing</option>";
  html += "<option value=\"7\">Favorite</option>";
  html += "<option value=\"8\">Police Effect</option>";
  html += "<option value=\"9\">Dual Rainbow</option>";
  html += "<option value=\"10\">Rainbow Rain</option>";
  html += "<option value=\"11\">Multi Color</option>";
  html += "<option value=\"12\">Rainbow</option>";
  html += "</select>";
  html += "<input type=\"submit\" value=\"Start Animation\" style='border-radius: 4px; padding: 10px 18px; font-size: 18px; border: 2px solid transparent; background-color: #333;' />";
  html += "</form>";


  html += "<h3>Close Lights</h3>";
  html += "<button onclick=\"location.href='/closeLights'\">Close Lights</button>";

  html += "</body></html>";
  server.send(200, "text/html", html);
}


void handleSetColor1() {
  handleCloseLights(); // Önce ışıkları kapat

  int r = server.arg("r").toInt();
  int g = server.arg("g").toInt();
  int b = server.arg("b").toInt();

  for (int i = 0; i < NUM_LEDS; i++) {
    strip1.setPixelColor(i, strip1.Color(r, g, b));
  }
  strip1.show();
  server.send(200, "text/html", "<html><head><style>"
                                "body { background-color: #121212; color: #FFFFFF; font-family: Arial, sans-serif; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; }"
                                "h1 { color: #FFFFFF; font-family: Arial, sans-serif; font-size: 48px; text-align: center; }"
                                "</style></head><body><h1>LED Color Set</h1></body></html>");
}

void handleSetBrightness1() {
  brightness1 = server.arg("brightness").toInt();
  strip1.setBrightness(brightness1);
  strip1.show();
  server.send(200, "text/html", "<html><head><style>"
                                "body { background-color: #121212; color: #FFFFFF; font-family: Arial, sans-serif; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; }"
                                "h1 { color: #FFFFFF; font-family: Arial, sans-serif; font-size: 48px; text-align: center; }"
                                "</style></head><body><h1>LED Brightness Set</h1></body></html>");
}

void handleSetSpeed1() {
  effectSpeed1 = server.arg("speed").toInt();
  server.send(200, "text/html", "<html><head><style>"
                                "body { background-color: #121212; color: #FFFFFF; font-family: Arial, sans-serif; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; }"
                                "h1 { color: #FFFFFF; font-family: Arial, sans-serif; font-size: 48px; text-align: center; }"
                                "</style></head><body><h1>LED Speed Set</h1></body></html>");
}

void handleAnimation1() {
  currentAnimation1 = server.arg("num").toInt();
  server.send(200, "text/html", "<html><head><style>"
                                "body { background-color: #121212; color: #FFFFFF; font-family: Arial, sans-serif; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; }"
                                "h1 { color: #FFFFFF; font-family: Arial, sans-serif; font-size: 48px; text-align: center; }"
                                "</style></head><body><h1>Animation Started</h1></body></html>");
}

void handleCloseLights() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip1.setPixelColor(i, strip1.Color(0, 0, 0));
  }
  strip1.show();
  currentAnimation1 = -1;
  server.send(200, "text/html", "<html><head><style>"
                                "body { background-color: #121212; color: #FFFFFF; font-family: Arial, sans-serif; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; }"
                                "h1 { color: #FFFFFF; font-family: Arial, sans-serif; font-size: 48px; text-align: center; }"
                                "</style></head><body><h1>Lights Closed</h1></body></html>");
}



void redBreathing(Adafruit_NeoPixel &strip) {
  static int intensity = 0;
  static bool increasing = true;
  static unsigned long lastUpdate = 0;

  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate >= setSpeed1) {
    lastUpdate = currentMillis;

    if (increasing) {
      intensity += 5;
      if (intensity >= 255) increasing = false;
    } else {
      intensity -= 5;
      if (intensity <= 0) increasing = true;
    }

    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(intensity, 0, 0));
    }
    strip.show();
  }
}

void greenBreathing(Adafruit_NeoPixel &strip) {
  static int intensity = 0;
  static bool increasing = true;
  static unsigned long lastUpdate = 0;

  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate >= setSpeed1) {
    lastUpdate = currentMillis;

    if (increasing) {
      intensity += 5;
      if (intensity >= 255) increasing = false;
    } else {
      intensity -= 5;
      if (intensity <= 0) increasing = true;
    }

    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(0, intensity, 0));
    }
    strip.show();
  }
}

void blueBreathing(Adafruit_NeoPixel &strip) {
  static int intensity = 0;
  static bool increasing = true;
  static unsigned long lastUpdate = 0;

  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate >= setSpeed1) {
    lastUpdate = currentMillis;

    if (increasing) {
      intensity += 5;
      if (intensity >= 255) increasing = false;
    } else {
      intensity -= 5;
      if (intensity <= 0) increasing = true;
    }

    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(0, 0, intensity));
    }
    strip.show();
  }
}

void yellowBreathing(Adafruit_NeoPixel &strip) {
  static int intensity = 0;
  static bool increasing = true;
  static unsigned long lastUpdate = 0;

  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate >= setSpeed1) {
    lastUpdate = currentMillis;

    if (increasing) {
      intensity += 5;
      if (intensity >= 255) increasing = false;
    } else {
      intensity -= 5;
      if (intensity <= 0) increasing = true;
    }

    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(intensity, intensity, 0));
    }
    strip.show();
  }
}

void purpleBreathing(Adafruit_NeoPixel &strip) {
  static int intensity = 0;
  static bool increasing = true;
  static unsigned long lastUpdate = 0;

  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate >= setSpeed1) {
    lastUpdate = currentMillis;

    if (increasing) {
      intensity += 5;
      if (intensity >= 255) increasing = false;
    } else {
      intensity -= 5;
      if (intensity <= 0) increasing = true;
    }

    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(intensity, 0, intensity));
    }
    strip.show();
  }
}

void whiteBreathing(Adafruit_NeoPixel &strip) {
  static int intensity = 0;
  static bool increasing = true;
  static unsigned long lastUpdate = 0;

  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate >= setSpeed1) {
    lastUpdate = currentMillis;

    if (increasing) {
      intensity += 5;
      if (intensity >= 255) increasing = false;
    } else {
      intensity -= 5;
      if (intensity <= 0) increasing = true;
    }

    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(intensity, intensity, intensity));
    }
    strip.show();
  }
}

void turquoiseBreathing(Adafruit_NeoPixel &strip) {
  static int intensity = 0;
  static bool increasing = true;
  static unsigned long lastUpdate = 0;

  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate >= setSpeed1) {
    lastUpdate = currentMillis;

    if (increasing) {
      intensity += 5;
      if (intensity >= 255) increasing = false;
    } else {
      intensity -= 5;
      if (intensity <= 0) increasing = true;
    }

    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(0, intensity, intensity));
    }
    strip.show();
  }
}

void favorite(Adafruit_NeoPixel &strip) {
  static int intensity = 0;
  static bool increasing = true;
  static unsigned long lastUpdate = 0;

  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate >= setSpeed1) {
    lastUpdate = currentMillis;

    if (increasing) {
      intensity += 5;
      if (intensity >= 255) increasing = false;
    } else {
      intensity -= 5;
      if (intensity <= 0) increasing = true;
    }

    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(intensity, 165, 0));
    }
    strip.show();
  }
}

void policeEffect(Adafruit_NeoPixel &strip) {
  static bool isRed = true;

  unsigned long currentMillis = millis();
  if (currentMillis - lastPoliceEffectChange >= policeEffectDelay) {
    lastPoliceEffectChange = currentMillis;

    if (isRed) {
      strip.fill(strip.Color(255, 0, 0));
    } else {
      strip.fill(strip.Color(0, 0, 255));
    }
    strip.show();
    isRed = !isRed;
  }
}

void dualRainbow(Adafruit_NeoPixel &strip) { 
  int groupSize = 8;
  int numGroups = NUM_LEDS / groupSize;
  
  for (int group = 0; group < numGroups; group++) {
    int direction = (group % 2 == 0) ? 1 : -1;
    int groupStart = group * groupSize;

    for (int j = 0; j < groupSize; j++) {

      int color = (millis() / setSpeed1 + (groupStart + j) * 3 * direction) % 255;
      strip.setPixelColor(groupStart + j, Wheel(color));
    }
  }
  
  strip.show();
}


void rainbowRain(Adafruit_NeoPixel &strip) {
  for (int j = 0; j < NUM_LEDS; j++) {
    strip.setPixelColor(j, random(0, 255), random(0, 255), random(0, 255));
  }
  strip.show();
}

void multiColor(Adafruit_NeoPixel &strip) {
  for (int j = 0; j < NUM_LEDS; j++) {
    int color = (millis() / setSpeed1 + j * 3) % 255;
    strip.setPixelColor(j, Wheel(color));
  }
  strip.show();
}

void rainbow(Adafruit_NeoPixel &strip) {
  unsigned long currentMillis = millis();
  if (currentMillis - lastRainbowUpdate >= rainbowTransitionSpeed) {
    lastRainbowUpdate = currentMillis;

    for (int i = 0; i < NUM_LEDS; i++) {
      int pos = (i * 256 / NUM_LEDS) + (millis() / 10);
      strip.setPixelColor(i, Wheel(pos & 255));
    }
    strip.show();
  }
}



uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip1.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip1.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip1.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
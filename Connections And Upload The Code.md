You need to use the Arduino IDE software.
Add the Wi-Fi password and SSID in the code, and you need to be connected to the same Wi-Fi (this uses an IP address for the website).
Use a digital pin for the connections.
Add your LED number in the "LED number" area. The code is basically adapted for your LED number.
Later, upload the code, open the serial monitor, and look for the "connected" text. Then copy the IP address and paste it into your browser.
Use the ESP8266 board manager and the Adafruit NeoPixel.h library. Go to "Board Extensions," type "ESP8266," and download the "ESP8266 Wi-Fi" board extension.
Use a minimum of 8 LEDs and a maximum of 40 LEDs.
If you are using NodeMCU v1.0, download the CP2102 UART USB bridge driver.
If you are using NodeMCU v0.9, download the CH340 USB driver.
The same project can be done on the ESP32, but you will need to modify the code.

Code by: Muhammed Raid

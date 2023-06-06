#include <NativeEthernet.h>
#include <Artnet.h>
#include <FastLED.h>

// Ethernet settings
byte mac[] = {0x04, 0xE9, 0xE5, 0x15, 0x71, 0x28};  // MAC address of your Teensy
IPAddress ip(192, 168, 1, 123);                     // IP address of your Teensy
IPAddress gateway(192, 168, 1, 1);                  // IP address of your gateway/router
IPAddress subnet(255, 255, 255, 0);                 // Subnet mask for your network

// Artnet settings
Artnet artnet;
const uint16_t universe = 1;  // DMX universe to listen for Artnet packets

// LED settings
#define NUM_LEDS 24         // Number of LEDs in the strip
#define DATA_PIN 11         // Data pin connected to the LED strip
#define CLOCK_PIN 13        // Clock pin connected to the LED strip
CRGB leds[NUM_LEDS];        // FastLED object

void setup() {
  Serial.begin(115200);

  // Start the Ethernet connection
  Ethernet.begin(mac, ip, gateway, subnet);

  // Initialize the Artnet library
  artnet.begin();
  artnet.setArtDmxCallback(onDmxPacket);

  // Initialize the FastLED library
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);
  FastLED.setBrightness(128);  // Set global brightness to 128
}

void loop() {
  // Handle incoming Artnet packets
  artnet.read();

  // Update the LED strip
  FastLED.show();
}

// Callback function for handling incoming Artnet packets
void onDmxPacket(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data) {
  // Check if the received universe matches the desired universe
  if (universe == ::universe) {
    // Update the LED strip with the Artnet data
    for (int i = 0; i < length && i < NUM_LEDS; i++) {
      // Calculate the RGB values based on the Artnet data
      uint8_t r = data[i * 3];
      uint8_t g = data[i * 3 + 1];
      uint8_t b = data[i * 3 + 2];
      leds[i] = CRGB(r, g, b);  // Set RGB values of the LED based on the Artnet data
    }
  }
}


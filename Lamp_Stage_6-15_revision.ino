#include <NativeEthernet.h>
#include <Artnet.h>
#include <FastLED.h>

// Ethernet settings
byte mac[] = { 0x04, 0xE9, 0xE5, 0x15, 0x71, 0x28 };  // MAC address of your Teensy
IPAddress ip(192, 168, 1, 123);                       // IP address of your Teensy
IPAddress gateway(192, 168, 1, 1);                    // IP address of your gateway/router
IPAddress subnet(255, 255, 255, 0);                   // Subnet mask for your network

// Artnet settings
Artnet artnet;
const uint16_t universes[] = { 1, 2, 3, 4, 5, 6, 7, 8 };  // DMX universes to listen for Artnet packets

// LED settings
#define CLOCK_PIN_1 2
#define CLOCK_PIN_2 23

#define NUM_LEDS_RUN1 144       // Number of LEDs in the first run
#define DATA_PIN_RUN1 4         // Data pin connected to the first LED run
CRGB leds_run1[NUM_LEDS_RUN1];  // FastLED object for the first run

#define NUM_LEDS_RUN2 206       // Number of LEDs in the second run
#define DATA_PIN_RUN2 5         // Data pin connected to the second LED run
CRGB leds_run2[NUM_LEDS_RUN2];  // FastLED object for the second run

#define NUM_LEDS_RUN3 144       // Number of LEDs in the third run
#define DATA_PIN_RUN3 6         // Data pin connected to the third LED run
CRGB leds_run3[NUM_LEDS_RUN3];  // FastLED object for the third run

#define NUM_LEDS_RUN4 127       // Number of LEDs in the fourth run
#define DATA_PIN_RUN4 21        // Data pin connected to the fourth LED run
CRGB leds_run4[NUM_LEDS_RUN4];  // FastLED object for the fourth run

#define NUM_LEDS_RUN5 172       // Number of LEDs in the fifth run
#define DATA_PIN_RUN5 20        // Data pin connected to the fifth LED run
CRGB leds_run5[NUM_LEDS_RUN5];  // FastLED object for the fifth run

#define NUM_LEDS_RUN6 127       // Number of LEDs in the sixth run
#define DATA_PIN_RUN6 19        // Data pin connected to the sixth LED run
CRGB leds_run6[NUM_LEDS_RUN6];  // FastLED object for the sixth run

void setup() {
  Serial.begin(115200);

  // Start the Ethernet connection
  Ethernet.begin(mac, ip, gateway, subnet);

  // Initialize the Artnet library
  artnet.begin();
  artnet.setArtDmxCallback(onDmxPacket);

  // Initialize the FastLED library for each strip
  FastLED.addLeds<APA102, DATA_PIN_RUN1, CLOCK_PIN_1, BGR>(leds_run1, NUM_LEDS_RUN1);
  FastLED.addLeds<APA102, DATA_PIN_RUN2, CLOCK_PIN_1, BGR>(leds_run2, NUM_LEDS_RUN2);
  FastLED.addLeds<APA102, DATA_PIN_RUN3, CLOCK_PIN_1, BGR>(leds_run3, NUM_LEDS_RUN3);
  FastLED.addLeds<APA102, DATA_PIN_RUN4, CLOCK_PIN_2, BGR>(leds_run4, NUM_LEDS_RUN4);
  FastLED.addLeds<APA102, DATA_PIN_RUN5, CLOCK_PIN_2, BGR>(leds_run5, NUM_LEDS_RUN5);
  FastLED.addLeds<APA102, DATA_PIN_RUN6, CLOCK_PIN_2, BGR>(leds_run6, NUM_LEDS_RUN6);

  FastLED.setBrightness(128);  // Set global brightness to 128
}

void loop() {
  // Handle incoming Artnet packets
  artnet.read();

  // Update the LED strips
  FastLED.show();
}

// Callback function for handling incoming Artnet packets
void onDmxPacket(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data) {
  // Check if the received universe matches any of the desired universes
  for (int i = 0; i < 8; i++) {
    if (universe == universes[i]) {
      // Update the corresponding LED strip with the Artnet data
      CRGB* leds = getLedStrip(i);
      int numLeds = getNumLeds(i);
      int startingIndex = getStartingIndex(i);

      for (int j = 0; j < length && j < numLeds; j++) {
        // Calculate the RGB values based on the Artnet data
        uint8_t r = data[j * 3];
        uint8_t g = data[j * 3 + 1];
        uint8_t b = data[j * 3 + 2];
        leds[startingIndex + j] = CRGB(r, g, b);  // Set RGB values of the LED based on the Artnet data
      }

      break;  // Exit the loop after finding the matching universe
    }
  }
}

// Helper function to get the corresponding LED strip based on the index
CRGB* getLedStrip(int index) {
  switch (index) {
    case 0:
      return leds_run1;
    case 1:
    case 2:
      return leds_run2;
    case 3:
      return leds_run3;
    case 4:
      return leds_run4;
    case 5:
    case 6:
      return leds_run5;
    case 7:
      return leds_run6;
    default:
      return nullptr;
  }
}

// Helper function to get the number of LEDs in the corresponding LED strip based on the index
int getNumLeds(int index) {
  switch (index) {
    case 0:
      return NUM_LEDS_RUN1;
    case 1:
    case 2:
      return NUM_LEDS_RUN2 / 2;
    case 3:
      return NUM_LEDS_RUN3;
    case 4:
      return NUM_LEDS_RUN4;
    case 5:
    case 6:
      return NUM_LEDS_RUN5 / 2;
    case 7:
      return NUM_LEDS_RUN6;
    default:
      return 0;
  }
}

// Helper function to get the starting index of the corresponding LED strip based on the index
int getStartingIndex(int index) {
  switch (index) {
    case 0:
    case 1:
      return 0;
    case 2:
      return NUM_LEDS_RUN2 / 2;
    case 3:
    case 4:
      return 0;
    case 5:
      return NUM_LEDS_RUN5 / 2;
    case 6:
    case 7:
      return 0;
    default:
      return 0;
  }
}

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
#define CLOCK_PIN_1 32
#define CLOCK_PIN_2 28

#define NUM_LEDS_RUN1 144       // Number of LEDs in the first run
#define DATA_PIN_RUN1 31        // Data pin connected to the first LED run
CRGB leds_run1[NUM_LEDS_RUN1];  // FastLED object for the first run

#define NUM_LEDS_RUN2 206       // Number of LEDs in the second run
#define DATA_PIN_RUN2 29        // Data pin connected to the second LED run
CRGB leds_run2[NUM_LEDS_RUN2];  // FastLED object for the second run

#define NUM_LEDS_RUN3 144       // Number of LEDs in the third run
#define DATA_PIN_RUN3 26        // Data pin connected to the third LED run
CRGB leds_run3[NUM_LEDS_RUN3];  // FastLED object for the third run

#define NUM_LEDS_RUN4 127       // Number of LEDs in the fourth run
#define DATA_PIN_RUN4 30        // Data pin connected to the fourth LED run
CRGB leds_run4[NUM_LEDS_RUN4];  // FastLED object for the fourth run

#define NUM_LEDS_RUN5 172       // Number of LEDs in the fifth run
#define DATA_PIN_RUN5 25        // Data pin connected to the fifth LED run
CRGB leds_run5[NUM_LEDS_RUN5];  // FastLED object for the fifth run

#define NUM_LEDS_RUN6 127       // Number of LEDs in the sixth run
#define DATA_PIN_RUN6 27        // Data pin connected to the sixth LED run
CRGB leds_run6[NUM_LEDS_RUN6];  // FastLED object for the sixth run

// Precomputed LED strip pointers and lengths
CRGB* ledStrips[] = { leds_run1, leds_run2, leds_run2, leds_run3, leds_run4, leds_run5, leds_run5, leds_run6 };
int numLeds[] = { NUM_LEDS_RUN1, NUM_LEDS_RUN2 / 2, NUM_LEDS_RUN2 / 2, NUM_LEDS_RUN3, NUM_LEDS_RUN4, NUM_LEDS_RUN5 / 2, NUM_LEDS_RUN5 / 2, NUM_LEDS_RUN6 };
int startingIndices[] = { 0, 0, NUM_LEDS_RUN2 / 2, 0, 0, NUM_LEDS_RUN5 / 2, 0, 0 };

void setup() {
  delay(1000);  // power-up safety delay
  Serial.begin(115200);

  // Start the Ethernet connection
  Ethernet.begin(mac, ip, gateway, subnet);

  // Initialize the Artnet library
  artnet.begin();
  artnet.setArtDmxCallback(onDmxPacket);

  // Initialize the FastLED library for each strip
  FastLED.addLeds<APA102, DATA_PIN_RUN1, CLOCK_PIN_1, BGR>(leds_run1, NUM_LEDS_RUN1);
  FastLED.addLeds<APA102, DATA_PIN_RUN2, CLOCK_PIN_1, BGR>(leds_run2, NUM_LEDS_RUN2);
  FastLED.addLeds<APA102, DATA_PIN_RUN3, CLOCK_PIN_2, BGR>(leds_run3, NUM_LEDS_RUN3);
  FastLED.addLeds<APA102, DATA_PIN_RUN4, CLOCK_PIN_1, BGR>(leds_run4, NUM_LEDS_RUN4);
  FastLED.addLeds<APA102, DATA_PIN_RUN5, CLOCK_PIN_2, BGR>(leds_run5, NUM_LEDS_RUN5);
  FastLED.addLeds<APA102, DATA_PIN_RUN6, CLOCK_PIN_2, BGR>(leds_run6, NUM_LEDS_RUN6);

  FastLED.setBrightness(32);  // Set global brightness
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
      CRGB* leds = ledStrips[i];
      int numLedsStrip = numLeds[i];
      int startingIndex = startingIndices[i];

      memmove(&leds[startingIndex], &data[0], min(length, numLedsStrip) * 3);

      break;  // Exit the loop after finding the matching universe
    }
  }
}

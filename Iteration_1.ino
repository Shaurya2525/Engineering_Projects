#include <Adafruit_NeoPixel.h>

const int buttonPin = 2;
const int stopButton = 3;
const int ledPin = 4;
const int buzzerPin = 5;

#define NUMPIXELS 50
#define BASEOFFSET 4

Adafruit_NeoPixel pixels(NUMPIXELS, ledPin, NEO_GRB + NEO_KHZ800);

bool running = false;

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin, INPUT);
  pinMode(stopButton, INPUT);
  pinMode(buzzerPin, OUTPUT);

  pixels.begin();
}

void loop() {

  // Start button (active LOW)
  if (digitalRead(buttonPin) == HIGH) {
    running = true;
  }

  // Stop button (active LOW)
  if (digitalRead(stopButton) == HIGH) {
    running = false;
    pixels.clear();
    pixels.show();
  }

  if (!running) return;

  int sensorValue = analogRead(A0);

  // map potentiometer to delay (50ms to 500ms)
  int speedDelay = map(sensorValue, 0, 1023, 50, 150);

  for (int i = 0; i < NUMPIXELS - BASEOFFSET; i++) {

    pixels.clear();
    pixels.setPixelColor(i, pixels.Color(0, 0, 150));
    if (i > 0)
    {
      pixels.setPixelColor(i - 1, pixels.Color(0, 0, 150));
    }
    pixels.show();

    int delayTime = 0;
    while (delayTime < speedDelay)
    {
      if (digitalRead(stopButton) == HIGH) {
        pixels.clear();
        pixels.setPixelColor(i, pixels.Color(150, 0, 0));
        if (i > 0)
        {
          pixels.setPixelColor(i - 1, pixels.Color(150, 0, 0));
        }
        pixels.show();
        tone(buzzerPin, 1000, 3000);
        delay(3000);
        running = false;
        pixels.clear();
        pixels.show();
        Serial.println(i);
        return;
      }
      delay(1);
      delayTime++;
    }
  }

  pixels.clear();
  for (int i = NUMPIXELS - BASEOFFSET; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
  }
  pixels.show();
  running = false;
  tone(buzzerPin, 500, 3000);
  delay(3000);

  pixels.clear();
  pixels.show();
}
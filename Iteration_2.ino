#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>

const int buttonPin = 2;
const int stopButton = 3;
const int ledPin = 4;
const int buzzerPin = 5;

#define NUMPIXELS 50
#define BASEOFFSET 4

Adafruit_NeoPixel pixels(NUMPIXELS, ledPin, NEO_GRB + NEO_KHZ800);

bool running = false;

// lcd display
const int rs = 6, en = 7, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin, INPUT);
  pinMode(stopButton, INPUT);
  pinMode(buzzerPin, OUTPUT);

  pixels.begin();

  // lcd display
  lcd.begin(16, 2);
  lcd.print("Ready");
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

  lcd.clear();
  long startTime = millis();
  lcd.print("Running...");

  for (int i = 0; i < NUMPIXELS - BASEOFFSET; i++) {

    lcd.setCursor(0, 1);
    lcd.print(((millis() - startTime) / 1000.0), 1);
    lcd.setCursor(0, 0);

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

        lcd.print("                ");
        lcd.setCursor(0, 0);
        lcd.print("OUT");

        tone(buzzerPin, 1000, 3000);
        delay(3000);
        running = false;
        pixels.clear();
        pixels.show();
        Serial.println(i);

        lcd.clear();
        lcd.print("Ready");

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

  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print("SAFE");

  running = false;
  tone(buzzerPin, 500, 3000);
  delay(3000);

  lcd.clear();
  lcd.print("Ready");

  pixels.clear();
  pixels.show();
}
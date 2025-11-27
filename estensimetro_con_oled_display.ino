#include <HX711.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Servo.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// Creiamo un oggetto per le bilance
HX711 scale;
// Creiamo un oggetto per il motore
Servo myServo;

uint8_t dataPin = 6;   // DT
uint8_t clockPin = 5;  // SCK

// Parametri di calibrazione impostati

const long offset = 85793;           // peso zero
const float scaleFactor = 40.964282; // coefficiente di conversione in grammi

float weight;



void setup() {
  Serial.begin(115200);
  delay(500);

  myServo.write(0);
  delay(500);
  myServo.write(180);
  delay(500);
  
  // inizializza l'oggetto OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Non procedere, ciclo infinito
  }
  // Clear the buffer
  display.clearDisplay();




  scale.begin(dataPin, clockPin);
  myServo.attach(9);
  // Impostiamo il peso zero e il coefficiente
  scale.set_offset(offset);
  scale.set_scale(scaleFactor);
  
  Serial.println("La bilancia è pronta! Indicazioni in grammi:");
}

void loop() 
{
  pesatura();
  schermo();
  motore();
}

void pesatura()
{
  weight = scale.get_units(10);
  
  // Se il peso e' negativo — 0
  if (weight < 0) weight = 0;
  
  Serial.print("Peso: ");
  Serial.print(weight, 1); // una cifra dopo la virgola
  Serial.println(" grammi");
  
  delay(250);
}

void schermo()
{
    // Display Text
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 28);

  display.print("Peso:" );
  display.print(weight, 1);
  display.println("grammi");
  display.display();
}

void motore()
{
  int val;
  val = map(weight, 0, 10000, 180, 0);
  val = constrain(val, 0, 180);
  myServo.write(val);
  Serial.println(val);
}
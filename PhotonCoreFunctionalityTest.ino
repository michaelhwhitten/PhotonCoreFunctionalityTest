#include "OneWire.h"
#include "spark-dallas-temperature.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

// use hardware SPI for OLED
#define OLED_DC     D3
#define OLED_CS     D4
#define OLED_RESET  D5
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


volatile int loopCount = 0; // used to display a running count
int pushButtonPin = A0; // momentary pushbutton wired into pin A0
volatile bool countFlag = TRUE; //used to decide whether to increment and display the flag. Toggled by a button press interrupt.
void buttonPressed (void); // This will be the interrupt service routine



void setup()   {
    Serial.begin(9600);
    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // init done

  //set up the button ISR
  pinMode(pushButtonPin, INPUT_PULLUP); // set the pin up to be a pulldown pin
  attachInterrupt(pushButtonPin, buttonPressed, FALLING); //trigger the ISR on the falling edge of button press

  // show that we're at the end of setup.
  displayHelloWorld();
  display.startscrollright(0x00, 0x0F);
  delay(4000);
}


void loop() {

  if (countFlag == TRUE) {
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(loopCount, DEC);
  display.display();
  loopCount++;
  delay(1);
  }
}


void buttonPressed()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();

  if (interrupt_time - last_interrupt_time > 150)  // debounce time
  {
    countFlag = !countFlag;
  }
  last_interrupt_time = interrupt_time;
}


void displayHelloWorld() {
  display.clearDisplay();
  display.display();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hello, Mike!");
  display.display();
}

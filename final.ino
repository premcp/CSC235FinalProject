#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip(45, 10);
int state = 0;
int buttonPin = 3;
boolean currentState = false;
boolean prevState = false;

//////// ROTARY ENCODER ////////////


int SW = 4;
int DT = 5;
int CLK = 6;

int currentState2;
int lastState2;
String currentDir = "";





///////////////////////





void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  strip.begin();

    pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  //  pinMode(SW, INPUT_PULLUP);
  lastState2 = digitalRead(CLK);
 
}

void loop() {


  

calculateRpm();

  /// Button

  prevState = currentState;


  currentState = debounce(buttonPin, prevState);

  if (currentState == true && prevState == false) {
    state++;
  }

  // if statement to return to state 0
  if (state > 4) {
    state = 0;
  }







  switch (state) {
    case 0:
      //strip.clear();
      colorChange(strip.Color(255, 0, 255), strip.Color(255, 0, 0), strip.Color(0, 0, 255), 500);
      strip.show();
      Serial.println("CASE 0!!!!");
      break;

    case 1:
      strip.clear();
      bouncingLight(strip.Color(0, 255, 0), 20);
      strip.show();
      Serial.println("CASE 1!!!!");
      break;

    case 2:
      //strip.clear();
      bouncingBrightness(strip.Color(255, 0, 255), 2);
      strip.show();
      Serial.println("CASE 2!!!!");
      break;

    case 3:
      strip.clear();
      loopingLight(strip.Color(255, 0, 255), 50);
      strip.show();
      Serial.println("CASE 3!!!!");
      break;
    case 4:
     
  calculateRpm();
 Serial.println("CASE 4!!!!");
      break;


  }

}

// debounce
boolean debounce(int buttonPin, boolean prevButtonReading) {
  // get current reading of a button
  boolean currentReading = digitalRead(buttonPin);

  // compare the current reading to the previous reading
  if (currentReading == true && prevButtonReading == false) {
    delay(20);
  }

  // return the reading
  return currentReading;
}

/// PART 4
void colorChange(uint32_t c1, uint32_t c2, uint32_t c3, int waitTime) {
  static int state = 0;
  static unsigned long startTime = millis();
  unsigned long currentTime = millis();

  if (currentTime - startTime >= waitTime) {
    state++;
    //strip.clear()
    startTime = millis();
    if (state > 2) {
      state = 0;
    }
  }

  if (state == 0) {
    for (int index = 0; index < strip.numPixels(); index ++) {
      // check for color one
      if (index % 3 == 0) {
        strip.setPixelColor(index, c1);
      } else if (index % 3 == 1) {
        strip.setPixelColor(index, c2);
      } else if (index % 3 == 2) {
        strip.setPixelColor(index, c3);
      }
      strip.show();
    }
  }

  if (state == 1) {
    for (int index = 0; index < strip.numPixels(); index ++) {
      // check for color one
      if (index % 3 == 0) {
        strip.setPixelColor(index, c2);
      } else if (index % 3 == 1) {
        strip.setPixelColor(index, c3);
      } else if (index % 3 == 2) {
        strip.setPixelColor(index, c1);
      }
      strip.show();
    }
  }

  if (state == 2) {
    for (int index = 0; index < strip.numPixels(); index ++) {
      // check for color one
      if (index % 3 == 0) {
        strip.setPixelColor(index, c3);
      } else if (index % 3 == 1) {
        strip.setPixelColor(index, c1);
      } else if (index % 3 == 2) {
        strip.setPixelColor(index, c2);
      }
      strip.show();
    }
  }




}

//// PART 2

void bouncingLight(uint32_t aColor, int aWaitTime) {
  static boolean isIncreasing = true;
  static int index = 0;
  static unsigned long startTime = millis();
  unsigned long currentTime = millis();

  if (isIncreasing == true) {
    if (currentTime - startTime >= aWaitTime) {
      index++;
      //strip.clear();
      startTime = millis();

      // if you reach the end of the strip, then reset the index
      if (index >= strip.numPixels() - 1) {
        isIncreasing = false;
      }
    }

    // turn on current neopixel
    strip.setPixelColor(index, aColor);
    strip.show();
  }

  if (isIncreasing == false) {
    if (currentTime - startTime >= aWaitTime) {
      index--;
      //strip.clear();
      startTime = millis();
      if (index <= 0) {
        isIncreasing = true;
      }
    }

    // turn on current neopixel
    strip.setPixelColor(index, aColor);
    strip.show();
  }
}

//// PART 3

void bouncingBrightness(uint32_t aColor, int aWaitTime) {
  static boolean isIncreasing = true;
  static int brightness = 0;
  static int index = 0;
  static unsigned long startTime = millis();
  unsigned long currentTime = millis();

  if (isIncreasing == true) {
    if (currentTime - startTime >= aWaitTime) {
      brightness++;
      //strip.clear();
      startTime = millis();

      // if you reach the end of the strip, then reset the index
      if (brightness >= 100) {
        isIncreasing = false;
      }
    }
  }




  if (isIncreasing == false) {
    if (currentTime - startTime >= aWaitTime) {
      brightness --;
      //strip.clear();
      startTime = millis();
      if (brightness <= 0) {
        isIncreasing = true;
      }
    }
  }



  // turn on current neopixel
  for (int index = 0; index < strip.numPixels(); index ++) {
    strip.setPixelColor(index, aColor);
    strip.setBrightness(brightness);
    strip.show();
  }
}

void loopingLight(uint32_t aColor, int aWaitTime) {
  static int index = 0;
  static unsigned long startTime = millis();
  unsigned long currentTime = millis();

  if (currentTime - startTime >= aWaitTime) {
    index++;
    //strip.clear();
    startTime = millis();
  }

  Serial.println(index);

  // turn on current neopixel
  strip.setPixelColor(index, aColor);
  strip.show();




  // if you reach the end of the strip, then reset the index
  if (index >= strip.numPixels()) {
    index = 0;
  }


}


/////// ROTARY ENCODER FUNCS //////

void calculateRpm() {

  boolean isSpinning = false;

  currentState2 = digitalRead(CLK);
  Serial.println(currentState2);

  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentState2 != lastState2  && currentState2 == 1) {
isSpinning = true;
  }
  /*if (isSpinning = true){
  rainbowTimer();
}*/
if (!currentState2) {
  rainbowTimer();
}
  
  lastState2 = currentState2;
 
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


void rainbowTimer() { // modified from Adafruit example to make it a state machine
  static unsigned long startTime = millis();
  int interval = 20;
  static uint16_t j = 0;


  if (millis() - startTime >= interval){
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    j++;
    if (j >= 256) j = 0;
    startTime = millis(); // time for next change to the display
  }
}

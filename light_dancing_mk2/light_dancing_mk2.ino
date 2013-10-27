int xPin = A0, yPin = A1, zPin = A2;
int redPin = 7, greenPin = 5, bluePin = 6;
int redVal, greenVal, blueVal, whiteVal;
int redDelay, greenDelay, blueDelay;
const float sampleCoef = 0.0625f;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

int bound(int aVal) {
  if( aVal<0 ) return 0;
  return aVal;
}

void loop() {
  redVal = analogRead(xPin);
  greenVal = analogRead(zPin);
  blueVal = analogRead(yPin);
  
  redVal -= 512;
  greenVal -= 512;
  blueVal -= 512;
  if( redVal<0 ) redVal = -redVal;
  if( greenVal<0 ) greenVal = -greenVal;
  if( blueVal<0 ) blueVal = -blueVal;

  // compute delays for each color
  redDelay = (int)((float)redVal * sampleCoef * 0.5f);
  greenDelay = (int)((float)greenVal * sampleCoef * 0.5f);
  blueDelay = (int)((float)blueVal * sampleCoef * 0.5f);
  int cycle = 256;
  
  // at the beginning of each cycle, turn the lights on
  if( redDelay>0 ) {
    digitalWrite(redPin, LOW);
  }
  if( greenDelay>0 ) {
    digitalWrite(greenPin, LOW);
  }
  if( blueDelay>0 ) {
    digitalWrite(bluePin, LOW);
  }

  // wait for each color in sequence
  if( redDelay<=blueDelay && redDelay<=greenDelay ) {
    // red first
    delayMicroseconds(bound(redDelay));
    cycle -= redDelay;
    greenDelay -= redDelay;
    blueDelay -= redDelay;
    digitalWrite(redPin, HIGH);
    if( greenDelay<=blueDelay ) {
      // then green
      delayMicroseconds(bound(greenDelay));
      cycle -= greenDelay;
      blueDelay -= greenDelay;
      digitalWrite(greenPin, HIGH);
      delayMicroseconds(bound(blueDelay));
      cycle -= blueDelay;
      digitalWrite(bluePin, HIGH);
    }else{
      // then blue
      delayMicroseconds(bound(blueDelay));
      cycle -= blueDelay;
      greenDelay -= blueDelay;
      digitalWrite(bluePin, HIGH);
      delayMicroseconds(bound(greenDelay));
      cycle -= greenDelay;
      digitalWrite(greenPin, HIGH);
    }
  }else if( greenDelay<=redDelay && greenDelay<=blueDelay ) {
    // green first
    delayMicroseconds(bound(greenDelay));
    cycle -= greenDelay;
    redDelay -= greenDelay;
    blueDelay -= greenDelay;
    digitalWrite(greenPin, HIGH);
    if( redDelay<=blueDelay ) {
      // then red
      delayMicroseconds(bound(redDelay));
      cycle -= redDelay;
      blueDelay -= redDelay;
      digitalWrite(redPin, HIGH);
      delayMicroseconds(bound(blueDelay));
      cycle -= blueDelay;
      digitalWrite(bluePin, HIGH);
    }else{
      // then blue
      delayMicroseconds(bound(blueDelay));
      cycle -= blueDelay;
      greenDelay -= blueDelay;
      digitalWrite(bluePin, HIGH);
      delayMicroseconds(bound(greenDelay));
      cycle -= greenDelay;
      digitalWrite(greenPin, HIGH);
    }
  }else{
    // blue first
    delayMicroseconds(bound(blueDelay));
    cycle -= blueDelay;
    greenDelay -= blueDelay;
    redDelay -= blueDelay;
    digitalWrite(bluePin, HIGH);
    if( greenDelay<=redDelay ) {
      // then green
      delayMicroseconds(bound(greenDelay));
      cycle -= greenDelay;
      redDelay -= greenDelay;
      digitalWrite(greenPin, HIGH);
      delayMicroseconds(bound(redDelay));
      cycle -= redDelay;
      digitalWrite(redPin, HIGH);
    }else{
      // then red
      delayMicroseconds(bound(redDelay));
      cycle -= redDelay;
      greenDelay -= redDelay;
      digitalWrite(redPin, HIGH);
      delayMicroseconds(bound(greenDelay));
      cycle -= greenDelay;
      digitalWrite(greenPin, HIGH);
    }
  }
  if( cycle>0 ) {
    delayMicroseconds(cycle);
  }

//  analogWrite(redPin, (int)((float)redVal * rCoef));
//  analogWrite(greenPin, (int)((float)greenVal * gCoef));
//  analogWrite(bluePin, (int)((float)blueVal * bCoef));
}


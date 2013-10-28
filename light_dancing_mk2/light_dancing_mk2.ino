int xPin = A0, yPin = A1, zPin = A2;
int xRaw = 1, yRaw = 1, zRaw = 1;
int redPin = 7, greenPin = 5, bluePin = 6;
int redVal = 0, greenVal = 0, blueVal = 0, whiteVal;
int redDelay, greenDelay, blueDelay;
const float sampleCoef = 1.0f;
const int kThresholdMin = 64;
const float kSampleWeight = 0.10f;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

int bound(int aVal) {
  if( aVal<0 ) return 0;
  return aVal;
}

void readAccel() {
  int tX = analogRead(xPin);
  int tY = analogRead(yPin);
  int tZ = analogRead(zPin);
  
  // smoothing algorithm
  xRaw = (int)((float)xRaw*(1.0f-kSampleWeight) + (float)tX*kSampleWeight);
  yRaw = (int)((float)yRaw*(1.0f-kSampleWeight) + (float)tY*kSampleWeight);
  zRaw = (int)((float)zRaw*(1.0f-kSampleWeight) + (float)tZ*kSampleWeight);
}

void referenceOffset() {
  redVal = zRaw - 512;
  greenVal = xRaw - 512;
  blueVal = yRaw - 512;
  if( redVal<0 ) redVal = -redVal;
  if( greenVal<0 ) greenVal = -greenVal;
  if( blueVal<0 ) blueVal = -blueVal;
}

void loop() {
  readAccel();
  
  referenceOffset();

  if( true ) {
    thresholdAxisMapping();
  }else{
    portionedPulseWidthModulation();
  }
  
}

void portionedPulseWidthModulation() {
  // compute delays for each color
  redDelay = bound((int)((float)redVal * sampleCoef));
  greenDelay = bound((int)((float)greenVal * sampleCoef));
  blueDelay = bound((int)((float)blueVal * sampleCoef));
  int cycle = 512;
  
  // at the beginning of each cycle, turn the lights on
  if( redDelay>0 ) {
    digitalWrite(redPin, LOW);
  }else{
    digitalWrite(redPin, HIGH);
  }
  if( greenDelay>0 ) {
    digitalWrite(greenPin, LOW);
  }else{
    digitalWrite(greenPin, HIGH);
  }
  if( blueDelay>0 ) {
    digitalWrite(bluePin, LOW);
  }else{
    digitalWrite(bluePin, HIGH);
  }

  // wait for each color in sequence
  if( redDelay<=blueDelay && redDelay<=greenDelay ) {
    // red first
    delayMicroseconds(redDelay);
    cycle = bound(cycle - redDelay);
    greenDelay = bound(greenDelay - redDelay);
    blueDelay = bound(blueDelay - redDelay);
    digitalWrite(redPin, HIGH);
    if( greenDelay<=blueDelay ) {
      // then green
      delayMicroseconds(greenDelay);
      cycle = bound(cycle - greenDelay);
      blueDelay = bound(blueDelay - greenDelay);
      digitalWrite(greenPin, HIGH);
      delayMicroseconds(blueDelay);
      cycle = bound(cycle - blueDelay);
      digitalWrite(bluePin, HIGH);
    }else{
      // then blue
      delayMicroseconds(blueDelay);
      cycle = bound(cycle - blueDelay);
      greenDelay = bound(greenDelay - blueDelay);
      digitalWrite(bluePin, HIGH);
      delayMicroseconds(greenDelay);
      cycle = bound(cycle - greenDelay);
      digitalWrite(greenPin, HIGH);
    }
  }else if( greenDelay<=redDelay && greenDelay<=blueDelay ) {
    // green first
    delayMicroseconds(greenDelay);
    cycle = bound(cycle - greenDelay);
    redDelay = bound(redDelay - greenDelay);
    blueDelay = bound(blueDelay - greenDelay);
    digitalWrite(greenPin, HIGH);
    if( redDelay<=blueDelay ) {
      // then red
      delayMicroseconds(redDelay);
      cycle = bound(cycle - redDelay);
      blueDelay = bound(blueDelay - redDelay);
      digitalWrite(redPin, HIGH);
      delayMicroseconds(blueDelay);
      cycle = bound(cycle - blueDelay);
      digitalWrite(bluePin, HIGH);
    }else{
      // then blue
      delayMicroseconds(blueDelay);
      cycle = bound(cycle - blueDelay);
      greenDelay = bound(greenDelay - blueDelay);
      digitalWrite(bluePin, HIGH);
      delayMicroseconds(greenDelay);
      cycle = bound(cycle - greenDelay);
      digitalWrite(greenPin, HIGH);
    }
  }else{
    // blue first
    delayMicroseconds(blueDelay);
    cycle = bound(cycle - blueDelay);
    greenDelay = bound(greenDelay - blueDelay);
    redDelay = bound(redDelay - blueDelay);
    digitalWrite(bluePin, HIGH);
    if( greenDelay<=redDelay ) {
      // then green
      delayMicroseconds(greenDelay);
      cycle = bound(cycle - greenDelay);
      redDelay = bound(redDelay - greenDelay);
      digitalWrite(greenPin, HIGH);
      delayMicroseconds(redDelay);
      cycle = bound(cycle - redDelay);
      digitalWrite(redPin, HIGH);
    }else{
      // then red
      delayMicroseconds(redDelay);
      cycle = bound(cycle - redDelay);
      greenDelay = bound(greenDelay - redDelay);
      digitalWrite(redPin, HIGH);
      delayMicroseconds(greenDelay);
      cycle = bound(cycle - greenDelay);
      digitalWrite(greenPin, HIGH);
    }
  }
  if( cycle>0 ) {
    delayMicroseconds(cycle);
  }
}

void thresholdAxisMapping() {
  if( redVal>kThresholdMin ) {
    digitalWrite(redPin, LOW);
  }else{
    digitalWrite(redPin, HIGH);
  }
  if( greenVal>kThresholdMin ) {
    digitalWrite(greenPin, LOW);
  }else{
    digitalWrite(greenPin, HIGH);
  }
  if( blueVal>kThresholdMin ) {
    digitalWrite(bluePin, LOW);
  }else{
    digitalWrite(bluePin, HIGH);
  }
  delayMicroseconds(512);
}


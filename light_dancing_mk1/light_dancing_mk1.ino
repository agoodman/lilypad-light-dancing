int xPin = A0, yPin = A1, zPin = A2;
int redPin = 7, greenPin = 9, bluePin = 8, whitePin = 11;
int redVal, greenVal, blueVal, whiteVal;
int slowPin = 10, medPin = 2, fastPin = 3;
boolean isSlow, isMed, isFast;
int flashPeriod;
float rCoef = 0.5f, gCoef = 0.5f, bCoef = 0.5f;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(whitePin, OUTPUT);
  pinMode(slowPin, INPUT);
  pinMode(medPin, INPUT);
  pinMode(fastPin, INPUT);
}

void loop() {
  redVal = analogRead(zPin);
  greenVal = analogRead(yPin);
  blueVal = analogRead(xPin);
//  analogWrite(redPin, (int)(rCoef * (float)redVal / 4.0f));
//  analogWrite(greenPin, (int)(gCoef * (float)greenVal / 4.0f));
//  analogWrite(bluePin, (int)(bCoef * (float)blueVal / 4.0f));
//  analogWrite(whitePin, whiteVal / 4);
  
  isSlow = digitalRead(slowPin);
  isMed = digitalRead(medPin);
  isFast = digitalRead(fastPin);
  whiteVal = 255;
  
  if( isFast ) {
    flashPeriod = 50000;
  }else if( isMed ) {
    flashPeriod = 100000;
  }else if( isSlow ) {
    flashPeriod = 150000;
  }else{
    flashPeriod = 0;
  }

  flashPeriod = 1000000;
  
  if( flashPeriod>0 ) {
    analogWrite(redPin, redVal);
    analogWrite(greenPin, greenVal);
    analogWrite(bluePin, blueVal);
    delayMicroseconds(flashPeriod / 2);
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
    delayMicroseconds(flashPeriod / 2);
  }else{
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
  }
}


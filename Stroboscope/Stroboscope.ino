int motorPin = 3;
int redPin = 9;  
int greenPin = 10; 
int bluePin = 11;  

bool motorActive = false;
bool strobeActive = false;
int strobeDelay = 1; 
int motorSpeed = 0;
bool motorDirection = true; 

void setup() {
  pinMode(motorPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  analogWrite(redPin, 255);
  analogWrite(greenPin, 255);
  analogWrite(bluePin, 255);
  analogWrite(motorPin, 0);

  Serial.begin(9600);
}

// Kada motor radi na 6V manuelnim testiranjem smo dobili da za strobe od 12ms i brzinom motora 31 pomocu arduina, soba za testiranje je bila u potpunom mraku.
// dobijamo stroboskopski efekat. Posto imamo 6 markera i motor se okrece veoma brzo to znaci da od markera do susednog markera potrebno je 0.012s(12ms) sto znaci da jedan obrtaj u sekundi dobijamo kada izracunamo
// RPS=1/(6*0.012) => 13,8 RPS motora u sekundama
// RPM ili broj obrtaja motora u minutu dobijamo kada pomnozimo sa 60 -> RPM=13,8*60 => 833RPM 

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.startsWith("MOTOR_SPEED")) {
      motorSpeed = command.substring(12).toInt();
      if (motorActive) {
        analogWrite(motorPin, motorSpeed);
      }
    } 
    else if (command == "START_MOTOR") {
      motorActive = true;
      analogWrite(motorPin, motorSpeed);
    } 
    else if (command == "STOP_MOTOR") {
      motorActive = false;
      analogWrite(motorPin, 0);
    } 
    else if (command.startsWith("STROBE_DELAY")) {
      strobeDelay = command.substring(13).toInt();
      strobeDelay = constrain(strobeDelay, 1, 1000);
    } 
    else if (command == "START_STROBE") {
      strobeActive = true;
      turnRGBOn();
      delay(strobeDelay);
    } 
    else if (command == "STOP_STROBE") {
      strobeActive = false;
      turnRGBOff();
    }
  }

  if (strobeActive) {
    turnRGBOn();
    delay(strobeDelay);
    turnRGBOff();
    delay(strobeDelay);
  }
}

void turnRGBOff() {
  analogWrite(redPin, 255);
  analogWrite(greenPin, 255);
  analogWrite(bluePin, 255);
}

void turnRGBOn() {
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
}
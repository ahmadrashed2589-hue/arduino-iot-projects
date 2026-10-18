// motor driver (digital pins)
int ENA = 9;  // right motor
int In1 = 8;
int In2 = 7;
int ENB = 3;  // left motor
int In3 = 5;
int In4 = 4;

int maxSpeed = 45;  // analog write pin (0 -> 255)

// IR sensors (digital pins)
int L_IR = 13;
int R_IR = 12;

int L_IR_Value;
int R_IR_Value;

int white = 1;
int black = 0;

int ledPin = 11;

void setup() {
  //LED
  pinMode(ledPin, OUTPUT);

  // IR sensors
  pinMode(L_IR, INPUT);
  pinMode(R_IR, INPUT);

  // Motor driver pins
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);

  // Set initial motor speed
  analogWrite(ENA, maxSpeed);  // setting the speed (EN pin) to "maxSpeed"
  analogWrite(ENB, maxSpeed);
}

void led(){
  //LED
  
  // زيادة السطوع تدريجياً
  for (int brightness = 0; brightness <= 255; brightness++) {
    analogWrite(ledPin, brightness);  // ضبط سطوع الـ LED
    delay(3);  // تأخير قليل بين كل خطوة لتوفير تأثير تدريجي
  }

  // تقليل السطوع تدريجياً
  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(ledPin, brightness);  // ضبط سطوع الـ LED
    delay(3);  // تأخير قليل بين كل خطوة لتوفير تأثير تدريجي
  }
  
}

void forward() {
  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);
}

void backward() {
  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);
  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);
}

void left() {
  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);
}

void right() {
  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);
  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);
}

void stop() {
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW);
}

void loop() {
  // Bang bang control
  L_IR_Value = digitalRead(L_IR);
  R_IR_Value = digitalRead(R_IR);

  if (L_IR_Value == white && R_IR_Value == white) {
    forward();
    digitalWrite(ledPin, LOW);
  } else if (L_IR_Value == black && R_IR_Value == white) {
    right();
    digitalWrite(ledPin, HIGH);
    delay(1);
    //digitalWrite(ledPin, LOW);

  } else if (L_IR_Value == white && R_IR_Value == black) {
    left();
    digitalWrite(ledPin, HIGH);
    //delay(0.5);
    //digitalWrite(ledPin, LOW);
  } else if (L_IR_Value == black && R_IR_Value == black) {
    stop();
    led();
  }
}

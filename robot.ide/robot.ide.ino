int ENA = 5;  // right motor
int In1 = 2;
int In2 = 4;
int ENB = 3;  // left motor
int In3 = 6;
int In4 = 7 ;

int leftMotorSpeed = 70;  // قيمة سرعة المحرك الأيسر
int rightMotorSpeed = 82; // قيمة سرعة المحرك الأيمن
int turnSpeedL = 60;
int turnSpeedR = 73;

// IR sensors (digital pins)
int L2_IR = 8;  // left IR sensor
int L1_IR = 9;  // front right IR sensor
int C_IR = 10;  // central IR sensor
int R1_IR = 11;  // right IR sensor
int R2_IR = 12;  // back IR sensor

int white = 1;
int black = 0;

// PID control variables
float Kp = 1.7;
float Ki = 0.0;
float Kd = 100.0;
float previousError = 0;
float integral = 10;

enum Direction {
  FORWARD,
  LEFT,
  RIGHT
};

Direction lastDirection = FORWARD;

void setup() {
  // IR sensors
  pinMode(L2_IR, INPUT);
  pinMode(C_IR, INPUT);
  pinMode(R1_IR, INPUT);
  pinMode(R2_IR, INPUT);  // set the back sensor pin as input
  pinMode(L1_IR, INPUT);  // set the front right sensor pin as input

  // Motor control pins
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  analogWrite(ENA, leftMotorSpeed);  // تطبيق إشارة PWM للمحرك الأيسر
  analogWrite(ENB, rightMotorSpeed); // تطبيق إشارة PWM للمحرك الأيمن

  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);

  Serial.begin(9600);  // Start serial communication for debugging
}

void forward() {
  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);

  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);
}

void left() {
  analogWrite(ENA, turnSpeedR - 40);
  analogWrite(ENB, turnSpeedL + 10);
  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);

  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);
}

void right() {
  analogWrite(ENA, turnSpeedL + 22);
  analogWrite(ENB, turnSpeedR - 40);
  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);

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
  // Read IR sensor values
  int L2_IR_Value = digitalRead(L2_IR);
  int C_IR_Value = digitalRead(C_IR);
  int R2_IR_Value = digitalRead(R2_IR);
  int R1_IR_Value = digitalRead(R1_IR);
  int L1_IR_Value = digitalRead(L1_IR);

  // Check if the robot reaches the white line
  if (L2_IR_Value == white && L1_IR_Value == white  && R1_IR_Value == white && R2_IR_Value == white) {
    // Invert white and black values
    int temp = white;
    white = black;
    black = temp;
  }
  if (L2_IR_Value == black && L1_IR_Value == black  && R1_IR_Value == black && R2_IR_Value == black) {
    // Invert white and black values
    int temp = white;
    white = black;
    black = temp;
  }

  // Calculate error
  float error = 0.0;
  if (L2_IR_Value == black) error -= 22.0;
  if (L1_IR_Value == black) error -= 18.0;
  if (C_IR_Value == black) error += 0.0;
  if (R1_IR_Value == black) error += 18.0;
  if (R2_IR_Value == black) error += 22.0;

  // Calculate PID terms
  float P = error;
  integral += error;
  float I = integral;
  float D = error - previousError;
  float correction = Kp * P + Ki * I + Kd * D;

  // Calculate motor speeds
  int leftSpeed = leftMotorSpeed + correction;
  int rightSpeed = rightMotorSpeed - correction;

  // Constrain speeds to maxSpeed
  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);
  
  // Move the robot
  analogWrite(ENA, rightSpeed);
  analogWrite(ENB, leftSpeed);

  // Update previous error
  previousError = error;

 /* if (  R1_IR_Value == black && L1_IR_Value == black || R2_IR_Value == black && L2_IR_Value == black){
  analogWrite(ENA, turnSpeedL );
  analogWrite(ENB, turnSpeedR - 100);
  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
  }*/
  // Determine direction based on sensor values
  if (L2_IR_Value == black || L1_IR_Value == black || C_IR_Value == black || R1_IR_Value == black || R2_IR_Value == black) {
    if (L2_IR_Value == black  ||  L1_IR_Value == black && C_IR_Value == black || L2_IR_Value == black && L1_IR_Value == black &&  C_IR_Value == black ) {
      lastDirection = LEFT;
    } else if (R2_IR_Value == black || R1_IR_Value == black && C_IR_Value == black || C_IR_Value == black && R1_IR_Value == black && R2_IR_Value == black) {
      lastDirection = RIGHT;
    } else {
      lastDirection = FORWARD;
    }

    forward();
  } else {
    // If all sensors read white, move in the last correction direction
    if (lastDirection == LEFT) {
      right();
    } else if (lastDirection == RIGHT ) {
      left();
    } else {
      forward();
    }

  }
  delay(5); // Delay for stability
}

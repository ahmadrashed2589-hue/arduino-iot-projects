const int in1Pin = 9;
const int in2Pin = 8;
const int enaPin = 10;
const int recvPin = A0;

void setup() {
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enaPin, OUTPUT);
  pinMode(recvPin, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  analogWrite(enaPin, 200); // ضبط سرعة المحرك (التردد)

  int signal = analogRead(recvPin);
  digitalWrite(in2Pin, LOW);
  digitalWrite(in1Pin, HIGH);
  
  //Serial.print("1 = ");
  //Serial.println(signal);
  delay(2);
  digitalWrite(in2Pin, HIGH);
  digitalWrite(in1Pin, LOW);
  
  //delay(20);
  //Serial.print("2 = ");
  //Serial.println(signal);
  
delay(100);
}

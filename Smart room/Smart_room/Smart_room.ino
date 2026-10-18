#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mySerial(11, 10); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

const int ldr = A0;          // حساس الإضاءة
const int pirPin = 2;        // حساس الحركة
const int trig2 = 3;         // حساس المسافة الثاني (الباب الثاني)
const int echo2 = 4;         // حساس المسافة الثاني (الباب الثاني)
const int trig1 = 5;         // حساس المسافة الأول (الباب الأول)
const int echo1 = 6;         // حساس المسافة الأول (الباب الأول)
const int relayLEDRGB = 7;   // ريليه إضاءة RGB
const int relayLED = 8;      // ريليه الإضاءة
const int relayFan = 9;      // ريليه المروحة
const int LED1 = 12;        // LED1
const int buttonFan = 13;
const int buttonLEDrgb = 14;
long duration1, distance1; // مدة و مسافة حساس المسافة الأول
long duration2, distance2; // مدة و مسافة حساس المسافة الثاني
bool sensor1Triggered = false;
bool sensor2Triggered = false;
int RLED1 = LOW;
int RrelayLED = LOW;         // حالة الـ LED
bool rgbState = false;
int lastButtonState = HIGH;

//Millis
unsigned long previousMillisLED = 0;
unsigned long previousMillisLED1 = 0;
unsigned long previousMillisFan = 0;
unsigned long previousMillisSound = 0;
unsigned long lastMotionTime = 0;


const long intervalLED = 10000;
const long intervalLED1 = 100;   //  ثواني
const long intervalFan = 5000;   // 5 ثواني
const long intervalSound = 10000; // 10 ثواني
const unsigned long timeout = 500000; // 10 ثواني
bool lightIsOn = false;
unsigned long doorTriggerTime = 0;
const unsigned long doorLEDduration = 5000; // 5 ثواني
bool doorLEDOn = false;


void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(relayLEDRGB, OUTPUT);
  pinMode(relayLED, OUTPUT);
  pinMode(relayFan, OUTPUT);
  pinMode(buttonFan, INPUT);
  pinMode(buttonLEDrgb, INPUT_PULLUP);


 // تهيئة الحالة الابتدائية للريليه
  digitalWrite(relayLEDRGB, HIGH);
  digitalWrite(relayLED, RrelayLED);
  digitalWrite(relayFan, HIGH);
  digitalWrite(LED1, RLED1);

  Serial.begin(9600);
  mySerial.begin(9600); // تهيئة الاتصال التسلسلي البرمجي مع DFPlayer Mini

}

void loop() {
  // قراءة الحساسات
  int lightLevel = analogRead(ldr);
  int pir = digitalRead(pirPin);
  int butonFan = digitalRead(butonFan);
  int butonLEDrgb = digitalRead(butonLEDrgb);
  int buttonState = digitalRead(buttonLEDrgb);


  unsigned long currentMillis = millis();
    // قراءة مسافة الحساس الأول
  digitalWrite(trig1, LOW);
  delayMicroseconds(2);
  digitalWrite(trig1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1, LOW);
  duration1 = pulseIn(echo1, HIGH);
  distance1 = duration1/ 58.2;

  // قراءة مسافة الحساس الثاني
  digitalWrite(trig2, LOW);
  delayMicroseconds(2);
  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);
  duration2 = pulseIn(echo2, HIGH);
  distance2 = duration2/ 58.2;

static unsigned long lastPrint = 0;
if (currentMillis - lastPrint >= 1000) {
  lastPrint = currentMillis;
  Serial.print("Light Level: ");
  Serial.println(lightLevel);
  Serial.print("PIR: ");
  Serial.println(pir);
  Serial.print("Distance 1: ");
  Serial.println(distance1);
  Serial.print("Distance 2: ");
  Serial.println(distance2);
  Serial.print("relayLEDRGB  ");
  Serial.println(rgbState);
  Serial.println("**********************************");

}
   // التحقق من وجود شخص في الغرفه والضوء منخفض
    // تشغيل إضاءة الغرفة
// إذا فيه حركة
  if (pir == HIGH) {
    lastMotionTime = currentMillis;

    if (lightLevel < 6 && rgbState == LOW) {
      digitalWrite(relayLED, LOW); // شغّل الضوء
      lightIsOn = true;
    }
  }

  // إذا مضى وقت بدون حركة
  if (lightIsOn && (currentMillis - lastMotionTime >= timeout)||lightLevel > 15||rgbState == HIGH) {
    digitalWrite(relayLED, HIGH); // طفي الضوء
    lightIsOn = false;
  }

// إذا في حدا قدام الباب (المسافة أقل من 190)
  if (distance1 < 190 && lightLevel < 15) {
    doorTriggerTime = currentMillis;
    digitalWrite(LED1, HIGH);
    doorLEDOn = true;
  }

  // إذا مضى وقت وما عاد في حدا قدام الباب
  if (doorLEDOn && (currentMillis - doorTriggerTime >= doorLEDduration)) {
    if (distance2 >= 190) {
      digitalWrite(LED1, LOW);
      doorLEDOn = false;
    }
  }
  //LEDRGB

  // التحقق من النزول من HIGH إلى LOW (يعني تم الضغط)
  if (buttonState == LOW && lastButtonState == HIGH) {
    rgbState = !rgbState;  // قلب الحالة
    digitalWrite(relayLEDRGB, rgbState ? LOW : HIGH); // الريليه يشتغل على LOW
    delay(200); // منع الاهتزاز البسيط (software debounce)
  }

  lastButtonState = buttonState;
  }  




// تعريف الدبابيس
const int pirPin = 2;  // دبوس إشارة حساس الحركة
const int ledPin = 12; // دبوس الـ LED

// متغير لتخزين حالة حساس الحركة
int dpirState = LOW;
int apirState = 0 ;

void setup() {
  // إعداد الدبابيس
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // بدء الاتصال التسلسلي لمراقبة القيم (اختياري)
  Serial.begin(9600);
}

void loop() {
  // قراءة قيمة حساس الحركة
  apirState = analogRead(pirPin);
  dpirState = digitalRead(pirPin);
    Serial.println(apirState);
  if (dpirState == HIGH) {
    // إذا تم اكتشاف حركة، إضاءة الـ LED
    digitalWrite(ledPin, HIGH);
    Serial.println("Motion detected!"); // إخراج رسالة للحالة (اختياري)
  } else {
    // إذا لم يتم اكتشاف حركة، إطفاء الـ LED
    digitalWrite(ledPin, LOW);
    Serial.println("No motion."); // إخراج رسالة للحالة (اختياري)
  }

  // تأخير صغير لتقليل الاهتزاز
  delay(200);
}

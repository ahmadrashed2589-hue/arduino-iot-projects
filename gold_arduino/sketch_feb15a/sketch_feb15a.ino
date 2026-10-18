// تعريف أرجل التحكم في وحدة L298N
const int in1 = 9;
const int in2 = 10;
const int enA = 11;
const int sensorPin = A0; // مدخل حساس كشف المعادن

void setup() {
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(enA, OUTPUT);
    pinMode(sensorPin, INPUT);
    analogWrite(enA, 200); // ضبط سرعة المحرك (يمكن تعديلها بين 0 و 255)
     Serial.begin(9600);
}

void loop() {
    // تشغيل التيار المتردد (تبديل الاتجاه باستمرار)
    // توليد تيار متردد لمدة قصيرة
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        delay(1);
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        delay(1);
    
    
    // إيقاف التيار المتردد مؤقتًا
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    delay(50);
    
    // قراءة الحساس
    int sensorValue = analogRead(sensorPin);
    if (sensorValue > 500) { // عتبة استشعار المعادن
        // تم الكشف عن معدن
      //  Serial.println("معدن مكتشف!");
    }
    Serial.println(sensorValue);
    // إعادة تشغيل التيار المتردد
    delay(1);
}

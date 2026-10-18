// تعريف مداخل ومخارج L298N
#define IN1 27     // متصل مع IN1 في L298N
#define IN2 26    // متصل مع IN2 في L298N
#define ENA 25    // تفعيل L298N (يمكنك تركه بدون تغيير)

// تعريف مدخل قراءة الإشارة من الترانزستور
#define SIGNAL_PIN 34  // متصل بالمجمع (Collector) للترانزستور

// تعريف المتغيرات
int signalValue = 0;  // لتخزين قراءة الإشارة
int threshold = 100;  // الحد الحساس للكشف عن المعادن

void setup() {
    Serial.begin(115200);  // تهيئة المنفذ التسلسلي
    
    // إعداد مداخل ومخارج L298N
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENA, OUTPUT);
    


    // إعداد مدخل ADC لقراءة الإشارة
    pinMode(SIGNAL_PIN, INPUT);
}

void loop() {

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 200);
    delay(150);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, 200);
    delay(150);
    // قراءة الإشارة القادمة من الترانزستور
    signalValue = analogRead(SIGNAL_PIN);
    /*for(int i = 0 ; i < 100; i++){
    // طباعة القيم للمتابعة
    Serial.print("Signal Value: ");
    Serial.println(signalValue);
    
    // الكشف عن وجود معدن
    if (signalValue > threshold) {
        Serial.println("⚡ معدن مكتشف!");
    }}*/
    
}

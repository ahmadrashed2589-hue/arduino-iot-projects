#define HALL_SENSOR_PIN A5  // تعريف مدخل مستشعر هال
#define BUZZER_PIN 9
#define LED_PIN 10
#define METAL_DETECTED_THRESHOLD 10000  // عتبة لاكتشاف التغيير في المجال المغناطيسي
#define METAL_DETECT_DURATION 1000  // فترة استمرار الإشعار عن اكتشاف المعدن
#define INACTIVE_DURATION 3000  // فترة عدم النشاط قبل إيقاف التشغيل

unsigned long lastDetectionTime = 0;

void setup() {
  pinMode(HALL_SENSOR_PIN, INPUT);  // تعيين مدخل مستشعر هال كمدخل
  pinMode(BUZZER_PIN, OUTPUT);  // تعيين دبوس المنبه كمخرج
  pinMode(LED_PIN, OUTPUT);  // تعيين دبوس الإشارة LED كمخرج
  digitalWrite(BUZZER_PIN, LOW);  // إيقاف المنبه
  digitalWrite(LED_PIN, LOW);  // إيقاف الإشارة LED
  Serial.begin(9600);  // بدء الاتصال المسلسل
}

void loop() {
  int hallSensorValue = analogRead(HALL_SENSOR_PIN);  // قراءة قيمة مستشعر هال
  Serial.println(hallSensorValue);  // طباعة قيمة المستشعر للمراقبة في مرسل السلسلة
  delay(500); // تأخير لتقليل سرعة الطباعة
  // إذا تجاوزت قيمة المستشعر العتبة المحددة، يتم اكتشاف تغيير في المجال المغناطيسي
  if (hallSensorValue > METAL_DETECTED_THRESHOLD) {
    lastDetectionTime = millis();  // تحديث الوقت لآخر مرة تم فيها اكتشاف المعدن
    metalDetected();  // تشغيل دالة التنبيه
  }
  
  // التحقق مما إذا كانت مدة اللا نشاط قد تجاوزت الحد المسموح به
  if (millis() - lastDetectionTime > INACTIVE_DURATION) {
    // قم بإيقاف المنبه والإشارة LED
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  }
}

// دالة التنبيه عن اكتشاف المعدن
void metalDetected() {
  tone(BUZZER_PIN, 1000);  // تشغيل المنبه
  digitalWrite(LED_PIN, HIGH);  // تشغيل الإشارة LED
}

#define vinPin A5 // تعريف لميناء الإدخال الذي يتم استخدامه لقراءة الجهد من مستشعر الكابستانس.
#define buz 8 //(السماعه)تعريف لميناء الإخراج الذي يتم استخدامه لتوصيل البازر.
#define pulsePin A4 //تعريف لميناء الإخراج الذي يستخدم لتوليد إشارات النبض.
#define led 10 //تعريف لميناء الإخراج الذي يستخدم لتحديد حالة الكشاف.
#include <TimerOne.h>

const int outputPin = 9;  // تحديد رقم الدبوس الذي يتم توصيله بالحمل (مثلاً مصباح)


long sumExpect=0; //يُستخدم لتخزين مجموع القراءات على مر 64 قراءة. يُستخدم لحساب القيمة المتوقعة. 
long ignor=0;   //يُستخدم كعداد لتتبع عدد القراءات التي تم تجاوزها أو تجاوزها.
long diff=0;        //يُستخدم لتخزين الفارق بين القيمة الحالية ومتوسط القراءات.
long pTime=0;   //يُستخدم لتخزين الوقت الحالي بالميلي ثانية.
long buzPeriod=0;   // يُستخدم لتخزين فترة التشغيل المتوقعة للمنبه.
bool isMetal = false;  // متغير بولي (boolean) يُستخدم لتمثيل ما إذا كان هناك اكتشاف لمعدن أم لا.
int threshold =100;   // يُستخدم لتحديد عتبة (قيمة حد) يتم فيها اعتبار القراءة كاكتشاف لمعدن.

void applyPulses() //توليد نبضات
{
    for (int i = 0; i < 1000; i++) 
    {
      digitalWrite(pulsePin, LOW);  // إرسال إشارة HIGH للـ pulsePin
      delayMicroseconds(2);  // التأخير لمدة 2 ميكروثانية
      digitalWrite(pulsePin, HIGH);  // إرسال إشارة LOW للـ pulsePin
      delayMicroseconds((2));  // التأخير لمدة 2 ميكروثانية
    }
}

void setup() 
{// تكوين ميناء الاتصال التسلسلي لتحديثات الإخراج
  Serial.begin(9600);
   // تكوين ميناء الإخراج لتوليد إشارات النبض
  pinMode(pulsePin, OUTPUT); 
  // تكوين ميناء الإخراج ايقاف إشارات النبض
  digitalWrite(pulsePin, LOW);
  // تكوين ميناء الإدخال لقياس قيمة الجهد من مستشعر الكابستانس
  pinMode(vinPin, INPUT);  
  // (الصوت)تكوين ميناء الإخراج لتوصيل البازر
  pinMode(buz, OUTPUT);
  // يعين حالها الميناء في البدايه من ما يؤدي  الى ايقاف الصوت 
  digitalWrite(buz, LOW);
  // يعين ميناء الاخراج على تشغيل الضوء
  pinMode(led, OUTPUT);

 pinMode(outputPin, OUTPUT);
  int frequency = 3;  // هنا يتم تحديد التردد بوحدة هرتز
  Timer1.initialize(1000000 / frequency);  // تحديد التردد بالميكروثانية
  Timer1.pwm(outputPin, 100);  // تحديد نسبة العمل لتكوين موجة PWM مستطيلة

}

void loop() 
{
  int minval = 1042;  // قيمة بداية للحد الأدنى
  int maxval = 0;     // قيمة بداية للحد الأقصى
  long unsigned int sum = 0;  // متغير لحساب مجموع القراءات

  for (int i = 0; i < 200; i++)
  {
    // إعادة تعيين الكاباسيتور
    pinMode(vinPin, OUTPUT);
    digitalWrite(vinPin, HIGH);
    delayMicroseconds(100);
    pinMode(vinPin, INPUT);
    applyPulses();

    // قراءة قيمة الشحنة على الكاباسيتور
    int val = analogRead(vinPin);  // يأخذ 13x8=104 ميكروثانية
    minval = min(val, minval);  // تحديث قيمة الحد الأدنى إذا كانت القراءة الحالية أقل
    maxval = max(val, maxval);  // تحديث قيمة الحد الأقصى إذا كانت القراءة الحالية أكبر
    sum += val;  // إضافة القراءة إلى المجموع

  //Serial.println(sum);

    // التمييز بين المعدن والغير معدن
   if (val > threshold) {
      isMetal = true;
    }

    long unsigned int cTime = millis();  // الوقت الحالي بالميلي ثانية
    char buzState = 0;  // حالة المنبه (0، 1، 2)

    // فحص حالة المنبه استنادًا إلى الفرق بين القراءات والزمن
    if (cTime < pTime + 1)
    {
      if (diff > 0)
        buzState = 1;
      else if (diff < 0)
        buzState = 0;
    }
    if (cTime > pTime + buzPeriod)
    {
      if (diff > 0)
        buzState = 1;
      else if (diff < 0)
        buzState = 2;
      pTime = cTime;  // تحديث الزمن
    }
    if (buzPeriod > 100)
      buzState = 0;  // إيقاف المنبه إذا كانت فترة المنبه كبيرة

    // التحكم في المنبه والصمام بناءً على حالة المنبه
    if (buzState == 0)
    {
      digitalWrite(led, LOW);
      noTone(buz);
    }
    else if (buzState == 1)
    {
      tone(buz, 2000);
      digitalWrite(led, HIGH);
    }
    else if (buzState == 2)
    {
      tone(buz, 500);
      digitalWrite(led, HIGH);
    }
    // التحكم في المنبه والصمام بناءً على وجود المعدن
  if (isMetal) {
    tone(buz, 2000);
    digitalWrite(led, HIGH);
  } else {
    noTone(buz);
    digitalWrite(led, LOW);
  }

  // إعادة تعيين متغير وجود المعدن للدورة القادمة
  isMetal = false;
  }

  // خصم القيم الحد الأدنى والحد الأقصى لإزالة الذروة
  sum -= minval; 
  sum -= maxval;
  
  if (sumExpect == 0) 
    sumExpect = sum << 6;  // تعيين sumExpect إلى القيمة المتوقعة
  long int avgsum = (sumExpect + 32) >> 6;  // حساب متوسط القيم
  diff = sum - avgsum;  // حساب الفرق بين القيمة الحالية والمتوسطة

  // إذا كانت الفرق صغيرة بما فيه الكفاية، قم بتحديث sumExpect وإعادة تعيين العداد
  if (abs(diff) < avgsum >> 10)
  {
    sumExpect = sumExpect + sum - avgsum;
    ignor = 0;
  } 
  else 
    ignor++;

  // إذا كان العداد تجاوز الحد المسموح، قم بإعادة تعيين sumExpect والعداد
  if (ignor > 64)
  { 
    sumExpect = sum << 6;
    ignor = 0;
  }

  // إذا كان الفرق يساوي صفر، قم بتعيين buzPeriod إلى قيمة كبيرة لإيقاف المنبه
  if (diff == 0) 
    buzPeriod = 1000000;
  else 
    buzPeriod = avgsum / (3 * abs(diff));  // حساب فترة المنبه
}
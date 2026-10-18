// Number of cycles from external counter needed to generate a signal event
#define CYCLES_PER_SIGNAL 5000

// Base tone frequency (speaker)
#define BASE_TONE_FREQUENCY 280

// Frequency delta threshold for fancy spinner to trigger
#define SPINNER_THRESHOLD 700

// Pin definitions
#define SENSITIVITY_POT_APIN 1
#define SPEAKER_PIN 2
#define SPINNER_PIN 9
#define TRIGGER_BTN_PIN 11
#define RESET_BTN_PIN 12

unsigned long lastSignalTime = 0;
unsigned long signalTimeDelta = 0;

boolean firstSignal = true;
unsigned long storedTimeDelta = 0;

// This signal is called whenever OCR1A reaches 0
// (Note: OCR1A is decremented on every external clock cycle)
SIGNAL(TIMER1_COMPA_vect)
{
  unsigned long currentTime = micros();
  signalTimeDelta =  currentTime - lastSignalTime;
  lastSignalTime = currentTime;

  if (firstSignal)
  {
    firstSignal = false;
  }
  else if (storedTimeDelta == 0)
  {
    storedTimeDelta = signalTimeDelta;
  }

  // Reset OCR1A
  OCR1A += CYCLES_PER_SIGNAL;
}

void setup()
{
  // Set WGM(Waveform Generation Mode) to 0 (Normal)
  TCCR1A = 0b00000000;
  
  // Set CSS(Clock Speed Selection) to 0b111 (External clock source on T0 pin
  // (ie, pin 5 on UNO). Clock on rising edge.)
  TCCR1B = 0b00000111;

  // Enable timer compare interrupt A (ie, SIGNAL(TIMER1_COMPA_VECT))
  TIMSK1 |= (1 << OCIE1A);

  // Set OCR1A (timer A counter) to 1 to trigger interrupt on next cycle
  OCR1A = 1;

  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(SPINNER_PIN, OUTPUT);
  pinMode(TRIGGER_BTN_PIN, INPUT_PULLUP);
  pinMode(RESET_BTN_PIN, INPUT_PULLUP);
}

void loop()
{
  if (digitalRead(TRIGGER_BTN_PIN) == LOW)
  {
    float sensitivity = mapFloat(analogRead(SENSITIVITY_POT_APIN), 0, 1023, 0.5, 10.0);
    int storedTimeDeltaDifference = (storedTimeDelta - signalTimeDelta) * sensitivity;
    tone(SPEAKER_PIN, BASE_TONE_FREQUENCY + storedTimeDeltaDifference);

    if (storedTimeDeltaDifference > SPINNER_THRESHOLD)
    {
      digitalWrite(SPINNER_PIN, HIGH);
    }
    else
    {
      digitalWrite(SPINNER_PIN, LOW);
    }
  }
  else
  {
    noTone(SPEAKER_PIN);
    digitalWrite(SPINNER_PIN, LOW);
  }

  if (digitalRead(RESET_BTN_PIN) == LOW)
  {
    storedTimeDelta = 0;
  }
}

float mapFloat(int input, int inMin, int inMax, float outMin, float outMax)
{
  float scale = (float)(input - inMin) / (inMax - inMin);
  return ((outMax - outMin) * scale) + outMin;
}

int SpeakerPin = D0;
int PumpButton = D3;
int PumpPin = D2;
int SpeakerButton = D4;
int soundPin = A0;
int count = 0;
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

void setup() {
  // set up Serial Monitor
  Serial.begin(115200);
  delay(10000);
  // set up pins
  pinMode(SpeakerButton, INPUT);
  pinMode(PumpPin, OUTPUT);
  pinMode(SpeakerPin, OUTPUT);
  pinMode(PumpButton, INPUT);
  pinMode(soundPin, INPUT);
}


void loop()
{
  if (Particle.connected() == false) {
    Particle.connect();
  }
//  PumpOutput();
//  SpeakerOutput(); 
  micInput();
}

int PumpOutput()
{
  //Particle.variable("cmd", cmd);
  int PumpButtonState = digitalRead(PumpButton);
  if (PumpButtonState == HIGH)
  {
    digitalWrite(PumpPin, HIGH);
  }
  if (PumpButtonState == LOW)
  {
    digitalWrite(PumpPin, LOW);
  }
}

int SpeakerOutput()
{
  int buttonState = digitalRead(SpeakerButton);
  if (buttonState == HIGH)
  {
    tone(SpeakerPin, 226);
  }
  if (buttonState == LOW)
  {
    noTone(SpeakerPin);
  }
}

int micInput() {
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(0);
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  double volts = (peakToPeak * 5.0) / 1024; // convert to volts
  Particle.publish("Mic Reading", String(volts) + " V");
  delay(1000);
}

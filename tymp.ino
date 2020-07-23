int SpeakerPin = 5;
int SpeakerButton = 7;
int PumpPin = 4;
int PumpButton = 1;
int soundPin = A0;
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
void setup() {
  // set up Serial Monitor
  // Serial.begin();
  
  // set up pins
  Serial.begin(9600);
  pinMode(SpeakerButton, INPUT);
  pinMode(PumpPin, OUTPUT);
  pinMode(SpeakerPin, OUTPUT);
  pinMode(PumpButton, INPUT);
  pinMode(soundPin, INPUT);
}


void loop() 
{
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // collect data for 50 mS
   SpeakerOutput();
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
   float volts = (peakToPeak * 5.0)/ 1024;  // convert to volts
   Serial.println(volts);
    //PumpOutput();
}

int PumpOutput()
{
    //Particle.variable("cmd", cmd);
    int buttonState = digitalRead(PumpButton);
    if (buttonState == LOW)
    {
        digitalWrite(PumpPin, HIGH);
        bool success;
    }
    if (buttonState == HIGH)
    {
        digitalWrite(PumpPin, LOW);
        bool success;
    }
}

int SpeakerOutput()
{
//    Particle.variable("cmd", cmd);
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

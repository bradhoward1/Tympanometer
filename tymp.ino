int SpeakerPin = 5;
int PumpButton = 7;
int PumpPin = 4;
int SpeakerButton = 8;
int soundPin = A0;
int count = 0;
int micButton = 13;
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

void setup() {
  // set up Serial Monitor
  
  // set up pins
  Serial.begin(9600);
  pinMode(SpeakerButton, INPUT);
  pinMode(PumpPin, OUTPUT);
  pinMode(SpeakerPin, OUTPUT);
  pinMode(PumpButton, INPUT);
  pinMode(soundPin, INPUT);
  pinMode(micButton, INPUT);
}


void loop() 
{
  PumpOutput();
  SpeakerOutput();
  micInput();
}

int PumpOutput()
{
    //Particle.variable("cmd", cmd);
    int PumpButtonState = digitalRead(PumpButton);
    if (PumpButtonState == HIGH)
    {
        digitalWrite(PumpPin, HIGH);
        bool success;
    }
    if (PumpButtonState == LOW)
    {
        digitalWrite(PumpPin, LOW);
        bool success;
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

int micInput(){
    unsigned long startMillis= millis();  // Start of sample window
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
   float volts = (peakToPeak * 5.0)/ 1024;  // convert to volts
   int button = digitalRead(micButton);
   Serial.println(peakToPeak);
}

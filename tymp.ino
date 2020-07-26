// packages to include
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// initial variables and pin definitions
int SpeakerPin = 5;
int PumpButton = 7;
int PumpPin = 4;
int SpeakerButton = 8;
int soundPin = A0;
int count = 0;
int micButton = 13;
int micValue = 0;
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
int lastMicValue;
unsigned long lastTime;

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte server[] = { 192, 168, 111, 50 };
byte ip[]     = { 192, 168, 111, 240 };


void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived (no messages expected though)
}

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);


void setup() {
  // set up server connection
  Ethernet.begin(mac, ip);
  if (client.connect("arduinoClient")) {
    client.publish("demo/status/arduino01","online");
    lastMicValue=0;
    lastTime=0;
  }
  
  // set up Serial Monitor
  Serial.begin(9600);
  
  // set up pins
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
  micValue = micInput();
  sendToServer(micValue);
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
   unsigned float peakToPeak = 0;   // peak-to-peak level  // this was originally an int, change from float if there are problems
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
//   float volts = (peakToPeak * 5.0)/ 1024;  // convert to volts
   return peakToPeak;
}


int sendToServer(float micVal) {
  if(micVal!=lastMicValue) {
    if(millis()>(lastTime+100)) {
      sprintf(buffer,"%d",micVal);
      client.publish("demo/device/arduino01",buffer);
      lastMicValue=micVal;
      lastTime=millis();
    }
  }
  client.loop();
}

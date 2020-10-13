#define stp D6
#define dir D7
#define MS1 D5
#define EN  RX
#define PUMPPIN D1


Servo myservo;



int SpeakerPin = D0;
int SpeakerPin2 = D2;
int PumpButton = D3;
//int PumpPin = D2;
int SpeakerButton = D4;
int soundPin = A0;
int PressurePin = A1;
int count = 0;
int step_count = 0;
int x;
int loop_count = 0;
int overall_count = 0;
uint16_t server_info[9];
uint16_t pressure_info[9];
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
uint16_t sample;
uint16_t PressureSample;

void setup() {
  // set up Serial Monitor
  Serial.begin(115200);
  delay(5000);
  // set up pins
  pinMode(SpeakerButton, INPUT);
 // pinMode(PumpPin, OUTPUT);
  pinMode(SpeakerPin, OUTPUT);
  pinMode(PumpButton, INPUT);
  pinMode(soundPin, INPUT);
  pinMode(PressurePin, INPUT);
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
//  pinMode(MS2, OUTPUT);
  pinMode(EN, OUTPUT);
  resetEDPins();
  myservo.attach(PUMPPIN);
  myservo.write(180);
  noTone(SpeakerPin);
  noTone(SpeakerPin2);

}


void loop()
{
  if (Particle.connected() == false) {
    Particle.connect();
  }
//  PumpOutput();
  SpeakerOutput();
  uint16_t data = micInput();
  uint16_t P_data = PressureInput();
  server_info[count] = data;
  pressure_info[count] = P_data;
//  Serial.println(count);
  if (count == 8){
    char payload[255];
    snprintf(payload, sizeof(payload)
            , "{ \"s\":\"Patient_8\""
              ", \"v1\": %d"
              ", \"v2\": %d"
              ", \"v3\": %d"
              ", \"v4\": %d"
              ", \"v5\": %d"
              ", \"v6\": %d"
              ", \"v7\": %d"
              ", \"v8\": %d"
              ", \"v9\": %d"
              ", \"v10\": %d"
              ", \"v11\": %d"
              ", \"v12\": %d"
              ", \"v13\": %d"
              ", \"v14\": %d"
              ", \"v15\": %d"
              ", \"v16\": %d"
              ", \"v17\": %d"
              ", \"v18\": %d"
              "}"
            , (uint16_t) server_info[0]
            , (uint16_t) server_info[1]
            , (uint16_t) server_info[2]
            , (uint16_t) server_info[3]
            , (uint16_t) server_info[4]
            , (uint16_t) server_info[5]
            , (uint16_t) server_info[6]
            , (uint16_t) server_info[7]
            , (uint16_t) server_info[8]
            , (uint16_t) pressure_info[0]
            , (uint16_t) pressure_info[1]
            , (uint16_t) pressure_info[2]
            , (uint16_t) pressure_info[3]
            , (uint16_t) pressure_info[4]
            , (uint16_t) pressure_info[5]
            , (uint16_t) pressure_info[6]
            , (uint16_t) pressure_info[7]
            , (uint16_t) pressure_info[8]
            );
    count = 0;
    Serial.println(payload);
    Particle.publish("SendToServer", payload, PRIVATE);
  }
  else {
      count += 1;
      step_count += 1; 
  }
//  if (step_count = 40){
//      StepForwardDefault();
//      Particle.publish("Merp",PRIVATE);
//      step_count = 0;
//  }
delay(100);
  overall_count += 1;
  if (overall_count >= 200) {
    delay(10000);
    myservo.write(90);
    get_mic_info();
    delay(4000);
    send_email_out();
    delay(1000);
    noTone(SpeakerPin);
    noTone(SpeakerPin2);
    int stopper_variable = 1;
    while (stopper_variable == 1) {
      Particle.process();
    }
  }
} 



//int PumpOutput()
//{
//  //Particle.variable("cmd", cmd);
//  int PumpButtonState = digitalRead(PumpButton);
//  if (PumpButtonState == HIGH)
//  {
//    digitalWrite(PumpPin, HIGH);
//  }
//  if (PumpButtonState == LOW)
//  {
//    digitalWrite(PumpPin, LOW);
//  }
//}


int SpeakerOutput()
{
    tone(SpeakerPin, 226);
    tone(SpeakerPin2, 226);
}


int micInput() {
  unsigned long startMillis = millis(); // Start of sample window
  int peakToPeak = 0;   // peak-to-peak level
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 mS
//  while (millis() - startMillis < sampleWindow)
//  {
    sample = analogRead(A3);
//    if (sample < 1024)  // toss out spurious readings
//    {
//      if (sample > signalMax)
//      {
//        signalMax = sample;  // save just the max levels
//      }
//      else if (sample < signalMin)
//      {
//        signalMin = sample;  // save just the min levels
//      }
//    }
//  }
//  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
//  double volts = (peakToPeak * 5.0) / 1024; // convert to volts
//  Particle.publish("Mic Reading", String(volts) + " V");
//  delay(1000);
  return sample;
}


int get_mic_info() {
  char payload[255];
  snprintf(payload, sizeof(payload)
          , "{ \"s\": Patient_8"
            "}"
          );
  Serial.println(payload);
  Particle.publish("GetMicData", payload, PRIVATE);
  return 1;
}


int send_email_out() {
  char payload[255];
  snprintf(payload, sizeof(payload)
          , "{ \"s\": \"Patient_8\""
            "}"
          );
  Serial.println(payload);
  Particle.publish("SendEmail", payload, PRIVATE);
  return 1;
}


int PressureInput() {
  double PressureOut = 0;
  double VoltageR = 0;
  PressureSample = analogRead(PressurePin);
//  VoltageR = PressureSample;
//  VoltageR = VoltageR * (5 / 4096);
//  if (VoltageR <= 0.275){
//    PressureOut = 3755.3648 * (VoltageR) - 1211.0515;
//  }
//  else if (VoltageR >= 0.275) {
//    PressureOut = 85.6409 * (VoltageR) - 213.3421;
//  }
  return PressureSample;
}


void resetEDPins()
{
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, LOW);
//  digitalWrite(MS2, LOW);
  digitalWrite(EN, HIGH);
}


void SmallStepMode()
{
  Serial.println("Stepping at 1/8th microstep mode.");
  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
  digitalWrite(MS1, HIGH); //Pull MS1, and MS2 high to set logic to 1/8th microstep resolution
//  digitalWrite(MS2, HIGH);
  for(x= 0; x<1000; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
}
//Default microstep mode function
void StepForwardDefault()
{
  Serial.println("Moving forward at default step mode.");
  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
  for(x= 0; x<1000; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  Serial.println("Enter new option");
  Serial.println();
}

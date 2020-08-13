int SpeakerPin = D0;
int PumpButton = D3;
int PumpPin = D2;
int SpeakerButton = D4;
int soundPin = A0;
int PressurePin = A1;
int count = 0;
int loop_count = 0;
int overall_count = 0;
double server_info[7];
double pressure_info[7];
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
unsigned int PressureSample;

void setup() {
  // set up Serial Monitor
  Serial.begin(115200);
  delay(5000);
  // set up pins
  pinMode(SpeakerButton, INPUT);
  pinMode(PumpPin, OUTPUT);
  pinMode(SpeakerPin, OUTPUT);
  pinMode(PumpButton, INPUT);
  pinMode(soundPin, INPUT);
  pinMode(PressurePin, INPUT);
  Particle.connect();
  tone(SpeakerPin, 226, 2000);
}


void loop()
{
  if (Particle.connected() == false) {
    Particle.connect();
  }
//  PumpOutput();
//  SpeakerOutput();
  double data = micInput();
  double P_data = PressureInput();
  server_info[count] = data;
  pressure_info[count] = P_data;
//  Serial.println(count);
  if (count == 6){
    char payload[255];
    snprintf(payload, sizeof(payload)
            , "{ \"s\":\"Patient_11\""
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
              "}"
            , (int) server_info[0]
            , (int) server_info[1]
            , (int) server_info[2]
            , (int) server_info[3]
            , (int) server_info[4]
            , (int) server_info[5]
            , (int) server_info[6]
            , (int) pressure_info[0]
            , (int) pressure_info[1]
            , (int) pressure_info[2]
            , (int) pressure_info[3]
            , (int) pressure_info[4]
            , (int) pressure_info[5]
            , (int) pressure_info[6]
            );
    count = 0;
    Serial.println(payload);
    Particle.publish("SendToServer", payload, PRIVATE);
    delay(100);
  }
  else {
      count += 1;
  }
  overall_count += 1;
  if (overall_count >= 300) {
    delay(10000);
    get_mic_info();
    delay(4000);
    send_email_out();
    delay(1000);
    int stopper_variable = 1;
    while (stopper_variable == 1) {
      Particle.process();
    }
  }
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
  double peakToPeak = 0;   // peak-to-peak level
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
//  Particle.publish("Mic Reading", String(volts) + " V");
//  delay(1000);
  return peakToPeak;
}


int get_mic_info() {
  char payload[255];
  snprintf(payload, sizeof(payload)
          , "{ \"s\": Patient_11"
            "}"
          );
  Serial.println(payload);
  Particle.publish("GetMicData", payload, PRIVATE);
  return 1;
}


int send_email_out() {
  char payload[255];
  snprintf(payload, sizeof(payload)
          , "{ \"s\": \"Patient_11\""
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
  VoltageR = PressureSample;
  VoltageR = VoltageR * (5 / 4096);
  if (VoltageR <= 0.275){
    PressureOut = 3755.3648 * (VoltageR) - 1211.0515;
  }
  else if (VoltageR >= 0.275) {
    PressureOut = 85.6409 * (VoltageR) - 213.3421;
  }
  return PressureOut;
}
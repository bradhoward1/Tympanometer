int SpeakerPin = D0;
int PumpButton = D3;
int PumpPin = D2;
int SpeakerButton = D4;
int soundPin = A0;
int PressurePin = A1;
int count = 0;
int overall_count = 0;
double server_info[15];
double pressure_info[15];
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
  if (count == 14){
    char payload[255];
    snprintf(payload, sizeof(payload)
            , "{ \"s\":\"Values\""
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              "}"
            // , "Values"
            , server_info[0]
            , server_info[1]
            , server_info[2]
            , server_info[3]
            , server_info[4]
            , server_info[5]
            , server_info[6]
            , server_info[7]
            , server_info[8]
            , server_info[9]
            , server_info[10]
            , server_info[11]
            , server_info[12]
            , server_info[13]
            , server_info[14]
            );
    Serial.println(payload);
    Particle.publish("SendToServer", payload, PRIVATE);
    char payload_pressure[255];
    snprintf(payload_pressure, sizeof(payload_pressure)
            , "{ \"s\":\"Values\""
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              ", \"v\": %f"
              "}"
            // , "Values"
            , pressure_info[0]
            , pressure_info[1]
            , pressure_info[2]
            , pressure_info[3]
            , pressure_info[4]
            , pressure_info[5]
            , pressure_info[6]
            , pressure_info[7]
            , pressure_info[8]
            , pressure_info[9]
            , pressure_info[10]
            , pressure_info[11]
            , pressure_info[12]
            , pressure_info[13]
            , pressure_info[14]
            );
    Serial.println(payload_pressure);
    Particle.publish("SendPressure", payload_pressure, PRIVATE);
    count = -1;
  }
  else {
      count += 1;
  }
  overall_count += 1;
  if (overall_count >= 2000) {
    get_mic_info("Values");
    int stopper_variable = 1;
    while (stopper_variable == 1) {
      delay(100);
      
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


int get_mic_info(String input_string) {
  char payload[255];
  snprintf(payload, sizeof(payload)
          , "{ \"s\": %s"
            "}"
          , input_string
          );
  Serial.println(payload);
  Particle.publish("GetMicData", payload, PRIVATE);
}


int PressureInput() {
  PressureSample = analogRead(PressurePin);
}



const int PHOTORESISTOR_PIN = A5;
const int POTENTIOMETER_PIN = A4;
const int PHOTOCOUPLER_PIN = 4;

const int POTENTIOMETER_LOWER_BOUND = 600;
const int POTENTIOMETER_UPPER_BOUND = 1024;

void setup() {
   Serial.begin(9600); // open the serial port at 9600 bps:

   pinMode(PHOTOCOUPLER_PIN, OUTPUT);
   pinMode(POTENTIOMETER_PIN, INPUT);
   pinMode(PHOTORESISTOR_PIN, INPUT);
}

int state = 0;

long photoInitiated = -1;

void loop() {
  // Reads current values
  float sensorValue = analogRead(PHOTORESISTOR_PIN);
  float potentiometerValue = analogRead(POTENTIOMETER_PIN);

  // Maps potentiometer values to new scale
  float sensorThreshold = map(potentiometerValue, 0, 1024, POTENTIOMETER_LOWER_BOUND, POTENTIOMETER_UPPER_BOUND);

  // Update state. TODO: if values flicker - filter may be needed
  int previousState = state;
  if (sensorValue > sensorThreshold) {
    state = 1;
  } else {
    state = 0;
  }

  // If state changed to 1 and photo is not being taken initiate process
  if (previousState == 0 && state == 1 && photoInitiated == -1) {
    photoInitiated = millis();
  } 

  if (photoInitiated != -1) {
    // Complete the process after fixed duration and reset the timer
    if (millis() - photoInitiated > 1000) {
        photoInitiated = -1;
    }
  
    // Connect octocoupler for fixed duration at the start of the process
    if (millis() - photoInitiated < 800) {
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(PHOTOCOUPLER_PIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(PHOTOCOUPLER_PIN, LOW);
    }
  }
  
  Serial.print("Sensor: ");
  Serial.print(sensorValue);
  Serial.print(" Threshold: ");
  Serial.print(sensorThreshold);
  Serial.print(" Timer: ");
  Serial.println(millis() - photoInitiated);
}

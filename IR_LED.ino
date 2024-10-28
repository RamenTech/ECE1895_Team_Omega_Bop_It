// ESP32 code for generating 38kHz on pin 18
const int outputPin = 18;  // Output for 38kHz square wave
const int inputPin = 19;   // Input to monitor
const int controlPin = 21; // Output controlled by inputPin

void setup() {
  // Set pin modes
  pinMode(inputPin, INPUT);
  pinMode(controlPin, OUTPUT);


  // Configure 38kHz PWM on outputPin (38kHz, 50% duty cycle)
  // In the new API, we use ledcAttach to configure the LEDC
  ledcAttach(outputPin, 38000, 8); // Attach pin, frequency 38kHz, 50% duty cycle
}

void loop() {
  // Read the input pin state
  int inputState = digitalRead(inputPin);
  ledcWrite(outputPin, 128);

  // Set the control pin based on input state
  if (inputState == LOW) {
    digitalWrite(controlPin, HIGH);
  } else {
    digitalWrite(controlPin, LOW);
  }
}

#define NBR_TGGL 1
#define NBR_SNSR 1

int toggle[NBR_TGGL] = {13};
int sensor[NBR_SNSR] = {16};

typedef struct {
  uint16_t pin;
  char action[2];
}Command;

void setup() {
  Serial.begin(115200);

  // Initializes toggle modules
  for (int i = 0; i < NBR_TGGL; i++) {
    pinMode(toggle[i], OUTPUT);  
  }

}

Command cmd;
boolean success;
String value; 

void loop() {
  value = "";
  cmd = readCommand();
  if (*cmd.action == 'p') {
    success = on(cmd.pin);
  } else if (*cmd.action == 'a') {
    success = off(cmd.pin);
  } else if (*cmd.action == 's') {
    value = getStatus(cmd.pin);
  }

  if (value != "") {
    Serial.print(value + ";");
  } else {
    if (success) {
      Serial.print("True");
    } else {
      Serial.print("False");
    }
  }
}

// Read command from serial and parse it into a command struct
Command readCommand() {
  while (Serial.available() < 3) {};
  uint16_t pin = Serial.parseInt();
  char c = Serial.read();
  return (Command) {
    pin, c
  };
}

// Checks what type a pin is, returns -1 if the pin is not initialized
int typeOfPin(int pin) {
  for (int i = 0; i < NBR_TGGL; i++) {
    if (toggle[i] == pin) {
      return 0;
    }
  }

  for (int i = 0; i < NBR_SNSR; i++) {
    if (sensor[i] == pin) {
      return 1;
    }
  }

  return -1;
}

String getStatus(int pin) {
  switch (typeOfPin(pin)) {
    case -1: return String(-1); 
    case 0: return String(statusDigital(pin)); 
    case 1: return String(statusAnalog(pin)); 
  }
}

// A couple of functions to get info and set pins on the arduino

// Sets pin to HIGH
boolean on(int pin) {
  if (typeOfPin(pin) != 0) {
    return false;
  }
  digitalWrite(pin, HIGH);
  return digitalRead(pin) == HIGH;
}

// Sets pin to LOW
boolean off(int pin) {
  if (typeOfPin(pin) != 0) {
    return false;
  }
  digitalWrite(pin, LOW);
  return digitalRead(pin) == LOW;
}

// Reads the value of a simple passive analog sensor
float statusAnalog(int pin) {
  return analogRead(A0);
}

// Reads the digital value of an output pin
int statusDigital(int pin) {
  return digitalRead(pin);
}

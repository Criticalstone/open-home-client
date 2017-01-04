#define NBR_MODULES 1

int modules[NBR_MODULES] = {13};

typedef struct {
  uint16_t pin;
  char action[2];
}Command;

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < NBR_MODULES; i++) {
    pinMode(modules[i], OUTPUT);  
  }
  digitalWrite(13, LOW);
}

Command cmd;

void loop() {
  cmd = readCommand();
  if (*cmd.action == 'p') {
    on(cmd.pin);
    Serial.println("Hej");
  } else if (*cmd.action == 'a') {
    off(cmd.pin);
  }
}

// Read command from serial and parse it into a command struct
Command readCommand() {
  while (Serial.available() < 3) {};
  uint16_t pin = Serial.parseInt();
  char c = Serial.read();
  Serial.println(pin);
  Serial.println(c);
  return (Command) {
    pin, c
  };
}

// A couple of functions to get info and set pins on the arduino

// Sets pin to HIGH
boolean on(int pin) {
  digitalWrite(pin, HIGH);
  return digitalRead(pin) == HIGH;
}

// Sets pin to LOW
boolean off(int pin) {
  digitalWrite(pin, LOW);
  return digitalRead(pin) == LOW;
}


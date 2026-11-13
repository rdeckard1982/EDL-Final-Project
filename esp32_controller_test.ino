#include <Bluepad32.h>

const int ledPin = 2;
ControllerPtr myController = nullptr; // empty pointer
bool ledToggleState = false;
bool lastAButtonState = false;

void onConnectedController(ControllerPtr controller) {
  if (myController == nullptr) {
    //Serial.println("8BitDo Pro Connected!");
    myController = controller;
    // Blink 3 times quickly to confirm pairing visually
    for (int i = 0; i < 3; i++) {
      digitalWrite(ledPin, HIGH);
      delay(100);
      digitalWrite(ledPin, LOW);
      delay(100);
    }
  }
}

void onDisconnectedController(ControllerPtr controller) {
  if (myController == controller) {
    //Serial.println("8BitDo Pro Disconnected.");
    myController = nullptr;
    digitalWrite(ledPin, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  BP32.setup(&onConnectedController, &onDisconnectedController);
}

void loop() {
  BP32.update();

  if (myController && myController->isConnected()) {
    // Option A: Press the right face button ('A' on 8BitDo) to toggle the LED on/off
    bool currentA = myController->b(); // Bluepad32 uses Xbox layout naming, so 'b()' is the right face button
    if (currentA && !lastAButtonState) {
      ledToggleState = !ledToggleState;
      digitalWrite(ledPin, ledToggleState ? HIGH : LOW);
    }
    lastAButtonState = currentA;

    // Option B: Hold the bottom face button ('B' on 8BitDo) for momentary light
    if (myController->a()) {
      digitalWrite(ledPin, HIGH);
    } else if (!ledToggleState) {
      digitalWrite(ledPin, LOW);
    }
  }

  delay(20);
}
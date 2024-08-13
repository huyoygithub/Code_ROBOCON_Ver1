#include <WiFi.h>
// #include <ESP8266WiFi.h>
#include <AccelStepper.h>

#include <WiFiClient.h>

#include <String.h>

// AccelStepper rotaryTop(1, 40, 39);
const char* ssid = "bigDick";
const char* password = "23456789";
WiFiServer server(80);
String command;


const int pwmFL = 4;
const int dirFL = 5;

const int pwmRL = 6;
const int dirRL = 7;

const int pwmFR = 1;
const int dirFR = 2;

const int pwmRR = 42;
const int dirRR = 41;

const int stepPin = 47;  // Step pin
const int dirPin = 48;   // Direction pin

bool isRun = false;
int stepRotary = 1000;

//////--- Tay kẹp//////////////
const int tk13 = 40;
const int tk2 = 39;
const int tk46 = 38;
const int tk5 = 37;
const int up = 36;
const int hut = 35;
const int mid = 0;
const int kick = 45;
//
void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  server.begin();
  Serial.println("Access Point started");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());  //

  pinMode(dirRL, OUTPUT);
  pinMode(pwmRL, OUTPUT);

  pinMode(dirFL, OUTPUT);
  pinMode(pwmFL, OUTPUT);

  pinMode(dirRR, OUTPUT);
  pinMode(pwmRR, OUTPUT);

  pinMode(dirFR, OUTPUT);
  pinMode(pwmFR, OUTPUT);

  analogWrite(pwmRL, 255);

  analogWrite(pwmFL, 255);

  analogWrite(pwmRR, 255);

  analogWrite(pwmFR, 255);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  digitalWrite(dirPin, HIGH);
  ////
  pinMode(tk13, OUTPUT);
  pinMode(tk2, OUTPUT);
  pinMode(tk46, OUTPUT);
  pinMode(tk5, OUTPUT);

  pinMode(hut, OUTPUT);
  pinMode(up, OUTPUT);
  pinMode(kick, OUTPUT);
  pinMode(mid, OUTPUT);

  digitalWrite(tk13, LOW);
  digitalWrite(tk2, LOW);
  digitalWrite(tk46, LOW);
  digitalWrite(tk5, LOW);

  digitalWrite(hut, LOW);
  digitalWrite(up, LOW);
  digitalWrite(kick, LOW);
  digitalWrite(mid, LOW);
}
void readEndSendCMD() {
  String all_command = "";

  WiFiClient client = server.available();

  if (client) {
    String request = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;
        if (c == '\r') {

          int start = request.indexOf("GET /") + 5;
          int end = request.indexOf("HTTP/");
          command = request.substring(start, end);

          Serial.println(command);
          command.replace("\n", "");
          command.replace("\r", "");
          command.replace(" ", "");   // removes all space characters
          command.replace("\t", "");  // removes all tab characters
          command.trim();

          all_command = command + " is run";  // green is on

          if (client.peek() == '\n') {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            String commandWithTags = "<html><body>" + all_command + "</body></html>";
            client.println(commandWithTags);
            break;
          }
        }
      }
    }
  }
}
void loop() {
  readEndSendCMD();
  handleCMD();

}

void handleCMD(){
    if (command.equals("F")) {
    // digitalWrite(relay_1, 1);
    // readCMD();

    forward(200);
  } else if (command.equals("L")) {
    // digitalWrite(relay_2, 1);
    moveLeft(200);
  } else if (command.equals("R")) {
    moveRight(200);
    // digitalWrite(relay_3, 1);
  } else if (command.equals("RL")) {
    rotaryL(200);

  } else if (command.equals("RR")) {
    rotaryR(200);

  } else if (command.equals("B")) {
    backward(200);
  } else if (command.equals("RL180")) {
    rotary180L(400, 200);

  }

  else if (command.equals("RR180")) {
    rotary180R(400, 200);

  }

  else if (command.equals("XLN_UP")) {
    digitalWrite(up, HIGH);
  } else if (command.equals("XLN_DOWN")) {
    digitalWrite(up, LOW);
  }




  else if (command.equals("XLM_UP")) {
    digitalWrite(mid, HIGH);
  } else if (command.equals("XLM_DOWN")) {
    digitalWrite(mid, LOW);
  }



  else if (command.equals("TK13_UP")) {
    digitalWrite(tk13, HIGH);
  } else if (command.equals("TK13_DOWN")) {
    digitalWrite(tk13, LOW);
  }

  else if (command.equals("TK46_UP")) {
    digitalWrite(tk46, HIGH);
  } else if (command.equals("TK46_DOWN")) {
    digitalWrite(tk46, LOW);
  }

  else if (command.equals("TK2_UP")) {
    digitalWrite(tk2, HIGH);
  } else if (command.equals("TK5_DOWN")) {
    digitalWrite(tk2, LOW);
  }

  else if (command.equals("SHOOT")) {
    digitalWrite(kick, HIGH);
  } else if (command.equals("C_SHOT")) {
    digitalWrite(kick, LOW);
  }

  else if (command.equals("XLH_UP")) {
    digitalWrite(hut, HIGH);
  } else if (command.equals("XLH_DOWN")) {
    digitalWrite(hut, LOW);
  }
}
void forward(int speed) {
  digitalWrite(dirRL, HIGH);
  analogWrite(pwmRL, speed);

  digitalWrite(dirFL, HIGH);
  analogWrite(pwmFL, speed);

  digitalWrite(dirRR, LOW);
  analogWrite(pwmRR, speed);

  digitalWrite(dirFR, LOW);
  analogWrite(pwmFR, speed);
  delay(100);
  stopNow();
  readCMD();
}
void backward(int speed) {
  digitalWrite(dirRL, LOW);
  analogWrite(pwmRL, speed);

  digitalWrite(dirFL, LOW);
  analogWrite(pwmFL, speed);

  digitalWrite(dirRR, HIGH);
  analogWrite(pwmRR, speed);

  digitalWrite(dirFR, HIGH);
  analogWrite(pwmFR, speed);
  delay(100);
  stopNow();
  readCMD();
}
void moveLeft(int speed) {
  digitalWrite(dirRL, HIGH);
  analogWrite(pwmRL, speed);

  digitalWrite(dirFL, LOW);
  analogWrite(pwmFL, speed);

  digitalWrite(dirRR, HIGH);
  analogWrite(pwmRR, speed);

  digitalWrite(dirFR, LOW);
  analogWrite(pwmFR, speed);
  delay(100);
  stopNow();
  readCMD();
}
void moveRight(int speed) {
  digitalWrite(dirRL, LOW);
  analogWrite(pwmRL, speed);
  digitalWrite(dirFL, HIGH);
  analogWrite(pwmFL, speed);
  digitalWrite(dirRR, LOW);
  analogWrite(pwmRR, speed);
  digitalWrite(dirFR, HIGH);
  analogWrite(pwmFR, speed);
  delay(100);
  stopNow();
  readCMD();
}
void rotaryL(int speed) {
  digitalWrite(dirRL, HIGH);
  analogWrite(pwmRL, speed);
  digitalWrite(dirFL, HIGH);
  analogWrite(pwmFL, speed);
  digitalWrite(dirRR, HIGH);
  analogWrite(pwmRR, speed);
  digitalWrite(dirFR, HIGH);
  analogWrite(pwmFR, speed);
  delay(100);
  stopNow();
  readCMD();
}
void rotaryR(int speed) {
  digitalWrite(dirRL, LOW);
  analogWrite(pwmRL, speed);
  digitalWrite(dirFL, LOW);
  analogWrite(pwmFL, speed);
  digitalWrite(dirRR, LOW);
  analogWrite(pwmRR, speed);
  digitalWrite(dirFR, LOW);
  analogWrite(pwmFR, speed);
  delay(100);
  stopNow();
  readCMD();
}
void rotary180R(int numberOfStep, int speed) {
  for (int i = 0; i <= numberOfStep; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speed);
  }
}
void rotary180L(int numberOfStep, int speed) {
  for (int i = 0; i <= numberOfStep; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(-speed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(-speed);
  }
}
void stopNow() {
  analogWrite(pwmRL, 255);
  analogWrite(pwmFL, 255);
  analogWrite(pwmRR, 255);
  analogWrite(pwmFR, 255);
}

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "FGTK9p2fTk9wE_zQ3upYsDX1HQVzQNOU";
//char auth[] = "JmtEX6pJgvIc35RENpKXOTA9OAcKbHvl"; // ลูกค้า
char ssid[] = "P";
char pass[] = "12345678";

int speed_control = 300;
int switch_left_right;
int on_off;

const int on_off_button = D3;
const int left_right_button = D4;
int count_button = 0;
int state = 0;

int count_button_ = 0;
int state_ = 0;

BlynkTimer timer;
BlynkTimer speed_write;

void myTimerEvent() {
  int read_left_right = digitalRead(left_right_button);
  int read_on_off = digitalRead(on_off_button);
  int read_speed = analogRead(A0);

  if (read_on_off == 1 && state == 0) {
    if (count_button == 1) {
      digitalWrite(D6 , 0);
    }
    else if (count_button == 2) {
      digitalWrite(D6 , 1);
      count_button = 0;
    }
    count_button++;
    state = 1;
  }
  else if (read_on_off == 0 && state == 1) {
    state = 0;
  }


  if (read_speed > 700 && speed_control < 900) {
    speed_control++;
  }
  else if (read_speed < 400  && speed_control > 80) {
    speed_control--;
  }


  if (read_left_right == 0 && state_ == 0) {
    if (count_button_ == 1) {
      switch_left_right = 0;
    }
    else if (count_button_ == 2) {
      switch_left_right = 1;
      count_button_ = 0;
    }
    count_button_++;
    state_ = 1;
  }
  else if (read_left_right == 1 && state_ == 1) {
    state_ = 0;
  }
}


void my_speed_write() {
  Blynk.virtualWrite(V2 , speed_control);
}


BLYNK_WRITE(V0) {
  on_off = param.asInt();
  digitalWrite(D6 , on_off);
}

BLYNK_WRITE(V1) {
  switch_left_right = param.asInt();
}

BLYNK_WRITE(V2) {
  speed_control = param.asInt();
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth , ssid , pass);
  pinMode(D1 , OUTPUT);
  pinMode(D2 , OUTPUT);
  pinMode(D6 , OUTPUT);

  digitalWrite(D6 , 1);
  digitalWrite(D2 , 0);
  digitalWrite(D1 , 0);

  pinMode(on_off_button , INPUT);
  pinMode(left_right_button , INPUT);
  timer.setInterval(10L, myTimerEvent);
  speed_write.setInterval(1000L, my_speed_write);
}

void loop() {
  Blynk.run();
  timer.run();
  speed_write.run();
  digitalWrite(D1 , 1);
  delayMicroseconds(speed_control);
  digitalWrite(D1 , 0);
  delayMicroseconds(speed_control);
  digitalWrite(D2 , switch_left_right);
}

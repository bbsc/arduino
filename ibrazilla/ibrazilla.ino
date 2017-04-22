#include <Servo.h>

Servo myservo;
int lowNote = 30;
int topNote = 770;
int eye1 = 7;
int eye2 = 6;

int updown = 0;
int sensor = 3;
int flip = 0;

int firebreath1 = 18;
int firebreath2 = 17;

int buzzer = 10;
int buzzer_gnd = 11;

Servo servo1;
char buffer [32];
int val = 0;

void setup() {
  myservo.attach(14);
  randomSeed(analogRead(0));
  pinMode(eye1, OUTPUT);
  pinMode(eye2, OUTPUT);
  pinMode(firebreath1, OUTPUT);
  pinMode(firebreath2, OUTPUT);
  pinMode(sensor, INPUT_PULLUP);
  pinMode(buzzer_gnd, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer_gnd, LOW);
}

void early_warning_alert() {
  for (int i = 0; i < 10; ++i) {
    tone(buzzer, 1200,  0);
    delay(100);
    tone(buzzer, 500,  0);
    delay(100);
  }
  noTone(buzzer);
}

void growl() {
  int loops = 400;
  int noteDuration = 10;
  for (int thisNote = 0; thisNote < loops; thisNote++) {
    int note = thisNote + random(lowNote, topNote);
    tone(8, note, noteDuration);
    delay(.25);
  }
  for (int thisNote = loops; thisNote > 0; thisNote--) {
    int note = thisNote + random(lowNote, topNote / 2);
    tone(8, note, noteDuration);
    delay(2);
  }
  noTone(8);
}

void blink_eyes() {
  for (int i = 0; i < 15; ++i) {
    digitalWrite(eye1, HIGH);
    digitalWrite(eye2, LOW);
    delay(50);
    digitalWrite(eye1, LOW);
    digitalWrite(eye2, HIGH);
    delay(50);
  }
}

void fade_eyes() {
  if (flip == 0) {
    for (int i = 10; i < 225; ++i) {
      analogWrite(eye2, i);
      delay(5);
    }
  } else {
    for (int i = 225; i > 10; --i) {
      analogWrite(eye2, i);
      delay(5);
    }
  }
  flip ^= 1;
}

void fat_big_boy() {
  for (int j = 50; j > 0; --j) {
    digitalWrite(firebreath1, LOW);
    digitalWrite(firebreath2, HIGH);
    delay(random(50));
    digitalWrite(firebreath1, HIGH);
    digitalWrite(firebreath2, LOW);
    delay(random(25));
  }
  // turn off fire breath
  digitalWrite(firebreath2, HIGH);
}

void move_arm() {
  int offset = random(55, 145);

  if (updown) {
    // up
    myservo.write(offset);
    delay(200);
    myservo.write(offset - 35);
    updown = 0;
  } else {
    // down
    myservo.write(0);
    updown = 1;
  }
}

int zilla_spotted = 0;
int loop_counter = 0;
void loop() {
  if (digitalRead(sensor) == HIGH) {
    // ibrazilla is comeing
    if (zilla_spotted == 0) {
      early_warning_alert();
      zilla_spotted = 1;
    }
    move_arm();
    // loop counter played 20 times growl
    if (loop_counter <= 0) {
      growl();
      loop_counter = 15;
    }
    move_arm();
    fat_big_boy();
    move_arm();
    blink_eyes();
    move_arm();
  } else {
    fade_eyes();
    zilla_spotted = 0;
  }
  loop_counter = loop_counter - 1;
}

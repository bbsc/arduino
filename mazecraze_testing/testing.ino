int cols[] = {11, 14, 15, 3, 17, 4, 8, 9};
int rows[] = {2, 10, 19, 7, 12, 18, 13, 16};

// the setup function runs once when you press reset or power the board
void setup() {
  for (int i = 0; i < 8; ++i) {
    pinMode(rows[i], OUTPUT);
    digitalWrite(rows[i], HIGH);
  }
  for (int i = 0; i < 8; ++i) {
    pinMode(cols[i], OUTPUT);
    digitalWrite(cols[i], LOW);
  }
}

void light_led(int x, int y) {
  digitalWrite(rows[x], LOW);
  digitalWrite(cols[y], HIGH);
  delay(250);
  digitalWrite(rows[x], HIGH);
  digitalWrite(cols[y], LOW);
}

void loop() {
  tone(5,350, 250);
  for (int x = 0; x < 8; ++x) {
    for (int y = 0; y < 8; ++y) {
      light_led(x, y);
    }
  }
}

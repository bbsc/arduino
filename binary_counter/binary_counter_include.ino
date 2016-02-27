#define POV_DELAY 5
#define SHORT_DELAY 25
#define LONG_DELAY 250
#define BLANK_IDX 10
#define ZERO_IDX 0

// pins that control which 7 segment display will be lit
int segment_control_pins[] = { 19, 18, 17 };
int switch_pins[] = { 7, 6, 5, 4, 3, 2, 1, 0 };

// test patterns
int off[] = { 0, 0, 0, 0, 0, 0, 0 };
int g[] = { 0, 0, 0, 0, 0, 0, 1 };
int f[] = { 0, 0, 0, 0, 0, 1, 1 };
int e[] = { 0, 0, 0, 0, 1, 1, 1 };
int d[] = { 0, 0, 0, 1, 1, 1, 1 };
int c[] = { 0, 0, 1, 1, 1, 1, 1 };
int b[] = { 0, 1, 1, 1, 1, 1, 1 };
int a[] = { 1, 1, 1, 1, 1, 1, 1 };
int* test_patterns_1[] = { a, b, c, d, e, f, g, off };
int* test_patterns_2[] = { g, f, e, d, c, b, a, off };

int d_seg[]   = { 0, 0, 0, 1, 0, 0, 0 };
int cde[]     = { 0, 0, 1, 1, 1, 0, 0 };
int cdeg[]    = { 0, 0, 1, 1, 1, 0, 1 };
int bcdefg[]  = { 0, 1, 1, 1, 1, 1, 1 };
int abcdefg[] = { 1, 1, 1, 1, 1, 1, 1 };

int* test_patterns_3[] = { d_seg, cde, cdeg, bcdefg, abcdefg, off };

void setup() {
  Serial.begin(9600);
  // while the serial stream is not open, do nothing:
  while (!Serial) ;

  // initialize 7 segment display pins
  for (int thisPin = 0; thisPin < sizeof(big_7segment_display) / sizeof(int); thisPin++)  {
    pinMode(big_7segment_display[thisPin], OUTPUT);
  }

  // initialize 3, 7 segment power control pins
  for (int thisPin = 0; thisPin < sizeof(segment_control_pins) / sizeof(int); thisPin++)  {
    pinMode(segment_control_pins[thisPin], OUTPUT);
  }

  // initialize 3, switch input pins
  for (int thisPin = 0; thisPin < sizeof(switch_pins) / sizeof(int); thisPin++)  {
    pinMode(switch_pins[thisPin], INPUT_PULLUP);
  }

  startup_led_test();
}

void startup_led_test() {
  // illuminate each segment separately for startup test
  for (int display = 0; display < 3; ++display) {
    select_display(display);
    for (int thisPin = 0; thisPin < 7; thisPin++) {
      digitalWrite(big_7segment_display[thisPin], HIGH);
      delay(SHORT_DELAY);
    }
    delay(LONG_DELAY);
    blank_display(big_7segment_display);
  }
  delay(LONG_DELAY);

  // test patterns
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 5; ++j) {
      select_display(0);
      light_segments(big_7segment_display, test_patterns_1[i], 0, 0);

      select_display(1);
      light_segments(big_7segment_display, test_patterns_1[i], 0, 0);

      select_display(2);
      light_segments(big_7segment_display, test_patterns_1[i], 0, 0);
    }
  }

  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 5; ++j) {
      select_display(0);
      light_segments(big_7segment_display, test_patterns_2[i], 0, 0);

      select_display(1);
      light_segments(big_7segment_display, test_patterns_2[i], 0, 0);

      select_display(2);
      light_segments(big_7segment_display, test_patterns_2[i], 0, 0);
    }
  }

  for (int i = 0; i < 6; ++i) {
    // j controls how many times the display is lit (duration)
    for (int j = 0; j < 5; ++j) {
      select_display(0);
      light_segments(big_7segment_display, test_patterns_3[i], 0, 0);

      select_display(1);
      light_segments(big_7segment_display, test_patterns_3[i], 0, 0);

      select_display(2);
      light_segments(big_7segment_display, test_patterns_3[i], 0, 0);
    }
  }

  delay(LONG_DELAY);
  select_display(0);
  blank_display(big_7segment_display);
  select_display(1);
  blank_display(big_7segment_display);
  select_display(2);
  blank_display(big_7segment_display);
  delay(SHORT_DELAY);
}

int blank_display(int* display) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(display[i], LOW);
  }
}

int light_segments(int* display, int* digit, int segment_delay, int pov_delay) {
  for (int i = 0; i <  7; i++) {
    int pin = display[i];
    int new_state = digit[i];
    int old_state = digitalRead(pin);

    if (old_state != new_state) {
      digitalWrite(pin, new_state);
    }
    delay(segment_delay);
  }
  delay(pov_delay);
  blank_display(display);
}

int select_display(int n) {
  // short-cut, just turn-off all displays before powering display n
  for (int i = 0; i < sizeof(segment_control_pins) / sizeof(int); i++) {
    digitalWrite(segment_control_pins[i], LOW);
  }
  digitalWrite(segment_control_pins[n], HIGH);
}

void set_place_values() {
  //Define as a float so that we can += pow() which returns a float
  float display_sum_float = 0;
  for (int i = 0; i < sizeof(switch_pins) / sizeof(int); i++) {
    if (digitalRead(switch_pins[i]) == LOW) {
      display_sum_float += pow(2, i);
    }
  }

  //Cast as int so that we can do mods.
  display_sum_float += 0.1;
  int display_sum_int = (int) display_sum_float;
//  Serial.print("Sum: ");
//  Serial.print(display_sum_int);

  place_values[0] = (display_sum_int) % 10;
  place_values[1] = ((display_sum_int - place_values[0]) / 10) % 10;
  place_values[2] = (display_sum_int - place_values[1] * 10 - place_values[0]) / 100;

  // left-pad with blanks when value is 0
  if (place_values[2] == 0) {
    place_values[2] = BLANK_IDX;
    if (place_values[1] == 0) {
      place_values[1] = BLANK_IDX;
      if (place_values[0] == 0) {
        place_values[0] = ZERO_IDX;
      }
    }
  }
}


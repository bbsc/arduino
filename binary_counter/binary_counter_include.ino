#define POV_DELAY 5
#define SHORT_DELAY 25
#define LONG_DELAY 250

// pins that control which 7 segment display will be lit
int segment_control_pins[] = { 19, 18, 17 };
int switch_pins[] = { 7, 6, 5, 4, 3, 2, 1, 0 };

int BLANK_IDX = 10;

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
  for (int thisPin = 0; thisPin < sizeof(big_7segment_display)/sizeof(int); thisPin++)  {
    pinMode(big_7segment_display[thisPin], OUTPUT);      
  }

  // initialize 3, 7 segment power control pins
  for (int thisPin = 0; thisPin < sizeof(segment_control_pins)/sizeof(int); thisPin++)  {
    pinMode(segment_control_pins[thisPin], OUTPUT);      
  }
  
  // initialize 3, switch input pins
  for (int thisPin = 0; thisPin < sizeof(switch_pins)/sizeof(int); thisPin++)  {
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
  if (pov_delay < POV_DELAY) {
    pov_delay = POV_DELAY;
  }
  for (int i = 0; i <  7; i++) {
    int pin = display[i];
    int new_state = digit[i];
    int old_state = digitalRead(pin);
  
    if (old_state != new_state) {
      digitalWrite(pin, new_state);
    }
    delay(segment_delay);
  }
  delay(pov_delay);  // stay lit  a short time to optimize PoV effect
}

int select_display(int n) {
  // short-cut, just turn-off all displays before powering display n
  for (int i = 0; i < sizeof(segment_control_pins)/sizeof(int); i++) {
    digitalWrite(segment_control_pins[i], LOW);
  }
  digitalWrite(segment_control_pins[n], HIGH);
}


int set_switch_values(int *ones, int *tens, int *hundreds) {
  int display_sum = 0;
  int switch_value = 1;

  for (int i = 0; i < sizeof(switch_pins)/sizeof(int); ++i) {
    if (digitalRead(switch_pins[i]) == LOW) {
      display_sum += switch_value;
    }
    switch_value = switch_value + switch_value;
  }

  *ones = (display_sum / 1) % 10;
  *tens  = (display_sum / 10) % 10;
  *hundreds = (display_sum / 100) % 10;

  // left-pad with blanks when value is 0
  if (*hundreds == 0) {
    *hundreds = BLANK_IDX;
    if (*tens == 0) {
      *tens = BLANK_IDX;
      if (*ones == 0) {
        *ones = BLANK_IDX;
      }
    }
  }
}


int display_digit = 0;
int long_wait = 250;
int short_wait = 25;
int pov_wait = 5;

// pins that control which 7 segment display will be lit
int segment_control_pins[] = { 14, 15, 16 };
int switch_pins[] = { 17, 18, 19, 11, 12, 13, 2, 3 };

// pin order for standard segments A,B,C,D,E,F,G
int big_7segment_display[] = { 4, 5, 6, 7, 8, 9, 10 };
int one[]   = { 0, 1, 2, 0, 0, 0, 0 };
int two[]   = { 1, 1, 0, 1, 1, 0, 1 };
int three[] = { 1, 1, 1, 1, 0, 0, 1 };
int four[]  = { 0, 1, 1, 0, 0, 1, 1 };
int five[]  = { 1, 0, 1, 1, 0, 1, 1 };
int six[]   = { 1, 0, 1, 1, 1, 1, 1 };
int seven[] = { 1, 1, 1, 0, 0, 0, 0 };
int eight[] = { 1, 1, 1, 1, 1, 1, 1 };
int nine[]  = { 1, 1, 1, 1, 0, 1, 1 };
int zero[]  = { 1, 1, 1, 1, 1, 1, 0 };
int blank[] = { 0, 0, 0, 0, 0, 0, 0 };
int* digits[] = { zero, one, two, three, four, five, six, seven, eight, nine, blank };

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

int* test_patterns_3[] = { d_seg, cde, cdeg, bcdefg, eight, off };

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
    pinMode(big_7segment_display[thisPin], OUTPUT);      
  }
  
  // initialize 3, switch input pins
  for (int thisPin = 0; thisPin < sizeof(switch_pins)/sizeof(int); thisPin++)  {
    pinMode(switch_pins[thisPin], INPUT_PULLUP);   //INPUT);      
  }

  // illuminate each segment separately for startup test
  for (int display = 0; display < 3; ++display) {
    select_display(display);    
    for (int thisPin = 0; thisPin < 7; thisPin++) { 
      digitalWrite(big_7segment_display[thisPin], HIGH);
      delay(short_wait);
    }
    delay(long_wait);
    blank_display(big_7segment_display);
  }
  delay(long_wait);

  // test patterns
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 5; ++j) {
      select_display(0);
      light_segments(big_7segment_display, test_patterns_1[i]);

      select_display(1);
      light_segments(big_7segment_display, test_patterns_1[i]);

      select_display(2);
      light_segments(big_7segment_display, test_patterns_1[i]);
    }
  }

  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 5; ++j) {
      select_display(0);
      light_segments(big_7segment_display, test_patterns_2[i]);

      select_display(1);
      light_segments(big_7segment_display, test_patterns_2[i]);

      select_display(2);
      light_segments(big_7segment_display, test_patterns_2[i]);
    }
  }
  
  for (int i = 0; i < 6; ++i) {
    // j controls how many times the display is lit (duration)
    for (int j = 0; j < 5; ++j) {
      select_display(0);
      light_segments(big_7segment_display, test_patterns_3[i]);

      select_display(1);
      light_segments(big_7segment_display, test_patterns_3[i]);

      select_display(2);
      light_segments(big_7segment_display, test_patterns_3[i]);
    }
  }

  delay(long_wait);
  select_display(0);
  blank_display(big_7segment_display);
  select_display(1);
  blank_display(big_7segment_display);
  select_display(2);
  blank_display(big_7segment_display);
  delay(short_wait);
}

void loop() {
  read_inputs();
  light_3_displays();
}

int blank_display(int* display) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(display[i], LOW);
  }
}

int light_segments(int* display, int* digit) {
  for (int i = 0; i <  7; i++) {
    int pin = display[i];
    int new_state = digit[i];
    int old_state = digitalRead(pin);
  
    if (old_state != new_state) {
      digitalWrite(pin, new_state);
    }
  }
  delay(pov_wait);  // stay lit  a short time to optimize PoV effect
}

int select_display(int n) {
  // short-cut, just turn-off all displays before powering display n
  for (int i = 0; i < sizeof(segment_control_pins)/sizeof(int); i++) {
    digitalWrite(segment_control_pins[i], LOW);
  }
/*
  if (n == 0) {
    digitalWrite(segment_control_pins[2], LOW);
  } else if (n == 1) {
    digitalWrite(segment_control_pins[0], LOW);
  } else if (n == 2) {
    digitalWrite(segment_control_pins[1], LOW);
  }
*/

  digitalWrite(segment_control_pins[n], HIGH);
}


int msg(char string[], unsigned int n) {
  Serial.print(string);
  Serial.print(": ");
  Serial.print(n);
  Serial.print("\n");
}

int read_inputs() {
  int display_sum = 0;
  int switch_value = 1;

  for (int i = 0; i < sizeof(switch_pins)/sizeof(int); ++i) {
//  for (int i = 0; i < 8; ++i) {

    if (digitalRead(switch_pins[i]) == LOW) {
      display_sum += switch_value;
    }

    switch_value = switch_value + switch_value;
  }

  display_digit = display_sum;
}

int light_3_displays() {

  int ones_digit   = (display_digit / 1) % 10;
  int tens_digit  = (display_digit / 10) % 10;
  int hundreds_digit = (display_digit / 100) % 10;

  // left-pad with blanks when value is 0
  if (hundreds_digit == 0) {
    hundreds_digit = BLANK_IDX;

    if (tens_digit == 0) {
      tens_digit = BLANK_IDX;

      if (ones_digit == 0) {
        ones_digit = BLANK_IDX;
      }
    }
  }
  
  select_display(0);
  light_segments(big_7segment_display, digits[hundreds_digit]);
  select_display(1);
  light_segments(big_7segment_display, digits[tens_digit]);
  select_display(2);
  light_segments(big_7segment_display, digits[ones_digit]);
}


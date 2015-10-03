#define OFF 0
#define ON 1
#define HUNDREDS_DISPLAY 0
#define TENS_DISPLAY 1
#define ONES_DISPLAY 2

int place_values[] = {0,0,0};

// Binary counter coding
//
// Team goals #1
//  Complete the number arrays below for digits 0 - 9
//  set the values to OFF or ON to light each segment
//
//   AAAA
//  F    B
//  F    B
//   GGGG
//  E    C
//  E    C
//   DDDD  Dot
//
// If you want a letter to light up, you need to change that "ON" in the definition.
// As you can see, there are 8 positions in the array. These positions correspond to: A, B, C, D, E, F, G, Dot
int zero[]  = { ON, ON, ON, ON, ON, ON, OFF, OFF };
int one[]   = { OFF, ON, ON, OFF, OFF, OFF, OFF, OFF };
int two[]   = { ON, ON, OFF, ON, ON, OFF, ON, OFF };
int three[] = { ON, ON, ON, ON, OFF, OFF, ON, OFF };
int four[]  = { OFF,ON, ON, OFF, OFF, ON, ON, OFF };
int five[]  = { ON, OFF, ON, ON, OFF, ON, ON, OFF };
int six[]   = { ON, OFF, ON, ON, ON, ON, ON, OFF};
int seven[] = { ON, ON, ON, OFF, OFF, OFF, OFF, OFF};
int eight[] = { ON, ON, ON, ON, ON, ON, ON, OFF};
int nine[]  = { ON, ON, ON, OFF, OFF, ON, ON, OFF};

// Oh, and don't forget that we need a blank element!
// The blank element is used to turn off displays.
// The effect of this is that the number 23 will appear as "23" and not "023"
int blank[] = { OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF };

// Did you know that you need to start at zero when you count positions in an array?
// For example, you would count "0, 1, 2, 3, ... and so forth"
// We are going to create an array called "digits".
// And when we create this array, we need to start with zero:
//   digits[0] = zero
int *digits[] = { zero, one, two, three, four, five, six, seven, eight, nine, blank};

// pin order for standard segments A,B,C,D,E,F,G
//int big_7segment_display[] = { 9, 10, 11, 12, 13, 14, 15, 16 };
//int big_7segment_display[] = {13,15,14,9,12,11,10,16};
int big_7segment_display[] = {12,15,14,13,9,11,10,16};

void loop() {
  set_place_values();
  light_3_displays(place_values[0], place_values[1], place_values[2]);
}

// Team goals #2 and #3
//   2. Light up all three display digits
//   3. Light them up so the numbers don't flash
int light_3_displays(int ones_digit, int tens_digit, int hundreds_digit) {
  int display_delay_time = 500;
  int segment_delay_time = 500;
  
  select_display(ONES_DISPLAY);
  light_segments(big_7segment_display, digits[ones_digit], segment_delay_time, display_delay_time);

  select_display(TENS_DISPLAY);
  light_segments(big_7segment_display,digits[tens_digit], segment_delay_time, display_delay_time);

  select_display(HUNDREDS_DISPLAY);
  light_segments(big_7segment_display,digits[hundreds_digit], segment_delay_time, display_delay_time);
}

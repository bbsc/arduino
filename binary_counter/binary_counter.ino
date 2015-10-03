#define OFF 0
#define ON 1
#define HUNDREDS_DISPLAY 0
#define TENS_DISPLAY 1
#define ONES_DISPLAY 2

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
//  Use a "1" to light a segment, and a "0" for off
//  pin order for standard segments A, B, C, D, E, F, G, Dot
int zero[]  = { OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF };
int one[]   = { OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF };
int two[]   = { OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF };








// blank is special in that it turns all the segments off
int blank[] = { OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF };

// Note that the position in the digits array needs to match
// the vlalue of the digit so, zero needs to be first because
// the code:
//   digits[0]
// is expected to return the pattern to light up a 0
//
// The blank element at the end is used to turn off displays
// when the most significant digit is 0. The effect of this
// is, for example, that the number 23 will appear as "23" and
// not "023"
int *digits[] = { zero, one, two, three, four, five, six, seven, eight, nine, blank};

// pin order for standard segments A,B,C,D,E,F,G
int big_7segment_display[] = { 9, 10, 11, 12, 13, 14, 15, 16 };

void loop() {
  int ones_place = 0;
  int tens_place = 0;
  int hundreds_place = 0;

  set_switch_values(&ones_place, &tens_place, &hundreds_place);
  light_3_displays(ones_place, tens_place, hundreds_place);
}

//
// Team goals #2 and #3
//   2. Light up all three display digits
//   3. Light them up so the numbers don't flash
int light_3_displays(int ones_digit, int tens_digit, int hundreds_digit) {
  int display_delay_time = 1000;
  int segment_delay_time = 1000;
  
  select_display(ONES_DISPLAY);
  light_segments(big_7segment_display, digits[ones_digit], segment_delay_time, display_delay_time);
  
}

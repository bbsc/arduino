//
//   AAAA
//  F    B
//  F    B
//   GGGG
//  E    C
//  E    C
//   DDDD  Dot
//
// use a "1" to light a segment, and a "0" for off
// pin order for standard segments A, B, C, D, E, F, G, Dot
int one[]   = { 0, 0, 0, 0, 0, 0, 0 };
int two[]   = { 0, 0, 0, 0, 0, 0, 0 };
int three[] = { 0, 0, 0, 0, 0, 0, 0 };
int four[]  = { 0, 0, 0, 0, 0, 0, 0 };
int five[]  = { 0, 0, 0, 0, 0, 0, 0 };
int six[]   = { 0, 0, 0, 0, 0, 0, 0 };
int seven[] = { 0, 0, 0, 0, 0, 0, 0 };
int eight[] = { 0, 0, 0, 0, 0, 0, 0 };
int nine[]  = { 0, 0, 0, 0, 0, 0, 0 };
int blank[] = { 0, 0, 0, 0, 0, 0, 0 };
int zero[]  = { 0, 0, 0, 0, 0, 0, 0 };

int *digits[] = { zero, one, two, three, four, five, six, seven, eight, nine, blank };

void loop() {
  read_inputs();
  light_3_displays();
}






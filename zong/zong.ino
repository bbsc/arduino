#define ENC_A 18
#define ENC_B 19
#define ENC_BUTTON 0
#define xUNKNOWN 0xFF
#define xOFF_OFF 0x00 // 00000000
#define xON_ON 0x03   // 00000011

#define turn_left 0
#define turn_none 1
#define turn_right 2
#define RESET 0

boolean display_ball = true;
int ball_update_cycles = 600;
int display_ball_cycles = 100;
int cycle_counter = 0;

int TURN_LEFT = turn_left;
int TURN_NONE = turn_none;
int TURN_RIGHT = turn_right;

boolean HALF_RIGHT = false;
boolean HALF_LEFT = false;
boolean reset_needed = false;

int A = 0;
int B = 0;
int lastA = 0;
int lastB = 0;
byte last_position = 0x00;

byte initial_position = xUNKNOWN;
byte next_position = xON_ON;


/*
 * NFM-12883Ax -- Positive goes into columns
 */
const int rows[8] = {
  2,3,4,5,6,7,8,9 };
const int cols[8] = {
  10,11,12,13,14,15,16,17 };


// 2-dimensional array of pixels:
int pixels[8][8];  

// paddle center pixel
int x = 0;
int y = 0;

// paddel right of center pixel
int rx = 1;
int ry = 0;

// paddel left of center pixel
int lx = 0;
int ly = 1;

int row = rows[x];
int col = cols[y];

int lrow = rows[x];
int lcol = cols[y];

int rrow = rows[x];
int rcol = cols[y];

int ballx = 2;
int bally = 1;
int ballx_vel = 1;
int bally_vel = 1;

int oldrow = row;
int oldcol = col;

int sleep_short_ms = 45;
int sleep_long_ms = 250;

int knob = 0;

const int left  = 104;
const int right = 108;

const int moves[2] = { left, right };

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  pinMode(ENC_A, INPUT);
  digitalWrite(ENC_A, HIGH);

  pinMode(ENC_B, INPUT);
  digitalWrite(ENC_B, HIGH);
  
  // initialize the digital pin as an output.
  for (int row = 0; row < 8; row++) {
    pinMode(rows[row], OUTPUT);
    digitalWrite(rows[row], HIGH);
    for (int col = 0; col < 8; col++) {
      pixels[row][col] = 0;
      pinMode(cols[col], OUTPUT);
      digitalWrite(cols[col], HIGH);
    }
  }
  pixels[x][y] = 1;
}

void blank_pixels() {
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      pixels[x][y] = 0;
    }
  }
}

void set_lr_dots() {
  switch (x) {
    case 0:
      switch (y) {
        case 0:
          rx = x + 1;
          ry = y;
          lx = x;
          ly = y + 1;
          break;
        case 7:
          rx = x;
          ry = y - 1;
          lx = x + 1;
          ly = y;
          break;
        default:
          rx = x;
          ry = y - 1;
          lx = x;
          ly = y + 1;
      }
      break;
    case 7:
      switch (y) {
        case 0:
          rx = x;
          ry = y + 1;
          lx = x - 1 ;
          ly = y;
          break;
        case 7:
          rx = x - 1;
          ry = y;
          lx = x;
          ly = y - 1;
          break;
        default:
          rx = x;
          ry = y + 1;
          lx = x;
          ly = y - 1;
      }
      break;
    default:
      switch (y) {
        case 0:
          rx = x + 1;
          ry = y;
          lx = x - 1 ;
          ly = y;
          break;
        case 7:
          rx = x - 1;
          ry = y;
          lx = x + 1 ;
          ly = y;
          break;
      break;
    }
  }
}

void set_ball_position() {
  boolean paddle_check = false;
  switch (ballx) {
    case 0:
    case 7:  //ball is against x barrier move in opposite direction for x, any y velocity
      ballx_vel = ballx_vel * -1;
      bally_vel = get_random_ball_velocity();
      paddle_check = true;
  }
  
  switch (bally) {
    case 0:
    case 7:  //ball is against y barrier move in opposite direction for x, any y velocity
      bally_vel = bally_vel * -1;
      ballx_vel = get_random_ball_velocity();
      paddle_check = true;
  }

  if (paddle_check) {
     if (! ((ballx == x && bally == y) || (ballx == rx && bally == ry) || (ballx == lx && bally == ly)) ) {
      Serial.println("RESET needed");
      Serial.print("ballx: ");
      Serial.print(ballx);
      Serial.print(" bally: ");
      Serial.println(bally);
      Serial.print("x: ");
      Serial.print(x);
      Serial.print(" y: ");
      Serial.println(y);
      Serial.print("lx: ");
      Serial.print(lx);
      Serial.print(" ly: ");
      Serial.println(ly);
      Serial.print("rx: ");
      Serial.print(rx);
      Serial.print(" ry: ");
      Serial.println(ry);
     
      reset_needed = true;
      return;
    }
  }

  ballx = ballx + ballx_vel;
  bally = bally + bally_vel;
}
  
void update_pixels() {
  blank_pixels();
  pixels[x][y] = 1;
  pixels[lx][ly] = 1;
  pixels[rx][ry] = 1;
  if (display_ball) {
    pixels[ballx][bally] = 1;
  }
}

void update_ball() {
  display_ball = !display_ball;
  if (display_ball) {
    pixels[ballx][bally] = 1;
  } else {
    pixels[ballx][bally] = 0;
  }
}

void go_right() {
  switch (x) {
    case 0:
      switch (y) {
        case 0:
          x = x + 1;
          break;
        default:
          y = y - 1;
          break;
      }
      break;
    case 7:
      switch (y) {
        case 7:
          x = x -1;
          break;
        default:
          y = y + 1;
          break;
      }
      break;
    default:
      // dot is in middle, y will be 0 or 7 so only x will change
      switch (y) {
        case 0:
          x=x+1;
          break;
        case 7:
          x=x-1;
          break;
      }
      break;
  }
}

void go_left() {
  switch (x) {
    case 0:
      switch(y) {
        case 7:
          x=x+1;
          break;
        default:
          y=y+1;
          break;
      }
      break;
    case 7:
      switch(y) {
        case 0:
          x=x-1;
          break;
        default:
          y=y-1;
          break;
      }
      break;
    default:
      switch (y) {
        case 0:
          x=x-1;
          break;
        case 7:
           x=x+1;
          break;
      }
      break;
  }
}

int get_random_paddle_move() {
  int movement = moves[random(2)];
  return movement;
}

int get_random_ball_velocity() {
  unsigned int movement = random(3) - 1;
  return movement;
}

void refresh_matrix() {      
  for (int col = 0; col < 8; col++) {
    digitalWrite(cols[col], HIGH);
    for (int row = 0; row < 8; row++) {
      int pixel = pixels[col][row];
      if (pixel) {
        digitalWrite(rows[row], LOW);
      }
    }

    for (int row = 0; row < 8; row++) {
      int pixel = pixels[col][row];
      digitalWrite(rows[row], HIGH);
    }
    digitalWrite(cols[col], LOW);
  }
}

byte invert_position(byte pos) {
  byte new_pos = ~pos & xON_ON;
  return new_pos;
}

// return true if knob turns right, false 
int eval_turn() {
  boolean A_CHANGED = false;
  boolean B_CHANGED = false;
  int turn = TURN_NONE;

  byte this_position = (A<<1 | B);

  // handle initial state, wait for "initial position" to be determined, return otherwise  
  if (initial_position == xUNKNOWN) {
    if (this_position == xOFF_OFF || this_position == xON_ON) {
      next_position = invert_position(this_position);
      last_position = this_position;
      initial_position = 0x00;
    }
    return TURN_NONE;
  }
  
  // do nothing if samples are the same
  if (last_position == this_position) {
    return TURN_NONE;
  }
  last_position = this_position;
  
  if (this_position == next_position) {

    if (HALF_RIGHT) {
      HALF_RIGHT = false;
      next_position = invert_position(next_position);
      return TURN_RIGHT;
    }
    
    if (HALF_LEFT) {      
      HALF_LEFT = false;
      next_position = invert_position(next_position);
      return TURN_LEFT;
    }

    Serial.println("ERROR: Next expected position reach, but no half turn recorded");
    next_position = invert_position(next_position);
  }

  // determine if only A changed (mid left turn)
  if (A != lastA) {
    if (B == lastB) {
       HALF_RIGHT = false;
       HALF_LEFT = true;
    }
  }

  // determine if only A changed (mid right turn)
  if (B != lastB) {
    if (A == lastA) {
      HALF_LEFT = false;
      HALF_RIGHT = true;
    }
  }

  return TURN_NONE;
}

void reset_board() {
  // paddle center pixel
  x = 0;
  y = 0;

  // paddel right of center pixel
  rx = 1;
  ry = 0;

  // paddel left of center pixel
  lx = 0;
  ly = 1;

  row = rows[x];
  col = cols[y];

  lrow = rows[x];
  lcol = cols[y];

  rrow = rows[x];
  rcol = cols[y];

  ballx = 2;
  bally = 1;
  ballx_vel = 1;
  bally_vel = 1;

  oldrow = row;
  oldcol = col;
}  
  
// the loop routine runs over and over again forever:
void loop() {
  ++cycle_counter;
  
  if (cycle_counter >= ball_update_cycles) {
    cycle_counter = 0;
    set_ball_position();
  }
  set_lr_dots();

  lastA = A;
  lastB = B;
  A = digitalRead(ENC_A);
  B = digitalRead(ENC_B);

  int turn = eval_turn();

  switch (turn) {

    case turn_right:
      go_right();
      break;

    case turn_left:
      go_left();
      break;  
  }
  int c = 0;
  while (reset_needed) {
    ++c;
    if (c >= display_ball_cycles) {
      c = 0;
      update_ball();
    }
    int reset = digitalRead(ENC_BUTTON);
    if (reset == RESET) {
      reset_needed = false;
      display_ball = true;
      reset_board();
    }
    refresh_matrix();
  }

  update_pixels();
  refresh_matrix();
}




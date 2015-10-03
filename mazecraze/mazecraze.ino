int left_button=10;
int right_button=0;

int ships_position_old=5;
int ships_possision=5;
int old_ships_possision=5;
int ships_row=7;
int walls_row=0;
int old_walls_row=0;
int the_wall[8] = {
  HIGH,HIGH,HIGH,HIGH,LOW,HIGH,HIGH,HIGH};
  
int columms[8] = {
  16,4,5,12,7,14,18,19};
int rows[8] = {
  13,17,9,15,2,8,3,6};

int last_right_button_state=1;
int last_left_button_state=1;
int loop_counting=0;
int loop_limit=800;

int pixels[8][8];
int speaker_pin = 11;
int frequency = 0;

void set_ships_possision(int calumm) {
  pixels[old_ships_possision][ships_row]=LOW;
  pixels[calumm][ships_row]=HIGH;
  old_ships_possision=calumm;
}

void random_wall(){
  for (int i = 0; i <8; i++) { 
    the_wall[i] = HIGH;
  }
  
  int random_number = random(8);
  the_wall[random_number] = LOW;
}

void play_tone() {
  int duration = 100;
  frequency += 10;
  if (frequency > 20000) {
    frequency = 0;
  }
  tone(speaker_pin, frequency, duration);  
}

void check_walls_hit() {
  if (ships_row == walls_row) {
    if (the_wall[ships_possision] == HIGH) {
      while (true) {
        // blink and draw ship here
        refresh_pixels();  
        play_tone();
      }
    }
  }
}

void move_wall() {
  walls_row = walls_row + 1;

  for (int col = 0; col < 8; col++) {
    pixels[col][old_walls_row] = LOW;
  }

  for (int col = 0; col < 8; col++) {
    pixels[col][walls_row] = the_wall[col];
  }

  old_walls_row = walls_row;


  if (walls_row > 7) {
    walls_row = 0;
    random_wall();
  }
}

void refresh_pixels() {
  for (int col = 0; col < 8; col++) {
    digitalWrite(columms[col], HIGH);

    for (int row = 0; row < 8; row++) {
      int pixel = pixels[col][row];
      if (pixel==HIGH) {
        digitalWrite(rows[row], LOW);
      }
    }

    for (int row = 0; row < 8; row++) {
      int pixel = pixels[col][row];
      digitalWrite(rows[row], HIGH);
    }

    digitalWrite(columms[col], LOW);
  }
}

void setup() {
  randomSeed(analogRead(0));
  // put your setup code here, to run once:
  for (int row = 0;row < 8;row=row+1) {
    pinMode(rows[row], OUTPUT);
    digitalWrite(rows[row],HIGH);

  }
  for (int columm = 0;columm <8;++columm) {
    pinMode(columms[columm], OUTPUT);
    digitalWrite(columms[columm],LOW);
  }

  Serial.begin(9800);
  pinMode(left_button,INPUT_PULLUP);
  pinMode(right_button,INPUT_PULLUP);
  digitalWrite(rows[ships_row],LOW); 

  pinMode(speaker_pin, OUTPUT);     
}

void loop() {
  loop_counting = loop_counting + 1;
  // put your main code here, to run repeatedly:
  int right_button_pressed= digitalRead(right_button);

  int left_button_pressed= digitalRead(left_button);

  if (right_button_pressed==0) {
    if(last_right_button_state==1 || loop_counting==loop_limit){
      //digitalWrite(columms[ships_possision],LOW);

      if(ships_possision==7){
        ships_possision=0;
      } 
      else {
        ships_possision = ships_possision + 1;
      }
    }
  }

  if (left_button_pressed==0) {
    if(last_left_button_state==1 || loop_counting==loop_limit){
      //digitalWrite(columms[ships_possision],LOW);

      if(ships_possision==0){
        ships_possision=7;
      }
      else {
        ships_possision = ships_possision - 1;
      }
    }
  }

  set_ships_possision(ships_possision);

  last_left_button_state = left_button_pressed;
  last_right_button_state = right_button_pressed;
  if (loop_counting==loop_limit) {
    loop_limit = loop_limit - 50;
    loop_counting=0;
    move_wall();
    check_walls_hit();
  }
  refresh_pixels();
}


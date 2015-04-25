int left_button=10;
int right_button=11;

int ships_position_old=5;
int ships_possision=5;
int old_ships_possision=5;
int ships_row=7;
int walls_row=0;
int old_walls_row=0;
int the_wall[8] = {
  HIGH,HIGH,LOW,HIGH,HIGH,HIGH,HIGH,HIGH};
int columms[8] = {
  16,4,5,12,7,14,18,19};
int rows[8] = {
  13,17,9,15,2,8,3,6};

int last_right_button_state=1;
int last_left_button_state=1;
int loop_counting=0;
int loop_limit=600;

int pixels[8][8];

<<<<<<< HEAD
void set_ships_possision(int calumm) {
  pixels[old_ships_possision][ships_row]=LOW;
  pixels[calumm][ships_row]=HIGH;
  old_ships_possision=calumm;
}

void check_walls_hit(){
  if (ships_row==walls_row) {
    if (pixels[walls_row][ships_possision] == HIGH) {
      while (true){


      }
    }
  }
}


void move_wall() {
  for (int col = 0; col < 8; col++) {
    pixels[col][old_walls_row] = LOW;
  }

  for (int col = 0; col < 8; col++) {
    pixels[col][walls_row] = the_wall[col];
  }

  old_walls_row = walls_row;

  walls_row = walls_row + 1;
  if (walls_row > 7) {
    walls_row = 0;

  }

=======
void set_ships_possision(int column) {
  pixels[ships_position_old][ships_row] = LOW;
  pixels[column][ships_row] = HIGH;
  ships_position_old = column;
>>>>>>> 1080d98e86e4cfe6d412429ab0f097f91486759e
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

}

void loop() {
  loop_counting = loop_counting + 1;
  check_walls_hit();
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

  // Serial.println(ships_possision);
  //delay(1000);
  //digitalWrite(columms[ships_possision],HIGH);
  last_left_button_state = left_button_pressed;
  last_right_button_state = right_button_pressed;
  if (loop_counting==loop_limit) {
    loop_counting=0;
    move_wall();
  }
  refresh_pixels();
}

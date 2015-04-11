int left_button=10;
int right_button=11;

int ships_possision=5;
int ships_row=7;
int columms[8] = {
  16,4,5,12,7,14,18,19};
int rows[8] = {
  13,17,9,15,2,8,3,6};

int last_right_button_state=1;
int last_left_button_state=1;
int loop_counting=0;
int loop_limit=1000 ;

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

  // put your main code here, to run repeatedly:
  int right_button_pressed= digitalRead(right_button);

  int left_button_pressed= digitalRead(left_button);

  if (right_button_pressed==0) {
    if(last_right_button_state==1 || loop_counting==loop_limit){
      digitalWrite(columms[ships_possision],LOW);

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
      digitalWrite(columms[ships_possision],LOW);

      if(ships_possision==0){
        ships_possision=7;
      }
      else {
        ships_possision = ships_possision - 1;
      }
    }
  }


  //Serial.print("ships_possision  ");  
 // Serial.println(ships_possision);
  //delay(1000);
  digitalWrite(columms[ships_possision],HIGH);
  last_left_button_state = left_button_pressed;
  last_right_button_state = right_button_pressed;
  if (loop_counting==loop_limit) {
    loop_counting=0;
  }
}

#include <PS2X_lib.h>
#include <Servo.h>     

#define PS2_DAT        13  //14    
#define PS2_CMD        11  //15
#define PS2_SEL        10  //16
#define PS2_CLK        12  //17


//int A0 bat tat dong co lay bong; A1 doi chieu dong co

int RL_EN_left = 5; // băm xung trái
int RL_EN_right = 9; // băm xung phải

int R_PWM_left = 2;// điều khiển chiều quay trái
int L_PWM_left = 3;// điều khiển chiều quay trái


int R_PWM_right = 7;// điều khiển chiều quay phải
int L_PWM_right = 6;// điều khiển chiều quay phải

//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false
PS2X ps2x;
Servo myservo , myservo2; 

int error = 0;
byte type = 0;
byte vibrate = 0;
int pos = 0;


bool bat = false;
bool daochieu=false;
void toggle_motor_bong()
{
  if(bat==true)
  {
    digitalWrite(A0,LOW);
  } else {
    digitalWrite(A0,HIGH);
  }
  bat=!bat;
}
void dao_chieu_motor_bong()
{
  if(daochieu==true)
  {
    digitalWrite(A1,LOW);
  } else {
    digitalWrite(A1,HIGH);
  }
  daochieu=!daochieu;
}

void set_up_bts(){
  
 pinMode(RL_EN_left, OUTPUT);
 pinMode(R_PWM_left, OUTPUT);
 pinMode(L_PWM_left, OUTPUT);

 digitalWrite(R_PWM_left, HIGH);
 digitalWrite(L_PWM_left, HIGH); //STOP


 
 pinMode(RL_EN_right, OUTPUT);
 pinMode(R_PWM_right, OUTPUT);
 pinMode(L_PWM_right, OUTPUT);

 digitalWrite(R_PWM_right, HIGH);
 digitalWrite(L_PWM_right, HIGH); //STOP
}

void move_dc_left(int x){
    
  digitalWrite(R_PWM_left, HIGH);
  digitalWrite(L_PWM_left, LOW);
  analogWrite(RL_EN_left, x); 

  
  
  digitalWrite(R_PWM_right, HIGH);
  digitalWrite(L_PWM_right, LOW);
  analogWrite(RL_EN_right, 0); 
}


void move_dc_right(int x){
    
  digitalWrite(R_PWM_right, HIGH);
  digitalWrite(L_PWM_right, LOW);
  analogWrite(RL_EN_right, x); 

  
  
  digitalWrite(R_PWM_left, HIGH);
  digitalWrite(L_PWM_left, LOW);
  analogWrite(RL_EN_left, 0); 
}

void move_straight(int x){
  digitalWrite(R_PWM_right, HIGH);
  digitalWrite(L_PWM_right, LOW);
  digitalWrite(R_PWM_left, HIGH);
  digitalWrite(L_PWM_left, LOW);
  
  analogWrite(RL_EN_right, x); 
  analogWrite(RL_EN_left, x); 
}

void move_back(int x){
  digitalWrite(R_PWM_right, LOW);
  digitalWrite(L_PWM_right, HIGH);
  digitalWrite(R_PWM_left, LOW);
  digitalWrite(L_PWM_left, HIGH);
  
  analogWrite(RL_EN_right, x); 
  analogWrite(RL_EN_left, x); 
}

void stop(){
  digitalWrite(R_PWM_right, LOW);
  digitalWrite(L_PWM_right, HIGH);
  digitalWrite(R_PWM_left, LOW);
  digitalWrite(L_PWM_left, HIGH);
  
  analogWrite(RL_EN_right, 0); 
  analogWrite(RL_EN_left, 0); 
}

void turn_on_servo(){
  myservo.write(160);
  myservo2.write(160);
  delay(50);
}

void turn_off_servo(){
  myservo.write(0);
  myservo2.write(0);
  delay(50);
}

void setup() {

  Serial.begin(57600);
  
  delay(300);  
   
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0){
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
  if (pressures)
    Serial.println("true ");
  else
    Serial.println("false");
  Serial.print("rumble = ");
  if (rumble)
    Serial.println("true)");
  else
    Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }  
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
  
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      Serial.print("GuitarHero Controller found ");
      break;
  case 3:
      Serial.print("Wireless Sony DualShock Controller found ");
      break;
   }
  set_up_bts();
  pinMode(A0,OUTPUT);
  pinMode(A1,OUTPUT);
  myservo.attach(13);
  myservo2.attach(9);
}

void loop() {

  
  if(error == 1) //skip loop if no controller found
    return; 
  if(type!=2){
    
      ps2x.read_gamepad(false, vibrate);
      
      if(ps2x.Button(PSB_PAD_UP)){
          Serial.println("di thang"); 
          move_straight(255);
      }

    if(ps2x.Button(PSB_PAD_DOWN)) {
            Serial.println("di lui");   
            move_back(255); 
    }
      
    if(ps2x.Button(PSB_PAD_LEFT)) {
            Serial.println("qua trai");
            move_dc_right(255);
    }

    
    if(ps2x.Button(PSB_PAD_RIGHT)) {
            Serial.println("qua phai");
            move_dc_left(255);
    }

    
    if(ps2x.NewButtonState(PSB_CROSS)) {
        Serial.println("dung lai");
        stop();
    } 
      
  } 
  if(ps2x.ButtonPressed(PSB_L1))
  {
    toggle_motor_bong();
  }
  if(ps2x.ButtonPressed(PSB_L2))
  {
    dao_chieu_motor_bong();
  }
  if(ps2x.ButtonPressed(PSB_CROSS)){
       Serial.println("X just changed");  
       turn_off_servo();
  }
  
  if(ps2x.ButtonPressed(PSB_TRIANGLE)){
       Serial.println("Triangle just changed");  
       turn_on_servo();
  }  
  delay(50);
}

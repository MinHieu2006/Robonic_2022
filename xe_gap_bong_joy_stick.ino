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
int motor_right_speed = 0;
int motor_left_speed = 0;


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

void move_straight_left(int x){ // dong co trai di thang
  digitalWrite(R_PWM_left, HIGH);
  digitalWrite(L_PWM_left, LOW);
  analogWrite(RL_EN_left, x); 
}


void move_straight_right(int x){  // dong co phai di thang
  digitalWrite(R_PWM_right, HIGH);
  digitalWrite(L_PWM_right, LOW);
  analogWrite(RL_EN_right, x); 
}

void move_back_left(int x){ // dong co trai di lui
  digitalWrite(R_PWM_left, LOW);
  digitalWrite(L_PWM_left, HIGH);
  analogWrite(RL_EN_left, x); 
}


void move_back_right(int x){  // dong co phai di lui
  digitalWrite(R_PWM_right, LOW);
  digitalWrite(L_PWM_right, HIGH);
  analogWrite(RL_EN_right, x); 
}void loop(){
   
 if(error == 1) //skip loop if no controller found
  return; 
  
 else { //DualShock Controller
  
    ps2x.read_gamepad(false, vibrate); // disable vibration of the controller   
    int nJoy_left_Y = ps2x.Analog(PSS_LY); // read y-postion left stick
    int nJoy_right_X = ps2x.Analog(PSS_RX); // read x-postion right stick
   
    nJoy_left_Y = map(nJoy_left_Y, 0, 255, 1023, -1023);
    nJoy_right_X = map(nJoy_right_X, 0, 255, -1023, 1023); 

    if(nJoy_left_Y>50) {
        move_back(255);
     } else if(nJoy_left_Y<-50) {
        move_back(255);
     }  
     
     if(nJoy_right_X>50) {
        move_dc_left(255);
     } else if(nJoy_right_X<-50) {
        move_dc_right(255);
     }  
     
   delay(50);
 }    
}

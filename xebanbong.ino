#include <Servo.h>     

//int A0 bat tat dong co lay bong; A1 doi chieu dong co


int RL_EN_right = 6; // băm xung phải
int R_PWM_right = 7;// điều khiển chiều quay phải
int L_PWM_right = 8;// điều khiển chiều quay phải

int RL_EN_left = 3; // băm xung trái
int R_PWM_left = 11;// điều khiển chiều quay trái
int L_PWM_left = 10;// điều khiển chiều quay trái



Servo myservo, myservo2; 

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

void set_up_bts() {
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

int Left=0, Right=0;
bool speedmode;
void updatemove() {
    if (Left == 0) {
        digitalWrite(R_PWM_left, LOW);
        digitalWrite(L_PWM_left, LOW);
        analogWrite(RL_EN_left, 0);
    } else if (Left < 0) {
        digitalWrite(R_PWM_left, LOW);
        digitalWrite(L_PWM_left, HIGH);
        analogWrite(RL_EN_left, -Left);
    } else if(Left > 0) {
        digitalWrite(R_PWM_left, HIGH);
        digitalWrite(L_PWM_left, LOW);
        analogWrite(RL_EN_left, Left);
    }
    if (Right == 0) {
        digitalWrite(R_PWM_right, LOW);
        digitalWrite(L_PWM_right, LOW);
        analogWrite(RL_EN_right, 0);
    } else if (Right < 0) {
        digitalWrite(R_PWM_right, LOW);
        digitalWrite(L_PWM_right, HIGH);
        analogWrite(RL_EN_right, -Right);
    } else if(Right > 0 ){
        digitalWrite(R_PWM_right, HIGH);
        digitalWrite(L_PWM_right, LOW);
        analogWrite(RL_EN_right, Right);
    }
}
void updatemove(int l, int r) { // thay đổi dần để Left, Right đặt tốc độ l, r
//    while (Left < l) {
//        ++Left;
//        updatemove();
//        delay(5);
//    }
//    while (Left > l) {
//        --Left;
//        updatemove();
//        delay(5);
//    }
//    while (Right < r) {
//        ++Right;
//        updatemove();
//        delay(5);
//    }
//    while (Right > r) {
//        --Right;+
//        updatemove();
//        delay(5);
//    }
    Left = l ; 
    Right = r;
    updatemove();
}
int serv = 0;
void updateservo() {
    if (serv == 0) {
        myservo.write(0);
        myservo2.write(0);
    }
    else if (serv == 1) myservo.write(100);
    else if (serv == 2) myservo2.write(100);
    serv = (serv + 1) % 3;
}

void setup() {
    Serial.begin(9600);
    delay(300);  
    
    set_up_bts();
    pinMode(A0,OUTPUT);
    pinMode(A1,OUTPUT);
    myservo.attach(13);
    myservo2.attach(9);

    Left = Right = 0;
    speedmode = 1;
    updatemove();
    serv = 0;
    updateservo();

//    
//          digitalWrite(R_PWM_left, HIGH);
//          digitalWrite(L_PWM_left, LOW);
//          analogWrite(RL_EN_left, 255);
//          
//        digitalWrite(R_PWM_right, HIGH);
//        digitalWrite(L_PWM_right, LOW);
//        analogWrite(RL_EN_right, 255);
        
    Left = Right = 0;
    speedmode = 1;
    updatemove();
}

void update(int &x, int y) {
    if (0 <= x+y && x+y <= 255)
        x += y;
}
void loop() {
    char bluetooth = Serial.read();
    Serial.print(Left);
    Serial.print(' ');
    Serial.println(Right);

    if (speedmode) { // nhanh, thay đổi dần tốc độc
        if (bluetooth == 'W') { // tiến
            Serial.println(bluetooth);
            
          digitalWrite(R_PWM_left, HIGH);
          digitalWrite(L_PWM_left, LOW);
          analogWrite(RL_EN_left, 255);
          
        digitalWrite(R_PWM_right, HIGH);
        digitalWrite(L_PWM_right, LOW);
        analogWrite(RL_EN_right, 255);
        }
        if (bluetooth == 'S') { // lùi
            Serial.println(bluetooth);
            updatemove(-255, -255);
        }
        if (bluetooth == 'A') { // quay trái
            Serial.println(bluetooth);
            updatemove(0, 255);
        }
        if (bluetooth == 'D') { // quay phải
            Serial.println(bluetooth); 
            updatemove(255, 0);
        }
    } else { // chậm
        if (bluetooth == 'W') { // tiến
            Serial.println(bluetooth);
            Left = 150;
            Right = 150;
            updatemove();
        }
        if (bluetooth == 'S') { // lùi
            Serial.println(bluetooth);
            Left = -150;
            Right = -150;
            updatemove();
        }
        if (bluetooth == 'A') { // quay trái
            Serial.println(bluetooth);
            Left = 0;
            Right = 150;
            updatemove();
        }
        if (bluetooth == 'D') { // quay phải
            Serial.println(bluetooth); 
            Left = 150;
            Right = 0;
            updatemove();
        }
        delay(100);
        Left = Right = 0;
        updatemove();
    }
    if (bluetooth == 'X') { // nút dấu X: dừng lại
        Serial.println(bluetooth);
        Left = Right = 0;
        updatemove();
    }
    if (bluetooth == 'H') { // nút hình vuông: đổi tốc độ di chuyển (nhanh/chậm)
        speedmode ^= 1;
    }
    if (bluetooth == 'A') { // nút tam giác: nâng/hạ động cơ gắp bóng
        
    }
    if (bluetooth == 'O') { // nút hình tròn: đóng mở servo
        updateservo();
    }
}

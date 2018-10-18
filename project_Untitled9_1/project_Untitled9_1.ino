#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include <MeMegaPi.h>


//Encoder Motor
MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);
MeEncoderOnBoard Encoder_3(SLOT3);
MeEncoderOnBoard Encoder_4(SLOT4);

void isr_process_encoder1(void)
{
      if(digitalRead(Encoder_1.getPortB()) == 0){
            Encoder_1.pulsePosMinus();
      }else{
            Encoder_1.pulsePosPlus();
      }
}

void isr_process_encoder2(void)
{
      if(digitalRead(Encoder_2.getPortB()) == 0){
            Encoder_2.pulsePosMinus();
      }else{
            Encoder_2.pulsePosPlus();
      }
}

void isr_process_encoder3(void)
{
      if(digitalRead(Encoder_3.getPortB()) == 0){
            Encoder_3.pulsePosMinus();
      }else{
            Encoder_3.pulsePosPlus();
      }
}

void isr_process_encoder4(void)
{
      if(digitalRead(Encoder_4.getPortB()) == 0){
            Encoder_4.pulsePosMinus();
      }else{
            Encoder_4.pulsePosPlus();
      }
}

void move(int direction, int speed)
{
      int leftSpeed = 0;
      int rightSpeed = 0;
      if(direction == 1){
            leftSpeed = -speed;
            rightSpeed = speed;
      }else if(direction == 2){
            leftSpeed = speed;
            rightSpeed = -speed;
      }else if(direction == 3){
            leftSpeed = speed;
            rightSpeed = speed;
      }else if(direction == 4){
            leftSpeed = -speed;
            rightSpeed = -speed;
      }
      Encoder_1.setTarPWM(rightSpeed);
      Encoder_2.setTarPWM(leftSpeed);
}
void moveDegrees(int direction,long degrees, int speed_temp)
{
      speed_temp = abs(speed_temp);
      if(direction == 1)
      {
            Encoder_1.move(degrees,(float)speed_temp);
            Encoder_2.move(-degrees,(float)speed_temp);
      }
      else if(direction == 2)
      {
            Encoder_1.move(-degrees,(float)speed_temp);
            Encoder_2.move(degrees,(float)speed_temp);
      }
      else if(direction == 3)
      {
            Encoder_1.move(degrees,(float)speed_temp);
            Encoder_2.move(degrees,(float)speed_temp);
      }
      else if(direction == 4)
      {
            Encoder_1.move(-degrees,(float)speed_temp);
            Encoder_2.move(-degrees,(float)speed_temp);
      }
    
}

double angle_rad = PI/180.0;
double angle_deg = 180.0/PI;
MeStepperOnBoard stepper_1(1);
MeStepperOnBoard stepper_3(3);



void setup(){
    //Set Pwm 8KHz
    TCCR1A = _BV(WGM10);
    TCCR1B = _BV(CS11) | _BV(WGM12);
    TCCR2A = _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(CS21);
    
    stepper_1.setMicroStep(16);
    stepper_1.enableOutputs();
    stepper_3.setMicroStep(16);
    stepper_3.enableOutputs();
    attachInterrupt(Encoder_4.getIntNum(), isr_process_encoder4, RISING);
    Encoder_4.setPulse(8);
    Encoder_4.setRatio(46.67);
    Encoder_4.setPosPid(1.8,0,1.2);
    Encoder_4.setSpeedPid(0.18,0,0);
    Serial.begin(9600);
//    for(int __i__=0;__i__<10;++__i__)
//    {
//        stepper_1.move(1000);
//        stepper_1.setMaxSpeed(3000);
//        stepper_1.setSpeed(3000);
//        stepper_3.move(1000);
//        stepper_3.setMaxSpeed(3000);
//        stepper_3.setSpeed(3000);
//        Encoder_4.move(1000,abs(60));
//        _delay(15);
//    }

}

void liftItem() {
    Serial.println("liftItem");
    Encoder_4.move(650,abs(60));
    _delay(4);
    Encoder_4.move(-650,abs(60));
    _delay(4);
    Serial.println("liftItem ends");
}

void moveLift() {
    Encoder_4.move(50,abs(60));
    _delay(1);
  }

void moveLiftBack() {
    Encoder_4.move(-50,abs(60));
    _delay(1);
  }

void rotateBy(int num) {
    Serial.println("rotate By");
    Serial.println(num);
    if(num > 0) {
        int steps = num * (1700/180);
        stepper_3.move(steps);
        stepper_3.setMaxSpeed(1500);
        stepper_3.setSpeed(1500);
        _delay(4);
      }
    Serial.println("rotate By ends");
  }

void rotateHalf() {
  Serial.println("rotateHalf");
  stepper_1.move(1600);
  stepper_1.setMaxSpeed(1500);
  stepper_1.setSpeed(1500);
  _delay(3);
}

void openTrap() {
  Serial.println("openTrap");
  stepper_1.move(1600);
  stepper_1.setMaxSpeed(1500);
  stepper_1.setSpeed(1500);
  _delay(4);
  Serial.println("openTrap ends");
}

void closeTrap() {
  Serial.println("closeTrap");
  stepper_1.move(-1600);
  stepper_1.setMaxSpeed(1500);
  stepper_1.setSpeed(1500);
  _delay(4);
  Serial.println("closeTrap ends");
}

void moveTrapdoor() {
  stepper_1.move(160);
  stepper_1.setMaxSpeed(1500);
  stepper_1.setSpeed(1500);
  _delay(1);
}

void moveSorter() {
  stepper_3.move(160);
  stepper_3.setMaxSpeed(500);
  stepper_3.setSpeed(500);
  _delay(1);
}

String msg = "";
int deg = 0;

void loop(){

      if(Serial.available() > 0) {
        msg = Serial.readStringUntil('$');
        Serial.println(msg);
        if(msg.startsWith("load_")) {
          Serial.println("start_load");
            int deg = msg.substring(5).toInt();
            rotateBy(deg);
            liftItem();
        }
        if(msg.startsWith("ejct_")) {
          Serial.println("start_eject");
          int deg = msg.substring(5).toInt();
          rotateBy(deg);
          openTrap();
          closeTrap();
        }
        if(msg.startsWith("test_sort")) {
          Serial.println("test_sort");
          rotateBy(30);
        }
        if(msg.startsWith("test_lift")) {
          Serial.println("test_lift");
          liftItem();
        }
        if(msg.startsWith("test_drop")) {
          Serial.println("test_drop");
          openTrap();
          closeTrap();
        }
        if(msg.startsWith("move_lift")) {
          Serial.println("move_lift");
          moveLift();
        }
        if(msg.startsWith("move_trapdoor")) {
          Serial.println("move_trapdoor");
          moveTrapdoor();
        }
        if(msg.startsWith("move_sorter")) {
          Serial.println("move_sorter");
          moveSorter();
        }
        if(msg.startsWith("move_rev_lift")) {
          Serial.println("move_rev_lift");
          moveLiftBack();
        }
        if(msg=="test_sequence") {
            liftItem();
            rotateHalf();
            openTrap();
            closeTrap();
          }
      }
    
    //_loop();
}

void _delay(float seconds){
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime)_loop();
}

void _loop(){
    stepper_1.runSpeedToPosition();
    
    stepper_3.runSpeedToPosition();
    
    Encoder_4.loop();
    
    
}

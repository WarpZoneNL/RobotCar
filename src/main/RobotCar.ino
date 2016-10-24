/*
  Patrick's Robot Car v0.04
  
  4 Wheels (L293 Quadruple Half-H Drivers Data Sheet)
  Front and Back Ultrasonic Eyes
    > Front
      > 1x pin VCC 5v
      > 1x pin ground
      > 1x Trig | DIGITAL 1
      > 1x Echo | DIGITAL 2
    > Rear
      > 1x pin VCC 5v | 
      > 1x pin ground
      > 1x Trig | DIGITAL 3
      > 1x Echo | DIGITAL 4
  Follow Line
  3 Axis Acceleration
    > 1x pin X | ANALOG 1
    > 1x pin Y | ANALOG 2
    > 1x pin Z | ANALOG 3
    > 2x pin VCC 3.3V
    > 3x pin Ground
  Bluetooth Controller
  Sound Effects
*/

#include <SoftwareSerial.h> //Bluetooth
SoftwareSerial myBluetooth(0,1); //RX, TX

int bluetooth_rx                = 0;
int bluetooth_tx                = 1;
int ultra_front_trig            = 2;
int shift_register_latch_led    = 3; //3 PMS //74HC595N
int ultra_front_echo            = 4;
int shift_register_clock        = 5; //5 PMS //Multi Use by L7805CP && 74HC595N PIN 3 & 12
int shift_register_data         = 6; //6 PMS //Multi Use by L7805CP && 74HC595N PIN 3 & 12
int ultra_rear_trig             = 7;
int ultra_rear_echo             = 8;
//9 PMS
//10 PMS
//11 PMS
int shift_register_latch_motor  = 12; //74HC595N
//13
//A0
const int three_axis_x                = A1;
const int three_axis_y                = A2;
const int three_axis_z                = A3;
//A3
//A4
//A5
int shift_register_array_led[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  /*
  0 = front left
  1 = front right
  2 = rear left
  3 = rear right
  4 = lights midd left
  5 = lights midd right
  6 = 
  7 = 
  */

int shift_register_array_motor[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  /*
  0 = front left forward
  1 = front left backward
  2 = front right forward
  3 = front right backward
  4 = rear left forward
  5 = rear left backward
  6 = rear right forward
  7 = rear right backward
  */

boolean shift_register_array_motor2[8];

int ultra_front;
int ultra_rear;
int three_axis_output_x;
int three_axis_output_y;
int three_axis_output_z;
int three_axis_level_x = 0;
int three_axis_level_y = 0;
int three_axis_level_z = 0;

long time = millis();
long ultrasonic_front_lasttime = 0;
long ultrasonic_rear_lasttime  = 0;

int switch_var = 1;
  /*
  1 = Automatic
  2 = Manual (Bluetooth)
  3 = Testing Ultrasonic
  4 = Testing Three Axis
  5 = Testing Shiftregister Led's
  6 = Testing Shiftregister Motor
  7 = Testing Bluetooth
  */

char bluetooth_command = 'S';

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin (9600);
  Serial.println("RobotCar V0.03 Setup;");  
  pinMode(ultra_front_trig, OUTPUT);
  pinMode(ultra_front_echo, INPUT);
  pinMode(ultra_rear_trig, OUTPUT);
  pinMode(ultra_rear_echo, INPUT);
  pinMode(shift_register_latch_led, OUTPUT);
  pinMode(shift_register_clock, OUTPUT);
  pinMode(shift_register_data, OUTPUT);
  
  funct_three_axis_level(three_axis_x, three_axis_y, three_axis_z);

  //Bluetooth
    //myBluetooth.begin(115200); //115200 //9600 //57600
    //myBluetooth.println("RoboCar v0.03 Bluetooth Setup");
}

// the loop routine runs over and over again forever:
void loop() {
  switch_var = 4; //Testing :)
  switch(switch_var){
    case 1: //1 = Automatic

    break;
    
    case 2: //2 = Manual (Bluetooth)
      if(myBluetooth.available()){
        byte myBluetoothRead = myBluetooth.read();
        Serial.print("Received: ");Serial.println(myBluetoothRead);
        myBluetooth.write(myBluetoothRead);
        Serial.print("Send back:");Serial.println(myBluetoothRead);
      }
    break;
        
    case 3: //3 = Testing Ultrasonic
      ultra_front = funct_ultrasonic_distance(ultra_front_trig, ultra_front_echo);
      ultra_rear  = funct_ultrasonic_distance(ultra_rear_trig, ultra_rear_echo);
    break;
    
    case 4: //4 = Testing Three Axis
      three_axis_output_x = funct_three_axis(three_axis_x, three_axis_level_x);
        Serial.print("Output Three Axis X: "); Serial.println(three_axis_output_x, DEC);
      three_axis_output_y = funct_three_axis(three_axis_y, three_axis_level_y);
        Serial.print("Output Three Axis Y: "); Serial.println(three_axis_output_y, DEC);
      three_axis_output_z = funct_three_axis(three_axis_z, three_axis_level_z);
        Serial.print("Output Three Axis Z: "); Serial.println(three_axis_output_z, DEC);        
      Serial.println("----------");
    break;
    
    case 5: //5 = Testing Shiftregister Led's
      funct_shiftregister_write(shift_register_latch_led, shift_register_clock, shift_register_data, shift_register_array_led);
        Serial.println("Shift Register LED is written!");
    break;
    
    case 6: //6 = Testing Shiftregister Motor
      funct_motor_drive(1);
      /*
      0 = Stop
      1 = Forward
      2 = Backward
      3 = Left_Wide
      4 = Left_Narrow
      5 = Right_Wide
      6 = Right_Narrow
      */  
    break;
    
    case 7: //7 = Testing Bluetooth
      bluetooth_command = myBluetooth.read();
      switch(bluetooth_command){
        case 'F':
                digitalWrite(13, HIGH);

          //Serial.print("Bluetooth: ");Serial.println(bluetooth_command);  
        break;
        case 'B':  
          Serial.print("Bluetooth: ");Serial.println(bluetooth_command);  
        break;
        case 'L':  
          Serial.print("Bluetooth: ");Serial.println(bluetooth_command);  
        break;
        case 'R':
          Serial.print("Bluetooth: ");Serial.println(bluetooth_command);  
        break;
        case 'S':  
          Serial.print("Bluetooth: ");Serial.println(bluetooth_command);  
        break;
        case 'I':  //FR  
          Serial.print("Bluetooth: ");Serial.println(bluetooth_command);  
        break;
        case 'J':  //BR  
          Serial.print("Bluetooth: ");Serial.println(bluetooth_command);  
        break;
        case 'G':  //FL  
          Serial.print("Bluetooth: ");Serial.println(bluetooth_command);  
        break;
        case 'H':  //BL
          Serial.print("Bluetooth: ");Serial.println(bluetooth_command);  
        break;
        case 'W':  //Font ON 
          Serial.print("Bluetooth: ");Serial.println(bluetooth_command);  
        break;
        case 'w':  //Font OFF
          Serial.print("Bluetooth: ");Serial.println(bluetooth_command);  
        break;
        case 'U':  //Back ON 
          Serial.print("Bluetooth: ");Serial.println(bluetooth_command);  
        break;
        case 'u':  //Back OFF 
          Serial.print("Bluetooth: ");Serial.println(bluetooth_command);  
        break;
        case 'D':  //Everything OFF 
          Serial.print("Bluetooth: ");Serial.println(bluetooth_command);  
        break;         
      }
    break;
    
    default:
      Serial.println("Do Nothing!");
    break;
  }  
}

int funct_ultrasonic_distance(int u_d_trigger, int u_d_echo){
  int duration;
  int distance;
  int maximumRange = 100; 
  int minimumRange = 0;
  long ultrasonic_lasttime = 0;
  char* side;
   
  if(u_d_trigger == 2){ ultrasonic_lasttime = ultrasonic_front_lasttime; }
  if(u_d_trigger == 7){ ultrasonic_lasttime = ultrasonic_rear_lasttime; }
  
  if( ( millis() - ultrasonic_lasttime ) > 250 ){
    Serial.print("RUN");Serial.println(millis());Serial.println(ultrasonic_lasttime);
    
    digitalWrite(u_d_trigger, LOW); 
    delayMicroseconds(2); 
  
    digitalWrite(u_d_trigger, HIGH);
    delayMicroseconds(10); 
    
    digitalWrite(u_d_trigger, LOW);
    duration = pulseIn(u_d_echo, HIGH);
    
    distance = duration/58.2; //Calculate the distance (in cm) based on the speed of sound.

    if(u_d_trigger == 2){ ultrasonic_front_lasttime = millis(); side = "Front"; }
    if(u_d_trigger == 7){ ultrasonic_rear_lasttime = millis();  side = " Rear"; }
 
    if (distance >= maximumRange || distance <= minimumRange){
      int outofrange = -1;
      return outofrange;
    } else {
      Serial.println("     ------------------------"); Serial.print("     | Distance "); Serial.print(side); Serial.print(": "); Serial.print(distance, DEC); Serial.println("cm |"); Serial.println("     ------------------------");
      return distance;
    }
  }
}

void funct_shiftregister_write(int latch, int clock, int data, int shift_array[8]){
    digitalWrite(latch, LOW); Serial.println("Shift Register Start!");

      for (int i = 0; i < 8; i++){
        digitalWrite(clock, LOW);
        digitalWrite(data, shift_array[i]); Serial.print("Value ");Serial.print(i);Serial.print(": ");Serial.print(shift_array[i]);Serial.println("written!");
        digitalWrite(clock, HIGH);
      }   
    digitalWrite(latch, HIGH); Serial.println("END");
}

int funct_three_axis(int three_axis_pin, int three_axis_level){
  int three_axis_output = analogRead(three_axis_pin);
  int level_value = three_axis_output - three_axis_level;

  if( level_value < 2 && level_value > -2 ){
   return 0; 
  } else {
   return level_value; 
  } 
}

int funct_three_axis_level(int three_axis_x, int three_axis_y, int three_axis_z){
  three_axis_level_x = (analogRead(three_axis_x) -6);
  three_axis_level_y = (analogRead(three_axis_y) -2);
  three_axis_level_z = (analogRead(three_axis_z) -5);
  // -6/-2/-5 = correction
}

void funct_motor_drive(int drive_direction){
  String drive_direction_print;
  
  switch(drive_direction){
    case 0: //Stop
      shift_register_array_motor[0] = 0; //front left forward
      shift_register_array_motor[1] = 0; //front left backward
      shift_register_array_motor[2] = 0; //front right forward
      shift_register_array_motor[3] = 0; //front right backward
      shift_register_array_motor[4] = 0; //rear left forward
      shift_register_array_motor[5] = 0; //rear left backward
      shift_register_array_motor[6] = 0; //rear right forward
      shift_register_array_motor[7] = 0; //rear right backward  
      drive_direction_print = "Stop";
    break;

    case 1: //Forward
      shift_register_array_motor[0] = 1; //front left forward
      shift_register_array_motor[1] = 0; //front left backward
      shift_register_array_motor[2] = 1; //front right forward
      shift_register_array_motor[3] = 0; //front right backward
      shift_register_array_motor[4] = 1; //rear left forward
      shift_register_array_motor[5] = 0; //rear left backward
      shift_register_array_motor[6] = 1; //rear right forward
      shift_register_array_motor[7] = 0; //rear right backward
      drive_direction_print = "Forward";
    break;
   
    case 2: //Backward
      shift_register_array_motor[0] = 0; //front left forward
      shift_register_array_motor[1] = 1; //front left backward
      shift_register_array_motor[2] = 1; //front right forward
      shift_register_array_motor[3] = 0; //front right backward
      shift_register_array_motor[4] = 0; //rear left forward
      shift_register_array_motor[5] = 1; //rear left backward
      shift_register_array_motor[6] = 0; //rear right forward
      shift_register_array_motor[7] = 1; //rear right backward
      drive_direction_print = "Backward";
    break; 
       
    case 3: //Left_Wide
      shift_register_array_motor[0] = 0; //front left forward
      shift_register_array_motor[1] = 1; //front left backward
      shift_register_array_motor[2] = 1; //front right forward
      shift_register_array_motor[3] = 0; //front right backward
      shift_register_array_motor[4] = 1; //rear left forward
      shift_register_array_motor[5] = 0; //rear left backward
      shift_register_array_motor[6] = 1; //rear right forward
      shift_register_array_motor[7] = 0; //rear right backward
      drive_direction_print = "Left_Wide";
    break;
    
    case 4: //Left_Narrow
      shift_register_array_motor[0] = 0; //front left forward
      shift_register_array_motor[1] = 1; //front left backward
      shift_register_array_motor[2] = 1; //front right forward
      shift_register_array_motor[3] = 0; //front right backward
      shift_register_array_motor[4] = 0; //rear left forward
      shift_register_array_motor[5] = 1; //rear left backward
      shift_register_array_motor[6] = 1; //rear right forward
      shift_register_array_motor[7] = 0; //rear right backward
      drive_direction_print = "Left_Narrow";
    break;
    
    case 5: //Right_Wide
      shift_register_array_motor[0] = 1; //front left forward
      shift_register_array_motor[1] = 0; //front left backward
      shift_register_array_motor[2] = 0; //front right forward
      shift_register_array_motor[3] = 1; //front right backward
      shift_register_array_motor[4] = 1; //rear left forward
      shift_register_array_motor[5] = 0; //rear left backward
      shift_register_array_motor[6] = 1; //rear right forward
      shift_register_array_motor[7] = 0; //rear right backward
      drive_direction_print = "Right_Wide";
    break;
    
    case 6: //Right_Narrow
      shift_register_array_motor[0] = 1; //front left forward
      shift_register_array_motor[1] = 0; //front left backward
      shift_register_array_motor[2] = 0; //front right forward
      shift_register_array_motor[3] = 1; //front right backward
      shift_register_array_motor[4] = 1; //rear left forward
      shift_register_array_motor[5] = 0; //rear left backward
      shift_register_array_motor[6] = 0; //rear right forward
      shift_register_array_motor[7] = 1; //rear right backward
      drive_direction_print = "Right_Narrow";
    break;
  }
  funct_shiftregister_write(shift_register_latch_motor, shift_register_clock, shift_register_data, shift_register_array_motor);
  Serial.print("Shift Register MOTOR is written to: "); Serial.println(drive_direction_print);
}

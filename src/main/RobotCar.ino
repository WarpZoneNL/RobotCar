/*
  Patrick's Robot Car
  
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
int three_axis_x                = A0;
int three_axis_y                = A1;
int three_axis_z                = A2;
//A3
//A4
//A5

int shift_register_array_led[8];
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

int shift_register_array_motor[8];
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

int ultra_front;
int ultra_rear;
int three_axis_output_x;
int three_axis_output_y;
int three_axis_output_z;

long time = millis();
long ultrasonic_front_lasttime = 0;
long ultrasonic_rear_lasttime  = 0;
int switch_var = 1;
  /*
  1 = Automatic
  2 = Manual (Bluetooth)
  3 = Driving Rounds and Rounds xD
  4 = Testing Ultrasonic
  5 = Testing Three Axis
  6 = Testing Shiftregister Led's
  7 = Testing Shiftregister Motor
  */

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin (9600);
  Serial.println("RobotCar V0.02 Setup;");  
  pinMode(ultra_front_trig, OUTPUT);
  pinMode(ultra_front_echo, INPUT);
  pinMode(ultra_rear_trig, OUTPUT);
  pinMode(ultra_rear_echo, INPUT);
  pinMode(three_axis_x, OUTPUT);
  pinMode(three_axis_y, OUTPUT);
  pinMode(three_axis_z, OUTPUT);
  pinMode(shift_register_latch_led, OUTPUT);
  pinMode(shift_register_clock, OUTPUT);
  pinMode(shift_register_data, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  switch_var = 4; //Testing :)
  switch(switch_var){
    case 1://1 = Automatic
    
    break;
    
    case 2://2 = Manual (Bluetooth)
    
    break;
    
    case 3://3 = Driving Rounds and Rounds xD
    
    break;
    
    case 4://4 = Testing Ultrasonic
      ultra_front = funct_ultrasonic_distance(ultra_front_trig, ultra_front_echo);
      ultra_rear  = funct_ultrasonic_distance(ultra_rear_trig, ultra_rear_echo);
    break;
    
    case 5://5 = Testing Three Axis
      three_axis_output_x = funct_three_axis(three_axis_x);
        Serial.print("Output Three Axis X: "); Serial.println(three_axis_output_x, DEC);
      three_axis_output_y = funct_three_axis(three_axis_y);
        Serial.print("Output Three Axis Y: "); Serial.println(three_axis_output_y, DEC);
      three_axis_output_z = funct_three_axis(three_axis_z);
        Serial.print("Output Three Axis Z: "); Serial.println(three_axis_output_z, DEC);  
    break;
    
    case 6://6 = Testing Shiftregister Led's
      funct_shiftregister_write(shift_register_latch_led, shift_register_clock, shift_register_data, shift_register_array_led);
        Serial.println("Shift Register LED is written!");
    break;
    
    case 7://7 = Testing Shiftregister Motor
      funct_shiftregister_write(shift_register_latch_motor, shift_register_clock, shift_register_data, shift_register_array_motor);
        Serial.println("Shift Register MOTOR is written!");
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

int funct_three_axis(int three_axis_pin){
  int three_axis_output = analogRead(three_axis_x);
  return three_axis_output;
}

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
void funct_motor_drive(char drive_direction){
  switch(drive_direction){
   
    case 'forward':
      shift_register_array_motor[] = { 1, 0, 1, 0, 1, 0, 1, 0 };
    break;
    
    case 'backward':
      shift_register_array_motor[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    break; 
       
    case 'left_wide':
      shift_register_array_motor[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    break;
    
    case 'left_narrow':
      shift_register_array_motor[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    break;
    
    case 'right_wide':
      shift_register_array_motor[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    break;
    
    case 'right_narrow':
      shift_register_array_motor[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    break;
    
    case 'stop':
      shift_register_array_motor[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    break;
  }
  funct_shiftregister_write(shift_register_latch_motor, shift_register_clock, shift_register_data, shift_register_array_motor);
  Serial.print("Shift Register MOTOR is written to !"); Serial.println(drive_direction);

}

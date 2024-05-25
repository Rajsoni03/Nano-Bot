#include <LiquidCrystal_I2C.h>

#define VOLTAGE_SENSOR_PIN A0
#define MID_VOLTAGE 12
#define LOW_VOLTAGE 11.1

// PWM FS-i6 Input Connections
#define CH1 40
#define CH2 41
#define CH3 42
#define CH4 43
#define CH5 44
#define CH6 45

////////////////////////////////////////////////////////////////////////////////////////////
/*                                   [ Initialization ]                                   */
////////////////////////////////////////////////////////////////////////////////////////////


// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

// power masurement
float power_voltage = 0;
uint8_t power_percentage = 0;
uint16_t voltage_sensor_value;

// Settings
char display_mode = 'N';

// Integers to represent values from sticks and pots
int ch1_value;
int ch2_value;
int ch3_value;
int ch4_value;
int ch5_value;
int ch6_value;

// rc reciever pins
int motor_pins[8] = { 3, 2, 4, 5, 7, 6, 8, 9 };
int motor_value[4] = { 0 };

int xs, ys, zs, dx, dy, dz;
int x, y, z;


////////////////////////////////////////////////////////////////////////////////////////////
/*                                   [ LCD Custom Char ]                                  */
////////////////////////////////////////////////////////////////////////////////////////////

byte aero_up_icon[] = { 0x04, 0x0E, 0x1F, 0x04, 0x04, 0x04, 0x04, 0x04 };
byte aero_down_icon[] = { 0x04, 0x04, 0x04, 0x04, 0x04, 0x1F, 0x0E, 0x04 };
byte aero_left_icon[] = { 0x00, 0x04, 0x0C, 0x1F, 0x0C, 0x04, 0x00, 0x00 };
byte aero_right_icon[] = { 0x00, 0x04, 0x06, 0x1F, 0x06, 0x04, 0x00, 0x00 };
byte battery_low_icon[] = { 0x0E, 0x1B, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F };
byte battery_mid_icon[] = { 0x0E, 0x1B, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x1F };
byte battery_full_icon[] = { 0x0E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F };
byte wheel_icon[] = { 0x00, 0x0E, 0x1B, 0x15, 0x1B, 0x0E, 0x00, 0x00 };


////////////////////////////////////////////////////////////////////////////////////////////
/*                                   [ Function Prototype ]                               */
////////////////////////////////////////////////////////////////////////////////////////////

int readChannel(int);
void lcd_init();


void setup() {
  Serial.begin(115200);

  // put your setup code here, to run once:
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
  pinMode(CH5, INPUT);
  pinMode(CH6, INPUT);

  for (int &i : motor_pins) {
    pinMode(i, OUTPUT);
  }
  // initialize voltage sensor
  pinMode(VOLTAGE_SENSOR_PIN, INPUT);

  // add custom char for lcd
  lcd_init();
}

void move() {
  // left front
  if (motor_value[0] > 0) {
    analogWrite(motor_pins[0], 2.5 * motor_value[0]);
    analogWrite(motor_pins[1], 0);
  } else {
    analogWrite(motor_pins[0], 0);
    analogWrite(motor_pins[1], 2.5 * -motor_value[0]);
  }

  // left back
  if (motor_value[1] > 0) {
    analogWrite(motor_pins[2], 2.5 * motor_value[1]);
    analogWrite(motor_pins[3], 0);
  } else {
    analogWrite(motor_pins[2], 0);
    analogWrite(motor_pins[3], 2.5 * -motor_value[1]);
  }

  // right back
  if (motor_value[2] > 0) {
    analogWrite(motor_pins[4], 2.5 * motor_value[2]);
    analogWrite(motor_pins[5], 0);
  } else {
    analogWrite(motor_pins[4], 0);
    analogWrite(motor_pins[5], 2.5 * -motor_value[2]);
  }

  // right front
  if (motor_value[3] > 0) {
    analogWrite(motor_pins[6], 2.5 * motor_value[3]);
    analogWrite(motor_pins[7], 0);
  } else {
    analogWrite(motor_pins[6], 0);
    analogWrite(motor_pins[7], 2.5 * -motor_value[3]);
  }
}

void loop() {

  // read_voltage();
  read_rc_value();

  // if (ch6_value < 50) {
  //   if (Serial.available() > 0) {
  //     if (display_mode != 'M') lcd.clear();
  //     String command = Serial.readStringUntil('\n');
  //     // routeCommandType(command);
  //   } else if (display_mode == 'N') {
  //     lcd.setCursor(0, 0);
  //     lcd.print("Waiting");
  //   }
  // } 
  // else {
    lcd.setCursor(0, 0);
    // lcd.print("RC Mode.");
    motor_ch_mixing();
    move();
  // }
}

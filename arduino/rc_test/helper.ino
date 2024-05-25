
////////////////////////////////////////////////////////////////////////////////////////////
/*                                [ RC Transreciever Functions ]                          */
////////////////////////////////////////////////////////////////////////////////////////////

int ch_pulse;
int ch_val;
int readChannel(int channelInput) {
  ch_pulse = pulseIn(channelInput, HIGH, 30000);
  if (ch_pulse < 100) return 0;
  ch_val = map(ch_pulse, 1000, 2000, -99, 103);
  if (ch_val > 100) return 100;
  if (ch_val < -100) return -100;
  return ch_val;
}


void read_rc_value() {

  // Get values for each channel
  ch1_value = readChannel(CH1);
  ch2_value = readChannel(CH2);
  ch3_value = readChannel(CH3);
  ch4_value = readChannel(CH4);
  ch5_value = readChannel(CH5);
  ch6_value = readChannel(CH6);

  // Print to Serial Monitor
  //  Serial.print("Ch1: ");
  //  Serial.println(ch1_value);
  //  Serial.print(" | Ch2: ");
  //  Serial.print(ch2_value);
  //  Serial.print(" | Ch3: ");
  //  Serial.print(ch3_value);
  //  Serial.print(" | Ch4: ");
  //  Serial.print(ch4_value);
  //  Serial.print(" | Ch5: ");
  //  Serial.print(ch5Value);
  //  Serial.print(" | Ch6: ");
  //  Serial.println(ch6Value);
}

void motor_ch_mixing(){

  // Joystick Direction 
  xs = (ch1_value > 0) ? 1 : -1;
  ys = (ch2_value > 0) ? 1 : -1;
  zs = (ch4_value > 0) ? 1 : -1;

  // Joystick Value
  x = abs(ch1_value);
  y = abs(ch2_value);
  z = abs(ch4_value);

  // Value with weightage 
  dx = x*x / (x+y+z);
  dy = y*y / (x+y+z);
  dz = z*z / (x+y+z);

  // Add Direction 
  dx *= xs;
  dy *= ys;
  dz *= zs;
  
  // Calculate Motor speed and direction based on 3 joy stick
  motor_value[0] = dx + dy - dz;
  motor_value[1] = - dx + dy - dz;
  motor_value[2] = dx + dy + dz;
  motor_value[3] = - dx + dy + dz;
}


////////////////////////////////////////////////////////////////////////////////////////////
/*                                    [ Helper Functions ]                                */
////////////////////////////////////////////////////////////////////////////////////////////

void drive_rover(char rover_movement, char rover_direction, char break_release, char wheel_drive, int16_t motor_speed, uint16_t duration) {

  ///////////////////////////////////////////////////////////
  // lcd motor speed print
  ///////////////////////////////////////////////////////////

  lcd.setCursor(10, 1);
  lcd.write(7);
  lcd.print("    ");
  lcd.setCursor(11, 1);
  lcd.print(String(motor_speed));
  lcd.print("%");

  ///////////////////////////////////////////////////////////
  // steering control left and right
  ///////////////////////////////////////////////////////////

  lcd.setCursor(0, 1);
  if (rover_direction == 'S') {
    lcd.write(0x80);
    lcd.write(0x80);
    lcd.write(0x80);
    // steering.write(mid_angle);
  } else if (rover_direction == 'L') {
    lcd.write(2);
    lcd.write(0x80);
    lcd.write(0x80);
    // steering.write(left_angle);
  } else if (rover_direction == 'R') {
    lcd.write(0x80);
    lcd.write(0x80);
    lcd.write(3);
    // steering.write(right_angle);
  }
  read_voltage();

  ///////////////////////////////////////////////////////////
  // movement forword and backword
  ///////////////////////////////////////////////////////////

  lcd.setCursor(4, 1);
  if (wheel_drive == '0') {
    lcd.print("STOP");
    // brake(front_motor, rear_motor);
    return;
  } else if (wheel_drive == '2') {
    lcd.print("2WD ");
  } else if (wheel_drive == '4') {
    lcd.print("4WD ");
  }


  lcd.setCursor(1, 1);
  if (rover_movement == 'F') {
    lcd.write(0);
    motor_speed = (float)motor_speed * 1.5;
  } else if (rover_movement == 'B') {
    lcd.write(1);
    motor_speed = (float)motor_speed * -1.5;
  } else if (rover_movement == 'S') {
    lcd.write(0x80);
    // release(front_motor, rear_motor);
    return;
  }


  if (wheel_drive == '4') {
    // rear_motor.drive(motor_speed);
    // front_motor.drive(motor_speed);
  } else if (wheel_drive == '2') {
    // front_motor.release();
    // rear_motor.drive(motor_speed);
  }

  return;
  read_voltage();
  delay(duration);

  ///////////////////////////////////////////////////////////
  // break system
  ///////////////////////////////////////////////////////////

  if (break_release == 'B') {
    // brake(front_motor, rear_motor);
    // steering.write(mid_angle);
    lcd.setCursor(0, 1);
    lcd.print("    ");
  } else if (break_release == 'R') {
    // release(front_motor, rear_motor);
    // steering.write(mid_angle);
    lcd.setCursor(0, 1);
    lcd.print("    ");
  }
}

void read_voltage() {
  voltage_sensor_value = analogRead(VOLTAGE_SENSOR_PIN);
  voltage_sensor_value = map(voltage_sensor_value, 0, 1023, 3, 1250);
  if (voltage_sensor_value < 10) voltage_sensor_value=0;
  power_voltage = (float)voltage_sensor_value / 50;

  lcd.setCursor(10, 0);
  if (power_voltage < LOW_VOLTAGE)
    lcd.write(4);
  else if (power_voltage < MID_VOLTAGE)
    lcd.write(5);
  else
    lcd.write(6);
  lcd.print("    ");
  lcd.setCursor(11, 0);
  lcd.print(String(power_voltage));
  lcd.print("v");
}


void lcd_init() {

  // initialize the lcd
  lcd.init();

  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Jetson Rovor");
  lcd.setCursor(0, 1);
  lcd.print("Loading");
  for (int i = 7; i < 16; i++) {
    lcd.setCursor(i, 1);
    lcd.print(".");
    delay(100);
  }
  lcd.clear();

  lcd.createChar(0, aero_up_icon);
  lcd.createChar(1, aero_down_icon);
  lcd.createChar(2, aero_left_icon);
  lcd.createChar(3, aero_right_icon);
  lcd.createChar(4, battery_low_icon);
  lcd.createChar(5, battery_mid_icon);
  lcd.createChar(6, battery_full_icon);
  lcd.createChar(7, wheel_icon);
}

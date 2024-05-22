int js_pins[6] = { 40, 41, 42, 43, 44, 45 };
int motor_pins[8] = { 3, 2, 4, 5, 7, 6, 8, 9 };

int js_value[6] = { 0 };
int motor_value[4] = { 0 };

// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value
int ch_pulse;
int ch_val;
int readChannel(int& channelInput) {
  ch_pulse = pulseIn(channelInput, HIGH, 30000);
  if (ch_pulse < 100) return 0;
  ch_val = map(ch_pulse, 1000, 2000, -99, 103);
  if (ch_val > 100) return 100;
  if (ch_val < -100) return -100;
  return ch_val;
}

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  for (int &i : js_pins) {
    pinMode(i, INPUT);
  }
  for (int &i : motor_pins) {
    pinMode(i, OUTPUT);
  }
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

int xs, ys, zs, dx, dy, dz;
float x, y, z;

int index;
void loop() {
  // // put your main code here, to run repeatedly:
  // for (index = 0; index < 6; index++) {
  //   js_value[index] = readChannel(js_pins[index]);
  // }

  js_value[0] = readChannel(js_pins[0]);
  js_value[1] = readChannel(js_pins[1]);
  js_value[3] = readChannel(js_pins[3]);

  // Joystick Direction 
  xs = (js_value[0] > 0) ? 1 : -1;
  ys = (js_value[1] > 0) ? 1 : -1;
  zs = (js_value[3] > 0) ? 1 : -1;

  // Joystick Value
  x = abs(js_value[0]);
  y = abs(js_value[1]);
  z = abs(js_value[3]);

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

  move();
}

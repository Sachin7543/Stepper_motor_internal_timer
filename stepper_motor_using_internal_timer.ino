#define dirPin 51
#define stepPin 49
int state = 0;
int Direction_control = 1;
float value1 = 0;
int flag = 0;
int flag1 = 0;
int value;
int steps = 10;
void setup()
{
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A  = 14;       // compare match register 16MHz/256/2Hz
  TCCR1B = TCCR1B | (1 << WGM12);  // CTC mode
  TCCR1B = TCCR1B | ((1 << CS12) | (1 << CS10)); // 256 prescaler
  TIMSK1 = TIMSK1 | (1 << OCIE1A); // enable timer compare interrup
  Serial.begin(9600);
  Serial.println("1.For changing the direction PRESS :->1 ");
  Serial.println("2.For changing the Speed TYPE:-> SPEED ");

}
void loop()
{
  if (Serial.available() > 0)
  {
    String read_input = Serial.readString();
    Serial.println(read_input);
    read_input.trim();
    read_input.toUpperCase();
    value = read_input.toInt();
    if (value == 1)
      Direction_control = !Direction_control;
    if (read_input == "SPEED")
    { flag = 1;
      Serial.println("Please pass motor speed in rpm Example:- 60,100");
    }
    if (flag == 1 && (value > 0 && value != 1))
    {
      value1 = read_timer_up(value);
      Serial.println(value1);
      flag = 0;
    }
    if ( read_input == "STEP")
    {
      flag1 = 1;
    }
    if (flag1 == 1 && value > 0) {
      Step(value);
      Serial.println(steps);
    }
  }
}
ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  OCR1A  = (int)value1;
  digitalWrite(dirPin, Direction_control);
  state = !state;
  digitalWrite(stepPin, state);
}
void Step(int number_of_steps)
{
  steps = number_of_steps;
}

float read_timer_up(int val)
{
  Serial.println(steps);
  return 60L * 1000L / steps / val;
}

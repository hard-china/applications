class rgb {
  public:
    int r;
    int g;
    int b;
    
    rgb() {}
    rgb(int r, int g, int b) {
      this->r = r;
      this->g = g;
      this->b = b;
    }
};

#define SERIAL_STOP_BYTE 0x0d
#define CATHODES 7

int data[255];
int byteCounter = 0;

int cathodes[CATHODES] = {7, 8, 9, 10, 11, 12, 13};
int iterator = 0;
rgb* lights[CATHODES];

void setup() {
  Serial.begin(9600);
  
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  
  writeRgb(255, 127, 127);
  
  iterator = 0;
  
  TCCR1A = 0;
  TCCR1B = 0 << CS12 | 0 << CS11 | 1 << CS10;
  TIMSK1 = 1 << TOIE1;
}

ISR(TIMER1_OVF_VECT) {
  TCNT1 = 0xFFFF;
  
  digitalWrite(cathodes[iterator], HIGH);
  
  iterator++;
  if (iterator >= CATHODES) {
    iterator = 0;
  }
  
  int index = iterator;
  writeRgb(lights[index]->r, lights[index]->g, lights[index]->b);
  
  digitalWrite(cathodes[iterator], LOW);  
}

void writeRgb(int r, int g, int b) {
  analogWrite(3, g);
  analogWrite(5, b);
  analogWrite(6, r);
}

void loop() {
}

void serialEvent() {
  while (Serial.available()) {
    int inChar = (int) Serial.read();
    data[byteCounter] = inChar;
    byteCounter++;
    if (inChar == SERIAL_STOP_BYTE) {
      runCommand();
      byteCounter = 0;
    }
  }
}

void runCommand() {
  int bright = map(data[1] - 48, 0, 9, 0, 255);
  
  switch (data[0]) {
    case 'r':
      analogWrite(6, bright);
      break;
    case 'g':
      analogWrite(3, bright);
      break;  
    case 'b':
      analogWrite(5, bright);
      break;  
  }
}

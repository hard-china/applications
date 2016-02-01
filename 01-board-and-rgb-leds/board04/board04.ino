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
  
  lights[0] = new rgb(0xff, 0, 0);
  lights[1] = new rgb(0, 0x7f, 0);
  lights[2] = new rgb(0, 0, 0x7f);
  lights[3] = new rgb(0xff, 0x7f, 0);
  lights[4] = new rgb(0xff, 0, 0x7f);
  lights[5] = new rgb(0, 0x7f, 0x7f);
  lights[6] = new rgb(0, 0, 0);
  
  iterator = 0;
  
  TCCR1A = 0;
  TCCR1B = 0 << CS12 | 0 << CS11 | 1 << CS10;
  TIMSK1 = 1 << TOIE2;
}

ISR(TIMER1_OVF_vect) {
  TCNT1 = 0xFFFF - 5005;
  
  digitalWrite(cathodes[iterator], HIGH);
  
  iterator++;
  if (iterator >= CATHODES) {
    iterator = 0;
  }

  writeRgb(lights[iterator]->r, lights[iterator]->g, lights[iterator]->b);
  
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
      lights[iterator]->r = bright;
      break;
    case 'g':
      lights[iterator]->g = bright;
      break;  
    case 'b':
      lights[iterator]->b = bright;
      break;  
  }
}

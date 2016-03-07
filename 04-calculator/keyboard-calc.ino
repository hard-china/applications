#include <Keypad.h> // Подключаем библиотеку 

const byte ROWS = 4; // 4 строки
const byte COLS = 4; // 4 столбца
char keys[ROWS][COLS] = {
  {'1','2','3','+'},
  {'4','5','6','-'},
  {'7','8','9','*'},
  {'C','0','=','/'}
}; 
byte rowPins[ROWS] = {13, 12, 11, 10}; 
byte colPins[COLS] = {7, 6, 5, 4}; 

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#define SEGMETNTS 7
char pins[SEGMETNTS] = {8, 9, 14, 15, 16, 17, 19};

char segments[10][SEGMETNTS] = {
  {8,    9,    14,   NULL, 16,   17,   19  }, // 0
  {8,    9,    NULL, NULL, NULL, NULL, NULL}, // 1
  {NULL, 9,    NULL, 15,   16,   17,   19  }, // 2
  {8,    9,    NULL, 15,   16,   NULL, 19  }, // 3
  {8,    9,    14,   15,   NULL, NULL, NULL}, // 4
  {8,    NULL, 14,   15,   16,   NULL, 19  }, // 5
  {8,    NULL, 14,   15,   16,   17,   19  }, // 6
  {8,    9,    NULL, NULL, 16,   NULL, NULL}, // 7
  {8,    9,    14,   15,   16,   17,   19  }, // 8
  {8,    9,    14,   15,   16,   NULL, 19  }  // 9
};

void setup(){
  Serial.begin(9600);
  
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  
  digitalWrite(18, LOW);
}

int operand1 = NULL;
int operand2 = NULL;
char action = NULL;
int screen;
void loop(){
  char keyC = keypad.getKey();
  if (keyC){
    int key = atoi(&keyC);
    Serial.println(key); // Передаем название нажатой клавиши в сериал порт
    if (operand1 == NULL) {
      screen = key;
      operand1 = key;
      cls();
      writeNum(key);
    } else if (action == NULL) {
      action = keyC;
      cls();
      writeNum(screen);
    } else if (operand2 == NULL) {
      operand2 = key;
      cls();
      writeNum(key);
    }
    
    Serial.println(operand1);
    Serial.println(action);
    Serial.println(operand2);
    Serial.println(' ');
    
    if (operand1 != NULL && operand2 != NULL && action != NULL) {
      int res;
      switch(action) {
        case '+':
          res = operand1 + operand2; break;
        case '-':
          res = operand1 - operand2; break;
        case '*':
          res = operand1 * operand2; break;
        case '/':
          res = operand1 / operand2; break;
      }
      
      writeLongNum(res);
      operand1 = NULL;
      operand2 = NULL;
      action = NULL;
    }
  }
}

void writeLongNum(int num) {
  writeNum((int) num / 10);
  delay(500);
  
  writeNum(num % 10);
  delay(1500);
}

void writeNum(int num) {
  cls();
  
  for (int i = 0; i < SEGMETNTS; i++) {
    if (segments[num][i] != NULL) {
      digitalWrite(segments[num][i], HIGH);
    }
  }
}

void cls() {
  for (int i = 0; i < SEGMETNTS; i++) {
    digitalWrite(pins[i], LOW);
  }
  delay(100);
}

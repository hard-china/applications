#include <IRremote.h>

class rgb {
  public:
    char r;
    char g;
    char b;
    
    char r1;
    char g1;
    char b1;
    
    int dir; // -1:1 fade down/off - fade up/on
    
    bool fadeFlag;
    bool blinkFlag;
    bool stopFlag;
    
    rgb() {
      this->r = 0;
      this->g = 0;
      this->b = 0;
      this->r1 = 0;
      this->g1 = 0;
      this->b1 = 0;
      this->dir = 1;
      this->fadeFlag = false;
      this->blinkFlag = false;
      this->stopFlag = false;
    }
    rgb(int r, int g, int b) {
      this->r = r;
      this->g = g;
      this->b = b;
      this->r1 = 0;
      this->g1 = 0;
      this->b1 = 0;
      this->dir = 1;
      this->fadeFlag = false;
      this->blinkFlag = false;
      this->stopFlag = false;
    }
    
    void set(char channel, char bright) {
      switch (channel) {
        case 'r': this->r = this->r == bright ? 0 : bright; break;
        case 'g': this->g = this->g == bright ? 0 : bright; break;
        case 'b': this->b = this->b == bright ? 0 : bright; break;
        case 'f': this->fadeFlag = !this->fadeFlag; break;
        case 'l': this->blinkFlag = !this->blinkFlag; dir = 1; break;
      }
    }
    
    void fade() {
      this->r += dir;
      this->g += dir;
      this->b += dir;
      
      this->write();
    }
    
    void blinkLed() { 
      if (this->dir == -1) {
        this->r = this->r1;
        this->g = this->g1;
        this->b = this->b1;
      } else {
        this->r1 = this->r;
        this->g1 = this->g;
        this->b1 = this->b;
        
        this->r = 0;
        this->g = 0;
        this->b = 0;
      }
 
      this->dir *= -1;     
      this->write();
    }
    
    void stop() {
      if (this->stopFlag) {
        this->r = this->r1;
        this->g = this->g1;
        this->b = this->b1;
      } else {
        this->r1 = this->r;
        this->g1 = this->g;
        this->b1 = this->b;
        
        this->r = 0;
        this->g = 0;
        this->b = 0;
        
        this->blinkFlag = false;
        this->fadeFlag = false;
      }
      
      this->stopFlag = !this->stopFlag;
    }
    
    void write() {
      analogWrite(9, this->r);
      analogWrite(5, this->g);
      analogWrite(6, this->b);
    }
};

#define B1 0xFD00FF // 1
#define B2 0xFD807F // 2
#define B3 0xFD40BF // 3
#define B4 0xFD20DF // 4
#define B5 0xFDA05F // 5
#define B6 0xFD609F // 6
#define B7 0xFD10EF // 7
#define B8 0xFD906F // 8
#define B9 0xFD50AF // 9
#define B0 0xFDB04F // 0

#define BLA    0xFD30CF // l*
#define BRA    0xFD708F // r*
#define BUP    0xFD8877 // up
#define BDOWN  0xFD9867 // down
#define BLEFT  0xFD28D7 // left
#define BRIGHT 0xFD6897 // right
#define BOK    0xFDA857 // ok

int RECV_PIN = 11;
rgb *led = new rgb();
IRrecv irrecv(RECV_PIN);
decode_results results;
int del = 0;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (led->fadeFlag) {
    del++;
    if (del == 1111) {
      del = 0;
      led->fade();
    }
  }
  
  if (led->blinkFlag) {
    del++;
    if (del == 8888) {
      del = 0;
      led->blinkLed();
    }
  }
  
  if (irrecv.decode(&results)) {
    dump(&results);
    irrecv.resume(); // Receive the next value
    
    switch (results.value) {
      case B1: led->set('r', 127); break;
      case B4: led->set('r', 63); break;
      case B7: led->set('r', 2); break;
      
      case B2: led->set('g', 127); break;
      case B5: led->set('g', 63); break;
      case B8: led->set('g', 2); break;
      
      case B3: led->set('b', 127); break;
      case B6: led->set('b', 63); break;
      case B9: led->set('b', 2); break;
      
      case B0: led->set('r', 0); led->set('g', 0); led->set('b', 0); break;
      
      case BLA: led->set('f', 1); break;
      case BRA: led->set('l', 1); break;

      case BOK: led->stop(); break;
      
    }
    led->write();

  }
 
}

void dump(decode_results *results) {
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN) {
    Serial.print("Unknown encoding: ");
  }
  else if (results->decode_type == NEC) {
    Serial.print("Decoded NEC: ");

  }
  else if (results->decode_type == SONY) {
    Serial.print("Decoded SONY: ");
  }
  else if (results->decode_type == RC5) {
    Serial.print("Decoded RC5: ");
  }
  else if (results->decode_type == RC6) {
    Serial.print("Decoded RC6: ");
  }
  else if (results->decode_type == PANASONIC) {
    Serial.print("Decoded PANASONIC - Address: ");
    Serial.print(results->address, HEX);
    Serial.print(" Value: ");
  }
  else if (results->decode_type == LG) {
    Serial.print("Decoded LG: ");
  }
  else if (results->decode_type == JVC) {
    Serial.print("Decoded JVC: ");
  }
  else if (results->decode_type == AIWA_RC_T501) {
    Serial.print("Decoded AIWA RC T501: ");
  }
  else if (results->decode_type == WHYNTER) {
    Serial.print("Decoded Whynter: ");
  }
  Serial.print(results->value, HEX);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
}

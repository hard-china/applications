void setup() {
  reset();
}

void loop() {
  reset();
  
  analogWrite(3, 33);
  delay(1000);
  
  reset();
  analogWrite(5, 33);
  delay(1000);
  
  reset();
  analogWrite(6, 33);
  delay(1000);
  
  reset();
  analogWrite(3, 33);
  analogWrite(5, 33);
  delay(1000);
  
  reset();
  analogWrite(5, 33);
  analogWrite(6, 33);
  delay(1000);
  
  reset();
  analogWrite(3, 33);
  analogWrite(6, 33);
  delay(1000);
  
  reset();
  analogWrite(3, 33);
  analogWrite(5, 33);
  analogWrite(6, 33);
  delay(1000);
}

void reset() {
  analogWrite(3, 0);
  analogWrite(5, 0);
  analogWrite(6, 0);
}

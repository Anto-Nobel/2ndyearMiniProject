void setup() {
  Serial.begin(9600); 
}

void loop() {
  int y1 = digitalRead(6);
  int y12= digitalRead();
  int y13= digitalRead();
  Serial.println(y1); 
  digitalWrite(9,y1);
  
  delay(100);
}

void setup() {
  Serial.begin(9600);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT); 
}

void loop() {
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
  digitalWrite(11,LOW);
  int y1 = digitalRead(6);
  int y12= digitalRead(5);
  int y13= digitalRead(3);
  Serial.println(y1); 
  digitalWrite(9,y1);
  digitalWrite(10,y12);
  digitalWrite(11,y13);
  delay(100);
}

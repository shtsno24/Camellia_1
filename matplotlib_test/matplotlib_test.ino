int axis_0, axis_1;

void setup() {
  Serial.begin(9600);
  while (!Serial);
}

void loop() {
  axis_0 = analogRead(A0);
  axis_1 = analogRead(A1);
  Serial.print(axis_0 - 300);
  Serial.print(',');
  Serial.print(axis_1 - 300);
  Serial.print(',');
  Serial.print(axis_0);
  Serial.print(',');
  Serial.println(axis_1);
  
  delay(50);
}

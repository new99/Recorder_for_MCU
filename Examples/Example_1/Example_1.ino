double x = 0;
void setup() {
  Serial.begin(9600);
  
}

void loop() {
  double f1 = sin(x);
  double f2 = sin(2*x);
  double f3 = 2*f1+f2; 
  x += 0.0003;
  Serial.println(f1); 
  Serial.println(f2); 
  Serial.println(f3); 
  Serial.println(); 
}

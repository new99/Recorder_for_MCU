double x = 0;
int a = 1;
void setup() {
  Serial.begin(9600);
  
}

void loop() {
  double f1 = exp(x)-1;
  if(x < -10)
  {
    a = 1;
  }
  if(x > 10)
  {
    a = -1;
  }
  x += a * 0.001;
  Serial.println(x, 5); 
  Serial.println(f1, 5); 
  Serial.println(); 
}

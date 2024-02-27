void setup() {
  // put your setup code here, to run once
  	Serial.begin(9600);


}

void loop() {
  // put your main code here, to run repeatedly:
  captura();
}

void captura()
{
  delay(1000);
  Serial.println('0');
  while (!Serial.available());
  String value_read = Serial.readString();

  if (value_read.indexOf('1') != -1)
  {
    Serial.println("ok");
    captura();
  }
  else
  {
    Serial.println(value_read);
  }
}
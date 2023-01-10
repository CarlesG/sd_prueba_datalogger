

/* Conectamos un sensor de tensión, que es un divisor de tensión, no obstante, tenemos que aplicar un factor de corrección.*/
#define ANALOG_IN_PIN 2
                                         
float volatile in_voltage = 0.0;
int counts_voltage = 0;
float const R1 = 30000.0;
float const R2 = 7500.0;
float const Vcc = 3.3;
float const correction_factor = 2.0683;

void setup() {
  Serial.begin(9600);
}

void loop() {
  
counts_voltage = analogRead(ANALOG_IN_PIN);
in_voltage = counts_voltage * correction_factor * ((R1 + R2)/(R2)) * (Vcc / 4095.0); // He tenido que introducir un factor de corrección

Serial.println(in_voltage);
Serial.println(counts_voltage);  
delay(1000);
}

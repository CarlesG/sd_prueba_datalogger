/* Conectamos un sensor de tensión, que es un divisor de tensión. La tensión máxima de entrada es de 16.5*/
// Revisar porque no me salia bien al conectar la placa y el sensor
#define ANALOG_IN_PIN 4
#include "esp_adc_cal.h"
                             
float volatile in_voltage = 0.0;
int counts_voltage = 0;
float const R1 = 29810.0;
float const R2 = 7470.0;
float const Vcc = 3.3;
float const voffset = 50;

void setup() {
  Serial.begin(19200);
  analogReadResolution(12);
}

void loop() {
float voltage = 0.0;
counts_voltage = analogRead(ANALOG_IN_PIN); // Si el voltage es de 3.3, las cuentas tendrían que ser: 825 cuentas
//double in_voltage = counts_voltage * ((R1 + R2)/(R2)) * (Vcc / 4095.0) + voffset; // He tenido que introducir un factor de corrección
//double in_voltage = map(counts_voltage, 0, 4095, 0, 1650);
//in_voltage = in_voltage / 100;


//Serial.print  ln(in_voltage);
//Serial.println(counts_voltage);  
voltage = readADC_Cal(counts_voltage);
voltage = voltage * ((R2 + R1)/R2);
Serial.println(voltage / 1000.0);
delay(500);
}

/* Function for calibrating the ADC */
uint32_t readADC_Cal(int ADC_Raw)
{
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  return(esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars));
}

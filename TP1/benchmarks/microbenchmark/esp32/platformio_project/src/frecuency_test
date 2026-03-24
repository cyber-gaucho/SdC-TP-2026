unsigned long inicio;
unsigned long fin;

void setup() {
  Serial.begin(115200);
  delay(2000);

  setCpuFrequencyMhz(160);   

  Serial.println("Prueba de tiempo");
  Serial.println("----------------");

  // ===== PRUEBA CON ENTEROS =====
  volatile int sumaEntera = 0;

  inicio = millis();

  for (long i = 0; i < 100000000; i++) {
    sumaEntera = sumaEntera + 1;
  }

  fin = millis();

  Serial.print("Tiempo con enteros: ");
  Serial.print((fin - inicio) / 1000.0);
  Serial.println(" s");

  // ===== PRUEBA CON FLOATS =====
  volatile float sumaFloat = 0;

  inicio = millis();

  for (long i = 0; i < 100000000; i++) {
    sumaFloat = sumaFloat + 1.0;
  }

  fin = millis();

  Serial.print("Tiempo con floats: ");
  Serial.print((fin - inicio) / 1000.0);
  Serial.println(" s");
}

void loop() {
}

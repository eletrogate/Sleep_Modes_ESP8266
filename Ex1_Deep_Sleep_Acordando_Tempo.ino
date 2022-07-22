/**
 * Exemplo mostrando como colocar o ESP8266 no modo Deep-sleep
 */
 
void setup() {
  Serial.begin(115200);
  Serial.setTimeout(2000);
  while(!Serial) { }
  
  Serial.println("Acordado!");
  Serial.println("Entrando em Deep-Sleep por 30 segundos");
  ESP.deepSleep(30e6); // 30e6 = 30000000 microssegundos = 30 segundos
}
void loop() {
}

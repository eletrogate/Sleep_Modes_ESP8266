/**
 * Exemplo de como acordar o ESP8266 do modo Deep-sleep com botão 
 */
 
void setup() 
{
  Serial.begin(115200);
  Serial.setTimeout(2000);

  while(!Serial) { }
  
  Serial.println("Acordado!");

  Serial.println("Estou acordado, mas vou entrar no modo de deep sleep até que o pino RESET esteja conectado a um sinal LOW");
  ESP.deepSleep(0);
}

void loop() {}

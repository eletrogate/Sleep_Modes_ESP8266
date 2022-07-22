/**
 * @file SleepMode_Test.ino
 * @author Saulo Aislan
 * @brief Firmware que ler o sensor de temperatura, envia os dados
 * via MQTT e entra no modo Deep Sleep, permanecendo por 30 segundos.
 * @version 0.1
 * 
 * @copyright Copyright (c) 2022
 * 
*/
 
/* Inclusão de bibliotecas */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/* WiFi credenciais */
const char* WIFI_SSID = "**********";
const char* WIFI_PASS = "**********";

/* MQTT credenciais */
const char* mqttServer = "broker.mqtt-dashboard.com";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

/**
 * @brief Conecta ao WiFi
 */
void connectToWifi()
{
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // WiFi fix: https://github.com/esp8266/Arduino/issues/2186
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  unsigned long wifiConnectStart = millis();

  while (WiFi.status() != WL_CONNECTED)
  {
    if (WiFi.status() == WL_CONNECT_FAILED)
    {
      Serial.println("Falha ao conectar no WiFi. Por favor verifique as credenciais: ");
    }

    delay(500);
    Serial.println("...");
    if (millis() - wifiConnectStart > 5000)
    {
      Serial.println("Falha ao conectar no WiFi");
      return;
    }
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
}


/**
 * @brief Envia os dados via MQTT
 * @param double tempC Temperatura em Celsius
 */
void publishMsg(double tempC)
{
  client.setServer(mqttServer, mqttPort);
  bool ret = false;
  String msg = "{Temperatura:" + String(tempC) + "}";
  
  while (!client.connected() && !ret)
  {
    Serial.println("Conectando ao MQTT...");
 
    if (client.connect("EletrogateClient"))
    {
      Serial.println("Conectado com Sucesso");  
      ret = client.publish("topic/eletrogate", (char*) msg.c_str()); 
      delay(5000);
    }
    else
    {
      Serial.print("Falha: ");
      Serial.print(client.state());
    }
  }
}

void setup()
{
  Serial.begin(115200);

  while (!Serial) { }

  Serial.println("-------------------------------------");
  Serial.println("Rodando o Firmware de Deep Sleep!");
  Serial.println("-------------------------------------");

  connectToWifi(); /* Conecta ao WiFi */
  
  int temp = analogRead(A0); /* Pino do sensor de Tmeperatura */

  double tempC = (((temp / 1024.0) * 3.2) - 0.5) * 100.0; /* Conversão para Celsius */

  Serial.println();
  Serial.print("Temperatura: ");
  Serial.print(tempC);
  Serial.println(" C°");
  
  publishMsg(tempC); /* Envia os dados via MQTT */
  
  Serial.println("Entrando em Deep-Sleep por 30 segundos");
  ESP.deepSleep(30e6); // 30e6 is 30 microseconds
}
void loop(){}

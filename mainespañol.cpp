#include<SoftwareSerial.h>
SoftwareSerial mSerial2(14, 12); // d5(GPIO14)>>RX  d6(GPIO12)>>TX



//#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Cambia las credenciales a continuación para que tu ESP8266 se conecte a tu enrutador
//const char* ssid = "POCOX3";
// const char* password = "AB*/12¡34567891..a";

// Cambia la variable a la dirección IP de tu Raspberry Pi, para que se conecte a tu broker MQTT
// const char* mqtt_server = "192.168.133.34";

// Inicializa el espClient. Debes cambiar el nombre de espClient si tienes múltiples ESP en tu sistema de automatización del hogar
WiFiClient espClient22;
PubSubClient client(espClient22);

// Lámpara - LED - GPIO 4 = D2 en la placa ESP-12E NodeMCU
const int led = 4;

//unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)

//char msgTemperaturaInterna[MSG_BUFFER_SIZE];
//char msgTemperaturaExterna[MSG_BUFFER_SIZE];

//char msgHumedadInterna1[MSG_BUFFER_SIZE];
//char msgHumedadInterna2[MSG_BUFFER_SIZE];
//char msgHumedadExterna1[MSG_BUFFER_SIZE];
//char msgHumedadExterna2[MSG_BUFFER_SIZE];

char msgBomba1[MSG_BUFFER_SIZE];
char msgBomba2[MSG_BUFFER_SIZE];
char msgBomba3[MSG_BUFFER_SIZE];
char msgBomba4[MSG_BUFFER_SIZE];

int value = 0;

bool stringComplete = false;
String concatenado = "";

float tempInterna = 29.9;
float tempExterna = 30.1;
float humInterna1 = 31.1;
float humInterna2 = 32.1;
float humExterna1 = 33.1;
float humExterna2 = 34.1;

int eb1 = 1;
int eb2 = 0;
int eb3 = 1;
int eb4 = 0;

// Conecta tu NodeMCU a tu enrutador
void setup_wifi() {
  delay(10);

  Serial.println();

  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi conectado - Dirección IP de NodeMCU: ");
  Serial.println(WiFi.localIP());
}

// Esta función se ejecuta cuando algún dispositivo publica un mensaje en un tema al que tu NodeMCU está suscrito

void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Mensaje recibido en el tema: ");
  Serial.print(topic);
  Serial.print(". Mensaje: ");
  String messageInfo;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageInfo += (char)message[i];
  }
  Serial.println();

  // Si se recibe un mensaje en el tema room/lamp, verifica si el mensaje es encendido (on) o apagado (off). Cambia el estado del LED según el mensaje
  if (topic == "room/light") {
    Serial.print("Cambiando la luz de la habitación a ");
    if (messageInfo == "on") {
      digitalWrite(led, HIGH);
      Serial.print("Encendido");
    } else if (messageInfo == "off") {
      digitalWrite(led, LOW);
      Serial.print("Apagado");
    }
  }
  Serial.println();
}

// Esta función vuelve a conectar tu ESP8266 a tu broker MQTT
// Cambia la función si deseas suscribirte a más temas con tu ESP8266
void reconnect() {
  // Bucle hasta que nos reconectemos
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    
    if (client.connect("ESP8266Client22")) {
      Serial.println("conectado");  
      // Suscríbete o vuelve a suscribirte a un tema
      // Puedes suscribirte a más temas (para controlar más LEDs en este ejemplo)
      client.subscribe("room/light");
    } else {
      Serial.print("falló, rc=");
      Serial.print(client.state());
      Serial.println(" intenta de nuevo en 5 segundos");
      // Espera 5 segundos antes de volver a intentarlo
      delay(5000);
    }
  }
}

// La función de configuración establece tus GPIO de ESP como salidas, inicia la comunicación serial a una velocidad de baudios de 115200
// Establece tu broker MQTT y establece la función de devolución de llamada
// La función de devolución de llamada es la que recibe los mensajes y controla realmente los LEDs
void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  mSerial2.begin(9600);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

// Para este proyecto, no necesitas cambiar nada en la función de bucle. Básicamente, asegura que el NodeMCU esté conectado al broker MQTT
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  if (!client.loop())
    client.connect("ESP8266Client22");

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;

 
    // TEMPERATURA INTERNA
    snprintf (msgTemperaturaInterna, MSG_BUFFER_SIZE, "%.2f", tempInterna);
    Serial.print("Publicar mensaje: ");
    Serial.println(msgTemperaturaInterna);
    client.publish("outTopicTemperaturaInterna", msgTemperaturaInterna);

    // TEMPERATURA EXTERNA
    snprintf (msgTemperaturaExterna, MSG_BUFFER_SIZE, "%.2f", tempExterna);
    Serial.print("Publicar mensaje: ");
    Serial.println(msgTemperaturaExterna);
    client.publish("outTopicTemperaturaExterna", msgTemperaturaExterna);

    // HUMEDAD INTERNA 1
    snprintf (msgHumedadInterna1, MSG_BUFFER_SIZE, "%.2f", humInterna1);
    Serial.print("Publicar mensaje: ");
    Serial.println(msgHumedadInterna1);
    client.publish("outTopicHumedadInterna1", msgHumedadInterna1);

    // HUMEDAD INTERNA 2
    snprintf (msgHumedadInterna2, MSG_BUFFER_SIZE, "%.2f", humInterna2);
    Serial.print("Publicar mensaje: ");
    Serial.println(msgHumedadInterna2);
    client.publish("outTopicHumedadInterna2", msgHumedadInterna2);

    // HUMEDAD EXTERNA 1
    snprintf (msgHumedadExterna1, MSG_BUFFER_SIZE, "%.2f", humExterna1);
    Serial.print("Publicar mensaje: ");
    Serial.println(msgHumedadExterna1);
    client.publish("outTopicHumedadExterna1", msgHumedadExterna1);

    // HUMEDAD EXTERNA 2
    snprintf (msgHumedadExterna2, MSG_BUFFER_SIZE, "%.2f",humExterna2);
    Serial.print("Publicar mensaje: ");
    Serial.println(msgHumedadExterna2);
    client.publish("outTopicHumedadExterna2", msgHumedadExterna2);

    // BOMBA 1
    snprintf (msgBomba1, MSG_BUFFER_SIZE, "%d",eb1);
    Serial.print("Publicar mensaje: ");
    Serial.println(msgBomba1);
    client.publish("outTopicBomba1", msgBomba1);

    // BOMBA 2
    snprintf (msgBomba2, MSG_BUFFER_SIZE, "%d",eb2);
    Serial.print("Publicar mensaje: ");
    Serial.println(msgBomba2);
    client.publish("outTopicBomba2", msgBomba2);

    // BOMBA 3
    snprintf (msgBomba3, MSG_BUFFER_SIZE, "%d",eb3);
    Serial.print("Publicar mensaje: ");
    Serial.println(msgBomba3);
    client.publish("outTopicBomba3", msgBomba3);

    // BOMBA 4
    snprintf (msgBomba4, MSG_BUFFER_SIZE, "%d",eb4);
    Serial.print("Publicar mensaje: ");
    Serial.println(msgBomba4);
    client.publish("outTopicBomba4", msgBomba4);
  }

  // COMUNICACIÓN SERIAL
  while(mSerial2.available()) {
    char inChar = (char)mSerial2.read();
    Serial.print(inChar);
    if(inChar == '\n') {
      int index = concatenado.indexOf(',');
      int indexmas = concatenado.indexOf('+');
      int indexmenos = concatenado.indexOf('-');
      int indexas = concatenado.indexOf('*');
      int indexpunto = concatenado.indexOf('[');
      int indexnumeral = concatenado.indexOf('#');
      int indexslash = concatenado.indexOf('/');
      int indexparentesisIzquierda = concatenado.indexOf('(');
      int indexparentesisDerecha = concatenado.indexOf(')');
      
      String ti = concatenado.substring(0,index);
      String te = concatenado.substring(index+1,indexmas); 
      String hi1 = concatenado.substring(indexmas+1,indexmenos);
      String hi2 = concatenado.substring(indexmenos+1,indexas);
      String he1 = concatenado.substring(indexas+1,indexpunto);
      String he2 = concatenado.substring(indexpunto+1,indexnumeral);
      String b1 = concatenado.substring(indexnumeral+1,indexslash);
      String b2 = concatenado.substring(indexslash+1,indexparentesisIzquierda);
      String b3 = concatenado.substring(indexparentesisIzquierda+1,indexparentesisDerecha);
      String b4 = concatenado.substring(indexparentesisDerecha+1);

      int e_ti  = ti.toInt();
      int e_te  = te.toInt();
      int e_hi1 = hi1.toInt();
      int e_hi2 = hi2.toInt();
      int e_he1 = he1.toInt();
      int e_he2 = he2.toInt();
      
      eb1  = b1.toInt();
      eb2  = b2.toInt();
      eb3  = b3.toInt();
      eb4  = b4.toInt();

      tempInterna = e_ti/10.0;
      tempExterna = e_te/10.0;
      humInterna1 = e_hi1/10.0;
      humInterna2 = e_hi2/10.0;
      humExterna1 = e_he1/10.0;
      humExterna2 = e_he2/10.0;

      concatenado = "";
      break;
    }
    concatenado += inChar;
  }
}

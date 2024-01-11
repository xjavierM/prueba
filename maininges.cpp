#include<SoftwareSerial.h>
SoftwareSerial mSerial2(14,12); // d5(GPIO14)>>RX  d6(GPIO12)>>TX

/*****

AKP's Tech Classroom
 
*****/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "POCOX3";
const char* password = "1234567891";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "192.168.133.34";

// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient espClient22;
PubSubClient client(espClient22);


// Lamp - LED - GPIO 4 = D2 on ESP-12E NodeMCU board
const int led = 4;

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)

char msgTemperaturaInterna[MSG_BUFFER_SIZE];
char msgTemperaturaExterna[MSG_BUFFER_SIZE];

char msgHumedadInterna1[MSG_BUFFER_SIZE];
char msgHumedadInterna2[MSG_BUFFER_SIZE];
char msgHumedadExterna1[MSG_BUFFER_SIZE];
char msgHumedadExterna2[MSG_BUFFER_SIZE];

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
    
int eb1  = 1;
int eb2  = 0;
int eb3  = 1;
int eb4  = 0;

//Connect your NodeMCU to your router
void setup_wifi() {
  delay(10);
  
  Serial.println();
 
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - NodeMCU IP address: ");
  Serial.println(WiFi.localIP());
}

// This functions is executed when some device publishes a message to a topic that your NodeMCU is subscribed to

void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageInfo;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageInfo += (char)message[i];
  }
  Serial.println();

  // If a message is received on the topic room/lamp, you check if the message is either on or off. Turns the lamp GPIO according to the message
  if(topic=="room/light"){
      Serial.print("Changing Room Light to ");
      if(messageInfo == "on"){
        digitalWrite(led, HIGH);
        Serial.print("On");
      }
      else if(messageInfo == "off"){
        digitalWrite(led, LOW);
        Serial.print("Off");
      }
  }
  Serial.println();
}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    
    if (client.connect("ESP8266Client22")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("room/light");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// The setup function sets your ESP GPIOs to Outputs, starts the serial communication at a baud rate of 115200
// Sets your mqtt broker and sets the callback function
// The callback function is what receives messages and actually controls the LEDs
void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  mSerial2.begin(9600);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

// For this project, you don't need to change anything in the loop function. Basically it ensures that the NodeMCU is connected to MQTT broker
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP8266Client22");

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;

    /*
    float tempInterna = 29.9;
    float tempExterna = 30.1;
    float humInterna1 = 31.1;
    float humInterna2 = 32.1;
    float humExterna1 = 33.1;
    float humExterna2 = 34.1;
    */
    //TEMPERATURA INTERNA
    snprintf (msgTemperaturaInterna, MSG_BUFFER_SIZE, "%.2f", tempInterna);
    Serial.print("Publish message: ");
    Serial.println(msgTemperaturaInterna);
    client.publish("outTopicTemperaturaInterna", msgTemperaturaInterna);

    
    
    //TEMPERATURA EXTERNA
    snprintf (msgTemperaturaExterna, MSG_BUFFER_SIZE, "%.2f", tempExterna);
    Serial.print("Publish message: ");
    Serial.println(msgTemperaturaExterna);
    client.publish("outTopicTemperaturaExterna", msgTemperaturaExterna);

    //HUMEDAD INTERNA 1
    snprintf (msgHumedadInterna1, MSG_BUFFER_SIZE, "%.2f", humInterna1);
    Serial.print("Publish message: ");
    Serial.println(msgHumedadInterna1);
    client.publish("outTopicHumedadInterna1", msgHumedadInterna1);

    //HUMEDAD INTERNA 2
    snprintf (msgHumedadInterna2, MSG_BUFFER_SIZE, "%.2f", humInterna2);
    Serial.print("Publish message: ");
    Serial.println(msgHumedadInterna2);
    client.publish("outTopicHumedadInterna2", msgHumedadInterna2);

    //HUMEDAD EXTERNA 1
    snprintf (msgHumedadExterna1, MSG_BUFFER_SIZE, "%.2f", humExterna1);
    Serial.print("Publish message: ");
    Serial.println(msgHumedadExterna1);
    client.publish("outTopicHumedadExterna1", msgHumedadExterna1);

    //HUMEDAD EXTERNA 2
    snprintf (msgHumedadExterna2, MSG_BUFFER_SIZE, "%.2f",humExterna2);
    Serial.print("Publish message: ");
    Serial.println(msgHumedadExterna2);
    client.publish("outTopicHumedadExterna2", msgHumedadExterna2);

    //BOMBA 1
    snprintf (msgBomba1, MSG_BUFFER_SIZE, "%d",eb1);
    Serial.print("Publish message: ");
    Serial.println(msgBomba1);
    client.publish("outTopicBomba1", msgBomba1);

    //BOMBA 2
    snprintf (msgBomba2, MSG_BUFFER_SIZE, "%d",eb2);
    Serial.print("Publish message: ");
    Serial.println(msgBomba2);
    client.publish("outTopicBomba2", msgBomba2);

    //BOMBA 3
    snprintf (msgBomba3, MSG_BUFFER_SIZE, "%d",eb3);
    Serial.print("Publish message: ");
    Serial.println(msgBomba3);
    client.publish("outTopicBomba3", msgBomba3);

    //BOMBA 4
    snprintf (msgBomba4, MSG_BUFFER_SIZE, "%d",eb4);
    Serial.print("Publish message: ");
    Serial.println(msgBomba4);
    client.publish("outTopicBomba4", msgBomba4);
  }

  //COMUNICACION SERIAL

  while(mSerial2.available())
  {
    char inChar = (char)mSerial2.read();
    Serial.print(inChar);
    if(inChar == '\n')
      {
        
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
        
        //Serial.println(indexpunto);
        //Serial.println(he1);
        //Serial.println(e_ti);
        //Serial.println(tempInterna);

        concatenado = "";
      break;
      }
      concatenado += inChar;
    }
  }

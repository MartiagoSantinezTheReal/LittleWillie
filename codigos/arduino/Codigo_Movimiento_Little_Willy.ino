#include <ESP8266WiFi.h>
#include <Servo.h>

const char* ssid = "Little Willie";
const char* password = "YMCA";
WiFiServer server(80);

Servo ESC;
Servo ESC2;
int valor = 128;
int PinLED = 2;                           //Definimos el pin de salida - GPIO2 / D4
int PWMA = 15;
int INA1 = 14;
int INA2 = 12;
int PWMB = 13;
int INB1 = 4;
int INB2 = 5;
int estado = HIGH;
int der = HIGH;
int izq = HIGH;
int atras = HIGH;
int ataque = HIGH;
String slider = "0"; //QUEREMOS PARA LA PROXIMA ITERACION DEL PROGRAMA HACER UN SLIDER EN LA APP QUE NOS PERMITA CAMBIAR LA VELOCIDAD MIENTRAS MANEJAMOS.

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(10);
  pinMode(PinLED, OUTPUT);        //Inicializamos el GPIO2 como salida
  pinMode(PWMA, OUTPUT);
  pinMode(INA1, OUTPUT);
  pinMode(INA2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(INB1, OUTPUT);
  pinMode(INB2, OUTPUT);
  digitalWrite(PinLED, HIGH);
  digitalWrite(PWMA, LOW);
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(INB1, LOW);
  digitalWrite(INB2, LOW);
  ESC.attach(0, 1000, 2000);
  ESC2.attach(2, 1000, 2000);
  delay(50);
  ESC.write(0);
  ESC2.write(0);


  server.begin();                         //inicializamos el servidor
  WiFi.mode(WIFI_AP);                     // Definimos el modo del WiFi como Access Point
  // WiFi.softAP(ssid, password);            //Red con clave, en el canal 1 y visible

  //WiFi.softAP(ssid, password,3,1);      //Otra opción: Red con clave, en el canal 3 y visible
  WiFi.softAP(ssid);                    //Otra opción: Red abierta

  Serial.println();                       // Enviamos un salto de línea en el monitor serial

  Serial.print("Direccion IP Access Point - por defecto: ");      //Imprime la dirección IP en el monitor serial
  Serial.println(WiFi.softAPIP());
  Serial.print("Direccion MAC Access Point: ");                   //Imprime la dirección MAC en el monitor serial
  Serial.println(WiFi.softAPmacAddress());
}

void loop() {
  // put your main code here, to run repeatedly:

  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  String peticion = client.readStringUntil('\r');
  Serial.println(peticion);
  client.flush();

  if (peticion.indexOf('\s=') != -1) {                          //Este codigo nos permite sacar un número entero de la url solicitada para entregarsela al controlador para que aumente o disminuya la velocidad del robot
    String numero1 = peticion.substring(peticion.indexOf('\s=abc'), 7);
    String numero2 = numero1.substring(0, 3);
    valor = numero2.toInt();
    Serial.println(numero2);
    Serial.println(valor);

  }

  if (peticion.indexOf('\c') != -1) { //Mover Izquierda
    izq = LOW;
  }

  else if (peticion.indexOf('\d') != -1) {  //Mover Derecha
    der = LOW;
  }

  else if (peticion.indexOf('\h') != -1) {  //Mover Adelante
    estado = LOW;
  }

  else if (peticion.indexOf('\g') != -1) {  //Mover Atrás
    atras = LOW;
  }

  else {
    estado = HIGH;
    der = HIGH;
    izq = HIGH;
    atras = HIGH;

  }


  if (peticion.indexOf('\q') != -1) { //Encender Arma
    ataque = LOW;
  }

  else if (peticion.indexOf('\p') != -1) { //Apagar Arma
    ataque = HIGH;
  }
  
  else {
    ataque = ataque;  //Mantiene Arma encendida a menos que haya un cambio de solicitud
  }


  if (estado == LOW) {
    digitalWrite(PinLED, LOW);
    analogWrite(PWMA, valor);
    analogWrite(PWMB, valor);
    digitalWrite(INA1, LOW);
    digitalWrite(INA2, HIGH);
    digitalWrite(INB1, LOW);
    digitalWrite(INB2, HIGH);
  }

  else if (izq == LOW) {
    digitalWrite(PinLED, LOW);
    analogWrite(PWMA, valor);
    analogWrite(PWMB, valor);
    digitalWrite(INA1, HIGH);
    digitalWrite(INA2, LOW);
    digitalWrite(INB1, LOW);
    digitalWrite(INB2, HIGH);
  }

  else if (der == LOW) {
    digitalWrite(PinLED, LOW);
    analogWrite(PWMA, valor);
    analogWrite(PWMB, valor);
    digitalWrite(INA1, LOW);
    digitalWrite(INA2, HIGH);
    digitalWrite(INB1, HIGH);
    digitalWrite(INB2, LOW);
  }

  else if (atras == LOW) {
    digitalWrite(PinLED, LOW);
    analogWrite(PWMA, valor);
    analogWrite(PWMB, valor);
    digitalWrite(INA1, HIGH);
    digitalWrite(INA2, LOW);
    digitalWrite(INB1, HIGH);
    digitalWrite(INB2, LOW);
  }

  else {
    digitalWrite(PinLED, HIGH);
    digitalWrite(PWMA, LOW);
    digitalWrite(PWMB, LOW);
    digitalWrite(INA1, LOW);
    digitalWrite(INA2, LOW);
    digitalWrite(INB1, LOW);
    digitalWrite(INB2, LOW);
  }


  if (ataque == LOW) {
    ESC.write(180);
    ESC2.write(180);
  }
   else {
    ESC.write(0);
    ESC2.write(0);
  }

  Serial.println("Client disonnected");

}

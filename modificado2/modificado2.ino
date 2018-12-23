#include <SPI.h>
#include <Ethernet.h>
#include <dht11.h>
#include <Wire.h>
#include "DHT.h" //cargamos la librería DHT
#define DHTPIN 2 //Seleccionamos el pin en el que se conectará el sensor
#define DHTTYPE DHT11 //Se selecciona el DHT22(hay otros DHT)

DHT dht(DHTPIN, DHTTYPE); //Se inicia una variable que será usada por Arduino para comunicarse con el sensor
String t1; 
double celcius = dht.readTemperature();
double humedad = dht.readHumidity();
double Fahrenheit = 1.8 * dht.readTemperature() + 32;
double Kelvin = dht.readTemperature() + 273.15;
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,0,101);

// Inicializa la libreria ethernet, el puerto 80 esta por defecto para HTTP
EthernetServer server(125);


void setup() {
  Serial.begin(9600);
  dht.begin();
  while (!Serial) {
}
 //Inicializar la conexion Ethernet y el servidor
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  
  float t = dht.readTemperature(); //Se lee la temperatura
  t1 = String (t); //Se convierte la temperatura a String
  Serial.println(t1);//Se envia la lectura del sensor de temperatura como string.
  delay (2000);//El dato se envia cada 100 milisegundos.

//Escucha clientes de entrada
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    //Una solicitud http cuya linea esta en blanco
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // Si la linea en la solicitud http esta en blanco y el cliente esta disponible se puede enviar una respuesta
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/javascript");
          client.println("Connnection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // Agregar una etiqueta meta que recargue la pagina cada segundo
          client.println("<meta http-equiv=\"refresh\" content=\"1\">");
          client.print("ESTOY VIVOOO!");   
          client.println("<br />");    

          //Obtener las lecturas del sensor e imprimir el resultado
          
//------------------------------------------------WEB--------------------------------------------------------------------------------------          
          
          
          client.print("<!DOCTYPE html>");
          client.print("<html lang='es'>");
          
          
          client.println("<head>");
          client.println("<title>SENSOR DHT11</title>");
          client.println("<style type='text/css'> body { color: red; background-image: url('http://frostcode.es/descargas/back%20servidor.jpg'); } </style>");//EL CSS
         
          client.println("<meta charset='utf-8'>");
          client.println("</head>");
          
          client.println("<body>");


          client.print("<center>");   
          client.print("<h1>SENSOR DE TEMPERATURA Y HUMEDAD</h1>");
          client.print("Temperatura (Farenheit): ");
          client.println(Fahrenheit, 1);
          client.println("<br />");  
          
          client.print("Temperatura (Kelvin): ");
          client.println(Kelvin, 1);
          client.println("<br />");   
          
          client.print("Humedad (%): ");
          client.println((float)humedad, 0);  
          client.println("<br />");   

          client.print("Temperaturas : ");
          client.println((float)celcius, 1);  
                    client.print("</center>");
          client.println("<br />");  
          
          

//Termina lectura del sensor
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          //Linea en blanco
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          //Si hay un caracter en la linea en blanco
          currentLineIsBlank = false;
        }
      }
    }
    //Darle al navegador web tiempo para recibir los datos 
    delay(30);
    //Se cierra la conexion
    client.stop();
    Serial.println("client disonnected");
  }
}
//Convertir de grados Centrigrados a Farenheit
double Fahrenheit(double celsius)
{
  return 1.8 * celsius + 32;
}

//Centigrados a Kelvin
double Kelvin(double celsius)
{
  return celsius + 273.15;
}

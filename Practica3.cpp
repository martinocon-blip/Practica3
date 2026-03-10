#include <WiFi.h>
#include <WebServer.h>
// SSID & Password
const char* ssid = "Nautilus"; // Enter your SSID here
const char* password = "20000Leguas"; //Enter your Password here
WebServer server(80); // Object of WebServer(HTTP port, 80 is defult)
void handle_root();
void setup() {
Serial.begin(115200);
Serial.println("Try Connecting to ");
Serial.println(ssid);
// Connect to your wi-fi modem
WiFi.begin(ssid, password);
// Check wi-fi is connected to wi-fi network
while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected successfully");
Serial.print("Got IP: ");
Serial.println(WiFi.localIP()); //Show ESP32 IP on serial
server.on("/", handle_root);
server.begin();
Serial.println("HTTP server started");
delay(100);
}
void loop() {
server.handleClient();
}
// HTML & CSS contents which display on web server
String HTML = "<!DOCTYPE html>\n\
<html lang=\"es\">\n\
<head>\n\
<meta charset=\"UTF-8\">\n\
<title>ESP32 Web Server</title>\n\
<style>\n\
  body { font-family: Arial, sans-serif; margin: 40px; background-color: #f4f4f9; color: #333; }\n\
  h1 { color: #0056b3; }\n\
  .container { background: white; padding: 20px; border-radius: 8px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); }\n\
</style>\n\
</head>\n\
<body>\n\
<div class=\"container\">\n\
  <h1>&#128640; Mi Proyecto ESP32: Práctica 3</h1>\n\
  <h2>Servidor Web en Modo Station</h2>\n\
  <p>¡Hola! Esta es una página web generada desde mi microcontrolador ESP32.</p>\n\
  <h3>¿Qué hemos aprendido en esta práctica?</h3>\n\
  <ul>\n\
    <li>Configurar la conexión WiFi.</li>\n\
    <li>Encontrar la dirección IP en el Monitor Serie.</li>\n\
    <li>Servir una página HTML estructurada desde Markdown.</li>\n\
  </ul>\n\
  <p><strong>¡Práctica completada con éxito! &#127881;</strong></p>\n\
</div>\n\
</body>\n\
</html>";
// Handle root url (/)
void handle_root() {
server.send(200, "text/html", HTML);
}
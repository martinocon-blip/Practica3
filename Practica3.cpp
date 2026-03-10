
#include <WiFi.h>
#include <WebServer.h>

// Tus credenciales WiFi
const char* ssid = "Nautilus"; 
const char* password = "20000Leguas"; 

WebServer server(80); 

// Prototipo
void handle_root();

// Nuestro HTML base. Fíjate en el "%TEMPERATURA%" y en la etiqueta "refresh"
String HTML = R"=====(
<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta http-equiv="refresh" content="2">
<title>Temperatura ESP32</title>
<style>
  body { margin: 0; height: 100vh; display: flex; justify-content: center; align-items: center; background-color: #2c3e50; font-family: 'Arial', sans-serif; color: white;}
  .tarjeta { background: #34495e; padding: 40px; border-radius: 15px; text-align: center; box-shadow: 0 15px 25px rgba(0,0,0,0.5); }
  h1 { margin-top: 0; color: #e74c3c; }
  .temp { font-size: 4em; font-weight: bold; margin: 20px 0; color: #f1c40f; }
  .nota { font-size: 0.9em; color: #bdc3c7; }
</style>
</head>
<body>
  <div class="tarjeta">
    <h1>&#127777; Termómetro Interno ESP32</h1>
    <p>Temperatura actual del microprocesador:</p>
    
    <div class="temp">%TEMPERATURA% &deg;C</div>
    
    <p class="nota">Esta página se actualiza automáticamente cada 2 segundos.</p>
  </div>
</body>
</html>
)=====";

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi conectada!");
  Serial.print("IP para el navegador: ");
  Serial.println(WiFi.localIP()); 
  
  server.on("/", handle_root);
  server.begin();
}

void loop() {
  // El ESP32 siempre está escuchando si alguien entra a la web
  server.handleClient();
}

// Cuando alguien entra a la web, se ejecuta esto:
void handle_root() {
  // 1. Leemos la temperatura real del chip
  float temp_celsius = temperatureRead();
  
  // Imprimimos en el Monitor Serie para comprobar
  Serial.print("Temperatura leída: ");
  Serial.println(temp_celsius);
  
  // 2. Hacemos una copia de nuestro HTML
  String pagina_final = HTML;
  
  // 3. Reemplazamos el comodín por el valor real convertido a texto
  pagina_final.replace("%TEMPERATURA%", String(temp_celsius));
  
  // 4. Enviamos la página ya modificada al navegador
  server.send(200, "text/html", pagina_final);
}
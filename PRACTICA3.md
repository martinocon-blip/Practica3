# Práctica 3 - WiFi y Web Server con ESP32

## Objetivo

El objetivo de esta práctica es comprender el funcionamiento de las comunicaciones WiFi en el ESP32 mediante la creación de un servidor web capaz de mostrar información en tiempo real desde el microcontrolador.

En esta implementación se utiliza el sensor de temperatura interno del ESP32 para generar una página web dinámica que muestra la temperatura actual del chip.

---

# Introducción teórica

El ESP32 incorpora conectividad WiFi y Bluetooth integrada, lo que permite desarrollar aplicaciones IoT, servidores web y sistemas de comunicación inalámbrica.

En esta práctica se utiliza el modo STA (Station Mode), donde el ESP32 se conecta a una red WiFi existente utilizando un SSID y contraseña.

Una vez conectado a la red, el ESP32 actúa como servidor web HTTP utilizando la librería:

```cpp
#include <WebServer.h>
```

El servidor escucha peticiones HTTP desde un navegador y responde enviando una página HTML generada dinámicamente.

---

# Material utilizado

- ESP32-S3-DevKitC-1
- Visual Studio Code
- PlatformIO
- Framework Arduino
- Red WiFi local
- Navegador web

---

# Configuración WiFi

## Credenciales utilizadas

```cpp
const char* ssid = "Nautilus";
const char* password = "20000Leguas";
```

El ESP32 se conecta automáticamente a esta red durante el `setup()`.

---

# Código implementado

## Archivo `main.cpp`

```cpp
#include <WiFi.h>
#include <WebServer.h>

// Credenciales WiFi
const char* ssid = "Nautilus";
const char* password = "20000Leguas";

WebServer server(80);

// Prototipo
void handle_root();

// HTML base
String HTML = R"=====(
<!DOCTYPE html>
<html lang="es">

<head>

<meta charset="UTF-8">

<meta http-equiv="refresh" content="2">

<title>Temperatura ESP32</title>

<style>

body {
  margin: 0;
  height: 100vh;
  display: flex;
  justify-content: center;
  align-items: center;
  background-color: #2c3e50;
  font-family: Arial;
  color: white;
}

.tarjeta {
  background: #34495e;
  padding: 40px;
  border-radius: 15px;
  text-align: center;
  box-shadow: 0 15px 25px rgba(0,0,0,0.5);
}

.temp {
  font-size: 4em;
  font-weight: bold;
  color: #f1c40f;
}

</style>
</head>

<body>

<div class="tarjeta">

<h1>🌡 Termómetro Interno ESP32</h1>

<p>Temperatura actual del microprocesador:</p>

<div class="temp">%TEMPERATURA% &deg;C</div>

<p>
Esta página se actualiza automáticamente cada 2 segundos.
</p>

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

  server.handleClient();
}

void handle_root() {

  float temp_celsius = temperatureRead();

  Serial.print("Temperatura leída: ");

  Serial.println(temp_celsius);

  String pagina_final = HTML;

  pagina_final.replace(
    "%TEMPERATURA%",
    String(temp_celsius)
  );

  server.send(200, "text/html", pagina_final);
}
```

---

# Funcionamiento del programa

El programa comienza conectando el ESP32 a una red WiFi utilizando las credenciales definidas.

Una vez conectado:

1. El ESP32 obtiene una dirección IP local.
2. Se inicia un servidor web HTTP en el puerto 80.
3. El servidor queda escuchando peticiones desde un navegador.

Cuando un usuario accede a la dirección IP del ESP32:

- Se ejecuta la función `handle_root()`.
- Se lee la temperatura interna del chip mediante:
  
```cpp
temperatureRead();
```

- El valor se inserta dinámicamente en el HTML.
- El servidor envía la página completa al navegador.

La página se actualiza automáticamente cada 2 segundos utilizando:

```html
<meta http-equiv="refresh" content="2">
```

---

# Explicación de elementos importantes

## WiFi.begin()

```cpp
WiFi.begin(ssid, password);
```

Inicia la conexión WiFi utilizando las credenciales proporcionadas.

---

## WiFi.status()

```cpp
WiFi.status()
```

Permite comprobar si el ESP32 ya está conectado a la red.

---

## WebServer server(80)

```cpp
WebServer server(80);
```

Crea un servidor HTTP utilizando el puerto 80.

---

## server.on()

```cpp
server.on("/", handle_root);
```

Asocia la URL principal `/` con la función `handle_root()`.

---

## server.handleClient()

```cpp
server.handleClient();
```

Permite que el ESP32 atienda peticiones HTTP constantemente.

---

## temperatureRead()

```cpp
temperatureRead();
```

Lee la temperatura interna del microcontrolador ESP32.

---

# Estructura de la página web

La página HTML incluye:

- Diseño responsive.
- CSS integrado.
- Actualización automática.
- Visualización dinámica de temperatura.

El valor `%TEMPERATURA%` actúa como marcador dinámico que posteriormente es reemplazado por el valor real leído desde el ESP32.

---

# Diagrama de flujo

```text
                ┌──────────────┐
                │   Inicio     │
                └──────┬───────┘
                       │
                       ▼
         ┌─────────────────────────┐
         │ Inicializar puerto      │
         │ serie                   │
         └──────────┬──────────────┘
                    │
                    ▼
         ┌─────────────────────────┐
         │ Conectar a la red WiFi │
         └──────────┬──────────────┘
                    │
            WiFi conectada
                    │
                    ▼
         ┌─────────────────────────┐
         │ Mostrar IP local        │
         └──────────┬──────────────┘
                    │
                    ▼
         ┌─────────────────────────┐
         │ Iniciar servidor web    │
         └──────────┬──────────────┘
                    │
                    ▼
         ┌─────────────────────────┐
         │ Esperar conexiones      │
         └──────────┬──────────────┘
                    │
         Usuario entra a la web
                    │
                    ▼
         ┌─────────────────────────┐
         │ Leer temperatura        │
         └──────────┬──────────────┘
                    │
                    ▼
         ┌─────────────────────────┐
         │ Insertar valor en HTML  │
         └──────────┬──────────────┘
                    │
                    ▼
         ┌─────────────────────────┐
         │ Enviar página web       │
         └─────────────────────────┘
```

---

# Salida por terminal serie

Ejemplo de salida observada:

```text
.
.
WiFi conectada!
IP para el navegador: 192.168.1.145

Temperatura leída: 47.20
Temperatura leída: 47.31
Temperatura leída: 47.25
```

---

# Visualización desde navegador

Al introducir la IP del ESP32 en el navegador se muestra una página web con:

- El título "Termómetro Interno ESP32".
- La temperatura actual del microcontrolador.
- Actualización automática cada 2 segundos.

---

# Ventajas del sistema

- Monitorización inalámbrica.
- Actualización en tiempo real.
- Interfaz visual accesible desde cualquier dispositivo conectado a la red.
- Base para proyectos IoT más avanzados.

---

# Conclusiones

En esta práctica se ha aprendido:

- Cómo conectar un ESP32 a una red WiFi.
- Cómo crear un servidor web HTTP.
- Cómo generar páginas HTML dinámicas.
- Cómo enviar información en tiempo real a un navegador.
- Cómo utilizar el sensor de temperatura interno del ESP32.

Además, se ha comprobado el potencial del ESP32 para aplicaciones IoT y monitorización remota.
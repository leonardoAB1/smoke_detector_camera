# Smoke Detector Camera - Servidor Móvil

## Descripción del proyecto

El proyecto Smoke Detector Camera utiliza la cámara ESP32-CAM para capturar video continuamente y enviarlo a un servidor para su procesamiento. El objetivo principal es detectar fumadores en las imágenes del video y tomar medidas en consecuencia.

El servidor móvil actúa como una interfaz de comunicación entre la cámara y el backend basado en Django. Recibe el video transmitido por la cámara y lo envía al servidor para su procesamiento. El servidor utiliza algoritmos de detección de fumadores para analizar las imágenes del video y generar resultados.

Este proyecto se basa en la plataforma ESP-IDF para el desarrollo del firmware de la cámara ESP32-CAM y en Django para el desarrollo del servidor.

## Características

- Captura y transmisión de video en tiempo real desde la cámara ESP32-CAM al servidor.
- Procesamiento de imágenes del video para detectar fumadores.
- Interfaz de comunicación entre la cámara y el servidor.
- Configuración sencilla y flexible a través de archivos de configuración.

## Requisitos previos

Antes de ejecutar el servidor móvil, asegúrate de tener los siguientes requisitos previos:

1. Cámara ESP32-CAM con firmware actualizado.
2. Servidor basado en Django correctamente configurado y en funcionamiento.
3. Conexión estable a internet.

# Web Server API Documentation

This document provides the documentation for the REST API of the web server implemented in the `web_server.c` file. The web server runs on an ESP32-CAM development board and provides various functionalities accessible through different URI endpoints.

## Base URL

The base URL for accessing the web server API is `http://localhost/` or `http://<ip_address>/` where `<ip_address>` is the IP address of the ESP32-CAM board.

## API Endpoints

The following table lists the available API endpoints along with their corresponding URIs, HTTP methods, and handlers:

| URI                  | Method | Handler                      | Description                                |
|----------------------|--------|------------------------------|--------------------------------------------|
| `/image`             | GET    | `image_httpd_handler`        | Get a single image                         |
| `/image64`           | GET    | `image_base64_httpd_handler` | Get a single image base64 encoded           |
| `/status`            | GET    | `status_httpd_handler`       | Get the camera status                       |
| `/admin`             | POST   | `admin_httpd_handler`        | Admin functionality                        |
| `/motor_01/set`      | POST   | `handle_set_motor_angle`     | Set the angle of motor 01                   |
| `/motor_01/get`      | GET    | `handle_get_motor_angle`     | Get the angle of motor 01                   |
| `/motor_02/set`      | POST   | `handle_set_motor_angle`     | Set the angle of motor 02                   |
| `/motor_02/get`      | GET    | `handle_get_motor_angle`     | Get the angle of motor 02                   |
| `/ldr/set`           | POST   | `handle_set_ldr`             | Set the state of the LDR                     |
| `/ldr/get`           | GET    | `handle_get_ldr`             | Get the state of the LDR                     |
| `/led/set`           | POST   | `handle_set_led`             | Set the state of the LED                     |
| `/led/get`           | GET    | `handle_get_led`             | Get the state of the LED                     |
| `/smoke_sensor/set`  | POST   | `handle_set_smoke_sensor`    | Set the state of the smoke sensor (testing)  |
| `/smoke_sensor/get`  | GET    | `handle_get_smoke_sensor`    | Get the state of the smoke sensor            |

## Request and Response Formats

### `/image` - Get a single image

**Request:**

- Method: GET

**Response:**

- Content Type: image/jpeg
- Body: Binary image data

### `/image64` - Get a single image base64 encoded

**Request:**

- Method: GET

**Response:**

- Content Type: image/jpeg
- Body: Base64-encoded image data

### `/status` - Get the camera status

**Request:**

- Method: GET

**Response:**

- Content Type: application/json
- Body: JSON object representing the status
  - Example response:
    ```json
    {
      "status": "online"
    }
    ```

### `/admin` - Admin functionality

**Request:**

- Method: POST
- Headers:
  - `Content-Length`: Length of the request body in bytes
- Body: JSON object containing admin-specific data
  - Example request body:
    ```json
    {
      "value": 42
    }
    ```

**Response:**

- Content Type: text/plain
- Body: Success message indicating the value was updated successfully

### `/motor_01/set - Set the angle of motor 01

**Request:**

- Method: POST
- Headers:
  - `Content-Length`: Length of the request body in bytes
- Body: JSON object containing the angle value
  - Example request body:
    ```json
    {
      "angle": 90
    }
    ```

**Response:**

- Content Type: text/plain
- Body: Success message indicating the angle was set successfully

### `/motor_01/get` - Get the angle of motor 01

**Request:**

- Method: GET

**Response:**

- Content Type: application/json
- Body: JSON object containing the current angle value
  - Example response:
    ```json
    {
      "angle": 90
    }
    ```

### `/motor_02/set` - Set the angle of motor 02

**Request:**

- Method: POST
- Headers:
  - `Content-Length`: Length of the request body in bytes
- Body: JSON object containing the angle value
  - Example request body:
    ```json
    {
      "angle": 180
    }
    ```

**Response:**

- Content Type: text/plain
- Body: Success message indicating the angle was set successfully

### `/motor_02/get` - Get the angle of motor 02

**Request:**

- Method: GET

**Response:**

- Content Type: application/json
- Body: JSON object containing the current angle value
  - Example response:
    ```json
    {
      "angle": 180
    }
    ```

### `/ldr/set` - Set the state of the LDR

**Request:**

- Method: POST
- Headers:
  - `Content-Length`: Length of the request body in bytes
- Body: JSON object containing the state value (`1` for high, `0` for low)
  - Example request body:
    ```json
    {
      "state": 1
    }
    ```

**Response:**

- Content Type: text/plain
- Body: Success message indicating the state was set successfully

### `/ldr/get` - Get the state of the LDR

**Request:**

- Method: GET

**Response:**

- Content Type: application/json
- Body: JSON object containing the current state value (`1` for high, `0` for low)
  - Example response:
    ```json
    {
      "state": 1
    }
    ```

### `/led/set` - Set the state of the LED

**Request:**

- Method: POST
- Headers:
  - `Content-Length`: Length of the request body in bytes
- Body: JSON object containing the state value (`1` for on, `0` for off)
  - Example request body:
    ```json
    {
      "state": 1
    }
    ```

**Response:**

- Content Type: text/plain
- Body: Success message indicating the state was set successfully


# Licencia

Este proyecto está licenciado bajo la [Licencia MIT](https://opensource.org/licenses/MIT).

# Contacto

Si tienes alguna pregunta, no dudes en contactarnos.
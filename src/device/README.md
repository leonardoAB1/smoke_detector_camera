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

## Licencia

Este proyecto está licenciado bajo la [Licencia MIT](https://opensource.org/licenses/MIT).

## Contacto

Si tienes alguna pregunta, no dudes en contactarnos.
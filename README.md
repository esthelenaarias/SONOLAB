# SONOLAB - Sistema de Monitoreo de Ruido Escolar

SONOLAB es un anteproyecto escolar de grado 8.° del Colegio Jorge Gaitán Cortés (Bogotá). Desarrolla un sistema IoT con el microcontrolador ESP32 y sensores para medir el ruido escolar en tiempo real y emitir alertas visuales por un semáforo LED.

---

## 🛠️ Componentes de Hardware

- **Microcontrolador:** ESP32 Dev Module
- **Sensor de Audio:** Micrófono analógico MAX4466
- **Indicador Visual:** Módulo Semáforo LED de 4 pines
- **Conexiones:** Cables Dupont Hembra-Hembra

---

## 🔌 Esquema de Conexiones (Pinout)

| Componente | Pin Módulo | Pin ESP32 | Descripción |
| :--- | :--- | :--- | :--- |
| **Micrófono MAX4466** | VCC | 3V3 / 5V | Alimentación |
| | GND | GND | Tierra principal |
| | OUT | GPIO 33 | Entrada analógica de audio |
| **Módulo Semáforo** | G (Verde) | GPIO 21 | Luz Verde (Silencio) |
| | Y (Amarillo)| GPIO 19 | Luz Amarilla (Ruido moderado) |
| | R (Rojo) | GPIO 17 | Luz Roja (Ruido alto) |
| | GND | GND | Tierra compartida |

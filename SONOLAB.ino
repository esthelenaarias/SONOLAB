#include <math.h>

// --- PINES DE HARDWARE ---
#define MIC_PIN       33  // Entrada analógica del micrófono MAX4466
#define LED_ROJO      17  // Módulo Semáforo - Pin R
#define LED_AMARILLO  19  // Módulo Semáforo - Pin Y
#define LED_VERDE     21  // Módulo Semáforo - Pin G
#define SEMAFORO_GND  16  // Pin digital configurado como GND virtual

// --- UMBRALES DE RUIDO EN DECIBELES (dB) ---
float umbralAmarillodB = 45.0; 
float umbralRojodb     = 60.0; 
const float DB_OFFSET  = 55.0; 

float dBActual = 0.0;
String estadoSemaforo = "VERDE";

// --- FUNCIÓN DE MEDICIÓN DE AUDIO ---
float medirDecibeles() {
  unsigned long startMillis = millis();
  unsigned int signalMax = 0;
  unsigned int signalMin = 4095;

  // Ventana de muestreo de 50 ms para capturar picos de sonido
  while (millis() - startMillis < 50) {
    int sample = analogRead(MIC_PIN);
    if (sample < 4095) {
      if (sample > signalMax) signalMax = sample;
      if (sample < signalMin) signalMin = sample;
    }
  }

  int peakToPeak = signalMax - signalMin;       
  float volts = (peakToPeak * 3.3) / 4095.0;    

  if (volts < 0.001) volts = 0.001;             

  float decibeles = 20.0 * log10(volts) + DB_OFFSET;
  if (decibeles < 30.0) decibeles = 30.0;       

  return decibeles;
}

// --- CONTROL FÍSICO DE LOS LEDS ---
void actualizarSemaforoFisico(bool verde, bool amarillo, bool rojo) {
  digitalWrite(LED_VERDE, verde ? HIGH : LOW);
  digitalWrite(LED_AMARILLO, amarillo ? HIGH : LOW);
  digitalWrite(LED_ROJO, rojo ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);

  // 1. Configurar GPIO 16 como GND Falso (0V)
  pinMode(SEMAFORO_GND, OUTPUT);
  digitalWrite(SEMAFORO_GND, LOW); 

  // 2. Configuración de pines para el semáforo
  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_AMARILLO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);

  // Iniciar apagados
  actualizarSemaforoFisico(false, false, false);

  delay(1000);
  Serial.println("==========================================");
  Serial.println("   EcoAcustica IA - Sistema Iniciado     ");
  Serial.println("==========================================");
}

unsigned long ultimoImpreso = 0;

void loop() {
  // 1. Muestreo de ruido
  dBActual = medirDecibeles();

  // 2. Lógica de estados del semáforo
  if (dBActual >= umbralRojodb) {
    estadoSemaforo = "ROJO (Ruido Alto)";
    actualizarSemaforoFisico(false, false, true);
  } 
  else if (dBActual >= umbralAmarillodB) {
    estadoSemaforo = "AMARILLO (Moderado)";
    actualizarSemaforoFisico(false, true, false);
  } 
  else {
    estadoSemaforo = "VERDE (Silencio)";
    actualizarSemaforoFisico(true, false, false);
  }

  // 3. Impresión en Monitor Serie
  if (millis() - ultimoImpreso > 200) {
    Serial.print("Nivel: ");
    Serial.print(dBActual, 1);
    Serial.print(" dB | Estado: ");
    Serial.println(estadoSemaforo);
    
    ultimoImpreso = millis();
  }
}
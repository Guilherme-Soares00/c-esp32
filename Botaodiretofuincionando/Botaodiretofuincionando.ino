#include <Wire.h>

#define MPU_ADDR 0x68
#define BOTAO 0

bool estadoAnterior = HIGH;
bool gravando = false;

unsigned long ultimaLeitura = 0;
const unsigned long INTERVALO = 20; // ~50 Hz


// Escreve em um registrador do MPU6050
bool escreverMPU(uint8_t registrador, uint8_t valor) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(registrador);
  Wire.write(valor);

  return Wire.endTransmission() == 0;
}


// Inicializa o MPU6050
bool iniciarMPU() {

  // Verifica se existe algo em 0x68
  Wire.beginTransmission(MPU_ADDR);

  if (Wire.endTransmission() != 0) {
    return false;
  }

  // Acorda o MPU6050
  if (!escreverMPU(0x6B, 0x00)) {
    return false;
  }

  delay(100);

  // Acelerômetro +/- 8g
  escreverMPU(0x1C, 0x10);

  // Giroscópio +/- 500 graus/s
  escreverMPU(0x1B, 0x08);

  // Filtro digital
  escreverMPU(0x1A, 0x04);

  return true;
}


// Lê aceleração + giroscópio
bool lerMPU(
  float &ax,
  float &ay,
  float &az,
  float &gx,
  float &gy,
  float &gz
) {

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);

  if (Wire.endTransmission(false) != 0) {
    return false;
  }

  size_t recebidos = Wire.requestFrom(
    (uint8_t)MPU_ADDR,
    (size_t)14,
    true
  );

  if (recebidos != 14) {
    return false;
  }

  int16_t rawAX =
      ((int16_t)Wire.read() << 8) | Wire.read();

  int16_t rawAY =
      ((int16_t)Wire.read() << 8) | Wire.read();

  int16_t rawAZ =
      ((int16_t)Wire.read() << 8) | Wire.read();

  // Temperatura (não vamos usar)
  Wire.read();
  Wire.read();

  int16_t rawGX =
      ((int16_t)Wire.read() << 8) | Wire.read();

  int16_t rawGY =
      ((int16_t)Wire.read() << 8) | Wire.read();

  int16_t rawGZ =
      ((int16_t)Wire.read() << 8) | Wire.read();


  // +/- 8g => 4096 LSB/g
  ax = (rawAX / 4096.0) * 9.80665;
  ay = (rawAY / 4096.0) * 9.80665;
  az = (rawAZ / 4096.0) * 9.80665;

  // +/- 500 graus/s => 65.5 LSB/(graus/s)
  gx = (rawGX / 65.5) * DEG_TO_RAD;
  gy = (rawGY / 65.5) * DEG_TO_RAD;
  gz = (rawGZ / 65.5) * DEG_TO_RAD;

  return true;
}


void setup() {

  Serial.begin(115200);

  pinMode(BOTAO, INPUT_PULLUP);

  Wire.begin(21, 22);
  Wire.setClock(100000);

  delay(500);

  Serial.println();
  Serial.println("=== PROJETO GESTO ===");
  Serial.println("Procurando MPU6050...");

  // Tenta algumas vezes
  bool encontrado = false;

  for (int tentativa = 1; tentativa <= 5; tentativa++) {

    if (iniciarMPU()) {
      encontrado = true;
      break;
    }

    Serial.print("Tentativa ");
    Serial.print(tentativa);
    Serial.println(" falhou...");

    delay(300);
  }

  if (!encontrado) {
    Serial.println("ERRO_MPU");

    while (true) {
      delay(1000);
    }
  }

  Serial.println("MPU_OK");
  Serial.println("SISTEMA_PRONTO");
}


void loop() {

  bool estadoAtual = digitalRead(BOTAO);


  // Pressionou BOOT
  if (estadoAtual == LOW && estadoAnterior == HIGH) {

    gravando = true;

    Serial.println("START");

    delay(25);
  }


  // Soltou BOOT
  if (estadoAtual == HIGH && estadoAnterior == LOW) {

    gravando = false;

    Serial.println("STOP");

    delay(25);
  }


  estadoAnterior = estadoAtual;


  // Captura do MPU
  if (
    gravando &&
    millis() - ultimaLeitura >= INTERVALO
  ) {

    ultimaLeitura = millis();

    float ax, ay, az;
    float gx, gy, gz;

    if (lerMPU(ax, ay, az, gx, gy, gz)) {

      Serial.print("DATA,");

      Serial.print(ax, 4);
      Serial.print(",");

      Serial.print(ay, 4);
      Serial.print(",");

      Serial.print(az, 4);
      Serial.print(",");

      Serial.print(gx, 4);
      Serial.print(",");

      Serial.print(gy, 4);
      Serial.print(",");

      Serial.println(gz, 4);

    } else {

      Serial.println("ERRO_LEITURA_MPU");
    }
  }

  delay(2);
}
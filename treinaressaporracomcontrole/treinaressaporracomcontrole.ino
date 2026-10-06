#include <Bluepad32.h>
#include <Wire.h>

// =====================================================
// PROJETO GESTO
// MPU6050 + ESP32 + CONTROLE PS4
// X pressionado = START
// X solto      = STOP
// =====================================================


// ===============================
// MPU6050
// ===============================

#define MPU_ADDR 0x68

unsigned long ultimaLeitura = 0;

// 20 ms = aproximadamente 50 amostras por segundo
const unsigned long INTERVALO = 20;


// ===============================
// CONTROLE PS4
// ===============================

ControllerPtr controle = nullptr;

bool xAnterior = false;
bool gravando = false;


// =====================================================
// FUNÇÕES MPU6050
// =====================================================

bool escreverMPU(uint8_t registrador, uint8_t valor) {

  Wire.beginTransmission(MPU_ADDR);

  Wire.write(registrador);
  Wire.write(valor);

  return Wire.endTransmission() == 0;
}


// -----------------------------------------------------
// Inicializa MPU6050
// -----------------------------------------------------

bool iniciarMPU() {

  // Verifica se existe dispositivo em 0x68
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


// -----------------------------------------------------
// Faz uma leitura completa do MPU6050
// -----------------------------------------------------

bool lerMPU(
  float &ax,
  float &ay,
  float &az,
  float &gx,
  float &gy,
  float &gz
) {

  Wire.beginTransmission(MPU_ADDR);

  // ACCEL_XOUT_H
  Wire.write(0x3B);


  if (Wire.endTransmission(false) != 0) {
    return false;
  }


  // 14 bytes:
  // accel XYZ
  // temperatura
  // gyro XYZ

  size_t recebidos = Wire.requestFrom(
    (uint8_t)MPU_ADDR,
    (size_t)14,
    true
  );


  if (recebidos != 14) {
    return false;
  }


  // =========================
  // ACELERÔMETRO
  // =========================

  int16_t rawAX =
      ((int16_t)Wire.read() << 8) | Wire.read();

  int16_t rawAY =
      ((int16_t)Wire.read() << 8) | Wire.read();

  int16_t rawAZ =
      ((int16_t)Wire.read() << 8) | Wire.read();


  // =========================
  // TEMPERATURA
  // Não utilizada
  // =========================

  Wire.read();
  Wire.read();


  // =========================
  // GIROSCÓPIO
  // =========================

  int16_t rawGX =
      ((int16_t)Wire.read() << 8) | Wire.read();

  int16_t rawGY =
      ((int16_t)Wire.read() << 8) | Wire.read();

  int16_t rawGZ =
      ((int16_t)Wire.read() << 8) | Wire.read();


  // =========================
  // CONVERSÕES
  // =========================

  // Acelerômetro +/- 8g
  // 4096 LSB / g

  ax = (rawAX / 4096.0) * 9.80665;
  ay = (rawAY / 4096.0) * 9.80665;
  az = (rawAZ / 4096.0) * 9.80665;


  // Giroscópio +/- 500 graus/s
  // 65.5 LSB / (graus/s)

  gx = (rawGX / 65.5) * DEG_TO_RAD;
  gy = (rawGY / 65.5) * DEG_TO_RAD;
  gz = (rawGZ / 65.5) * DEG_TO_RAD;


  return true;
}


// =====================================================
// CONTROLE PS4
// =====================================================

void aoConectar(ControllerPtr ctl) {

  controle = ctl;

  xAnterior = false;
  gravando = false;

  Serial.println("CONTROLE_CONECTADO");
}


void aoDesconectar(ControllerPtr ctl) {

  if (controle == ctl) {
    controle = nullptr;
  }

  gravando = false;
  xAnterior = false;

  Serial.println("CONTROLE_DESCONECTADO");
}


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  delay(1000);


  Serial.println();
  Serial.println("=== PROJETO GESTO ===");


  // ===================================================
  // MPU6050
  //
  // SDA = GPIO21
  // SCL = GPIO22
  // ===================================================

  Wire.begin(21, 22);

  Wire.setClock(100000);


  Serial.println("Procurando MPU6050...");


  bool mpuEncontrado = false;


  // Algumas tentativas caso o sensor demore
  // um pouco para responder ao ligar

  for (int tentativa = 1; tentativa <= 5; tentativa++) {

    if (iniciarMPU()) {

      mpuEncontrado = true;

      break;
    }


    Serial.print("Tentativa MPU ");
    Serial.print(tentativa);
    Serial.println(" falhou...");

    delay(300);
  }


  if (!mpuEncontrado) {

    Serial.println("ERRO_MPU");

    while (true) {
      delay(1000);
    }
  }


  Serial.println("MPU_OK");


  // ===================================================
  // BLUEPAD32
  // ===================================================

  BP32.setup(
    &aoConectar,
    &aoDesconectar
  );


  // Não precisamos do touchpad como mouse
  BP32.enableVirtualDevice(false);


  Serial.println("Bluetooth iniciado");
  Serial.println("Aguardando controle PS4...");
  Serial.println("SISTEMA_PRONTO");
}


// =====================================================
// LOOP
// =====================================================

void loop() {

  // Atualiza o Bluetooth
  BP32.update();


  // ===================================================
  // BOTÃO X DO CONTROLE
  // ===================================================

  if (
    controle != nullptr &&
    controle->isConnected()
  ) {

    // No DualShock 4:
    // X = a() no Bluepad32

    bool xPressionado = controle->a();


    // -------------------------------------------------
    // Apertou X
    // -------------------------------------------------

    if (
      xPressionado &&
      !xAnterior
    ) {

      gravando = true;

      Serial.println("START");
    }


    // -------------------------------------------------
    // Soltou X
    // -------------------------------------------------

    if (
      !xPressionado &&
      xAnterior
    ) {

      gravando = false;

      Serial.println("STOP");
    }


    xAnterior = xPressionado;
  }


  // ===================================================
  // CAPTURA DO MPU6050
  // ===================================================

  if (
    gravando &&
    millis() - ultimaLeitura >= INTERVALO
  ) {

    ultimaLeitura = millis();


    float ax;
    float ay;
    float az;

    float gx;
    float gy;
    float gz;


    if (
      lerMPU(
        ax,
        ay,
        az,
        gx,
        gy,
        gz
      )
    ) {

      // Formato esperado pelo Python:
      //
      // DATA,AX,AY,AZ,GX,GY,GZ

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
    }

    else {

      Serial.println("ERRO_LEITURA_MPU");
    }
  }


  delay(5);
}
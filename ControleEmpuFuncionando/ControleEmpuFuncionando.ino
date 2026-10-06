#include <Bluepad32.h>
#include <Wire.h>

// ===============================
// CONTROLE PS4
// ===============================

ControllerPtr controle = nullptr;

bool xAnterior = false;
bool gravando = false;

// ===============================
// MPU6050
// ===============================

#define MPU_ADDR 0x68

unsigned long ultimaLeitura = 0;

// 20 ms = aproximadamente 50 leituras por segundo
const unsigned long INTERVALO = 20;


// ===============================
// FUNÇÕES DO MPU6050
// ===============================

void escreverMPU(uint8_t registrador, uint8_t valor) {

  Wire.beginTransmission(MPU_ADDR);

  Wire.write(registrador);
  Wire.write(valor);

  Wire.endTransmission();
}


// ===============================
// INICIAR MPU6050
// ===============================

bool iniciarMPU() {

  Wire.beginTransmission(MPU_ADDR);

  if (Wire.endTransmission() != 0) {
    return false;
  }

  // Acorda o MPU6050
  escreverMPU(0x6B, 0x00);

  delay(100);

  // Acelerômetro em +/- 8g
  escreverMPU(0x1C, 0x10);

  // Giroscópio em +/- 500 graus/s
  escreverMPU(0x1B, 0x08);

  // Filtro digital
  escreverMPU(0x1A, 0x04);

  return true;
}


// ===============================
// LER MPU6050
// ===============================

bool lerMPU(
  float &ax,
  float &ay,
  float &az,
  float &gx,
  float &gy,
  float &gz
) {

  Wire.beginTransmission(MPU_ADDR);

  // Registrador inicial:
  // ACCEL_XOUT_H
  Wire.write(0x3B);

  if (Wire.endTransmission(false) != 0) {
    return false;
  }

  // Força os tipos corretos para evitar
  // ambiguidade no core do Bluepad32
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
  // Não precisamos dela
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
  // CONVERTER ACELERAÇÃO
  // +/- 8g = 4096 LSB/g
  // =========================

  ax = (rawAX / 4096.0) * 9.80665;
  ay = (rawAY / 4096.0) * 9.80665;
  az = (rawAZ / 4096.0) * 9.80665;


  // =========================
  // CONVERTER GIROSCÓPIO
  // +/- 500 graus/s
  // 65.5 LSB/(graus/s)
  // =========================

  float gxGraus = rawGX / 65.5;
  float gyGraus = rawGY / 65.5;
  float gzGraus = rawGZ / 65.5;


  // DEG_TO_RAD já existe no ESP32.
  // Por isso usamos diretamente.

  gx = gxGraus * DEG_TO_RAD;
  gy = gyGraus * DEG_TO_RAD;
  gz = gzGraus * DEG_TO_RAD;


  return true;
}


// ===============================
// CONTROLE CONECTADO
// ===============================

void aoConectar(ControllerPtr ctl) {

  controle = ctl;

  Serial.println("CONTROLE_CONECTADO");
}


// ===============================
// CONTROLE DESCONECTADO
// ===============================

void aoDesconectar(ControllerPtr ctl) {

  if (controle == ctl) {
    controle = nullptr;
  }

  gravando = false;
  xAnterior = false;

  Serial.println("CONTROLE_DESCONECTADO");
}


// ===============================
// SETUP
// ===============================

void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("=== PROJETO GESTO ===");


  // =========================
  // MPU6050
  //
  // SDA = GPIO 21
  // SCL = GPIO 22
  // =========================

  Wire.begin(21, 22);

  Wire.setClock(100000);

  Serial.println("Procurando MPU6050...");


  if (!iniciarMPU()) {

    Serial.println("ERRO_MPU");

    while (true) {
      delay(1000);
    }
  }


  Serial.println("MPU_OK");


  // =========================
  // BLUEPAD32
  // =========================

  BP32.setup(
    &aoConectar,
    &aoDesconectar
  );

  BP32.enableVirtualDevice(false);

  Serial.println("Bluetooth iniciado");
  Serial.println("Aguardando controle PS4...");
  Serial.println("SISTEMA_PRONTO");
}


// ===============================
// LOOP
// ===============================

void loop() {

  BP32.update();


  // =========================
  // BOTÃO X DO CONTROLE
  // =========================

  if (
    controle != nullptr &&
    controle->isConnected()
  ) {

    bool xPressionado = controle->a();


    // Apertou X
    if (
      xPressionado &&
      !xAnterior
    ) {

      gravando = true;

      Serial.println("START");
    }


    // Soltou X
    if (
      !xPressionado &&
      xAnterior
    ) {

      gravando = false;

      Serial.println("STOP");
    }


    xAnterior = xPressionado;
  }


  // =========================
  // CAPTURA DO MPU
  // =========================

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

      // Formato:
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
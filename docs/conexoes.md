# Conexões usadas nos sketches

| Sinal do MPU6050 | ESP32 |
| --- | --- |
| SDA | GPIO 21 |
| SCL | GPIO 22 |
| GND | GND comum |

Os três sketches chamam `Wire.begin(21, 22)` e configuram o barramento em 100000 Hz. O endereço esperado é `0x68`, definido em `MPU_ADDR`.

A alimentação não é definida pelo código: confira os requisitos do módulo utilizado antes de ligar. O barramento deve usar níveis compatíveis com o ESP32.

O exemplo com botão usa GPIO 0 em `INPUT_PULLUP`: LOW significa pressionado e HIGH significa solto. Os exemplos com controle recebem o botão por Bluetooth, sem GPIO adicional para esse acionamento.

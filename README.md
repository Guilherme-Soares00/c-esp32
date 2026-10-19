# Projeto Gesto — ESP32 e MPU6050

Exemplos Arduino para capturar aceleração e velocidade angular de um MPU6050 com ESP32. A captura fica ativa enquanto o botão BOOT ou o botão X do controle está pressionado.

| Exemplo | Acionamento | Bibliotecas |
| --- | --- | --- |
| `Botaodiretofuincionando` | BOOT, GPIO 0 | Wire |
| `ControleEmpuFuncionando` | X do controle PS4 | Wire e Bluepad32 |
| `treinarESP32comcontrole` | X do controle PS4, com tentativas de inicialização | Wire e Bluepad32 |

Cada pasta contém um sketch `.ino` independente. Abra e grave apenas um exemplo por vez.

As amostras saem pela serial a 115200 baud, no formato `DATA,AX,AY,AZ,GX,GY,GZ`, com intervalo mínimo de 20 ms durante a captura.

Veja os guias na pasta [docs](docs) e os READMEs dos exemplos.

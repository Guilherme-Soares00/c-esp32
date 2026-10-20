# Preparação dos exemplos

1. Escolha uma pasta de exemplo e abra o `.ino` de mesmo nome no ambiente Arduino.
2. Configure o suporte à placa ESP32 utilizada e selecione a porta de gravação.
3. Para os exemplos com controle, configure um ambiente que forneça `Bluepad32.h`; apenas `Wire.h` é usado pelo exemplo BOOT.
4. Conecte o sensor conforme [conexoes.md](conexoes.md), grave o sketch e abra a serial em 115200 baud.
5. Aguarde `MPU_OK` e `SISTEMA_PRONTO` antes de iniciar a captura.

O repositório não fixa versões de bibliotecas, modelo exato de placa ou ferramentas de compilação. A compatibilidade do ambiente precisa ser validada na placa escolhida.

O código não inclui um programa Python receptor, embora um dos sketches mencione esse consumidor em um comentário.

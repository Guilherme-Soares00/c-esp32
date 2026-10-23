# Medidas e conversões

Os três sketches configuram o MPU6050 da mesma forma:

| Uso | Registrador | Valor escrito |
| --- | --- | --- |
| Acordar sensor | `0x6B` | `0x00` |
| Acelerômetro ±8 g | `0x1C` | `0x10` |
| Giroscópio ±500 graus/s | `0x1B` | `0x08` |
| Filtro digital | `0x1A` | `0x04` |

A leitura começa em `0x3B` e solicita 14 bytes: aceleração XYZ, temperatura e giroscópio XYZ. Os dois bytes de temperatura são descartados.

O código usa `aceleracao = (raw / 4096.0) * 9.80665`, emitindo AX, AY e AZ em m/s². Para GX, GY e GZ, usa `velocidade = (raw / 65.5) * DEG_TO_RAD`, emitindo rad/s.

Os eixos seguem os dados brutos do sensor: o código não faz rotação de referencial, calibração de offset ou remoção da gravidade. A aceleração enviada inclui a contribuição da gravidade.

`INTERVALO = 20` estabelece um intervalo mínimo de 20 ms entre tentativas durante a captura. Não há garantia de amostragem exata a 50 Hz.

# Captura pelo botão BOOT

Abra `Botaodiretofuincionando.ino` para usar o MPU6050 sem controle Bluetooth.

- O botão está no GPIO 0, configurado com `INPUT_PULLUP`.
- Ao pressionar, o sketch envia `START` e ativa `gravando`.
- Ao soltar, envia `STOP` e desativa a captura.
- Durante a captura, tenta ler o sensor a cada intervalo de pelo menos 20 ms.

A inicialização faz até cinco tentativas, com pausa de 300 ms após cada falha. Se nenhuma funcionar, imprime `ERRO_MPU` e permanece em espera.

A detecção das transições usa pausas de 25 ms. O loop também termina com `delay(2)`, por isso a frequência efetiva pode ficar abaixo de 50 Hz.

Consulte [o protocolo serial](../docs/protocolo-serial.md) para interpretar as amostras.

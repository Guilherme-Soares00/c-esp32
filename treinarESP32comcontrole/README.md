# Treinamento com controle

Abra `treinarESP32comcontrole.ino` para capturar dados do MPU6050 enquanto mantém X pressionado no controle PS4.

O sketch depende de Wire e Bluepad32. Ele tenta inicializar o sensor até cinco vezes e só inicia o Bluetooth depois de encontrar o MPU6050.

Na conexão do controle, `xAnterior` e `gravando` voltam a false. No loop, a transição de X solto para pressionado envia `START`; a transição inversa envia `STOP`.

A desconexão também zera esses estados e imprime `CONTROLE_DESCONECTADO`. O receptor deve tratar essa mensagem como fim de captura, pois esse callback não imprime `STOP`.

As leituras usam 14 bytes a partir de `0x3B`, descartam os dois bytes de temperatura e enviam seis valores com quatro casas decimais. Veja [protocolo-serial.md](../docs/protocolo-serial.md).

# Captura com controle e MPU6050

Abra `ControleEmpuFuncionando.ino` em um ambiente com Wire e Bluepad32.

O sketch inicializa o sensor e registra os callbacks `aoConectar` e `aoDesconectar` com `BP32.setup`. A chamada `BP32.enableVirtualDevice(false)` desativa o dispositivo virtual.

O loop chama `BP32.update()` e consulta `controle->a()`, identificado no código como o botão X do PS4. Pressionar inicia a captura com `START`; soltar encerra com `STOP`.

A desconexão limpa o ponteiro do controle correspondente e reinicia `gravando` e `xAnterior`. O callback imprime `CONTROLE_DESCONECTADO`, sem emitir `STOP`.

Esta variante faz uma tentativa de inicialização do sensor e sua função `escreverMPU` não retorna o resultado das escritas I2C. A frequência nominal de captura é 50 Hz, sujeita ao tempo do loop e da comunicação.

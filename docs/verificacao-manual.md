# Verificação manual na placa

Este roteiro descreve verificações a executar; não é um registro de testes já realizados.

1. Grave o exemplo escolhido e abra a serial em 115200 baud. Confirme `MPU_OK` e `SISTEMA_PRONTO`.
2. Com o botão solto, confirme que não saem linhas DATA.
3. Pressione BOOT ou X. Confirme uma linha START na transição e linhas DATA enquanto mantém o botão pressionado.
4. Confira sete campos por linha DATA e seis valores numéricos. Aceleração usa m/s²; giroscópio usa rad/s.
5. Solte o botão. Confirme STOP e o encerramento das amostras.
6. Repita a captura para verificar novas transições.
7. Nas variantes Bluetooth, desconecte o controle durante uma captura. Confirme CONTROLE_DESCONECTADO e a interrupção das amostras; reconecte e repita o acionamento.
8. Para verificar ausência de sensor, desligue a alimentação, desconecte o módulo e reinicie. Confirme ERRO_MPU após as tentativas previstas pela variante.

Registre o exemplo, a placa, as versões do ambiente, a ligação usada e as mensagens observadas. Sem timestamps nos pacotes, medir a cadência exige registrar o horário de recebimento no computador; isso inclui a latência da comunicação.

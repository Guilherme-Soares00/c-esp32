# Protocolo serial

Todos os exemplos usam `Serial.begin(115200)`. O fluxo mistura mensagens de estado, diagnóstico e dados. O consumidor deve classificar cada linha antes de convertê-la em números.

| Linha | Significado |
| --- | --- |
| `MPU_OK` | Inicialização do sensor concluída |
| `SISTEMA_PRONTO` | Setup concluído |
| `START` | Botão pressionado, captura iniciada |
| `STOP` | Botão solto, captura encerrada |
| `DATA,AX,AY,AZ,GX,GY,GZ` | Formato das amostras; os nomes são substituídos por valores |
| `ERRO_MPU` | Inicialização falhou; o sketch fica em espera |
| `ERRO_LEITURA_MPU` | Uma leitura falhou; nenhuma amostra é emitida nessa tentativa |
| `CONTROLE_CONECTADO` | Callback de conexão, nas variantes Bluetooth |
| `CONTROLE_DESCONECTADO` | Callback de desconexão, nas variantes Bluetooth |

Cada linha DATA tem sete campos separados por vírgula: prefixo mais seis números. Os valores são impressos com quatro casas decimais. Não há timestamp, contador nem rótulo de gesto no pacote.

As mensagens de apresentação, tentativas e espera pelo controle também aparecem na serial. Uma desconexão interrompe a captura nas variantes Bluetooth, mesmo sem uma linha STOP.

# Diagnóstico pelo comportamento do código

| Sintoma | O que verificar |
| --- | --- |
| `ERRO_MPU` no início | Endereço `0x68`, conexão SDA/GPIO21, SCL/GPIO22 e alimentação do módulo |
| `ERRO_LEITURA_MPU` durante captura | Comunicação I2C; a função falha ao não conseguir selecionar `0x3B` ou ao receber quantidade diferente de 14 bytes |
| Sem linhas DATA após `SISTEMA_PRONTO` | A captura depende de manter BOOT ou X pressionado |
| `CONTROLE_DESCONECTADO` | A captura é desativada; aguarde reconexão antes de pressionar X novamente |
| Dados com intervalos maiores que 20 ms | O agendamento depende do loop, do I2C, dos delays e da serial |

Em `ERRO_MPU`, o programa entra em um loop com `delay(1000)` e não tenta recuperar automaticamente. Após corrigir a causa, reinicie a placa para repetir o setup.

`MPU_OK` não comprova a validade de todas as configurações: algumas escritas de registradores têm retorno ignorado, e a variante ControleEmpuFuncionando não verifica retorno em `escreverMPU`.

O código não inclui calibração ou verificação automatizada em hardware. Valide primeiro os estados da serial e depois a coerência das medidas com o movimento aplicado.

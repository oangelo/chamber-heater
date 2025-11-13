# Estufa

Este projeto é um sistema automatizado para controle de temperatura e umidade em uma estufa. Ele utiliza sensores, atuadores e um controlador PID para manter as condições ideais.

## Funcionalidades
- Monitoramento de temperatura e umidade interna.
- Controle de temperatura da mesa utilizando um controlador PID.
- Controle automático de ventiladores com base em temperatura e umidade.
- Interface com display LCD para exibição de informações em tempo real.
- Configuração de setpoints e parâmetros diretamente no dispositivo, utilizando um encoder rotativo e botões.
- Emissão de dados via porta serial para monitoramento externo.

## Hardware
- **Microcontrolador**: Arduino Mega.
- **Sensores**:
  - DHT22: Sensor de temperatura e umidade.
  - Termistor: Para medição da temperatura da mesa.
- **Atuadores**:
  - Ventilador (Fan) controlado por PWM.
  - Buzzer para notificações sonoras.
- **Interface**:
  - Display LCD 20x4.
  - Encoder rotativo com botão integrado.

## Configuração de Pinos
- **Pinos do Arduino**:
  - **Pino 4**: Sensor DHT22.
  - **Pino 9**: Controle PWM do ventilador.
  - **Pino 8**: Saída do controlador PID.
  - **Pinos 16-29**: Conexão com o display LCD.
  - **Pino 37**: Buzzer.
  - **Pinos 31, 33, 35**: Encoder rotativo e botão.

## Bibliotecas Utilizadas
- `DHT.h`: Para leitura do sensor DHT22.
- `LiquidCrystal.h`: Para controle do display LCD.
- `PID_v1.h`: Para implementação do controlador PID.
- `RotaryEncoder.h`: Para leitura do encoder rotativo.
- `Pushbutton.h`: Para gerenciamento do botão do encoder.

## Estrutura do Projeto
A estrutura do projeto é organizada da seguinte forma:
```
Estufa/
├── docs/                     # Documentação (diagramas, imagens, datasheets)
├── firmware/                 # Código principal do Arduino
│   └── firmware.ino
├── hardware/                 # Arquivos e informações do hardware
├── lib/                      # Bibliotecas personalizadas ou adaptadas
├── test/                     # Scripts de teste (verificação, debug, etc.)
├── extras/                   # Scripts auxiliares (ex: leitura serial em Python)
├── .gitignore                # Arquivos e pastas ignorados pelo Git
├── README.md                 # Descrição do projeto
└── contributing.md           # (Opcional) Como contribuir com o projeto
```

## Como usar
1. Clone o repositório:
    ```bash
    git clone https://github.com/SerraRocketry/Estufa.git
    ```
2. Navegue até o diretório do projeto:
    ```bash
    cd Estufa
    ```
3. Abra o arquivo `firmware/firmware.ino` no Arduino IDE:
    ```bash
    arduino firmware/firmware.ino
    ```
    ou abra o Arduino IDE e selecione `File > Open` e escolha o arquivo `firmware.ino`.
4. Conecte o Arduino ao computador e carregue o código no microcontrolador.
5. Monte o hardware conforme descrito na seção **Configuração de Pinos**.

## 🤝 Contribuindo

Este projeto segue as **Boas Práticas do Serra Rocketry**. Antes de contribuir:

1. 📖 Leia nosso [Guia de Boas Práticas](https://github.com/Serra-Rocketry/best-practices/blob/main/README.md)
2. 🔄 Siga o fluxo: Fork → Branch → Pull Request
3. 📝 Documente suas mudanças

**Primeira vez contribuindo?** Não se preocupe! O guia explica tudo passo a passo.
## Notas Adicionais
- Certifique-se de instalar todas as bibliotecas necessárias antes de compilar o código.
- O sistema foi projetado para ser modular e facilmente adaptável a diferentes configurações de hardware.

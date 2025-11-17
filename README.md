# Chamber Heater (Estufa)

![Status](https://img.shields.io/badge/status-em%20desenvolvimento-yellow)
![Versão](https://img.shields.io/badge/versão-1.0.0-blue)
![Plataforma](https://img.shields.io/badge/plataforma-Arduino%20Mega-red)

## 📋 Sobre

Sistema automatizado para controle de temperatura e umidade em câmara térmica (estufa). Utiliza controladores PID para manter temperatura da mesa aquecida e controle automático de ventilação baseado em temperatura e umidade ambiente.

## 🚀 Quick Start

1. Clone o repositório
   ```bash
   git clone https://github.com/SerraRocketry/chamber-heater.git
   cd chamber-heater
   ```

2. Instale as bibliotecas necessárias no Arduino IDE

3. Configure o hardware conforme pinagem descrita em [hardware/README.md](./hardware/README.md)

4. Carregue `firmware/firmware.ino` no Arduino Mega

5. Configure setpoints usando o encoder rotativo e display LCD

## 📁 Estrutura do Projeto

```
chamber-heater/
├── docs/              → Documentação técnica detalhada
│   ├── hardware/      → Datasheets e esquemáticos
│   └── diagrams/      → Diagramas de fluxo e estados
├── firmware/          → Código do Arduino
│   └── firmware.ino
├── hardware/          → Arquivos de hardware e montagem
│   ├── pcb/          → Arquivos de PCB (futuros)
│   └── images/       → Fotos da montagem
├── lib/              → Bibliotecas customizadas
├── test/             → Testes e validação
│   └── fan_pid/      → Teste isolado do PID do ventilador
└── extras/           → Scripts auxiliares e versões antigas
```

## 🔧 Pré-requisitos

### Hardware
- Arduino Mega 2560 (ou compatível MKS GEN v1.4)
- DHT22 - Sensor de temperatura e umidade
- Termistor NTC 100kΩ (B=3950K)
- Display LCD 20x4 com interface paralela
- Encoder rotativo com botão integrado
- Ventilador 12V com controle PWM
- Buzzer para notificações
- Fonte de alimentação adequada

### Software
- Arduino IDE 1.8.x ou superior / PlatformIO
- Bibliotecas (ver [firmware/README.md](./firmware/README.md)):
  - DHT
  - LiquidCrystal
  - PID_v1_bc
  - RotaryEncoder
  - Pushbutton

## 📖 Documentação

- [Guia de Instalação Detalhado](./docs/INSTALACAO.md)
- [Hardware e Pinagem](./hardware/README.md)
- [Arquitetura do Firmware](./firmware/README.md)
- [Troubleshooting](./docs/TROUBLESHOOTING.md)
- [Changelog](./CHANGELOG.md)

## 📊 Status do Projeto

- [x] Leitura de sensores (DHT22 e termistor)
- [x] Controle PID da mesa aquecida
- [x] Controle automático de ventilador
- [x] Interface com LCD 20x4
- [x] Configuração via encoder rotativo
- [ ] Logging de dados em cartão SD
- [ ] Interface web para monitoramento remoto
- [ ] Gráficos de temperatura em tempo real

## 🎯 Características Principais

- **Controle PID**: Temperatura da mesa mantida com precisão usando PID
- **Monitoramento**: Display LCD mostra temperatura, umidade e setpoints em tempo real
- **Configuração fácil**: Ajuste de parâmetros via encoder rotativo sem necessidade de recompilar
- **Notificações**: Buzzer alerta sobre condições críticas
- **Serial output**: Dados transmitidos via serial para logging externo

## ✨ Autores

- [@oangelo](https://github.com/oangelo) - Desenvolvimento inicial

## 🤝 Contribuindo

Este projeto segue as **Boas Práticas do Serra Rocketry**. Antes de contribuir:

1. 📖 Leia nosso [Guia de Boas Práticas](https://github.com/Serra-Rocketry/best-practices/blob/main/README.md)
2. 🔄 Siga o fluxo: Fork → Branch → Pull Request
3. 📝 Documente suas mudanças (veja [contributing.md](./contributing.md))
4. ✅ Teste suas alterações antes de enviar

## 📄 Licença

Este projeto é open source e está disponível para a comunidade Serra Rocketry.

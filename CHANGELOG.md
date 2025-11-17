# Changelog

Todas as mudanças notáveis neste projeto serão documentadas neste arquivo.

O formato é baseado em [Keep a Changelog](https://keepachangelog.com/pt-BR/1.0.0/),
e este projeto adere ao [Semantic Versioning](https://semver.org/lang/pt-BR/).

## [1.1.0] - 2024-11-17

### Adicionado
- **LICENSE**: Arquivo MIT License adicionado à raiz do projeto
- **docs/API.md**: Documentação completa da API serial e protocolos de comunicação
- **docs/README.md**: Guia de organização da documentação
- **platformio.ini**: Configuração PlatformIO na raiz com múltiplos ambientes
- **requirements.txt**: Dependências Python para scripts de análise
- **software/data-analysis/**: Nova estrutura para análise de dados
  - `serial_logger.py`: Logger de dados serial para CSV
  - `plot_realtime.py`: Visualização em tempo real com matplotlib
  - `analyze_log.py`: Análise estatística de logs
  - `README.md`: Documentação dos scripts Python
- Estrutura `software/ground-station/` preparada para futura estação base

### Modificado
- Repositório completamente adaptado às Boas Práticas Serra Rocketry (Seções 4-6)
- Documentação expandida e melhor organizada
- Scripts Python tornados executáveis (+x)

## [1.0.0] - 2024-11-14

### Adicionado
- Reorganização completa do repositório seguindo boas práticas Serra Rocketry
- README.md principal conciso com badges e quick start
- README.md detalhado para `hardware/` com BOM completo e pinagem
- README.md detalhado para `firmware/` com arquitetura e documentação
- Documentação completa de instalação em `docs/INSTALACAO.md`
- Guia de troubleshooting em `docs/TROUBLESHOOTING.md`
- `.gitignore` expandido com padrões para Arduino, Python e IDEs
- Estrutura de diretórios organizada (hardware/, docs/, firmware/, test/)
- Movidos datasheets e esquemáticos para `docs/hardware/`
- Diretórios preparados para expansão futura (hardware/pcb/, hardware/images/)

### Modificado
- Estrutura de documentação distribuída (docs/, hardware/, firmware/)
- README principal agora tem menos de 100 linhas e serve como índice
- Documentação técnica detalhada movida para arquivos específicos

### Mantido
- Código do firmware funcional (`firmware/firmware.ino`)
- Testes em `test/fan_pid/`
- Arquivos extras em `extras/`
- Arquivo `contributing.md` original

## [0.1.0] - Data Original

### Inicial
- Sistema de controle PID para temperatura da mesa aquecida
- Controle PID para ventilador baseado em umidade
- Leitura de DHT22 (temperatura e umidade)
- Leitura de termistor NTC para temperatura da mesa
- Interface LCD 20x4 com encoder rotativo
- Menu de configuração de setpoints
- Saída de dados via Serial
- Notificações via buzzer

---

## Tipos de Mudanças

- **Adicionado**: para novas funcionalidades
- **Modificado**: para mudanças em funcionalidades existentes
- **Depreciado**: para funcionalidades que serão removidas em breve
- **Removido**: para funcionalidades removidas
- **Corrigido**: para correção de bugs
- **Segurança**: em caso de vulnerabilidades


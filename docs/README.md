# Documentação - Chamber Heater

Este diretório contém toda a documentação técnica detalhada do projeto.

## 📁 Organização

### Documentos Principais

| Arquivo | Descrição |
|---------|-----------|
| [INSTALACAO.md](./INSTALACAO.md) | Guia completo de instalação, montagem e configuração inicial |
| [API.md](./API.md) | Protocolos de comunicação serial e integração com software |
| [TROUBLESHOOTING.md](./TROUBLESHOOTING.md) | Problemas comuns e soluções |
| [CALIBRACAO.md](./CALIBRACAO.md) | ⏳ Procedimentos de calibração (futuro) |
| [TESTES.md](./TESTES.md) | ⏳ Plano detalhado de testes (futuro) |

### Diretórios

#### hardware/

Contém datasheets, esquemáticos e documentação de hardware:

```
hardware/
├── MKS GEN V1.4_004 BOM.pdf        # Bill of Materials
├── MKS GEN V1.4_004 BOTTOM.pdf     # Layout PCB inferior
├── MKS GEN V1.4_004 PIN.pdf        # Pinagem
├── MKS GEN V1.4_004 SCH.pdf        # Esquemático
├── MKS GEN V1.4_004 SIZE.pdf       # Dimensões
└── MKS GEN V1.4_004 TOP.pdf        # Layout PCB superior
```

**Quando usar:**
- Verificar pinagem do Arduino Mega / MKS GEN
- Consultar especificações elétricas
- Planejar modificações de hardware

#### diagrams/

Diagramas técnicos do sistema:

```
diagrams/
├── fluxograma.png         # ⏳ Fluxo de operação do sistema
├── state_machine.svg      # ⏳ Máquina de estados
└── arquitetura.png        # ⏳ Arquitetura geral
```

**Quando adicionar:**
- Diagramas de blocos
- Fluxogramas de processos
- Máquinas de estado
- Diagramas UML

**Formato recomendado:**
- PNG ou SVG (vetorial preferível)
- Máximo 500KB por arquivo
- Use ferramentas como: draw.io, PlantUML, Mermaid

#### meetings/

Atas de reuniões técnicas e decisões de projeto:

```
meetings/
└── 2024-11-13.md          # ⏳ Exemplo de ata
```

**Template para atas:**

```markdown
# Reunião - DD/MM/YYYY

## Participantes
- @fulano
- @ciclano

## Pauta
1. Tópico 1
2. Tópico 2

## Decisões
- [ ] Decisão 1
- [ ] Decisão 2

## Ações
- [ ] @fulano: Fazer X até DD/MM
- [ ] @ciclano: Fazer Y até DD/MM
```

## 🔍 Como Encontrar Informação

### Por Tipo de Problema

| Preciso... | Documento |
|-----------|-----------|
| Instalar o sistema do zero | [INSTALACAO.md](./INSTALACAO.md) |
| Resolver um problema | [TROUBLESHOOTING.md](./TROUBLESHOOTING.md) |
| Integrar com software externo | [API.md](./API.md) |
| Ver a pinagem | [../hardware/README.md](../hardware/README.md) |
| Entender o código | [../firmware/README.md](../firmware/README.md) |
| Fazer testes | [../test/README.md](../test/README.md) |
| Contribuir com o projeto | [../contributing.md](../contributing.md) |

### Por Componente

| Componente | Documentação |
|------------|--------------|
| DHT22 | [INSTALACAO.md](./INSTALACAO.md#passo-2-conectar-o-dht22) + [TROUBLESHOOTING.md](./TROUBLESHOOTING.md#sensor-dht22) |
| Termistor NTC | [INSTALACAO.md](./INSTALACAO.md#passo-3-conectar-o-termistor-mesa) + [../firmware/README.md](../firmware/README.md#parâmetros-dos-sensores) |
| Display LCD | [INSTALACAO.md](./INSTALACAO.md#passo-4-conectar-o-display-lcd-20x4) + [TROUBLESHOOTING.md](./TROUBLESHOOTING.md#display-lcd) |
| Encoder Rotativo | [INSTALACAO.md](./INSTALACAO.md#passo-5-conectar-o-encoder-rotativo) + [TROUBLESHOOTING.md](./TROUBLESHOOTING.md#encoder-rotativo) |
| Mesa Aquecida | [INSTALACAO.md](./INSTALACAO.md#passo-8-conectar-a-mesa-aquecida-via-mosfetssr) + [TROUBLESHOOTING.md](./TROUBLESHOOTING.md#mesa-aquecida) |
| Ventilador | [INSTALACAO.md](./INSTALACAO.md#passo-7-conectar-o-ventilador-via-mosfet) + [TROUBLESHOOTING.md](./TROUBLESHOOTING.md#ventilador) |

## 📝 Adicionando Nova Documentação

### Quando criar um novo documento?

- Quando o conteúdo ultrapassa 200 linhas
- Quando trata de um tópico específico e isolado
- Quando será referenciado frequentemente

### Onde colocar?

```
docs/
├── NOME_DO_DOCUMENTO.md      # Documentos técnicos gerais
├── hardware/                  # Datasheets, esquemáticos (PDFs)
├── diagrams/                  # Diagramas (PNG, SVG)
└── meetings/                  # Atas de reunião
```

### Checklist para novo documento

- [ ] Nome em MAIÚSCULAS.md (ex: `CALIBRACAO.md`)
- [ ] Seguir estrutura com headers (`#`, `##`, `###`)
- [ ] Adicionar ao índice deste README
- [ ] Referenciar nos outros documentos relevantes
- [ ] Adicionar ao [README principal](../README.md) se aplicável
- [ ] Usar Markdown formatado (ver exemplos existentes)

## 📐 Padrões de Documentação

### Estrutura de Documento

```markdown
# Título Principal

Parágrafo introdutório (2-3 linhas).

## 📋 Seção 1

Conteúdo...

### Subseção 1.1

Conteúdo detalhado...

## 🔧 Seção 2

Mais conteúdo...

## 📞 Suporte

Links úteis e contatos.
```

### Emojis Recomendados

Use emojis para melhorar a legibilidade:

| Emoji | Uso |
|-------|-----|
| 📋 | Listas, itens |
| 🔧 | Configuração, ferramentas |
| 📦 | Componentes, pacotes |
| 📊 | Dados, gráficos |
| ⚠️ | Avisos importantes |
| ✅ | Checklist, sucesso |
| ❌ | Erros, problemas |
| 🔍 | Busca, inspeção |
| 📝 | Notas, observações |
| 🚀 | Futuro, melhorias |
| 🐛 | Bugs, troubleshooting |
| 📞 | Contato, suporte |

### Código e Comandos

#### Blocos de código

```cpp
// Código C++ (Arduino)
void setup() {
  Serial.begin(115200);
}
```

```bash
# Comandos shell
cd chamber-heater
pio run
```

```python
# Python
import serial
ser = serial.Serial('/dev/ttyUSB0', 115200)
```

#### Código inline

Use `código inline` para nomes de funções, arquivos, comandos curtos.

### Links

#### Links internos (relativos)

```markdown
[Texto](./INSTALACAO.md)
[Seção específica](./API.md#comunicação-serial)
[Outro diretório](../firmware/README.md)
```

#### Links externos

```markdown
[Arduino](https://www.arduino.cc/)
[Datasheet DHT22](https://www.sparkfun.com/datasheets/Sensors/Temperature/DHT22.pdf)
```

### Tabelas

```markdown
| Coluna 1 | Coluna 2 | Coluna 3 |
|----------|----------|----------|
| Valor 1  | Valor 2  | Valor 3  |
```

### Imagens

```markdown
![Descrição da imagem](./diagrams/exemplo.png)
```

**Atenção:**
- Comprimir imagens (< 500KB)
- Usar PNG para screenshots, SVG para diagramas
- Salvar em `diagrams/` ou `../hardware/images/`

## 🔄 Manutenção

### Quando atualizar documentação?

- ✅ Ao adicionar nova funcionalidade
- ✅ Ao corrigir bug que afeta uso
- ✅ Ao mudar pinagem ou hardware
- ✅ Ao receber feedback de usuários
- ✅ Ao descobrir problema comum (adicionar em TROUBLESHOOTING)

### Revisão Periódica

- **Mensal**: Verificar se documentos estão atualizados
- **A cada release**: Atualizar versões e datas
- **Após testes**: Adicionar findings em TROUBLESHOOTING

## 📚 Documentação Externa

### Componentes

- [Arduino Mega 2560](https://docs.arduino.cc/hardware/mega-2560)
- [DHT22 Datasheet](https://www.sparkfun.com/datasheets/Sensors/Temperature/DHT22.pdf)
- [HD44780 LCD Datasheet](https://www.sparkfun.com/datasheets/LCD/HD44780.pdf)

### Bibliotecas

- [DHT Library](https://github.com/adafruit/DHT-sensor-library)
- [PID Library](https://github.com/br3ttb/Arduino-PID-Library)
- [RotaryEncoder](https://github.com/mathertel/RotaryEncoder)

### Tutoriais

- [PID Control Tutorial](https://playground.arduino.cc/Code/PIDLibrary/)
- [Thermistor Tutorial](https://learn.adafruit.com/thermistor/using-a-thermistor)

## 🤝 Contribuindo

Ao melhorar a documentação:

1. Siga os padrões deste guia
2. Teste os comandos/instruções
3. Adicione exemplos quando possível
4. Peça review de outro membro
5. Atualize CHANGELOG.md se relevante

Ver também: [contributing.md](../contributing.md)

---

**Última atualização**: 2024-11-14  
**Mantenedores**: [@oangelo](https://github.com/oangelo)


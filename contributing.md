# Contribuindo com Chamber Heater

Obrigado por considerar contribuir com este projeto! Este projeto segue as **Boas Práticas do Serra Rocketry**.

## 📖 Antes de Começar

1. Leia o [Guia de Boas Práticas](https://github.com/Serra-Rocketry/best-practices/blob/main/README.md)
2. Familiarize-se com a estrutura do projeto (ver [README.md](./README.md))
3. Verifique as [issues abertas](../../issues) para ver se já existe discussão sobre sua ideia

## 🔄 Fluxo de Trabalho

### 1. Fork e Clone

```bash
# Fork no GitHub (clique no botão "Fork")
git clone https://github.com/SEU_USUARIO/chamber-heater.git
cd chamber-heater
```

### 2. Crie uma Branch

Use nomes descritivos para branches:

```bash
# ✅ BOM
git checkout -b fix/corrige-leitura-dht22
git checkout -b feature/adiciona-logging-sd
git checkout -b docs/atualiza-pinagem

# ❌ RUIM
git checkout -b minha-branch
git checkout -b ajustes
```

### 3. Faça suas Alterações

- Mantenha commits pequenos e focados
- Teste suas mudanças antes de commitar
- Documente código novo

### 4. Commits de Qualidade

```bash
# ❌ RUIM
git commit -m "ajustes"
git commit -m "correções"

# ✅ BOM
git commit -m "Corrige leitura do DHT22 em temperaturas extremas"
git commit -m "Adiciona filtro de média móvel para termistor"
git commit -m "Atualiza documentação de pinagem do LCD"
```

**Formato recomendado**:
```
Tipo: Descrição curta (max 50 caracteres)

Descrição detalhada do que foi feito e por quê.
Pode ter múltiplas linhas.

- Lista de mudanças
- Se necessário

Refs: #123 (se relacionado a uma issue)
```

**Tipos de commit**:
- `fix:` - Correção de bug
- `feat:` - Nova funcionalidade
- `docs:` - Mudança em documentação
- `style:` - Formatação, espaços (sem mudança de lógica)
- `refactor:` - Refatoração de código
- `test:` - Adiciona ou corrige testes
- `chore:` - Manutenção geral

### 5. Push e Pull Request

```bash
git push origin fix/corrige-leitura-dht22
```

Então abra um Pull Request no GitHub com:

## 📝 Template de Pull Request

```markdown
## O quê
Descrição clara das mudanças realizadas.

## Por quê
Qual problema isso resolve? Qual melhoria traz?

## Como testar
1. Passo a passo para testar
2. Hardware necessário
3. Resultado esperado

## Breaking changes?
- [ ] Sim (descreva o que pode quebrar)
- [x] Não

## Checklist
- [ ] Código testado
- [ ] Documentação atualizada
- [ ] Sem warnings de compilação
- [ ] CHANGELOG.md atualizado (se relevante)
```

## 📋 Diretrizes de Código

### Arduino/C++

```cpp
// ✅ BOM - Código documentado e claro

// Converte leitura ADC para temperatura usando equação Steinhart-Hart
// RT0: 100kΩ @ 25°C, B: 3950K
float readBedTemp() {
  float VRT = analogRead(BED_INPUT_PIN);
  VRT = (5.00 / 1023.00) * VRT;  // ADC → Tensão
  
  float VR = VCC - VRT;
  float RT = VRT / (VR / R);
  
  // Steinhart-Hart simplificada
  float ln = log(RT / RT0);
  float T0 = 25 + 273.15;  // K
  float TX = (1 / ((ln / B) + (1 / T0)));
  
  return TX - 273.15;  // K → °C
}

// ❌ RUIM - Sem comentários, variáveis obscuras
float readBedTemp() {
  float x = analogRead(14);
  x = (5.00 / 1023.00) * x;
  float y = 5 - x;
  float z = x / (y / 4700);
  return (1 / ((log(z / 100000) / 3950) + (1 / 298.15))) - 273.15;
}
```

### Constantes e Definições

```cpp
// ✅ BOM - Valores significativos e documentados
#define THERMISTOR_RT0 100000  // Resistência a 25°C (Ω)
#define THERMISTOR_B 3950      // Constante B (K)
const float ADC_TO_VOLTAGE = 5.0 / 1023.0;

// ❌ RUIM - Magic numbers
float temp = analogRead(14) * 0.48828;
```

## 📚 Documentação

### Quando atualizar documentação:

- Nova funcionalidade → Atualizar README relevante
- Mudança de pinagem → Atualizar `hardware/README.md`
- Novo comando → Atualizar `firmware/README.md`
- Problema resolvido → Adicionar em `docs/TROUBLESHOOTING.md`
- Mudança significativa → Adicionar em `CHANGELOG.md`

### Formato de Documentação

Use Markdown e siga o estilo existente:
- Títulos claros e hierárquicos
- Exemplos de código quando relevante
- Imagens comprimidas (< 500KB)
- Links relativos para outros documentos

## 🧪 Testes

Antes de enviar PR:

- [ ] Código compila sem erros/warnings
- [ ] Testado em hardware real (se possível)
- [ ] Não quebra funcionalidades existentes
- [ ] Documentação atualizada

Para adicionar testes:
- Criar novo teste em `test/nome_do_teste/`
- Documentar em `test/README.md`
- Incluir critérios de aceitação

## 🚫 O que NÃO fazer

- ❌ Commitar arquivos grandes (logs, CSVs, binários)
- ❌ Commitar `config.h` ou configurações pessoais
- ❌ Fazer commits gigantes com múltiplas mudanças não relacionadas
- ❌ Deixar código comentado "para depois"
- ❌ Ignorar warnings do compilador
- ❌ Esquecer de testar antes de commitar

## 🎯 Áreas que Precisam de Ajuda

Procurando por onde começar? Veja issues com labels:
- `good-first-issue` - Bom para iniciantes
- `help-wanted` - Precisamos de ajuda
- `documentation` - Melhorias na documentação
- `enhancement` - Novas funcionalidades

## 💬 Comunicação

- **Issues**: Para bugs, ideias, discussões
- **Pull Requests**: Para contribuições de código
- **Discussions**: Para perguntas gerais

## 🙏 Reconhecimento

Todos os contribuidores serão creditados no README principal e no CHANGELOG.

## 📄 Licença

Ao contribuir, você concorda que suas contribuições serão licenciadas sob a mesma licença do projeto.

---

**Dúvidas?** Abra uma issue ou entre em contato com os mantenedores!

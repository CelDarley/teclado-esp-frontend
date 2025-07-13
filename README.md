# 🌐 Sistema de Controle de Acesso - Frontend & Firmware

Este projeto contém:
- **Frontend Vue 3** para gerenciamento de usuários, logs e configurações
- **Firmware ESP32** para controle do teclado matricial e fechadura

## Estrutura
```
teclado-esp-frontend/
├── frontend/   # Aplicação Vue 3
├── firmware/   # Códigos para ESP32
```

## Frontend (Vue 3)

### Instalação
```bash
cd frontend
npm install
npm run dev
```

- Acesse em: http://localhost:5173 ou http://SEU_IP:5173

### Configuração de API
- O frontend espera o backend rodando em `http://192.168.0.118:8000`
- Altere a URL da API nos arquivos de serviço se necessário

## Firmware (ESP32)

- Abra a pasta `firmware/` na Arduino IDE
- Edite o arquivo `sistema_final_integrado.ino`:
  - Configure `WIFI_SSID`, `WIFI_PASSWORD` e `SERVER_URL`
- Faça upload para o ESP32

## Integração
- O frontend consome a API do backend para autenticação, logs e usuários
- O firmware envia PINs para o backend e aciona o relé/LED/buzzer

## Observações
- Para acessar o frontend de outros dispositivos, rode com `--host` ou configure o Vite para `host: '0.0.0.0'`.
- O firmware e o frontend podem ser usados separadamente ou juntos. 
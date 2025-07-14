# Sistema de Controle de Acesso - Frontend

Este projeto é um frontend para sistema de controle de acesso multidispositivo, desenvolvido com Vue 3, TypeScript e Vite.

## 🚀 Desenvolvimento Local

### Instalação
```bash
npm install
```

### Executar em desenvolvimento
```bash
npm run dev
```

Acesse em: http://localhost:5173

### Build para produção
```bash
npm run build
```

O build será gerado na pasta `dist/`.

## 📦 Deploy no Servidor

### Pré-requisitos
- Acesso SSH ao servidor (10.102.0.108)
- Usuário `darley` no servidor
- Python 3 instalado no servidor

### Passo a Passo do Deploy

#### 1. Build Local
```bash
# No computador local
npm run build
```

#### 2. Enviar arquivos para o servidor
```bash
# Enviar arquivos buildados para o servidor
scp -P 22 -r dist/* darley@10.102.0.108:~/teclado-esp-frontend/
```

#### 3. Configurar servidor web no servidor

**Opção A: Servidor Python (Recomendado)**
```bash
# Conectar ao servidor
ssh darley@10.102.0.108

# Navegar para o diretório do frontend
cd ~/teclado-esp-frontend

# Executar servidor web Python
python3 server_web.py
```

O servidor estará disponível em: `http://10.102.0.108:8080`

**Opção B: Nginx (Alternativo)**
```bash
# No servidor, copiar arquivos para o diretório do Nginx
sudo cp -r ~/teclado-esp-frontend/* /var/www/html/
sudo chown -R www-data:www-data /var/www/html
sudo chmod -R 755 /var/www/html
```

O frontend estará disponível em: `http://10.102.0.108`

### 4. Configurar Backend Django

**Verificar se o backend está rodando:**
```bash
# No servidor
cd ~/teclado-esp-backend
source venv/bin/activate
python3 manage.py runserver 0.0.0.0:8191
```

**URL da API:** `http://10.102.0.108:8191/api/`

## 🔧 Configurações Importantes

### Variáveis de Ambiente
O frontend está configurado para acessar a API em:
```javascript
const API_BASE_URL = 'http://10.102.0.108:8191/api'
```

### Estrutura de Arquivos no Servidor
```
/home/darley/teclado-esp-frontend/
├── index.html
├── vite.svg
├── assets/
│   ├── index-*.js
│   └── index-*.css
└── server_web.py
```

## 🛠️ Scripts Úteis

### Deploy Completo (Local → Servidor)
```bash
# Build
npm run build

# Enviar para servidor
scp -P 22 -r dist/* darley@10.102.0.108:~/teclado-esp-frontend/

# Reiniciar servidor web (no servidor)
ssh darley@10.102.0.108 "cd ~/teclado-esp-frontend && pkill -f server_web.py && nohup python3 server_web.py > server.log 2>&1 &"
```

### Verificar Status dos Serviços
```bash
# Verificar se o frontend está rodando
ssh darley@10.102.0.108 "netstat -tlnp | grep 8080"

# Verificar se o backend está rodando
ssh darley@10.102.0.108 "netstat -tlnp | grep 8191"
```

## 📱 Funcionalidades

- ✅ **Login com usuário e senha**
- ✅ **Seleção de dispositivos**
- ✅ **Gestão de usuários por dispositivo**
- ✅ **Histórico de acessos**
- ✅ **Configurações do sistema**
- ✅ **Interface responsiva**

## 🔗 URLs de Acesso

- **Frontend:** `http://10.102.0.108:8080`
- **API Backend:** `http://10.102.0.108:8191/api/`
- **Status da API:** `http://10.102.0.108:8191/api/status/`

## 🐛 Troubleshooting

### Problema: Frontend não carrega
```bash
# Verificar se o servidor está rodando
ssh darley@10.102.0.108 "ps aux | grep server_web"

# Reiniciar servidor
ssh darley@10.102.0.108 "cd ~/teclado-esp-frontend && python3 server_web.py"
```

### Problema: API não responde
```bash
# Verificar se o Django está rodando
ssh darley@10.102.0.108 "cd ~/teclado-esp-backend && source venv/bin/activate && python3 manage.py runserver 0.0.0.0:8191"
```

### Problema: Usuários não aparecem
```bash
# Associar usuários ao device correto
ssh darley@10.102.0.108 "cd ~/teclado-esp-backend && source venv/bin/activate && python3 manage.py shell -c \"from api.models import User; User.objects.update(device=1)\""
```

## 📝 Notas de Desenvolvimento

- O frontend usa Vue 3 com Composition API
- TypeScript para type safety
- Vite para build e desenvolvimento
- API REST com Django no backend
- Sistema multidispositivo com seleção de devices

<script setup lang="ts">
import { ref, onMounted, computed } from 'vue'

// State
const isLoggedIn = ref(false)
const loginForm = ref({
  username: '',
  password: ''
})
const message = ref('')
const messageType = ref('info')
const activeTab = ref('devices')
const showUserForm = ref(false)
const showDeviceForm = ref(false)
const selectedDevice = ref<Device | null>(null)

// Types
interface Device {
  id: number
  name: string
  ip_address: string
  is_active: boolean
}

interface User {
  id: number
  first_name: string
  last_name: string
  username: string
  pin: string
  is_active_user: boolean
  device: number
}

interface AccessLog {
  id: number
  user_name: string
  access_time: string
  ip_address?: string
  success: boolean
  device: number
}

interface SystemStatus {
  total_users: number
  recent_accesses: number
  system_status: string
}

// Data
const devices = ref<Device[]>([])
const users = ref<User[]>([])
const accessLogs = ref<AccessLog[]>([])
const systemStatus = ref<SystemStatus>({
  total_users: 0,
  recent_accesses: 0,
  system_status: 'offline'
})
const config = ref({
  admin_pin: '8729',
  door_open_duration: 5
})
const newUser = ref({
  first_name: '',
  last_name: '',
  username: '',
  pin: '',
  password: ''
})
const newDevice = ref({
  name: '',
  ip_address: ''
})

const API_BASE_URL = 'http://10.102.0.108:8191/api'

// Methods
const showMessage = (msg: string, type: 'success' | 'error' | 'info' = 'info') => {
  message.value = msg
  messageType.value = type
  setTimeout(() => {
    message.value = ''
  }, 5000)
}

// Verificar se está logado ao carregar a página
const checkAuthStatus = async () => {
  try {
    // Primeiro verificar localStorage
    const savedLogin = localStorage.getItem('isLoggedIn')
    const savedUserType = localStorage.getItem('userType')
    
    if (savedLogin === 'true' && savedUserType) {
      // Verificar com o servidor se ainda está válido
      const response = await fetch(`${API_BASE_URL}/check-auth/`)
      const data = await response.json()
      
      if (data.success) {
        isLoggedIn.value = true
        selectedDevice.value = null  // Reset selected device
        loadDevices()
        return true
      } else {
        // Token inválido, limpar localStorage
        localStorage.removeItem('isLoggedIn')
        localStorage.removeItem('userType')
        isLoggedIn.value = false
        return false
      }
    } else {
      isLoggedIn.value = false
      return false
    }
  } catch (error) {
    console.error('Erro ao verificar autenticação:', error)
    // Em caso de erro, limpar localStorage e ir para login
    localStorage.removeItem('isLoggedIn')
    localStorage.removeItem('userType')
    isLoggedIn.value = false
    return false
  }
}

const handleLogin = async () => {
  if (!loginForm.value.username || !loginForm.value.password) {
    showMessage('Usuário e senha são obrigatórios', 'error')
    return
  }
  
  await login()
}

const login = async () => {
  try {
    const response = await fetch(`${API_BASE_URL}/login/`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify({
        username: loginForm.value.username,
        password: loginForm.value.password
      })
    })
    
    const data = await response.json()
    
    if (data.success) {
      isLoggedIn.value = true
      selectedDevice.value = null  // Reset selected device
      
      // Salvar no localStorage
      localStorage.setItem('isLoggedIn', 'true')
      localStorage.setItem('userType', data.user_type || 'user')
      localStorage.setItem('username', loginForm.value.username)
      
      loginForm.value = { username: '', password: '' }
      showMessage('Login realizado com sucesso!', 'success')
      loadDevices()
    } else {
      showMessage(data.message, 'error')
      loginForm.value.password = ''
    }
  } catch (error) {
    showMessage('Erro de conexão', 'error')
    loginForm.value.password = ''
  }
}

const logout = () => {
  isLoggedIn.value = false
  loginForm.value = { username: '', password: '' }
  selectedDevice.value = null
  
  // Limpar localStorage
  localStorage.removeItem('isLoggedIn')
  localStorage.removeItem('userType')
  localStorage.removeItem('username')
  
  showMessage('Logout realizado com sucesso!', 'success')
}

const backToDevices = () => {
  selectedDevice.value = null
  activeTab.value = 'devices'
  showMessage('Voltando para lista de dispositivos', 'info')
}

const loadDevices = async () => {
  try {
    const response = await fetch(`${API_BASE_URL}/devices/`)
    devices.value = await response.json()
  } catch (error) {
    console.error('Erro ao carregar dispositivos:', error)
    showMessage('Erro ao carregar dispositivos', 'error')
  }
}

const selectDevice = async (device: Device) => {
  selectedDevice.value = device
  activeTab.value = 'users'
  await loadData()
}

const loadData = async () => {
  if (!selectedDevice.value) return
  
  try {
    // Carregar usuários do dispositivo selecionado
    const usersResponse = await fetch(`${API_BASE_URL}/users/?device_id=${selectedDevice.value.id}`)
    users.value = await usersResponse.json()
    
    // Carregar logs do dispositivo selecionado
    const logsResponse = await fetch(`${API_BASE_URL}/logs/?device_id=${selectedDevice.value.id}`)
    accessLogs.value = await logsResponse.json()
    
    // Carregar status (por enquanto mantém o mesmo)
    const statusResponse = await fetch(`${API_BASE_URL}/system-status/`)
    systemStatus.value = await statusResponse.json()
  } catch (error) {
    console.error('Erro ao carregar dados:', error)
    showMessage('Erro ao carregar dados do dispositivo', 'error')
  }
}

const createDevice = async () => {
  try {
    const response = await fetch(`${API_BASE_URL}/devices/create/`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(newDevice.value)
    })
    
    const data = await response.json()
    
    if (data.success) {
      showMessage('Dispositivo criado com sucesso!', 'success')
      showDeviceForm.value = false
      newDevice.value = {
        name: '',
        ip_address: ''
      }
      loadDevices()
    } else {
      showMessage(data.message || 'Erro ao criar dispositivo', 'error')
    }
  } catch (error) {
    showMessage('Erro de conexão', 'error')
  }
}

const deleteDevice = async (deviceId: number) => {
  if (!confirm('Tem certeza que deseja excluir este dispositivo? Todos os usuários e logs associados serão perdidos.')) {
    return
  }
  
  try {
    const response = await fetch(`${API_BASE_URL}/devices/${deviceId}/delete/`, {
      method: 'DELETE',
      headers: {
        'Content-Type': 'application/json'
      }
    })
    
    const data = await response.json()
    
    if (data.success) {
      showMessage('Dispositivo excluído com sucesso!', 'success')
      if (selectedDevice.value?.id === deviceId) {
        selectedDevice.value = null
        activeTab.value = 'devices'
      }
      loadDevices()
    } else {
      showMessage(data.message || 'Erro ao excluir dispositivo', 'error')
    }
  } catch (error) {
    showMessage('Erro de conexão', 'error')
  }
}

const createUser = async () => {
  if (!selectedDevice.value) {
    showMessage('Selecione um dispositivo primeiro', 'error')
    return
  }
  
  try {
    const userData = {
      ...newUser.value,
      device: selectedDevice.value.id
    }
    
    const response = await fetch(`${API_BASE_URL}/users/create/`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(userData)
    })
    
    const data = await response.json()
    
    if (data.success) {
      showMessage('Usuário criado com sucesso!', 'success')
      showUserForm.value = false
      newUser.value = {
        first_name: '',
        last_name: '',
        username: '',
        pin: '',
        password: ''
      }
      loadData()
    } else {
      showMessage(data.message || 'Erro ao criar usuário', 'error')
    }
  } catch (error) {
    showMessage('Erro de conexão', 'error')
  }
}

const deleteUser = async (userId: number) => {
  if (!confirm('Tem certeza que deseja excluir este usuário?')) {
    return
  }
  
  try {
    const response = await fetch(`${API_BASE_URL}/users/${userId}/delete/`, {
      method: 'DELETE',
      headers: {
        'Content-Type': 'application/json'
      }
    })
    
    const data = await response.json()
    
    if (data.success) {
      showMessage('Usuário excluído com sucesso!', 'success')
      loadData()
    } else {
      showMessage(data.message || 'Erro ao excluir usuário', 'error')
    }
  } catch (error) {
    showMessage('Erro de conexão', 'error')
  }
}

const saveConfig = async () => {
  showMessage('Configurações salvas!', 'success')
}

const formatDate = (dateString: string) => {
  return new Date(dateString).toLocaleString('pt-BR')
}

onMounted(async () => {
  // Verificar se está logado ao carregar a página
  await checkAuthStatus()
})
</script>

<template>
  <div id="app">
    <!-- Login Screen -->
    <div v-if="!isLoggedIn" class="login-container">
      <div class="login-card">
        <h1>🔐 Controle de Acesso</h1>
        <p>Faça login com usuário e senha</p>
        
        <div class="login-form">
          <div class="form-group">
            <label>Usuário:</label>
            <input 
              v-model="loginForm.username" 
              type="text" 
              placeholder="Digite seu usuário"
              @keyup.enter="handleLogin"
            />
          </div>
          
          <div class="form-group">
            <label>Senha:</label>
            <input 
              v-model="loginForm.password" 
              type="password" 
              placeholder="Digite sua senha"
              @keyup.enter="handleLogin"
            />
          </div>
          
          <button @click="handleLogin" class="login-btn">
            Entrar
          </button>
        </div>
        
        <div class="message" v-if="message">
          <div class="message-content" :class="messageType">
            {{ message }}
          </div>
        </div>
      </div>
    </div>

    <!-- Main Dashboard -->
    <div v-else class="dashboard">
      <header class="dashboard-header">
        <h1>🏢 Sistema de Controle de Acesso</h1>
        <div class="user-info">
          <span v-if="selectedDevice">Dispositivo: {{ selectedDevice.name }}</span>
          <span>Administrador</span>
          <button v-if="selectedDevice" @click="backToDevices" class="back-btn">← Voltar aos Dispositivos</button>
          <button @click="logout" class="logout-btn">Sair</button>
        </div>
      </header>

      <main class="dashboard-main">
        <!-- Device Selection -->
        <div v-if="!selectedDevice" class="device-selection">
          <h2>Selecione um Dispositivo</h2>
          <div class="devices-grid">
            <div v-for="device in devices" :key="device.id" class="device-card" @click="selectDevice(device)">
              <h3>{{ device.name }}</h3>
              <p>IP: {{ device.ip_address }}</p>
              <div class="device-status" :class="{ active: device.is_active }">
                {{ device.is_active ? '🟢 Ativo' : '🔴 Inativo' }}
              </div>
            </div>
            <div class="device-card add-device" @click="showDeviceForm = true">
              <h3>+ Novo Dispositivo</h3>
              <p>Adicionar novo dispositivo</p>
            </div>
          </div>
        </div>

        <!-- Device Management -->
        <div v-else>
          <!-- Status Cards -->
          <div class="status-grid">
            <div class="status-card">
              <h3>👥 Usuários Ativos</h3>
              <div class="status-value">{{ systemStatus.total_users || 0 }}</div>
            </div>
            
            <div class="status-card">
              <h3>📊 Acessos Hoje</h3>
              <div class="status-value">{{ systemStatus.recent_accesses || 0 }}</div>
            </div>
            
            <div class="status-card">
              <h3>🔧 Status Sistema</h3>
              <div class="status-value" :class="{ 'online': systemStatus.system_status === 'online' }">
                {{ systemStatus.system_status === 'online' ? '🟢 Online' : '🔴 Offline' }}
              </div>
            </div>
          </div>

          <!-- Navigation Tabs -->
          <div class="nav-tabs">
            <button 
              @click="activeTab = 'users'"
              :class="{ active: activeTab === 'users' }"
              class="tab-btn"
            >
              👥 Usuários
            </button>
            <button 
              @click="activeTab = 'logs'"
              :class="{ active: activeTab === 'logs' }"
              class="tab-btn"
            >
              📋 Histórico
            </button>
            <button 
              @click="activeTab = 'config'"
              :class="{ active: activeTab === 'config' }"
              class="tab-btn"
            >
              ⚙️ Configurações
            </button>
            <button 
              @click="activeTab = 'devices'"
              :class="{ active: activeTab === 'devices' }"
              class="tab-btn"
            >
              🖥️ Dispositivos
            </button>
          </div>

          <!-- Users Tab -->
          <div v-if="activeTab === 'users'" class="tab-content">
            <div class="section-header">
              <h2>Gestão de Usuários - {{ selectedDevice?.name }}</h2>
              <button @click="showUserForm = true" class="add-btn">+ Novo Usuário</button>
            </div>
            
            <div class="users-list">
              <div v-for="user in users" :key="user.id" class="user-card">
                <div class="user-info">
                  <h4>{{ user.first_name }} {{ user.last_name }}</h4>
                  <p>PIN: {{ user.pin }}</p>
                  <p>Usuário: {{ user.username }}</p>
                </div>
                <div class="user-actions">
                  <div class="user-status" :class="{ active: user.is_active_user }">
                    {{ user.is_active_user ? 'Ativo' : 'Inativo' }}
                  </div>
                  <button 
                    @click="deleteUser(user.id)" 
                    class="delete-btn"
                    title="Excluir usuário"
                  >
                    🗑️
                  </button>
                </div>
              </div>
            </div>
          </div>

          <!-- Logs Tab -->
          <div v-if="activeTab === 'logs'" class="tab-content">
            <h2>Histórico de Acessos - {{ selectedDevice?.name }}</h2>
            <div class="logs-list">
              <div v-for="log in accessLogs" :key="log.id" class="log-card" :class="{ success: log.success }">
                <div class="log-info">
                  <h4>{{ log.user_name || 'Usuário Desconhecido' }}</h4>
                  <p>{{ formatDate(log.access_time) }}</p>
                  <p v-if="log.ip_address">IP: {{ log.ip_address }}</p>
                </div>
                <div class="log-status">
                  {{ log.success ? '✅ Acesso' : '❌ Negado' }}
                </div>
              </div>
            </div>
          </div>

          <!-- Config Tab -->
          <div v-if="activeTab === 'config'" class="tab-content">
            <h2>Configurações do Sistema - {{ selectedDevice?.name }}</h2>
            <div class="config-form">
              <div class="form-group">
                <label>PIN do Administrador:</label>
                <input v-model="config.admin_pin" type="text" maxlength="4" pattern="[0-9]{4}" />
              </div>
              <div class="form-group">
                <label>Duração da Abertura (segundos):</label>
                <input v-model="config.door_open_duration" type="number" min="1" max="30" />
              </div>
              <button @click="saveConfig" class="save-btn">Salvar Configurações</button>
            </div>
          </div>

          <!-- Devices Tab -->
          <div v-if="activeTab === 'devices'" class="tab-content">
            <div class="section-header">
              <h2>Gestão de Dispositivos</h2>
              <button @click="showDeviceForm = true" class="add-btn">+ Novo Dispositivo</button>
            </div>
            
            <div class="devices-list">
              <div v-for="device in devices" :key="device.id" class="device-item">
                <div class="device-info">
                  <h4>{{ device.name }}</h4>
                  <p>IP: {{ device.ip_address }}</p>
                  <div class="device-status" :class="{ active: device.is_active }">
                    {{ device.is_active ? '🟢 Ativo' : '🔴 Inativo' }}
                  </div>
                </div>
                <div class="device-actions">
                  <button 
                    @click="selectDevice(device)" 
                    class="select-btn"
                    :class="{ active: selectedDevice?.id === device.id }"
                  >
                    {{ selectedDevice?.id === device.id ? 'Selecionado' : 'Selecionar' }}
                  </button>
                  <button 
                    @click="deleteDevice(device.id)" 
                    class="delete-btn"
                    title="Excluir dispositivo"
                  >
                    🗑️
                  </button>
                </div>
              </div>
            </div>
          </div>
        </div>
      </main>

      <!-- User Form Modal -->
      <div v-if="showUserForm" class="modal-overlay" @click="showUserForm = false">
        <div class="modal" @click.stop>
          <h3>Novo Usuário - {{ selectedDevice?.name }}</h3>
          <form @submit.prevent="createUser">
            <div class="form-group">
              <label>Nome:</label>
              <input v-model="newUser.first_name" type="text" required />
            </div>
            <div class="form-group">
              <label>Sobrenome:</label>
              <input v-model="newUser.last_name" type="text" required />
            </div>
            <div class="form-group">
              <label>Usuário:</label>
              <input v-model="newUser.username" type="text" required />
            </div>
            <div class="form-group">
              <label>PIN (4 dígitos):</label>
              <input v-model="newUser.pin" type="text" maxlength="4" pattern="[0-9]{4}" required />
            </div>
            <div class="form-group">
              <label>Senha:</label>
              <input v-model="newUser.password" type="password" required />
            </div>
            <div class="form-actions">
              <button type="button" @click="showUserForm = false" class="cancel-btn">Cancelar</button>
              <button type="submit" class="submit-btn">Criar Usuário</button>
            </div>
          </form>
        </div>
      </div>

      <!-- Device Form Modal -->
      <div v-if="showDeviceForm" class="modal-overlay" @click="showDeviceForm = false">
        <div class="modal" @click.stop>
          <h3>Novo Dispositivo</h3>
          <form @submit.prevent="createDevice">
            <div class="form-group">
              <label>Nome do Dispositivo:</label>
              <input v-model="newDevice.name" type="text" required />
            </div>
            <div class="form-group">
              <label>Endereço IP:</label>
              <input v-model="newDevice.ip_address" type="text" required placeholder="192.168.1.100" />
            </div>
            <div class="form-actions">
              <button type="button" @click="showDeviceForm = false" class="cancel-btn">Cancelar</button>
              <button type="submit" class="submit-btn">Criar Dispositivo</button>
            </div>
          </form>
        </div>
      </div>
    </div>

    <!-- Global Messages -->
    <div class="message" v-if="message">
      <div class="message-content" :class="messageType">
        {{ message }}
      </div>
    </div>
  </div>
</template>

<style scoped>
#app {
  min-height: 100vh;
  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
}

/* Login Screen */
.login-container {
  min-height: 100vh;
  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 2rem;
}

.login-card {
  background: white;
  border-radius: 16px;
  padding: 3rem;
  box-shadow: 0 20px 40px rgba(0, 0, 0, 0.1);
  text-align: center;
  max-width: 400px;
  width: 100%;
}

.login-card h1 {
  margin: 0 0 1rem 0;
  color: #333;
  font-size: 2rem;
}

.login-card p {
  color: #666;
  margin-bottom: 2rem;
}

.login-form {
  margin-bottom: 2rem;
}

.login-form .form-group {
  margin-bottom: 1.5rem;
  text-align: left;
}

.login-form label {
  display: block;
  margin-bottom: 0.5rem;
  font-weight: 500;
  color: #333;
}

.login-form input {
  width: 100%;
  padding: 0.75rem;
  border: 1px solid #ddd;
  border-radius: 6px;
  font-size: 1rem;
  box-sizing: border-box;
}

.login-form input:focus {
  outline: none;
  border-color: #007bff;
  box-shadow: 0 0 0 2px rgba(0, 123, 255, 0.25);
}

.login-btn {
  width: 100%;
  background: #007bff;
  color: white;
  border: none;
  padding: 0.75rem;
  border-radius: 6px;
  font-size: 1rem;
  font-weight: 500;
  cursor: pointer;
  transition: background-color 0.2s;
}

.login-btn:hover {
  background: #0056b3;
}

/* Dashboard */
.dashboard {
  min-height: 100vh;
  background: #f8f9fa;
}

.dashboard-header {
  background: white;
  padding: 1.5rem 2rem;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.dashboard-header h1 {
  margin: 0;
  color: #333;
  font-size: 1.8rem;
}

.user-info {
  display: flex;
  align-items: center;
  gap: 1rem;
}

.back-btn {
  background: #6c757d;
  color: white;
  border: none;
  padding: 0.5rem 1rem;
  border-radius: 6px;
  cursor: pointer;
  margin-right: 1rem;
}

.back-btn:hover {
  background: #5a6268;
}

.logout-btn {
  background: #dc3545;
  color: white;
  border: none;
  padding: 0.5rem 1rem;
  border-radius: 6px;
  cursor: pointer;
}

.dashboard-main {
  padding: 2rem;
  max-width: 1200px;
  margin: 0 auto;
}

/* Device Selection */
.device-selection {
  text-align: center;
  padding: 2rem;
}

.device-selection h2 {
  margin-bottom: 2rem;
  color: #333;
}

.devices-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
  gap: 1.5rem;
  max-width: 800px;
  margin: 0 auto;
}

.device-card {
  background: white;
  padding: 2rem;
  border-radius: 12px;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
  cursor: pointer;
  transition: all 0.2s;
  border: 2px solid transparent;
}

.device-card:hover {
  transform: translateY(-2px);
  box-shadow: 0 8px 16px rgba(0, 0, 0, 0.15);
  border-color: #007bff;
}

.device-card h3 {
  margin: 0 0 1rem 0;
  color: #333;
}

.device-card p {
  color: #666;
  margin-bottom: 1rem;
}

.device-status {
  padding: 0.5rem 1rem;
  border-radius: 6px;
  font-weight: 500;
  display: inline-block;
}

.device-status.active {
  background: #d4edda;
  color: #155724;
}

.device-card.add-device {
  border: 2px dashed #007bff;
  background: #f8f9fa;
  color: #007bff;
}

.device-card.add-device:hover {
  background: #e3f2fd;
}

/* Status Grid */
.status-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
  gap: 1.5rem;
  margin-bottom: 2rem;
}

.status-card {
  background: white;
  padding: 1.5rem;
  border-radius: 12px;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
}

.status-card h3 {
  margin: 0 0 1rem 0;
  color: #666;
  font-size: 1rem;
}

.status-value {
  font-size: 2rem;
  font-weight: bold;
  color: #333;
}

.status-value.online {
  color: #28a745;
}

/* Navigation Tabs */
.nav-tabs {
  display: flex;
  gap: 0.5rem;
  margin-bottom: 2rem;
  flex-wrap: wrap;
}

.tab-btn {
  background: white;
  border: none;
  padding: 1rem 1.5rem;
  border-radius: 8px;
  cursor: pointer;
  transition: all 0.2s;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
}

.tab-btn.active {
  background: #007bff;
  color: white;
}

.tab-content {
  background: white;
  padding: 2rem;
  border-radius: 12px;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
}

.section-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 2rem;
}

.add-btn {
  background: #28a745;
  color: white;
  border: none;
  padding: 0.75rem 1.5rem;
  border-radius: 6px;
  cursor: pointer;
}

/* Lists */
.users-list, .logs-list, .devices-list {
  display: grid;
  gap: 1rem;
}

.user-card, .log-card, .device-item {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 1rem;
  border: 1px solid #e9ecef;
  border-radius: 8px;
  background: #f8f9fa;
}

.user-card.success, .log-card.success {
  border-color: #28a745;
  background: #d4edda;
}

.user-actions, .device-actions {
  display: flex;
  align-items: center;
  gap: 1rem;
}

.user-status {
  padding: 0.25rem 0.75rem;
  border-radius: 4px;
  font-size: 0.875rem;
  font-weight: 500;
}

.user-status.active {
  background: #28a745;
  color: white;
}

.select-btn {
  background: #007bff;
  color: white;
  border: none;
  padding: 0.5rem 1rem;
  border-radius: 4px;
  cursor: pointer;
  font-size: 0.875rem;
}

.select-btn.active {
  background: #28a745;
}

.delete-btn {
  background: #dc3545;
  color: white;
  border: none;
  padding: 0.5rem;
  border-radius: 4px;
  cursor: pointer;
  font-size: 1rem;
  transition: background-color 0.2s;
}

.delete-btn:hover {
  background: #c82333;
}

.log-status {
  font-weight: bold;
}

/* Config Form */
.config-form {
  max-width: 500px;
}

.form-group {
  margin-bottom: 1.5rem;
}

.form-group label {
  display: block;
  margin-bottom: 0.5rem;
  font-weight: 500;
  color: #333;
}

.form-group input {
  width: 100%;
  padding: 0.75rem;
  border: 1px solid #ddd;
  border-radius: 6px;
  font-size: 1rem;
}

.save-btn {
  background: #007bff;
  color: white;
  border: none;
  padding: 0.75rem 1.5rem;
  border-radius: 6px;
  cursor: pointer;
}

/* Modal */
.modal-overlay {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background: rgba(0, 0, 0, 0.5);
  display: flex;
  align-items: center;
  justify-content: center;
  z-index: 1000;
}

.modal {
  background: white;
  padding: 2rem;
  border-radius: 12px;
  max-width: 500px;
  width: 90%;
}

.modal h3 {
  margin: 0 0 1.5rem 0;
  color: #333;
}

.form-actions {
  display: flex;
  gap: 1rem;
  justify-content: flex-end;
  margin-top: 2rem;
}

.cancel-btn {
  background: #6c757d;
  color: white;
  border: none;
  padding: 0.75rem 1.5rem;
  border-radius: 6px;
  cursor: pointer;
}

.submit-btn {
  background: #28a745;
  color: white;
  border: none;
  padding: 0.75rem 1.5rem;
  border-radius: 6px;
  cursor: pointer;
}

/* Messages */
.message {
  position: fixed;
  top: 2rem;
  right: 2rem;
  z-index: 1000;
}

.message-content {
  padding: 1rem 1.5rem;
  border-radius: 8px;
  color: white;
  font-weight: 500;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.15);
  animation: slideIn 0.3s ease-out;
}

.message-content.success {
  background: #28a745;
}

.message-content.error {
  background: #dc3545;
}

.message-content.info {
  background: #17a2b8;
}

@keyframes slideIn {
  from {
    transform: translateX(100%);
    opacity: 0;
  }
  to {
    transform: translateX(0);
    opacity: 1;
  }
}

@media (max-width: 768px) {
  .dashboard-header {
    flex-direction: column;
    gap: 1rem;
    text-align: center;
  }
  
  .nav-tabs {
    flex-direction: column;
  }
  
  .status-grid {
    grid-template-columns: 1fr;
  }
  
  .devices-grid {
    grid-template-columns: 1fr;
  }
  
  .user-card, .log-card, .device-item {
    flex-direction: column;
    gap: 1rem;
    text-align: center;
  }
}
</style>

<template>
  <div class="auth-page">
    <div class="auth-form">
      <h2>用户认证</h2>
      <div class="auth-tabs">
        <button
            :class="['tab-button', activeTab === 'login' ? 'active' : '']"
            @click="activeTab = 'login'"
        >
          登录
        </button>
        <button
            :class="['tab-button', activeTab === 'register' ? 'active' : '']"
            @click="activeTab = 'register'"
        >
          注册
        </button>
      </div>

      <!-- 登录表单 -->
      <div v-if="activeTab === 'login'" class="form-section">
        <input
            v-model="loginForm.username"
            type="text"
            placeholder="用户名"
            class="form-input"
        >
        <input
            v-model="loginForm.password"
            type="password"
            placeholder="密码"
            class="form-input"
        >
        <button @click="handleLogin" class="form-button" :disabled="loading">
          {{ loading ? '登录中...' : '登录' }}
        </button>
      </div>

      <!-- 注册表单 -->
      <div v-else class="form-section">
        <input
            v-model="registerForm.username"
            type="text"
            placeholder="用户名"
            class="form-input"
        >
        <input
            v-model="registerForm.password"
            type="password"
            placeholder="密码"
            class="form-input"
        >
        <button @click="handleRegister" class="form-button" :disabled="loading">
          {{ loading ? '注册中...' : '注册' }}
        </button>
      </div>

      <div v-if="message" :class="['alert-banner', isError ? 'error' : 'success']">
        {{ message }}
      </div>
    </div>
  </div>
</template>

<script>
import { authAPI, setCookie } from '../utils/api'

export default {
  name: 'Auth',
  data() {
    return {
      activeTab: 'login',
      loading: false,
      message: '',
      isError: false,
      loginForm: {
        username: '',
        password: ''
      },
      registerForm: {
        username: '',
        password: ''
      }
    }
  },
  methods: {
    async handleLogin() {
      if (!this.loginForm.username || !this.loginForm.password) {
        this.showMessage('请输入用户名和密码', true)
        return
      }

      this.loading = true
      this.message = ''

      try {
        const result = await authAPI.login(this.loginForm)

        if (result.success) {
          // 保存用户信息到cookie
          const userData = result.data.user
          setCookie('userId', userData.id, 1)
          setCookie('userName', userData.name, 1)
          setCookie('isAdmin', userData.isAdmin, 1)
          setCookie('token', result.data.token, 1)

          // 根据用户角色跳转
          if (userData.isAdmin) {
            this.$router.push('/admin')
          } else {
            this.$router.push('/common_home')
          }
        } else {
          this.showMessage(result.message, true)
        }
      } catch (error) {
        this.showMessage('登录失败: ' + (error.response?.data?.message || error.message), true)
      } finally {
        this.loading = false
      }
    },

    async handleRegister() {
      if (!this.registerForm.username || !this.registerForm.password) {
        this.showMessage('请输入用户名和密码', true)
        return
      }

      this.loading = true
      this.message = ''

      try {
        const result = await authAPI.register(this.registerForm)

        if (result.success) {
          this.showMessage('注册成功，请登录')
          this.activeTab = 'login'
          this.registerForm.username = ''
          this.registerForm.password = ''
        } else {
          this.showMessage(result.message, true)
        }
      } catch (error) {
        this.showMessage('注册失败: ' + (error.response?.data?.message || error.message), true)
      } finally {
        this.loading = false
      }
    },

    showMessage(msg, isError = false) {
      this.message = msg
      this.isError = isError
      setTimeout(() => {
        this.message = ''
      }, 3000)
    }
  }
}
</script>

<style scoped>
.auth-tabs {
  display: flex;
  margin-bottom: 20px;
  border-bottom: 1px solid #eee;
}

.tab-button {
  flex: 1;
  padding: 12px;
  border: none;
  background: none;
  cursor: pointer;
  border-bottom: 2px solid transparent;
}

.tab-button.active {
  border-bottom-color: var(--secondary-color);
  color: var(--secondary-color);
  font-weight: bold;
}

.form-section {
  margin-top: 20px;
}

.alert-banner.success {
  background: var(--primary-color);
}

.alert-banner.error {
  background: var(--danger-color);
}
</style>
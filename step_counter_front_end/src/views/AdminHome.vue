<template>
  <div class="admin-home page">
    <NavBar :is-admin="true" />

    <div class="page-container">
      <h2>管理员控制台</h2>

      <div class="quick-stats">
        <p>当前用户：<span>{{ currentUser.userName }} (管理员)</span></p>
        <p>系统状态：<span style="color: var(--primary-color)">正常运行</span></p>
        <p>在线用户：<span>{{ onlineUsersCount }}</span></p>
        <p>总用户数：<span>{{ users.length }}</span></p>
      </div>

      <div class="admin-features">
        <div class="feature-card" @click="$router.push('/monitor')">
          <h3>📊 数据监控</h3>
          <p>查看所有用户的实时健康数据</p>
        </div>

        <div class="feature-card" @click="$router.push('/user-management')">
          <h3>👥 用户管理</h3>
          <p>管理用户权限和状态</p>
        </div>

        <div class="feature-card">
          <h3>📈 系统统计</h3>
          <p>查看系统使用情况统计</p>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import NavBar from '../components/NavBar.vue'
import { getCurrentUserInfo } from '../utils/auth'
import { userAPI } from '../utils/api'

export default {
  name: 'AdminHome',
  components: {
    NavBar
  },
  data() {
    return {
      currentUser: getCurrentUserInfo(),
      users: [],
      selectedUserLimit: 100,
      totalUsersCount: 0
    }
  },
  computed: {
    onlineUsersCount() {
      return this.users.filter(user => user.isOnline).length
    }
  },
  async mounted() {
    await this.loadUsers()
  },
  methods: {
    async loadUsers() {
      try {
        const result = await userAPI.getUsersWithLimit(this.selectedUserLimit)
        if (result.success) {
          this.users = result.data
        }
      } catch (error) {
        this.showAlert('加载用户列表失败: ' + error.message, 'error')
      }
    },
    showAlert(message, type = 'info') {
      this.alertMessage = message
      this.alertType = type
      setTimeout(() => {
        this.alertMessage = ''
      }, 3000)
    },
  }
}
</script>

<style scoped>
.admin-features {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
  gap: 20px;
  margin-top: 30px;
}

.feature-card {
  background: white;
  padding: 25px;
  border-radius: 12px;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
  cursor: pointer;
  transition: all 0.3s ease;
}

.feature-card:hover {
  transform: translateY(-5px);
  box-shadow: 0 8px 15px rgba(0, 0, 0, 0.15);
}

.feature-card h3 {
  margin: 0 0 10px 0;
  color: var(--secondary-color);
}

.feature-card p {
  margin: 0;
  color: #666;
  font-size: 0.9em;
}
</style>
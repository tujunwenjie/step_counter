<template>
  <div class="admin page">
    <NavBar :is-admin="true" />

    <div class="page-container">
      <h3>用户管理</h3>

      <div v-if="alertMessage" :class="['alert-banner', alertType]">
        {{ alertMessage }}
      </div>

      <div class="admin-stats">
        <p>注册用户总数：<span>{{ users.length }}</span></p>
        <p>在线用户：<span>{{ onlineUsersCount }}</span></p>
      </div>

      <div class="user-controls">
        <div class="control-group">
          <label>显示用户数：</label>
          <select v-model="selectedUserLimit" @change="loadUsers">
            <option value="10">10</option>
            <option value="20">20</option>
            <option value="50">50</option>
            <option value="100">100</option>
          </select>
        </div>

        <input
            v-model="searchKeyword"
            type="text"
            placeholder="搜索用户..."
            class="search-input"
        >
      </div>

      <div class="user-list">
        <UserCard
            v-for="user in filteredUsers"
            :key="user.id"
            :user="user"
            :show-actions="true"
            @userUpdated="loadUsers"
        />
      </div>

      <div class="button-group">
        <button @click="checkUserStatus" class="nav-btn">
          检查用户状态
        </button>
        <button @click="sendReminderToAll" class="nav-btn" style="background: var(--primary-color);">
          发送全体提醒
        </button>
        <button @click="resetAllSteps" class="danger-btn">
          全部用户步数清零
        </button>
      </div>
    </div>
  </div>
</template>

<script>
import NavBar from '../components/NavBar.vue'
import UserCard from '../components/UserCard.vue'
import { userAPI } from '../utils/api'
import { healthAPI } from '../utils/api'

export default {
  name: 'Admin',
  components: {
    NavBar,
    UserCard
  },
  data() {
    return {
      users: [],
      selectedUserLimit: 20,
      searchKeyword: '',
      alertMessage: '',
      alertType: 'info',
      autoRefreshInterval: null
    }
  },
  computed: {
    filteredUsers() {
      if (!this.searchKeyword) {
        return this.users
      }
      const keyword = this.searchKeyword.toLowerCase()
      return this.users.filter(user =>
          user.name.toLowerCase().includes(keyword) ||
          user.id.toString().includes(keyword)
      )
    },
    onlineUsersCount() {
      return this.users.filter(user => user.isOnline).length
    }
  },
  async mounted() {
    await this.loadUsers()
    this.startAutoRefresh()
  },
  beforeUnmount() {
    this.stopAutoRefresh()
  },
  methods: {
    async loadUsers() {
      try {
        const result = await userAPI.getUsersWithLimit(this.selectedUserLimit)

        console.log('Loaded users:', result)
        if (result.success) {
          this.users = result.data
          for (const user of this.users) {
            try {
              const statusResult = await healthAPI.getLatestRecord(user.id)
              console.log(`Loaded latest record for user ${user.id}:`, statusResult)
              if (statusResult.success) {
                user.latestRecord = statusResult.data[0] 

              } else {
                user.latestRecord = null
              }
            } catch (error) {
              user.latestRecord = null
            }
          }
        }
      } catch (error) {
        this.showAlert('加载用户列表失败: ' + error.message, 'error')
      }
    },

    async checkUserStatus() {
      try {
        // 这里可以添加更详细的用户状态检查逻辑
        const offlineUsers = this.users.filter(user => !user.isOnline)
        if (offlineUsers.length > 0) {
          this.showAlert(`有 ${offlineUsers.length} 个用户处于离线状态`, 'warning')
        } else {
          this.showAlert('所有用户状态正常', 'success')
        }
      } catch (error) {
        this.showAlert('状态检查失败: ' + error.message, 'error')
      }
    },

    async sendReminderToAll() {
      if (!confirm('确定要向所有用户发送提醒吗？')) {
        return
      }

      try {
        // 这里可以实现向所有用户发送提醒的逻辑
        this.showAlert('提醒已发送给所有用户', 'success')
      } catch (error) {
        this.showAlert('发送失败: ' + error.message, 'error')
      }
    },

    async resetAllSteps() {
      if (!confirm('确定要重置所有用户的步数吗？此操作不可撤销！')) {
        return
      }

      try {
        // 这里可以实现重置所有用户步数的逻辑
        const promises = this.users.map(user =>
            userAPI.resetUserSteps(user.id)
        )
        await Promise.all(promises)
        this.showAlert('所有用户步数已重置', 'success')
        await this.loadUsers() // 刷新列表
      } catch (error) {
        this.showAlert('重置失败: ' + error.message, 'error')
      }
    },

    showAlert(message, type = 'info') {
      this.alertMessage = message
      this.alertType = type
      setTimeout(() => {
        this.alertMessage = ''
      }, 3000)
    },

    startAutoRefresh() {
      this.autoRefreshInterval = setInterval(this.loadUsers, 30000) // 每30秒刷新一次
    },

    stopAutoRefresh() {
      if (this.autoRefreshInterval) {
        clearInterval(this.autoRefreshInterval)
      }
    }
  }
}
</script>

<style scoped>
.admin-stats {
  display: flex;
  gap: 20px;
  margin-bottom: 20px;
  padding: 15px;
  background: white;
  border-radius: 8px;
  box-shadow: 0 2px 4px rgba(0,0,0,0.1);
}

.admin-stats p {
  margin: 0;
  font-weight: bold;
}

.user-controls {
  display: flex;
  gap: 20px;
  margin-bottom: 20px;
  align-items: center;
  flex-wrap: wrap;
}

.search-input {
  padding: 8px 12px;
  border: 1px solid #ddd;
  border-radius: 4px;
  min-width: 200px;
}

.alert-banner.success {
  background: var(--primary-color);
}

.alert-banner.warning {
  background: #ff9800;
}

.alert-banner.error {
  background: var(--danger-color);
}

.alert-banner.info {
  background: var(--secondary-color);
}
</style>
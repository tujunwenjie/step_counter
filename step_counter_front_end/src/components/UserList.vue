<template>
  <div class="user-list">
    <h3>用户列表</h3>
    <div class="user-grid">
      <div 
        v-for="user in users" 
        :key="user.id"
        :class="['user-card', { selected: selectedUser === user.id }]"
        @click="selectUser(user.id)"
      >
        <div class="user-info">
          <div class="username">{{ user.username }}</div>
          <div class="user-status">
            <span :class="['status-dot', user.online ? 'online' : 'offline']"></span>
            {{ user.online ? '在线' : '离线' }}
          </div>
        </div>
        <div class="admin-control">
          <label class="checkbox-label">
            <input 
              type="checkbox" 
              :checked="user.isAdmin" 
              @change="toggleAdmin(user.id, $event.target.checked)"
            >
            管理员
          </label>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import axios from 'axios'

export default {
  name: 'UserList',
  props: {
    users: {
      type: Array,
      default: () => []
    }
  },
  data() {
    return {
      selectedUser: null
    }
  },
  methods: {
    selectUser(userId) {
      this.selectedUser = userId
      this.$emit('user-selected', userId)
    },
    async toggleAdmin(userId, isAdmin) {
      if (confirm(`确定要${isAdmin ? '设置为' : '取消'}管理员吗？`)) {
        try {
          await axios.put('/api/user-admin', {
            userId,
            isAdmin
          })
          this.$emit('admin-updated')
        } catch (error) {
          alert('操作失败')
        }
      }
    }
  }
}
</script>

<style scoped>
.user-list h3 {
  margin-bottom: 1rem;
  color: #333;
}

.user-grid {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(250px, 1fr));
  gap: 1rem;
}

.user-card {
  background: white;
  padding: 1rem;
  border-radius: 8px;
  box-shadow: 0 2px 5px rgba(0,0,0,0.1);
  cursor: pointer;
  border: 2px solid transparent;
  transition: all 0.3s ease;
}

.user-card:hover {
  box-shadow: 0 4px 10px rgba(0,0,0,0.15);
}

.user-card.selected {
  border-color: #007bff;
}

.user-info {
  margin-bottom: 1rem;
}

.username {
  font-weight: bold;
  margin-bottom: 0.5rem;
}

.user-status {
  display: flex;
  align-items: center;
  gap: 0.5rem;
  font-size: 0.9rem;
  color: #666;
}

.status-dot {
  width: 8px;
  height: 8px;
  border-radius: 50%;
}

.status-dot.online {
  background: #28a745;
}

.status-dot.offline {
  background: #dc3545;
}

.checkbox-label {
  display: flex;
  align-items: center;
  gap: 0.5rem;
  cursor: pointer;
}

.checkbox-label input {
  margin: 0;
}
</style>
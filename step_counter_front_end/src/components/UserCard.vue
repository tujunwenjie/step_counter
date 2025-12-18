<template>
  <div class="user-card">
    <div :class="['online-status', user.isOnline ? 'online' : 'offline']"></div>
    <h4>{{ user.name }} {{ user.isAdmin ? '(管理员)' : '' }}</h4>
    <p>ID: {{ user.id || '未知ID' }}</p>
    <p>最后活动: {{ formatLastActivity(user.lastActivity) }}</p>
    <p>累计步数: {{ user.latestRecord?.steps || 0 }}</p>
    <div v-if="showActions" class="user-actions">
      <button @click="toggleAdmin" class="nav-btn" style="padding: 8px 16px;">
        {{ user.isAdmin ? '取消管理员' : '设为管理员' }}
      </button>
      <button @click="sendReminder" class="nav-btn" style="padding: 8px 16px; background: var(--primary-color);">
        发送提醒
      </button>
      <button @click="resetSteps" class="danger-btn" style="padding: 8px 16px;">
        重置步数
      </button>
    </div>
  </div>
</template>

<script>
import { userAPI } from '../utils/api'

export default {
  name: 'UserCard',
  props: {
    user: {
      type: Object,
      required: true
    },
    showActions: {
      type: Boolean,
      default: false
    }
  },
  methods: {
    formatLastActivity(activity) {
      if (!activity) return '暂无记录'
      return new Date(activity).toLocaleString()
    },
    async toggleAdmin() {
      if (!confirm(`确定要${this.user.isAdmin ? '取消' : '设置'}该用户的管理员权限吗？`)) {
        return
      }

      try {
        const result = await userAPI.setUserAdmin(this.user.id, !this.user.isAdmin)
        if (result.success) {
          this.$emit('userUpdated')
          alert('操作成功')
        }
      } catch (error) {
        alert('操作失败: ' + error.message)
      }
    },
    async sendReminder() {
      try {
        const result = await userAPI.sendReminderToUser(this.user.id)
        if (result.success) {
          alert('提醒发送成功')
        }
      } catch (error) {
        alert('发送失败: ' + error.message)
      }
    },
    async resetSteps() {
      if (!confirm('确定要重置该用户的步数吗？')) {
        return
      }

      try {
        const result = await userAPI.resetUserSteps(this.user.id)
        if (result.success) {
          this.$emit('userUpdated')
          alert('步数重置成功')
        }
      } catch (error) {
        alert('重置失败: ' + error.message)
      }
    }
  }
}
</script>

<style scoped>
.user-actions {
  display: flex;
  gap: 10px;
  margin-top: 10px;
  flex-wrap: wrap;
}
</style>
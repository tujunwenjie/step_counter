<template>
  <div class="nav-bar">
    <button v-for="item in navItems" :key="item.name"
            class="nav-btn" @click="navigateTo(item.path)">
      {{ item.name }}
    </button>
    <button class="nav-btn" @click="logout" style="margin-left: auto;">
      退出登录
    </button>
  </div>
</template>

<script>
import { authAPI } from '../utils/api'
import { clearAuth } from '../utils/auth'

export default {
  name: 'NavBar',
  props: {
    isAdmin: {
      type: Boolean,
      default: false
    }
  },
  computed: {
    navItems() {
      const baseItems = [
        { name: '主界面', path: this.isAdmin ? '/admin' : '/common_home' }
      ]

      if (this.isAdmin) {
        baseItems.push(
            { name: '监控中心', path: '/monitor' },
            { name: '用户管理', path: '/user-management' }
        )
      }

      return baseItems
    }
  },
  methods: {
    async logout() {
      try {
        console.log('开始执行登出流程...')

        // 调用后端登出API
        const response = await authAPI.logout()
        console.log('后端登出响应:', response)

        if (response && response.success) {
          console.log('后端登出成功')
        } else {
          console.warn('后端登出返回失败状态:', response)
        }
      } catch (error) {
        console.error('调用登出API失败:', error)
        // 即使API调用失败，也继续执行前端清除逻辑
      }

      // 无论后端API是否成功，都执行前端清除和跳转
      this.performFrontendLogout()
    },
    navigateTo(path) {
      if (!path) return
      
      // 使用 setTimeout 避免在 Vue 更新周期内进行导航
      setTimeout(() => {
        try {
          if (this.$router && path !== this.$route.path) {
            this.$router.push(path)
          }
        } catch (error) {
          console.warn('导航失败:', error)
        }
      }, 0)
    },
    performFrontendLogout() {
      try {
        console.log('执行前端登出清理...')

        // 清除认证信息
        clearAuth()

        // 强制刷新页面以确保所有状态被清除
        // 或者使用路由跳转
        console.log('跳转到登录页面...')

        // 方法1: 使用路由跳转（推荐）
        this.$router.push('/').then(() => {
          console.log('路由跳转成功')
          // 可选：刷新页面以确保状态完全重置
          window.location.reload()
        }).catch(err => {
          console.error('路由跳转失败:', err)
          // 备用方案：直接跳转
          window.location.href = '/'
        })

      } catch (error) {
        console.error('前端登出过程出错:', error)
        // 最终备用方案
        window.location.href = '/'
      }
    }
  }
}
</script>
<template>
  <div class="access-denied page">
    <div class="denied-container">
      <h2>⚠️ 访问被拒绝</h2>
      <p>您没有权限访问此页面</p>
      <p>将在 {{ countdown }} 秒后自动跳转到登录页面</p>
      <button @click="redirectToLogin" class="nav-btn">
        立即返回登录页面
      </button>
    </div>
  </div>
</template>

<script>
export default {
  name: 'AccessDenied',
  data() {
    return {
      countdown: 10,
      countdownInterval: null
    }
  },
  mounted() {
    this.startCountdown()
  },
  beforeUnmount() {
    this.stopCountdown()
  },
  methods: {
    startCountdown() {
      this.countdownInterval = setInterval(() => {
        this.countdown--
        if (this.countdown <= 0) {
          this.redirectToLogin()
        }
      }, 1000)
    },
    stopCountdown() {
      if (this.countdownInterval) {
        clearInterval(this.countdownInterval)
      }
    },
    redirectToLogin() {
      this.stopCountdown()
      this.$router.push('/')
    }
  }
}
</script>

<style scoped>
.access-denied {
  display: flex;
  justify-content: center;
  align-items: center;
  min-height: 100vh;
}

.denied-container {
  text-align: center;
  background: white;
  padding: 40px;
  border-radius: 16px;
  box-shadow: var(--card-shadow);
  max-width: 400px;
  width: 100%;
}

.denied-container h2 {
  color: var(--danger-color);
  margin-bottom: 20px;
}

.denied-container p {
  margin: 10px 0;
  color: #666;
}

.nav-btn {
  margin-top: 20px;
}
</style>
<template>
  <div class="common-home page">
    <NavBar :is-admin="false" />

    <div class="page-container">
      <h2>欢迎使用健康管理系统</h2>

      <div class="quick-stats">
        <p>当前用户：<span id="currentUser">{{ currentUser.userName }}</span></p>
        <p>今日步数：<span id="userSteps">{{ latestRecord?.steps || 0 }}</span></p>
        <p>消耗热量：
          <span class="calories-display" id="userCalories">
            <span v-for="(digit, index) in latestRecord?.calories.toString().padStart(3, '0').split('')" :key="index" class="digit">
              {{ digit }}
            </span>卡
          </span>
          
        </p>
        <p>当前时间：<span id="currentTime">{{ currentTime }}</span></p>
      </div>

      <HealthChart
          :data="chartData"
          :title="'步数趋势'"
          ref="healthChart"
      />

      <button @click="resetSteps" class="danger-btn">
        步数清零
      </button>
    </div>
  </div>
</template>

<script>
import NavBar from '../components/NavBar.vue'
import HealthChart from '../components/HealthChart.vue'
import { healthAPI, userAPI } from '../utils/api'
import { getCurrentUserInfo } from '../utils/auth'

export default {
  name: 'CommonHome',
  components: {
    NavBar,
    HealthChart
  },
  data() {
    return {
      currentUser: getCurrentUserInfo(),
      currentTime: '',
      selectedRecordsCount: 50,
      latestRecord: null,
      chartData: [],
      updateInterval: null,
      timeInterval: null
    }
  },
  computed: {
    calorieDigits() {
      const calories = this.latestRecord?.calories || 0
      return String(Math.round(calories)).padStart(3, '0').split('')
    }
  },
  async mounted() {
    await this.loadUserData()
    this.startRealTimeUpdates()
    this.startTimeUpdates()
  },
  beforeUnmount() {
    this.stopRealTimeUpdates()
    this.stopTimeUpdates()
  },
  methods: {
    async loadUserData() {
      try {
        // 获取最新记录
        const recordResult = await healthAPI.getCurrentUserLatestRecord()
        if (recordResult.success && recordResult.data.length > 0) {
          this.latestRecord = recordResult.data[0]
        }

        // 获取图表数据
        const chartResult = await healthAPI.getChartData(50)
        if (chartResult.success) {
          console.log('加载用户数据成功chartData:', chartResult.data)
          this.chartData = chartResult.data
        }
      } catch (error) {
        console.error('加载用户数据失败:', error)
      }
    },

    startRealTimeUpdates() {
      this.updateInterval = setInterval(async () => {
        try {
          const result = await healthAPI.getChartData(this.selectedRecordsCount)
          if (result.success) {
            console.log('实时更新图表数据chartData:', result.data)
            this.chartData = result.data
            // 更新最新记录
            if (result.data.length > 0) {
              this.latestRecord = result.data[0]
            }
          }
        } catch (error) {
          console.error('更新数据失败:', error)
        }
      }, 2000) // 每2秒更新一次
    },

    stopRealTimeUpdates() {
      if (this.updateInterval) {
        clearInterval(this.updateInterval)
      }
    },

    startTimeUpdates() {
      this.updateTime()
      this.timeInterval = setInterval(this.updateTime, 1000)
    },

    stopTimeUpdates() {
      if (this.timeInterval) {
        clearInterval(this.timeInterval)
      }
    },

    updateTime() {
      this.currentTime = new Date().toLocaleString()
    },

    async resetSteps() {
      if (!confirm('确定要重置步数吗？')) {
        return
      }

      try {
        const result = await userAPI.resetUserSteps(this.currentUser.userId)
        if (result.success) {
          alert('步数重置成功')
          await this.loadUserData() // 重新加载数据
        }
      } catch (error) {
        alert('重置失败: ' + error.message)
      }
    }
  }
}
</script>
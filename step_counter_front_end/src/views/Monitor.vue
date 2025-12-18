<template>
  <div class="monitor page">
    <NavBar :is-admin="true" />

    <div class="page-container">
      <h2>监控中心</h2>

      <div class="monitor-controls">
        <div class="control-group">
          <label>数据条数：</label>
          <select v-model="selectedRecordsCount" @change="updateCharts">
            <option value="20">20条</option>
            <option value="50">50条</option>
            <option value="100">100条</option>
          </select>
        </div>

        <div class="control-group">
          <label>刷新间隔：</label>
          <select v-model="refreshInterval" @change="updateRefreshInterval">
            <option value="2000">2秒</option>
            <option value="5000">5秒</option>
            <option value="10000">10秒</option>
          </select>
        </div>
      </div>

      <div class="chart-row">
        <div class="chart-item">
          <h3>实时数据（最近 {{ selectedRecordsCount }} 条）</h3>
          <HealthChart
              :data="chartData"
              :title="'实时步数'"
              ref="realTimeChart  "
          />
        </div>

        <div class="chart-item">
          <h3>历史趋势（最近 200 条，间隔 1 分钟）</h3>
          <HealthChart
              :data="historyData"
              :title="'历史步数'"
              type="line"
              ref="historyChart"
          />
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import NavBar from '../components/NavBar.vue'
import HealthChart from '../components/HealthChart.vue'
import { healthAPI, dailySummaryAPI } from '../utils/api'

export default {
  name: 'Monitor',
  components: {
    NavBar,
    HealthChart
  },
  data() {
    return {
      selectedRecordsCount: 50,
      refreshInterval: 2000,
      chartData: [],
      historyData: [],
      updateInterval: null
    }
  },
  async mounted() {
    await this.loadChartsData()
    this.startAutoUpdate()
  },
  beforeUnmount() {
    this.stopAutoUpdate()
  },
  methods: {
    async loadChartsData() {
      try {
        // 加载实时数据
        const chartResult = await healthAPI.getRealTimeData(this.selectedRecordsCount)
        if (chartResult.success) {
          console.log('Loaded real-time chart data:', chartResult.data) 
          this.chartData = chartResult.data
        }

        // 加载历史数据
        const historyResult = await dailySummaryAPI.getSummariesWithLimit(200)
        if (historyResult.success) {
          console.log('Loaded history data:', historyResult.data)
          this.historyData = historyResult.data
        }
      } catch (error) {
        console.error('加载图表数据失败:', error)
      }
    },

    startAutoUpdate() {
      this.updateInterval = setInterval(this.loadChartsData, this.refreshInterval)
    },

    stopAutoUpdate() {
      if (this.updateInterval) {
        clearInterval(this.updateInterval)
      }
    },

    updateCharts() {
      this.stopAutoUpdate()
      this.loadChartsData()
      this.startAutoUpdate()
    },

    updateRefreshInterval() {
      this.stopAutoUpdate()
      this.startAutoUpdate()
    }
  }
}
</script>

<style scoped>
.monitor-controls {
  display: flex;
  gap: 20px;
  margin-bottom: 20px;
  padding: 15px;
  background: white;
  border-radius: 8px;
  box-shadow: 0 2px 4px rgba(0,0,0,0.1);
}

.control-group {
  display: flex;
  align-items: center;
  gap: 10px;
}

.control-group label {
  font-weight: bold;
}

.control-group select {
  padding: 8px 12px;
  border: 1px solid #ddd;
  border-radius: 4px;
  background: white;
}

.chart-row {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 20px;
}

.chart-container {
  background: white;
  border-radius: 8px;
  padding: 20px;
  box-shadow: 0 2px 4px rgba(0,0,0,0.1);
}

.chart-container h3 {
  margin: 0 0 15px 0;
  font-size: 16px;
  color: #333;
}

.chart-wrapper {
  height: 300px; /* 固定高度 */
  position: relative;
}

@media (max-width: 768px) {
  .chart-row {
    grid-template-columns: 1fr;
  }

  .monitor-controls {
    flex-direction: column;
    gap: 10px;
  }
  
  .chart-wrapper {
    height: 250px; /* 移动端稍小一些 */
  }
}
</style>
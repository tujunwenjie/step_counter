<template>
  <div class="chart-container">
    <canvas ref="chartCanvas" :key="chartKey"></canvas>
  </div>
</template>

<script>
import { healthAPI } from '../utils/api';

export default {
  name: 'HealthChart',
  props: {
    title: {
      type: String,
      default: '步数统计'
    },
    numOfRecords: {
      type: Number,
      default: 10
    },
    userId: {
      type: Number,
      required: true
    }
  },
  data() {
    return {
      chart: null,
      pollInterval: null,
      chartKey: 0 // 用于强制重新创建 canvas
    }
  },
  mounted() {
    this.startPolling()
  },
  beforeUnmount() {
    this.stopPolling()
    this.destroyChart()
  },
  methods: {
    initChart(records) {
      this.destroyChart()
      if (!this.$refs.chartCanvas) return
      
      const ctx = this.$refs.chartCanvas.getContext('2d')
      if (!ctx) return

      try {
        const chartData = this.buildChartData(records)
        
        this.chart = new window.Chart(ctx, {
          type: 'line',
          data: chartData,
          options: {
            responsive: true,
            maintainAspectRatio: false,
            animation: false,
            plugins: { 
              legend: { display: false }, 
              tooltip: { 
                enabled: true,
                mode: 'nearest',
                intersect: false,
                callbacks: {
                  label: function(context) {
                    return `步数: ${context.parsed.y}`
                  },
                  title: function(context) {
                    return `时间: ${context[0].label}`
                  }
                }
              } 
            },
            scales: {
              y: { 
                beginAtZero: true, 
                title: { display: true, text: '步数' },
                grid: { display: false }
              },
              x: { 
                title: { display: true, text: '时间' },
                grid: { display: false }
              }
            },
            elements: {
              point: {
                radius: 4, // 红点大小
                hoverRadius: 6, // 鼠标悬停时变大
                backgroundColor: '#e74c3c', // 红点颜色
                borderColor: '#ffffff', // 白边
                borderWidth: 2 // 白边宽度
              },
              line: {
                tension: 0.1 // 保持线条平滑度
              }
            },
            interaction: {
              mode: 'nearest',
              intersect: false
            }
          }
        })
        
        console.log(`图表初始化成功，显示 ${records.length} 条记录`)
      } catch (error) {
        console.error('图表初始化失败:', error)
      }
    },

    startPolling() {
      this.updateChart()
      this.pollInterval = setInterval(this.updateChart, 2000)
    },

    stopPolling() {
      if (this.pollInterval) {
        clearInterval(this.pollInterval)
      }
    },

    async updateChart() {
      try {
        const records = await this.fetchLatestRecords()
        if (!records || records.length === 0) return
        
        // 每次重新创建图表，避免更新操作
        this.recreateChart(records)
        
      } catch (error) {
        console.error('更新图表失败:', error)
      }
    },

    recreateChart(records) {
      // 强制重新创建 canvas 元素
      this.chartKey++
      
      this.$nextTick(() => {
        this.initChart(records)
      })
    },

    async fetchLatestRecords() {
      try {
        const response = await healthAPI.getChartData(this.numOfRecords)
        
        if (response && response.success) {
          return response.data
        } else {
          throw new Error(response?.message || '获取数据失败')
        }
      } catch (error) {
        console.error('获取实时数据失败:', error)
        return null
      }
    },

    buildChartData(records) {
      const sortedRecords = [...records].sort((a, b) => 
        new Date(a.recordedAt || a.recordAt) - new Date(b.recordedAt || b.recordAt)
      )
      
      return {
        labels: sortedRecords.map(record => this.formatTime(record.recordedAt || record.recordAt)),
        datasets: [{
          label: this.title,
          data: sortedRecords.map(record => record.steps || 0),
          borderColor: '#e74c3c', // 红色线条
          backgroundColor: 'rgba(231, 76, 60, 0.1)', // 浅红色填充
          borderWidth: 2,
          tension: 0.1,
          fill: true,
          // 点样式配置
          pointBackgroundColor: '#e74c3c', // 红点
          pointBorderColor: '#ffffff', // 白边
          pointBorderWidth: 2,
          pointRadius: 4, // 点大小
          pointHoverRadius: 6, // 悬停时变大
          pointHoverBackgroundColor: '#c0392b', // 悬停时深红色
          pointHoverBorderColor: '#ffffff',
          pointHoverBorderWidth: 2
        }]
      }
    },

    destroyChart() {
      if (this.chart) {
        try {
          this.chart.destroy()
        } catch (error) {
          // 静默处理
        }
        this.chart = null
      }
    },

    formatTime(recordedAt) {
      if (!recordedAt) return '--:--'
      try {
        const date = new Date(recordedAt)
        return `${date.getHours().toString().padStart(2, '0')}:${date.getMinutes().toString().padStart(2, '0')}`
      } catch {
        return '时间错误'
      }
    }
  }
}
</script>

<style scoped>
.chart-container {
  position: relative;
  height: 300px;
  width: 100%;
  max-width: 100%;
  overflow: hidden;
}

.chart-container canvas {
  max-width: 100% !important;
  width: 100% !important;
  height: 100% !important;
}
</style>
<template>
  <div id="app">
    <router-view></router-view>
  </div>
</template>

<script>
export default {
  name: 'App'
}
</script>

<style>
/* 全局样式 */
:root {
  --primary-color: #4CAF50;
  --secondary-color: #2196F3;
  --danger-color: #FF5252;
  --bg-gradient: linear-gradient(135deg, #f5f7fa 0%, #c3cfe2 100%);
  --card-shadow: 0 8px 32px rgba(31, 38, 135, 0.15);
}

body {
  font-family: 'Inter', -apple-system, BlinkMacSystemFont, 'Segoe UI', sans-serif;
  margin: 0;
  padding: 20px;
  background: var(--bg-gradient);
  min-height: 100vh;
}

/* 导航栏美化 */
.nav-bar {
  display: flex;
  gap: 20px;
  margin-bottom: 30px;
  padding: 15px 30px;
  background: rgba(255, 255, 255, 0.9);
  border-radius: 16px;
  box-shadow: var(--card-shadow);
  backdrop-filter: blur(10px);
  position: sticky;
  top: 20px;
  z-index: 100;
}

.nav-btn {
  background: linear-gradient(45deg, var(--secondary-color), #1976D2);
  color: white;
  padding: 12px 24px;
  border-radius: 8px;
  transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
  display: flex;
  align-items: center;
  gap: 8px;
  border: none;
  cursor: pointer;
}

.nav-btn:hover {
  transform: translateY(-2px);
  box-shadow: 0 4px 12px rgba(33, 150, 243, 0.3);
}

/* 主内容区优化 */
.page-container {
  background: rgba(255, 255, 255, 0.9);
  border-radius: 16px;
  padding: 30px;
  box-shadow: var(--card-shadow);
  backdrop-filter: blur(10px);
}

.quick-stats {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(240px, 1fr));
  gap: 24px;
  margin-bottom: 32px;
}

.quick-stats p {
  background: rgba(255, 255, 255, 0.95);
  padding: 20px;
  border-radius: 12px;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.05);
  font-size: 1.1em;
  color: #2d3436;
}

/* 卡路里显示优化 */
.calories-display {
  background: linear-gradient(45deg, #ff6b6b, #ff5252);
  padding: 12px 24px;
  border-radius: 8px;
  box-shadow: 0 4px 6px rgba(255, 82, 82, 0.15);
  transition: transform 0.3s ease;
  display: inline-flex;
  gap: 5px;
}

.calories-display:hover {
  transform: scale(1.05);
}

.digit {
  background: rgba(0, 0, 0, 0.15);
  padding: 8px 12px;
  border-radius: 4px;
  margin: 0 2px;
  transition: all 0.3s ease;
  color: white;
  font-weight: bold;
}

/* 图表容器优化 */
.chart-container {
  height: 500px;
  margin: 32px 0;
  background: white;
  border-radius: 12px;
  padding: 20px;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.05);
}

/* 按钮优化 */
.danger-btn {
  background: linear-gradient(45deg, var(--danger-color), #ff1744);
  padding: 14px 28px;
  border-radius: 8px;
  transition: all 0.3s ease;
  display: flex;
  align-items: center;
  gap: 8px;
  border: none;
  color: white;
  cursor: pointer;
}

.danger-btn:hover {
  transform: translateY(-2px);
  box-shadow: 0 4px 12px rgba(255, 82, 82, 0.3);
}

/* 响应式设计 */
@media (max-width: 768px) {
  .nav-bar {
    flex-wrap: wrap;
    padding: 12px;
    gap: 12px;
  }

  .nav-btn {
    flex: 1 1 45%;
    justify-content: center;
  }

  .chart-container {
    height: 400px;
    padding: 10px;
  }
}

@media (max-width: 480px) {
  body {
    padding: 10px;
  }

  .page-container {
    padding: 20px;
  }

  .quick-stats {
    grid-template-columns: 1fr;
  }
}

/* 加载动画 */
@keyframes fadeIn {
  from { opacity: 0; transform: translateY(20px); }
  to { opacity: 1; transform: translateY(0); }
}

.page {
  animation: fadeIn 0.6s ease-out;
}

.alert-banner {
  background: var(--danger-color);
  color: white;
  padding: 10px;
  border-radius: 5px;
  margin: 10px 0;
}

.user-list {
  display: grid;
  gap: 15px;
  margin: 20px 0;
}

.user-card {
  padding: 15px;
  border: 1px solid #eee;
  border-radius: 8px;
  position: relative;
  background: white;
}

.online-status {
  width: 10px;
  height: 10px;
  border-radius: 50%;
  position: absolute;
  right: 15px;
  top: 15px;
}

.online {
  background: var(--primary-color);
}

.offline {
  background: #95a5a6;
}

.button-group {
  display: flex;
  gap: 10px;
  margin-top: 15px;
}

.auth-form {
  max-width: 400px;
  margin: 50px auto;
  padding: 30px;
  background: white;
  border-radius: 16px;
  box-shadow: var(--card-shadow);
}

.form-input {
  width: 100%;
  padding: 12px;
  margin: 10px 0;
  border: 1px solid #ddd;
  border-radius: 8px;
  box-sizing: border-box;
}

.form-button {
  width: 100%;
  padding: 12px;
  margin: 10px 0;
  border: none;
  border-radius: 8px;
  background: var(--secondary-color);
  color: white;
  cursor: pointer;
}

.form-button:hover {
  background: #1976D2;
}
</style>
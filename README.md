# Step Counter 项目

这是一个基于STM32、Spring Boot和Vue.js的计步器项目，实现从硬件数据采集到前端展示的完整系统。

## 项目结构

- **step_counter_front_end**: Vue.js前端应用，提供用户界面和数据可视化
- **step_counter_back_end**: Spring Boot后端服务，处理数据存储和API接口
- **StepCounterSTM32**: STM32嵌入式代码，负责传感器数据采集和计步算法

## 功能特性

- 实时计步数据采集（STM32）
- 健康数据存储和管理（后端）
- 用户管理（注册、登录、权限控制）
- 数据可视化（前端图表展示）
- 跨平台支持

## 技术栈

### 前端
- Vue.js 3
- Vue Router
- Axios
- Vite

### 后端
- Spring Boot
- MySQL
- Redis
- JWT认证

### 嵌入式
- STM32F407
- LSM9DS0传感器
- LCD显示屏

## 安装和运行

### 前端
```bash
cd step_counter_front_end
npm install
npm run dev
```

### 后端
```bash
cd step_counter_back_end
# 确保安装了Maven和Java
mvn spring-boot:run
```

### STM32
使用STM32CubeIDE打开StepCounterSTM32项目，编译并烧录到开发板。

## 使用说明

1. 启动后端服务
2. 启动前端应用
3. STM32设备采集数据并通过串口发送到后端
4. 在前端查看计步数据和健康统计
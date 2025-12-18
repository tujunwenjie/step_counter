import axios from 'axios'

const BASE_URL = 'http://localhost:8080'

// 创建axios实例
const api = axios.create({
    baseURL: BASE_URL,
    timeout: 10000,
    withCredentials: true // 允许携带cookie
})

// 请求拦截器 - 添加token
api.interceptors.request.use(
    (config) => {
        const token = getCookie('token')
        if (token) {
            config.headers.Authorization = `Bearer ${token}`
        }
        return config
    },
    (error) => {
        return Promise.reject(error)
    }
)

// 响应拦截器 - 处理错误
api.interceptors.response.use(
    (response) => {
        return response.data
    },
    (error) => {
        if (error.response?.status === 401) {
            // token过期或无效，跳转到登录页
            window.location.href = '/'
        }
        return Promise.reject(error)
    }
)

// Cookie工具函数
function getCookie(name) {
    const value = `; ${document.cookie}`
    const parts = value.split(`; ${name}=`)
    if (parts.length === 2) return parts.pop().split(';').shift()
    return null
}

function setCookie(name, value, days) {
    const expires = new Date(Date.now() + days * 24 * 60 * 60 * 1000).toUTCString()
    document.cookie = `${name}=${value}; expires=${expires}; path=/`
}

function deleteCookie(name) {
    document.cookie = `${name}=; expires=Thu, 01 Jan 1970 00:00:00 GMT; path=/`
}

// 认证相关API
export const authAPI = {
    login: (data) => api.post('/user/login', data),
    register: (data) => api.post('/user/register', data),
    logout: () => api.post('/user/logout')
}

// 用户相关API
export const userAPI = {
    getCurrentUser: (userId) => api.get(`/user/get-current-user/${userId}`),
    getUsernameFromCookie: () => api.post('/user/get-username-from-cookie'),
    setUserAdmin: (userId, isAdmin) => api.post(`/user/set-admin/${userId}/${isAdmin}`),
    getUsersWithLimit: (num) => api.post(`/user/get-users-with-limit/${num}`),
    sendReminderToUser: (userId) => api.post(`/user/send-reminder/${userId}`),
    resetUserSteps: (userId) => api.post(`/user/reset-steps/${userId}`)
}

// 健康记录相关API
export const healthAPI = {
    getCurrentUserLatestRecord: () => api.get('/health-record/get-current-user-latest-record'),
    getLatestRecord: (userId) => api.get(`/health-record/get-latest-record-by-id/${userId}`),
    getChartData: (numOfRecords) => api.get(`/health-record/get-real-time-data?numOfRecords=${numOfRecords}`),
    getRealTimeData: (numOfRecords) => api.get(`/health-record/get-real-time-data?numOfRecords=${numOfRecords}`)
}

// 每日汇总API
export const dailySummaryAPI = {
    getSummariesWithLimit: (limit) => api.get(`/daily-summary/get-summaries-with-limit?num=${limit}`)
}

export { getCookie, setCookie, deleteCookie }
import { getCookie } from './api'

export function checkAuth() {
    const userId = getCookie('userId')
    const token = getCookie('token')
    return !!(userId && token)
}

export function getCurrentUserInfo() {
    return {
        userId: getCookie('userId'),
        userName: getCookie('userName'),
        isAdmin: getCookie('isAdmin') === 'true'
    }
}

export function clearAuth() {
    document.cookie.split(';').forEach(cookie => {
        const eqPos = cookie.indexOf('=')
        const name = eqPos > -1 ? cookie.substr(0, eqPos).trim() : cookie.trim()
        document.cookie = `${name}=;expires=Thu, 01 Jan 1970 00:00:00 GMT;path=/`
    })
}
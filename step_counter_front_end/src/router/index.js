import { createRouter, createWebHistory } from 'vue-router'
import { checkAuth } from '../utils/auth'

const routes = [
  {
    path: '/',
    name: 'Auth',
    component: () => import('../views/Auth.vue')
  },
  {
    path: '/common_home',
    name: 'CommonHome',
    component: () => import('../views/CommonHome.vue'),
    meta: { requiresAuth: true }
  },
  {
    path: '/admin',
    name: 'AdminHome',
    component: () => import('../views/AdminHome.vue'),
    meta: { requiresAuth: true, requiresAdmin: true }
  },
  {
    path: '/monitor',
    name: 'Monitor',
    component: () => import('../views/Monitor.vue'),
    meta: { requiresAuth: true, requiresAdmin: true }
  },
  {
    path: '/user-management',
    name: 'Admin',
    component: () => import('../views/Admin.vue'),
    meta: { requiresAuth: true, requiresAdmin: true }
  },
  {
    path: '/access-denied',
    name: 'AccessDenied',
    component: () => import('../views/AccessDenied.vue')
  }
]

const router = createRouter({
  history: createWebHistory(),
  routes
})

router.beforeEach((to, from, next) => {
  const isAuthenticated = checkAuth()
  const isAdmin = document.cookie.includes('isAdmin=true')

  if (to.meta.requiresAuth && !isAuthenticated) {
    next('/')
  } else if (to.meta.requiresAdmin && !isAdmin) {
    next('/access-denied')
  } else {
    next()
  }
})

export default router
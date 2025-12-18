package com.tu.interceptor;


import com.tu.utils.JwtUtil;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.stereotype.Component;
import org.springframework.web.servlet.HandlerInterceptor;

@Component
public class AuthInterceptor implements HandlerInterceptor {
    @Autowired
    private JwtUtil jwtUtil;

    @Autowired
    private RedisTemplate<String, Object> redisTemplate;

    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
        // 放行OPTIONS预检请求
        if ("OPTIONS".equalsIgnoreCase(request.getMethod())) {
            return true;
        }

        // 放行登录、注册和健康检查接口
        String path = request.getRequestURI();
        if (path.equals("/") || path.equals("/api/health") ||
                path.equals("/user/login") || path.equals("/user/register") ||
                path.equals("/api/user/login") || path.equals("/api/user/register")) {
            return true;
        }

        String token = request.getHeader("Authorization");
        if (token == null || !token.startsWith("Bearer ")) {
            response.setStatus(HttpServletResponse.SC_UNAUTHORIZED);
            return false;
        }

        token = token.substring(7);
        if (!jwtUtil.validateToken(token)) {
            response.setStatus(HttpServletResponse.SC_UNAUTHORIZED);
            return false;
        }

        // 从 token 中获取用户ID与用户名并设置到请求头中
        Integer userId = jwtUtil.getUserIdFromToken(token);
        request.setAttribute("userId", userId);
        String username = jwtUtil.getUsernameFromToken(token);
        request.setAttribute("username", username);

        // 验证 Redis 中的 token
        String redisKey = "user:token:" + userId;
        String redisToken = (String) redisTemplate.opsForValue().get(redisKey);
        if (!token.equals(redisToken)) {
            response.setStatus(HttpServletResponse.SC_UNAUTHORIZED);
            return false;
        }

        return true;
    }

}

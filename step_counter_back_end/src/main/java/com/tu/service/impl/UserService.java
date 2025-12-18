package com.tu.service.impl;

import com.tu.pojo.dto.LoginRequest;
import com.tu.pojo.dto.RegisterRequest;
import com.tu.pojo.entity.User;
import com.tu.mapper.UserMapper;
import com.tu.utils.JwtUtil;
import com.tu.utils.Result;
import jakarta.servlet.http.Cookie;
import jakarta.servlet.http.HttpServletResponse;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.DigestUtils;
import org.springframework.web.bind.annotation.CookieValue;

import java.time.LocalDateTime;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.TimeUnit;

@Slf4j
@Service
public class UserService{
    
    @Autowired
    private UserMapper userMapper;
    
    @Autowired
    private JwtUtil jwtUtil;
    
    @Autowired
    private RedisTemplate<String, Object> redisTemplate;

    /**
     * 根据username去数据库查找相同username的用户，
     * 如果相同加密算法后password相同则检查用户是否可用，
     * 若可用，则生成Token保存在redis   key = "user:token:{id}" 过期时间为24小时
     * 更新在线状态到cookie 包括"userId" "userName" "isAdmin"
     * @param request 登录请求封装类
     * @return HashMap  keys包括{"token","user","redirect"}
     */
    @Transactional
    public Result<Map<String, Object>> login(LoginRequest request, HttpServletResponse response) {
        User user = new User();
        try {
            user = userMapper.selectByName(request.getUsername());

            System.out.println(user);

            if (user == null) {
                return Result.error("用户不存在");
            }
            
            String encryptedPassword = DigestUtils.md5DigestAsHex(request.getPassword().getBytes());
            if (!user.getPassword().equals(encryptedPassword)) {
                return Result.error("密码错误");
            }
            
            if (!user.getStatus()) {
                return Result.error("账户已被冻结");
            }


            
            // 生成 token
            String token = jwtUtil.generateToken(user.getId(), user.getName());

            // 存储到 Redis
            String redisKey = "user:token:" + user.getId();
            redisTemplate.opsForValue().set(redisKey, token, 24, TimeUnit.HOURS);




            // 更新在线状态
            userMapper.updateOnlineStatus(user.getId(), true);
            Cookie userIdCookie = new Cookie("userId", user.getId()+"");
            userIdCookie.setMaxAge(24*60*60);
            userIdCookie.setPath("/");
            response.addCookie(userIdCookie);

            Cookie userNameCookie = new Cookie("userName", user.getName());
            userNameCookie.setMaxAge(24*60*60);
            userNameCookie.setPath("/");
            response.addCookie(userNameCookie);

            Cookie isAdminCookie = new Cookie("isAdmin", user.getIsAdmin()+"");
            isAdminCookie.setMaxAge(24*60*60);
            isAdminCookie.setPath("/");
            response.addCookie(isAdminCookie);

            // 封装结果
            Map<String, Object> result = new HashMap<>();
            result.put("token", token);
            result.put("user", toUserDTO(user));
            result.put("redirect", user.getIsAdmin() ? "/admin" : "/common_home");

            return Result.success("登录成功", result);
        } catch (Exception e) {
            log.error("登录错误", e);
            clearAuthCookies(response);
            if (user != null) redisTemplate.delete("user:token:"+user.getId());
            return Result.error("登录错误");
        }
    }
    
    
    public Result<String> register(RegisterRequest request) {
        try {
            if (userMapper.selectByName(request.getUsername()) != null) {
                return Result.error("用户名已存在");
            }
            
            User user = new User();
            user.setName(request.getUsername());
            user.setPassword(DigestUtils.md5DigestAsHex(request.getPassword().getBytes()));
            user.setCreateTime(LocalDateTime.now());
            user.setIsAdmin(false);
            user.setIsOnline(false);
            user.setStatus(true);
            
            userMapper.insert(user);
            return Result.success("注册成功");
        } catch (Exception e) {
            log.error("注册失败", e);
            return Result.error("注册失败");
        }
    }
    
    @Transactional
    public Result<String> logout(Integer userId,HttpServletResponse response) {
        try {
            userMapper.updateOnlineStatus(userId, false);
            clearAuthCookies(response);
            redisTemplate.delete("user:token:" + userId);
            return Result.success("退出成功");
        } catch (Exception e) {
            log.error("退出失败", e);
            return Result.error("退出失败");
        }
    }

    public Result<User> getUserInfo(String token) throws Exception {
        try {
            Integer id = jwtUtil.getUserIdFromToken(token);
            return Result.success(userMapper.selectById(id));
        } catch (Exception e) {
            log.error("获取用户信息失败",e);
            return Result.error("获取用户信息失败");
        }
    }
    
    public Result<User> getCurrentUser(Integer userId) {
        try {
            User user = userMapper.selectById(userId);
            if (user == null) {
                return Result.error("用户不存在");
            }
            return Result.success(toUserDTO(user));
        } catch (Exception e) {
            log.error("获取用户信息失败", e);
            return Result.error("获取用户信息失败");
        }
    }

    public Result<String> updateUserInfo(Integer userId,String name,String password) {
        try {
            int i = userMapper.updateUser(userId, name, password);
            return Result.success("更新用户信息完成");
        } catch (Exception e) {
            log.error("更新用户信息失败",e);
            return Result.error("更新用户信息失败");
        }

    }
    
    
    public Result<List<User>> getAllUsers() {
        try {
            List<User> users = userMapper.selectAll();
            users.forEach(this::updateUserOnlineStatus);
            return Result.success(users);
        } catch (Exception e) {
            log.error("获取用户列表失败", e);
            return Result.error("获取用户列表失败");
        }
    }

    
    public Result<User> getUserById(Integer userId) {
        try {
            User user = userMapper.selectById(userId);
            return Result.success(toUserDTO(user));
        }catch (Exception e) {
            log.error("获取ID为{}的用户失败", userId, e);
            return Result.error("获取指定ID用户失败");
        }
    }

    
    
    public Result<String> updateUserActivity(Integer userId) {
        try {
            userMapper.updateLastActivity(userId);
            return Result.success("活动状态更新成功");
        } catch (Exception e) {
            log.error("更新活动状态失败", e);
            return Result.error("更新活动状态失败");
        }
    }
    
    
    public Result<String> setUserAdmin(Integer userId, Boolean isAdmin) {
        try {
            userMapper.updateAdminStatus(userId, isAdmin);
            return Result.success(isAdmin ? "设置为管理员成功" : "取消管理员成功");
        } catch (Exception e) {
            log.error("设置管理员失败", e);
            return Result.error("设置管理员失败");
        }
    }


    private User toUserDTO(User user) {
        User dto = new User();
        dto.setId(user.getId());
        dto.setName(user.getName());
        dto.setIsAdmin(user.getIsAdmin());
        dto.setIsOnline(user.getIsOnline());
        dto.setLastActivity(user.getLastActivity());
        dto.setCreateTime(user.getCreateTime());
        dto.setStatus(user.getStatus());
        return dto;
    }

    /**
     * 清除认证相关的 Cookie
     */
    private void clearAuthCookies(HttpServletResponse response) {
        String[] cookieNames = {"userId", "userName","isAdmin"};

        for (String cookieName : cookieNames) {
            Cookie cookie = new Cookie(cookieName, "");
            cookie.setPath("/");
            cookie.setMaxAge(0); // 立即过期
            response.addCookie(cookie);
        }

        log.info("清除认证 Cookie 成功");
    }

    private void updateUserOnlineStatus(User user) {
        String redisKey = "user:token:" + user.getId();
        Boolean hasToken = redisTemplate.hasKey(redisKey);
        user.setIsOnline(hasToken != null && hasToken);
    }

    public Result<List<User>> getUsersWithLimit(int limit) {
        List<User> users = userMapper.selectWithLimit(limit);
        return Result.success(users);
    }
}
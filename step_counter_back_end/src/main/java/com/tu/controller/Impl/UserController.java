package com.tu.controller.Impl;

import com.tu.pojo.dto.LoginRequest;
import com.tu.pojo.dto.RegisterRequest;
import com.tu.pojo.dto.UpdateRequest;
import com.tu.pojo.entity.User;
import com.tu.service.impl.HealthService;
import com.tu.service.impl.UserService;
import com.tu.utils.Result;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Map;

/*
* export const userAPI = {
    getCurrentUser: () => api.get('/get-current-user'),
    getAllUsers: () => api.get('/get-all-users'),
    checkUserStatus: () => api.get('/check-user-status'),
    sendReminderToAll: () => api.post('/send-reminder'),
    resetAllSteps: () => api.post('/reset-all-steps'),
    sendReminderToUser: (userId) => api.post(`/send-reminder/${userId}`),
    resetUserSteps: (userId) => api.post(`/reset-steps/${userId}`),
    setUserAdmin: (userId, isAdmin) => api.post(`/set-admin/${userId}`, { is_admin: isAdmin })
}
* */


@RestController
@RequestMapping("/user")
public class UserController {

    @Autowired
    private UserService userService;
    @Autowired
    private HealthService healthService;


    @GetMapping("/get-current-user/{userId}")
    @ResponseBody
    public Result<User> getCurrentUser(@PathVariable Integer userId) {

        if (userId != null) {
            return userService.getCurrentUser(userId);
        }

        return Result.error("用户未登录");
    }


    @GetMapping("/check-user-status")
    public Result<String> checkUserStatus() {
        return Result.success("用户状态正常");
    }


    @PostMapping("/send-reminder")
    public Result<String> sendReminder() {
        return Result.success("提醒发送成功");
    }


    //    sendReminderToUser: (userId) => api.post(`/send-reminder/${userId}`),
    @PostMapping("/send-reminder/{userId}")
    public Result<String> sendReminder(@PathVariable Integer userId) {
        return Result.success("已给指定用户（" + userService.getUserById(userId) + "）发送提醒");
    }

    //    resetUserSteps: (userId) => api.post(`/reset-steps/${userId}`),
    @PostMapping("/reset-steps/{userId}")
    public Result<String> resetSteps(@PathVariable Integer userId) throws Exception {
        try {
            return healthService.resetSteps(userId);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    //    setUserAdmin: (userId, isAdmin) => api.post(`/set-admin/${userId}`, { is_admin: isAdmin })
    @PostMapping("/set-admin/{userId}/{isAdmin}")
    public Result<String> setUserAdmin(@PathVariable Integer userId, @PathVariable Boolean isAdmin) {
        return userService.setUserAdmin(userId, isAdmin);
    }

    @PostMapping("/login")
    public Result<Map<String, Object>> login(@RequestBody LoginRequest request, HttpServletResponse response) throws Exception {
        return userService.login(request, response);
    }

    @PostMapping("/logout")
    public Result<String> logout(@CookieValue("userId") Integer userId, HttpServletResponse response) throws Exception {
        return userService.logout(userId, response);
    }


    @PostMapping("/register")
    public Result<String> register(@RequestBody RegisterRequest request) throws Exception {
        return userService.register(request);
    }

    @PostMapping("/get-username-from-cookie")
    public Result<String> getUsernameFromCookie(@CookieValue("token") String token) throws Exception {
        String name = userService.getUserInfo(token).getData().getName();
        return Result.success(name);
    }

    @PostMapping("/update-user-info")
    public Result<String> updateUserInfo(@RequestBody UpdateRequest updateRequest, @CookieValue("userId") Integer userId) throws Exception {
        return userService.updateUserInfo(userId, updateRequest.getUsername(), updateRequest.getPassword());
    }

    @PostMapping("/get-users-with-limit/{num}")
    public Result<List<User>> getUsersWithLimit(@PathVariable("num") int num) throws Exception {
        return userService.getUsersWithLimit(num);
    }


}

package com.tu.pojo.entity;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import java.time.LocalDate;
import java.time.LocalDateTime;


@Data
@NoArgsConstructor
@AllArgsConstructor
public class User {
    // 主键ID
    private Integer id;

    // 用户名
    private String name;

    // 用户的密码
    private String password;

    // 是否为管理员，1代表是管理员，0代表普通用户
    private Boolean isAdmin;

    // 用户是否在线，1代表用户当前在线，0代表离线
    private Boolean isOnline;

    // 用户的最后活动时间
    private LocalDateTime lastActivity;

    // 用户创建时间
    private LocalDateTime createTime;

    // 用户状态，1代表可用，0代表被冻结
    private Boolean status;
}

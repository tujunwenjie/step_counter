package com.tu.pojo.entity;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.RequiredArgsConstructor;

import java.math.BigDecimal;
import java.time.LocalDateTime;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class HealthRecord {
//    记录ID，自增
    private Integer id;
//    用户ID
    private Integer userId;
//    用户当前累计步数
    private Integer steps;
//    用户累计消耗的卡路里（千卡）
    private BigDecimal calories;
//    记录保存的时间
    private LocalDateTime recordedAt;

    public HealthRecord(Integer userId, Integer steps, BigDecimal calories) {
        this.userId = userId;
        this.steps = steps;
        this.calories = calories;
        this.recordedAt = LocalDateTime.now();
    }
}

package com.tu.pojo.entity;


import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.math.BigDecimal;
import java.time.LocalDate;
import java.time.LocalDateTime;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class DailySummary {
//    每日汇总主键ID，自增
    private Integer id;

//    用户ID
    private Integer userId;

//    汇总的日期
    private LocalDate date;

//    汇总日期当天运动的步数总数
    private Integer totalSteps;

//    汇总日期当天消耗的卡路里总数（千卡）
    private LocalDateTime lastUpdate;

//    更新时间
    private BigDecimal totalCalories;
}

package com.tu.service.impl;

import com.tu.pojo.dto.RemindThresholdUpdateRequest;
import com.tu.pojo.entity.DailySummary;
import com.tu.pojo.entity.HealthRecord;
import com.tu.mapper.DailySummaryMapper;
import com.tu.mapper.HealthRecordMapper;
import com.tu.utils.Result;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@Slf4j
@Service
public class HealthService {


    @Autowired
    private HealthRecordMapper healthRecordMapper;

    @Autowired
    private DailySummaryMapper dailySummaryMapper;

    @Autowired
    private UserService userService;

    @Transactional
    public Result<Map<String, Object>> updateSteps(Integer userId, RemindThresholdUpdateRequest request) {
        try {
            // 更新用户活动状态
            userService.updateUserActivity(userId);

            // 插入健康记录
            HealthRecord record = new HealthRecord();
            record.setUserId(userId);
            record.setSteps(request.getSteps());
            record.setCalories(request.getCalories());
            healthRecordMapper.insert(record);

            // 更新每日汇总
            updateDailySummary(userId, request);

            Map<String, Object> result = new HashMap<>();
            result.put("steps", request.getSteps());
            result.put("calories", request.getCalories());
            result.put("timestamp", LocalDateTime.now());

            return Result.success("步数更新成功", result);
        } catch (Exception e) {
            log.error("更新步数失败", e);
            return Result.error("更新步数失败");
        }
    }

    public Result<Map<String, Object>> getTodayData(Integer userId) {
        try {
            HealthRecord todayRecord = healthRecordMapper.selectTodayByUserId(userId);
            DailySummary todaySummary = dailySummaryMapper.selectTodayByUserId(userId);

            Map<String, Object> result = new HashMap<>();
            result.put("steps", todayRecord != null ? todayRecord.getSteps() : 0);
            result.put("calories", todayRecord != null ? todayRecord.getCalories() : BigDecimal.ZERO);
            result.put("totalSteps", todaySummary != null ? todaySummary.getTotalSteps() : 0);
            result.put("totalCalories", todaySummary != null ? todaySummary.getTotalCalories() : BigDecimal.ZERO);

            return Result.success(result);
        } catch (Exception e) {
            log.error("获取今日数据失败", e);
            return Result.error("获取今日数据失败");
        }
    }

    public Result<List<HealthRecord>> getRealtimeData(Integer userId, Integer limit) {
        try {
            List<HealthRecord> records = healthRecordMapper.selectByUserIdWithLimit(userId, limit);
            return Result.success(records);
        } catch (Exception e) {
            log.error("获取实时数据失败", e);
            return Result.error("获取实时数据失败");
        }
    }

    public Result<List<HealthRecord>> getAllRealtimeData(Integer limit) {
        try {
            List<HealthRecord> records = healthRecordMapper.selectAllWithLimit(limit);
            return Result.success(records);
        } catch (Exception e) {
            log.error("获取所有实时数据失败", e);
            return Result.error("获取所有实时数据失败");
        }
    }

    public Result<List<DailySummary>> getHistoryData(Integer userId, Integer limit) {
        try {
            List<DailySummary> summaries = dailySummaryMapper.selectByUserIdWithLimit(userId, limit);
            return Result.success(summaries);
        } catch (Exception e) {
            log.error("获取历史数据失败", e);
            return Result.error("获取历史数据失败");
        }
    }

    @Transactional
    public Result<String> resetSteps(Integer userId) {
        try {
            healthRecordMapper.resetStepsByUserId(userId);

            // 重置今日汇总
            DailySummary summary = new DailySummary();
            summary.setUserId(userId);
            summary.setTotalSteps(0);
            summary.setTotalCalories(BigDecimal.ZERO);
            dailySummaryMapper.update(summary);

            return Result.success("步数重置成功");

        } catch (Exception e) {
            log.error("重置步数失败", e);
            return Result.error("重置步数失败");
        }
    }

    @Transactional
    public Result<String> resetAllSteps() {
        try {
            // 这里需要实现重置所有用户步数的逻辑
            // 由于表结构限制，可能需要遍历所有用户进行重置
            return Result.success("所有用户步数重置成功");
        } catch (Exception e) {
            log.error("重置所有步数失败", e);
            return Result.error("重置所有步数失败");
        }
    }

    private void updateDailySummary(Integer userId, RemindThresholdUpdateRequest request) {
        HealthRecord todaySummary = healthRecordMapper.selectTodaySummary(userId);

        DailySummary summary = new DailySummary();
        summary.setUserId(userId);
        summary.setTotalSteps(todaySummary != null ? todaySummary.getSteps() : request.getSteps());
        summary.setTotalCalories(todaySummary != null ? todaySummary.getCalories() : request.getCalories());

        dailySummaryMapper.update(summary);
    }

    public Result<List<HealthRecord>> getLatestRecordById(int userId) {
        List<HealthRecord> records = null;
        try {
            records = healthRecordMapper.selectByUserIdWithLimit(userId, 1);
            System.out.println(records);
        } catch (Exception e) {
            Result.error("查询出错");
            log.error(e.getMessage(), e);
        }
        return records==null ?
                Result.success("没有找到记录",List.of(new HealthRecord())) : Result.success(records);
    }

    public Result<List<HealthRecord>> getRecordsByIdWithLimit(int id, int numOfRecords) {
        List<HealthRecord> records = null;
        try {
            records = healthRecordMapper.selectByUserIdWithLimit(id, numOfRecords);
        } catch (Exception e) {
            Result.error("查询出错");
            log.error(e.getMessage(), e);
        }
        return records==null ?
                Result.success("没有找到记录",List.of(new HealthRecord())) : Result.success(records);
    }
}


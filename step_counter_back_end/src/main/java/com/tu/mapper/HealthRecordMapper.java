package com.tu.mapper;

import com.tu.pojo.entity.HealthRecord;
import org.apache.ibatis.annotations.*;

import java.util.List;

@Mapper
public interface HealthRecordMapper {
    
    @Insert("INSERT INTO health_records(user_id, steps, calories, recorded_at) VALUES(#{userId}, #{steps}, #{calories}, NOW())")
    int insert(HealthRecord record);
    
    @Select("SELECT * FROM health_records WHERE user_id = #{userId} ORDER BY recorded_at DESC LIMIT #{limit}")
    List<HealthRecord> selectByUserIdWithLimit(@Param("userId") Integer userId, @Param("limit") Integer limit);
    
    @Select("SELECT * FROM health_records ORDER BY recorded_at DESC LIMIT #{limit}")
    List<HealthRecord> selectAllWithLimit(Integer limit);
    
    @Select("SELECT * FROM health_records WHERE user_id = #{userId} AND DATE(recorded_at) = CURDATE() ORDER BY recorded_at DESC LIMIT 1")
    HealthRecord selectTodayByUserId(Integer userId);
    
    @Delete("DELETE FROM health_records WHERE user_id = #{userId}")
    int deleteByUserId(Integer userId);

    @Update("update health_records set steps = 0, health_records.calories = 0.0, recorded_at = now() where user_id = #{userId}")
    int resetStepsByUserId(int userId);
    
    @Select("SELECT SUM(steps) as total_steps, SUM(calories) as total_calories FROM health_records WHERE user_id = #{userId} AND DATE(recorded_at) = CURDATE()")
    HealthRecord selectTodaySummary(Integer userId);
}
package com.tu.mapper;

import com.tu.pojo.entity.DailySummary;
import org.apache.ibatis.annotations.*;

import java.util.List;

@Mapper
public interface DailySummaryMapper {
    
    @Insert("update daily_summaries  " +
            "set total_steps = #{totalSteps},total_calories = #{totalCalories}, daily_summaries.last_update = NOW()" +
            "where user_id = #{userId} and date = CURDATE()" )
    int update(DailySummary summary);
    
    @Select("SELECT * FROM daily_summaries WHERE user_id = #{userId} ORDER BY date DESC LIMIT #{limit}")
    List<DailySummary> selectByUserIdWithLimit(@Param("userId") Integer userId, @Param("limit") Integer limit);
    
    @Select("SELECT * FROM daily_summaries WHERE user_id = #{userId} AND date = CURDATE()")
    DailySummary selectTodayByUserId(Integer userId);
}
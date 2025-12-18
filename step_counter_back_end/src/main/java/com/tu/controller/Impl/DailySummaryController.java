package com.tu.controller.Impl;

import com.tu.pojo.entity.DailySummary;
import com.tu.pojo.entity.HealthRecord;
import com.tu.service.impl.HealthService;
import com.tu.utils.Result;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/daily-summary")
public class DailySummaryController {

    @Autowired
    private HealthService healthService;

    @GetMapping("/get-summaries-with-limit")
    public Result<List<DailySummary>> getDailySummariesWithLimit(@CookieValue("userId") Integer userId, @RequestParam("num") int limit) {
        return healthService.getHistoryData(userId, limit);
    }

}

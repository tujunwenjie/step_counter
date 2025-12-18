package com.tu.controller.Impl;

import com.tu.pojo.entity.HealthRecord;
import com.tu.service.impl.HealthService;
import com.tu.utils.Result;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/health-record")
public class HealthRecordController {
    @Autowired
    private HealthService healthService;


    @GetMapping("/get-current-user-latest-record")
    public Result<List<HealthRecord>> getCurrentUserLatestRecord(@CookieValue("userId") int id) {
        return healthService.getLatestRecordById(id);
    }

    @GetMapping("/get-latest-record-by-id/{userId}")
    public Result<List<HealthRecord>> getLatestRecordById(@PathVariable("userId") int id) {
        return healthService.getLatestRecordById(id);
    }

    /**
     * 获取最近的numOfRecords条记录
     * @param id
     * @param numOfRecords
     * @return
     */

    @GetMapping("/get-real-time-data")
    public Result<List<HealthRecord>> getRealTimeData(@CookieValue("userId") int id,@RequestParam int numOfRecords) {
        return healthService.getRecordsByIdWithLimit(id,numOfRecords);
    }


}

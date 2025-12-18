package com.tu.pojo.dto;

import lombok.Data;
import java.math.BigDecimal;

@Data
public class RemindThresholdUpdateRequest {
    private Integer steps;
    private BigDecimal calories;
}
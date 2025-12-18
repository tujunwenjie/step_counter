package com.tu.utils;

import com.tu.pojo.entity.HealthRecord;

import java.math.BigDecimal;
import java.nio.charset.StandardCharsets;

public class HealthDataParser {

    public static HealthRecord parsePacket(byte[] data) {
        if (data.length != 20) {
            throw new IllegalArgumentException("数据包长度必须为20字节");
        }

        // 验证包头包尾
        if (data[0] != 0x55 || data[1] != (byte) 0xAA || data[18] != (byte) 0xAA || data[19] != 0x55) {
            throw new IllegalArgumentException("数据包格式错误");
        }

        // 解析用户ID
        int userId = ((data[2] & 0xFF) << 24) |
                ((data[3] & 0xFF) << 16) |
                ((data[4] & 0xFF) << 8) |
                (data[5] & 0xFF);

        // 解析步数
        int stepCount = ((data[6] & 0xFF) << 24) |
                ((data[7] & 0xFF) << 16) |
                ((data[8] & 0xFF) << 8) |
                (data[9] & 0xFF);

        // 解析卡路里字符串
        byte[] caloriesBytes = new byte[8];
        System.arraycopy(data, 10, caloriesBytes, 0, 8);
        String caloriesStr = new String(caloriesBytes, StandardCharsets.US_ASCII).trim();
        BigDecimal calories = new BigDecimal(caloriesStr);

        return new HealthRecord(userId, stepCount, calories);
    }
}
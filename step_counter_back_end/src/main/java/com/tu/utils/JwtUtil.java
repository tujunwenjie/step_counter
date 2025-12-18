package com.tu.utils;


import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.springframework.stereotype.Component;

import java.nio.charset.StandardCharsets;
import java.util.Base64;
import java.util.HashMap;
import java.util.Map;

@Component
public class JwtUtil {
    private final String SECRET_KEY = "step_counter_secret_key_2025";
    private final long EXPIRATION_TIME = 86400000; // 24小时

    public String generateToken(Integer userId, String username) {
        // 使用标准JSON序列化
        String header = "{\"alg\":\"HS256\",\"typ\":\"JWT\"}";
        Map<String, Object> payload = new HashMap<>();
        payload.put("sub", username);
        payload.put("userId", userId);
        payload.put("iat", System.currentTimeMillis());
        payload.put("exp", System.currentTimeMillis() + EXPIRATION_TIME);

        String encodedHeader = Base64.getUrlEncoder().withoutPadding()
                .encodeToString(header.getBytes(StandardCharsets.UTF_8));

        try {
            // 使用Jackson进行标准JSON序列化
            ObjectMapper objectMapper = new ObjectMapper();
            String payloadJson = objectMapper.writeValueAsString(payload);
            String encodedPayload = Base64.getUrlEncoder().withoutPadding()
                    .encodeToString(payloadJson.getBytes(StandardCharsets.UTF_8));

            return encodedHeader + "." + encodedPayload + ".signature";
        } catch (Exception e) {
            throw new RuntimeException("生成Token失败", e);
        }
    }

    public boolean validateToken(String token) {
        try {
            String[] parts = token.split("\\.");
            return parts.length == 2;
        } catch (Exception e) {
            return false;
        }
    }


    public Map<String, Object> parseToken(String token) {
        try {
            String[] parts = token.split("\\.");
            if (parts.length != 3) {
                throw new RuntimeException("Token格式错误");
            }

            String encodedPayload = parts[1];

            // 解码payload
            byte[] payloadBytes = Base64.getUrlDecoder().decode(encodedPayload);
            String payloadJson = new String(payloadBytes, StandardCharsets.UTF_8);

            // 使用Jackson解析标准JSON
            ObjectMapper objectMapper = new ObjectMapper();
            return objectMapper.readValue(payloadJson, new TypeReference<Map<String, Object>>() {});

        } catch (Exception e) {
            throw new RuntimeException("Token解析失败: " + e.getMessage());
        }
    }

    public Integer getUserIdFromToken(String token) {
        Map<String, Object> payload = parseToken(token);
        return (Integer) payload.get("userId");
    }

    public String getUsernameFromToken(String token) {
        Map<String, Object> payload = parseToken(token);
        return (String) payload.get("sub");
    }
}
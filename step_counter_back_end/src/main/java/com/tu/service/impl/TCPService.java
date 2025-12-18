package com.tu.service.impl;

import com.tu.mapper.HealthRecordMapper;
import com.tu.pojo.entity.HealthRecord;
import com.tu.utils.HealthDataParser;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

// 服务端使用
@Slf4j
@Service
public class TCPService {

    private static final int PORT = 5001;
    private static final int PACKET_SIZE = 20;
    @Autowired
    private  HealthRecordMapper healthRecordMapper;
    private final ExecutorService threadPool = Executors.newFixedThreadPool(10);

    public void start() {
        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            System.out.println("TCP服务正在启动，监听：".concat(Inet4Address.getLocalHost().getHostAddress())+ ":" + PORT);

            while (!Thread.currentThread().isInterrupted()) {
                Socket socket = serverSocket.accept();
                System.out.println("客户端连接：" + socket.getRemoteSocketAddress());
                threadPool.execute(new ClientHandler(socket));
            }
        } catch (IOException e) {
            log.error(e.getMessage());
            System.out.println(e.getMessage());
        }
    }

    /**
     * 客户端处理类
     */
    private class ClientHandler implements Runnable {
        private final Socket socket;

        public ClientHandler(Socket socket) {
            this.socket = socket;
        }

        @Override
        public void run() {
            String clientAddress = socket.getRemoteSocketAddress().toString();
            System.out.println("开始处理客户端: " + clientAddress);

            try (InputStream input = socket.getInputStream();
                 OutputStream output = socket.getOutputStream()) {

                byte[] buffer = new byte[PACKET_SIZE];
                int bytesRead;

                // 持续读取数据
                while ((bytesRead = input.read(buffer)) != -1) {
                    if (bytesRead == PACKET_SIZE) {
                        // 完整接收到一个数据包
                        processPacket(buffer, output);
                    } else {
                        System.err.println("数据包长度错误: " + bytesRead + "字节，期望" + PACKET_SIZE + "字节");
                    }
                }

            } catch (IOException e) {
                System.err.println("客户端连接异常: " + e.getMessage());
            } catch (Exception e) {
                System.err.println("处理数据时出错: " + e.getMessage());
            } finally {
                try {
                    socket.close();
                    System.out.println("客户端断开连接: " + clientAddress);
                } catch (IOException e) {
                    System.err.println("关闭socket时出错: " + e.getMessage());
                }
            }
        }

        public void processPacket(byte[] receivedData, OutputStream output) {
            try {
                // 解析数据包
                HealthRecord healthData = HealthDataParser.parsePacket(receivedData);

                // 打印接收到的数据
                System.out.println("收到健康数据: " + healthData);

                // 保存到数据库
                saveToDatabase(healthData);

                // 发送响应给客户端
                sendResponse(output, "SUCCESS");

            } catch (Exception e) {
                System.err.println("解析数据包失败: " + e.getMessage());
                try {
                    sendResponse(output, "ERROR:" + e.getMessage());
                } catch (IOException ioException) {
                    System.err.println("发送错误响应失败: " + ioException.getMessage());
                }
            }
        }
        /**
         * 发送响应给客户端
         */
        private void sendResponse(OutputStream output, String response) throws IOException {
            byte[] responseBytes = response.getBytes(StandardCharsets.US_ASCII);
            output.write(responseBytes);
            output.flush();
            System.out.println("发送响应: " + response);
        }

        private void saveToDatabase(HealthRecord healthRecord) {
            System.out.println("将: " + healthRecord+ "   保存到数据库");

            try {
                healthRecordMapper.insert(healthRecord);
            } catch (Exception e) {
                log.error(e.getMessage());
                System.out.println(e.getMessage());
            }
        }
    }
}
package com.tu;

import com.tu.service.impl.TCPService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.context.event.ApplicationReadyEvent;
import org.springframework.context.event.EventListener;
import org.springframework.stereotype.Component;

@Component

public class TCPServiceStarter {
    @Autowired
    private TCPService tcpService;

    @EventListener(ApplicationReadyEvent.class)
    public void start() {
        tcpService.start();
    }
}

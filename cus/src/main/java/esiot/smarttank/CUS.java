package esiot.smarttank;

import java.util.Timer;
import java.util.TimerTask;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

public final class CUS {

    private static final String BROKER = "tcp://broker.mqtt-dashboard.com:1883";
    private static final String TOPIC_TMS = "tms/waterlevel"; 
    
    private static final double L1 = 60.0;
    private static final double L2 = 85.0;
    private static final long T1_LIMIT_MS = 3000;
    private static final long T2_TIMEOUT_MS = 5000;
    private static final int PORT = 5000;
    private static final int PERIOD = 100;

    public static class SystemState {
        public String mode = "AUTOMATIC";
        public double currentLevel = 0.0;
        public int valveOpening = 0;
        // transient per non comparire nel JSON
        public transient long lastMsgTime = System.currentTimeMillis();
        public transient long timeAboveL1Start = 0;
    }

    public final static SystemState state = new SystemState();
    protected static SerialComm serialComm;
    private static HttpServerTask httpTask;
    private static MqttClient mqttClient;

    public static void main(final String[] args) {
        serialComm = new SerialComm();
        if (serialComm.init("COM3")) {
            System.out.println("SERIAL: Connected");
        } else {
            System.err.println("SERIAL: Connection failed");
        }

        httpTask = new HttpServerTask();
        try {
            httpTask.startServer(PORT); 
        } catch (final Exception e) {
            e.printStackTrace();
        }

        setupMQTT();

        new Timer().scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                logicLoop();
            }
        }, 0, PERIOD); 
    }

    private static void setupMQTT() {
        try {
            final String clientId = "CUS_" + System.currentTimeMillis();
            mqttClient = new MqttClient(BROKER, clientId);
            mqttClient.setCallback(new MqttCallback() {
                public void connectionLost(final Throwable c) {}
                public void deliveryComplete(final IMqttDeliveryToken t) {}
                public void messageArrived(final String t, final MqttMessage m) {
                    try {
                        final String payload = new String(m.getPayload()).trim();
                        if (!payload.isEmpty()) {
                            state.currentLevel = Double.parseDouble(payload);
                            state.lastMsgTime = System.currentTimeMillis();
                        }
                    } catch (final Exception e) {
                        System.err.println("MQTT Parse Error: " + e.getMessage());
                    }
                }
            });
            mqttClient.connect();
            mqttClient.subscribe(TOPIC_TMS);
            System.out.println("MQTT: Subscribed");
        } catch (final MqttException e) {
            e.printStackTrace();
        }
    }

    private static void logicLoop() {
        final long now = System.currentTimeMillis();
        if (now - state.lastMsgTime > T2_TIMEOUT_MS) {
            if (!state.mode.equals("UNCONNECTED")) {
                state.mode = "UNCONNECTED";
                System.out.println("ALARM: Connection lost");
                serialComm.sendMode("UNCONNECTED");
            }
        } else if (state.mode.equals("UNCONNECTED")) {
            state.mode = "UNCONNECTED";
        }

        if (state.mode.equals("AUTOMATIC")) {
            if (state.currentLevel > L2) {
                state.valveOpening = 100;
                state.timeAboveL1Start = 0;
            } else if (state.currentLevel > L1) {
                if (state.timeAboveL1Start == 0) {
                    state.timeAboveL1Start = now;
                }
                if (now - state.timeAboveL1Start > T1_LIMIT_MS) {
                    state.valveOpening = 50;
                } else {
                    state.valveOpening = 0;
                }
            } else {
                state.valveOpening = 0;
                state.timeAboveL1Start = 0;
            }
        }
        serialComm.sendValve(state.valveOpening);
    }
}

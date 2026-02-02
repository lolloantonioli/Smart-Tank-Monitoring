package esiot.smarttank;

import java.util.Timer;
import java.util.TimerTask;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

public class CUS {

    // CONFIGURAZIONE ---
    private static final String BROKER = "tcp://broker.mqtt-dashboard.com:1883";
    private static final String TOPIC_TMS = "tms/waterlevel"; 
    
    // TEMPO E LIVELLI ACQUA
    private static final double L1 = 60.0;
    private static final double L2 = 85.0;
    private static final long T1_LIMIT_MS = 3000;
    private static final long T2_TIMEOUT_MS = 5000;

    // DEFINIZIONE STATO CONDIVISO ---
    public static class SystemState {
        public String mode = "AUTOMATIC"; // AUTOMATIC, MANUAL, UNCONNECTED
        public double currentLevel = 0.0;
        public int valveOpening = 0;
        
        public transient long lastMsgTime = System.currentTimeMillis();
        public transient long timeAboveL1Start = 0;
    }

    // ISTANZA GLOBALE DELLO STATO (Accessibile da HttpServerTask)
    public static SystemState state = new SystemState();
    
    protected static SerialComm serialComm;
    private static HttpServerTask httpTask;

    public static void main(String[] args) {
        System.out.println(">>> CUS STARTING <<<");

        // 1. SERIALE
        serialComm = new SerialComm();
        if (serialComm.init("COM3")) { // CAMBIARE COM3 SE NECESSARIO
            System.out.println("SERIAL: Connected");
        } else {
            System.out.println("SERIAL: Simulation Mode");
        }

        // 2. HTTP SERVER
        httpTask = new HttpServerTask();
        try {
            httpTask.startServer(5000); 
        } catch (Exception e) {
            e.printStackTrace();
        }

        // 3. MQTT
        setupMQTT();

        // 4. LOOP LOGICA
        new Timer().scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                logicLoop();
            }
        }, 0, 100); 
    }

    private static void setupMQTT() {
        try {
            String clientId = "CUS_" + System.currentTimeMillis();
            MqttClient client = new MqttClient(BROKER, clientId);
            client.setCallback(new MqttCallback() {
                public void connectionLost(Throwable c) {}
                public void deliveryComplete(IMqttDeliveryToken t) {}
                public void messageArrived(String t, MqttMessage m) {
                    try {
                        // MODIFICA: Nessun JSON. Leggiamo la stringa "xx.xx" direttamente.
                        String payload = new String(m.getPayload()).trim();
                        // Gestione base errore parse
                        if (!payload.isEmpty()) {
                            state.currentLevel = Double.parseDouble(payload);
                            state.lastMsgTime = System.currentTimeMillis();
                        }
                    } catch (Exception e) {
                        System.err.println("MQTT Parse Error: " + e.getMessage());
                    }
                }
            });
            client.connect();
            client.subscribe(TOPIC_TMS);
            System.out.println("MQTT: Subscribed");
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    private static void logicLoop() {
        long now = System.currentTimeMillis();
        // Timeout
        if (now - state.lastMsgTime > T2_TIMEOUT_MS) {
            if (!state.mode.equals("UNCONNECTED")) {
                state.mode = "UNCONNECTED";
                System.out.println("ALARM: Connection lost");
                serialComm.sendMode("UNCONNECTED");
            }
        } else if (state.mode.equals("UNCONNECTED")) state.mode = "UNCONNECTED";

        // Logic
        if (state.mode.equals("AUTOMATIC")) {
            if (state.currentLevel > L2) {
                state.valveOpening = 100;
                state.timeAboveL1Start = 0;
            } else if (state.currentLevel > L1) {
                if (state.timeAboveL1Start == 0) state.timeAboveL1Start = now;
                if (now - state.timeAboveL1Start > T1_LIMIT_MS) state.valveOpening = 50;
                else state.valveOpening = 0;
            } else {
                state.valveOpening = 0;
                state.timeAboveL1Start = 0;
            }
        }
        serialComm.sendValve(state.valveOpening);
    }
}

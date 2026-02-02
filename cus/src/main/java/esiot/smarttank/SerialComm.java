package esiot.smarttank;

import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;

public class SerialComm {
    private SerialPort port;

    public boolean init(String portName) {
        try {
            port = SerialPort.getCommPort(portName);
            port.setBaudRate(9600);
            
            // Importante: configurare i timeout per evitare blocchi
            port.setComPortTimeouts(SerialPort.TIMEOUT_READ_SEMI_BLOCKING, 0, 0);
            
            if (port.openPort()) {
                System.out.println("SERIAL: Port " + portName + " opened.");
                
                port.addDataListener(new SerialPortDataListener() {
                    // Buffer per accumulare i frammenti di messaggio
                    private StringBuilder messageBuffer = new StringBuilder();

                    @Override
                    public int getListeningEvents() {
                        return SerialPort.LISTENING_EVENT_DATA_AVAILABLE;
                    }

                    // ... (imports uguali a prima)

                    @Override
                    public void serialEvent(SerialPortEvent event) {
                        if (event.getEventType() != SerialPort.LISTENING_EVENT_DATA_AVAILABLE) return;
                        
                        byte[] newData = new byte[port.bytesAvailable()];
                        int numRead = port.readBytes(newData, newData.length);
                        
                        if (numRead > 0) {
                            messageBuffer.append(new String(newData));
                            int newlineIndex = messageBuffer.indexOf("\n");
                            while (newlineIndex != -1) {
                                String msg = messageBuffer.substring(0, newlineIndex).trim();
                                messageBuffer.delete(0, newlineIndex + 1);
                                
                                // DEBUG
                                // System.out.println("RECV: " + msg);

                                if (msg.equals("MOD:MANUAL")) {
                                    CUS.state.mode = "MANUAL";
                                } else if (msg.equals("MOD:AUTO")) {
                                    CUS.state.mode = "AUTOMATIC";
                                } else if (msg.equals("MOD:UNCONNECTED")) {
                                    CUS.state.mode = "UNCONNECTED";
                                } else if (msg.startsWith("LEVEL:")) {
                                    try {
                                        double level = Double.parseDouble(msg.substring(6));
                                        CUS.state.currentLevel = level;
                                        CUS.state.lastMsgTime = System.currentTimeMillis();
                                    } catch (Exception e) {}
                                } else if (msg.startsWith("VALVE:")) {
                                    // NUOVO: Sincronizza lo slider col potenziometro
                                    try {
                                        int val = Integer.parseInt(msg.substring(6));
                                        CUS.state.valveOpening = val;
                                    } catch (Exception e) {}
                                }
                                
                                newlineIndex = messageBuffer.indexOf("\n");
                            }
                        }
                    }
// ... (resto della classe uguale)
                });
                return true;
            } else {
                System.err.println("SERIAL: Failed to open port " + portName);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }

    public void sendValve(int value) {
        if (port != null && port.isOpen()) {
            String cmd = "VALVE:" + value + "\n";
            port.writeBytes(cmd.getBytes(), cmd.length());
        }
    }

    public void sendMode(String mode) {
        if (port != null && port.isOpen()) {
            String cmd;
            if (mode.equals("MANUAL")) {
                cmd = "CMD:MANUAL\n";
            } else if (mode.equals("AUTOMATIC")) {
                cmd = "CMD:AUTO\n";
            } else {
                cmd = "CMD:UNCONNECTED\n";
            }
            port.writeBytes(cmd.getBytes(), cmd.length());
            // DEBUG
            System.out.println("SERIAL SEND: " + cmd.trim());
        }
    }
}
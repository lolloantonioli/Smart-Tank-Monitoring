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
            if (port.openPort()) {
                port.setComPortTimeouts(SerialPort.TIMEOUT_READ_SEMI_BLOCKING, 0, 0);
                port.addDataListener(new SerialPortDataListener() {
                    @Override
                    public int getListeningEvents() {
                        return SerialPort.LISTENING_EVENT_DATA_AVAILABLE;
                    }

                    @Override
                    public void serialEvent(SerialPortEvent event) {
                        if (event.getEventType() != SerialPort.LISTENING_EVENT_DATA_AVAILABLE) {
                            return;
                        }
                        byte[] buffer = new byte[port.bytesAvailable()];
                        port.readBytes(buffer, buffer.length);
                        String msg = new String(buffer).trim();
                        if (msg.equals("MOD:MANUAL")) {
                            CUS.state.mode = "MANUAL";
                        } else if (msg.equals("MOD:AUTO")) {
                            CUS.state.mode = "AUTOMATIC";
                        }
                    }
                });
                return true;
            }
        } catch (Exception e) {}
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
            String cmd = (mode.equals("MANUAL")) ? "CMD:MANUAL\n" : "CMD:AUTO\n";
            port.writeBytes(cmd.getBytes(), cmd.length());
        }
    }
}
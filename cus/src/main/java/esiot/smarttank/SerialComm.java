package esiot.smarttank;

import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;

public final class SerialComm {

    private static final int BAUD_RATE = 9600;

    private SerialPort port;

    public boolean init(final String portName) {
        try {
            port = SerialPort.getCommPort(portName);
            port.setBaudRate(BAUD_RATE);
            port.setComPortTimeouts(SerialPort.TIMEOUT_READ_SEMI_BLOCKING, 0, 0);
            if (port.openPort()) {
                System.out.println("SERIAL: Port " + portName + " opened.");
                port.addDataListener(new SerialPortDataListener() {

                    private final StringBuilder messageBuffer = new StringBuilder();

                    @Override
                    public int getListeningEvents() {
                        return SerialPort.LISTENING_EVENT_DATA_AVAILABLE;
                    }

                    @Override
                    public void serialEvent(final SerialPortEvent event) {
                        if (event.getEventType() != SerialPort.LISTENING_EVENT_DATA_AVAILABLE) {
                            return;
                        }
                        
                        final byte[] newData = new byte[port.bytesAvailable()];
                        final int numRead = port.readBytes(newData, newData.length);
                        
                        if (numRead > 0) {
                            messageBuffer.append(new String(newData));
                            int newlineIndex = messageBuffer.indexOf("\n");
                            while (newlineIndex != -1) {
                                final String msg = messageBuffer.substring(0, newlineIndex).trim();
                                messageBuffer.delete(0, newlineIndex + 1);
                                
                                if (msg.equals("MOD:MANUAL")) {
                                    CUS.state.mode = "MANUAL";
                                } else if (msg.equals("MOD:AUTO")) {
                                    CUS.state.mode = "AUTOMATIC";
                                } else if (msg.equals("MOD:UNCONNECTED")) {
                                    CUS.state.mode = "UNCONNECTED";
                                } else if (msg.startsWith("LEVEL:")) {
                                    try {
                                        final double level = Double.parseDouble(msg.substring(6));
                                        CUS.state.currentLevel = level;
                                        CUS.state.lastMsgTime = System.currentTimeMillis();
                                    } catch (final Exception e) {
                                        e.printStackTrace();
                                    }
                                } else if (msg.startsWith("VALVE:")) {
                                    try {
                                        final int val = Integer.parseInt(msg.substring(6));
                                        CUS.state.valveOpening = val;
                                    } catch (final Exception e) {
                                        e.printStackTrace();
                                    }
                                }
                                newlineIndex = messageBuffer.indexOf("\n");
                            }
                        }
                    }
                });
                return true;
            } else {
                System.err.println("SERIAL: Failed to open port " + portName);
            }
        } catch (final Exception e) {
            e.printStackTrace();
        }
        return false;
    }

    public void sendValve(final int value) {
        if (port != null && port.isOpen()) {
            final String cmd = "VALVE:" + value + "\n";
            port.writeBytes(cmd.getBytes(), cmd.length());
        }
    }

    public void sendMode(final String mode) {
        if (port != null && port.isOpen()) {
            final String cmd;
            if (mode.equals("MANUAL")) {
                cmd = "CMD:MANUAL\n";
            } else if (mode.equals("AUTOMATIC")) {
                cmd = "CMD:AUTO\n";
            } else {
                cmd = "CMD:UNCONNECTED\n";
            }
            port.writeBytes(cmd.getBytes(), cmd.length());
            System.out.println("SERIAL SEND: " + cmd.trim());
        }
    }
}
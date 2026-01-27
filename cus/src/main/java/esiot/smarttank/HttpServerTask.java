package esiot.smarttank;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.InetSocketAddress;
import java.util.Map;

import com.google.gson.Gson;
import com.sun.net.httpserver.HttpServer;

public class HttpServerTask {

    // METODO CHE IL CUS CHIAMA
    public void startServer(int port) throws IOException {
        HttpServer server = HttpServer.create(new InetSocketAddress(port), 0);

        // GUI
        server.createContext("/", exchange -> {
            if (!exchange.getRequestMethod().equalsIgnoreCase("GET")) {
                exchange.sendResponseHeaders(405, -1); return;
            }
            InputStream is = getClass().getClassLoader().getResourceAsStream("index.html");
            if (is == null) {
                File f = new File("src/main/resources/index.html");
                if (f.exists()) is = new FileInputStream(f);
            }
            if (is != null) {
                byte[] bytes = is.readAllBytes();
                exchange.sendResponseHeaders(200, bytes.length);
                exchange.getResponseBody().write(bytes);
                is.close();
            } else {
                String r = "Not Found";
                exchange.sendResponseHeaders(404, r.length());
                exchange.getResponseBody().write(r.getBytes());
            }
            exchange.close();
        });

        // API DATA (Usa CUS.state)
        server.createContext("/api/data", exchange -> {
            String json = new Gson().toJson(CUS.state);
            exchange.getResponseHeaders().add("Content-Type", "application/json");
            exchange.sendResponseHeaders(200, json.length());
            exchange.getResponseBody().write(json.getBytes());
            exchange.close();
        });

        // API MODE
        server.createContext("/api/mode", exchange -> {
            if ("POST".equalsIgnoreCase(exchange.getRequestMethod())) {
                if (CUS.state.mode.equals("AUTOMATIC")) {
                    CUS.state.mode = "MANUAL";
                    CUS.serialComm.sendMode("MANUAL"); 
                } else {
                    CUS.state.mode = "AUTOMATIC";
                    CUS.serialComm.sendMode("AUTOMATIC");
                }
                exchange.sendResponseHeaders(200, 0);
            }
            exchange.close();
        });

        // API VALVE
        server.createContext("/api/valve", exchange -> {
            if ("POST".equalsIgnoreCase(exchange.getRequestMethod()) && CUS.state.mode.equals("MANUAL")) {
                InputStreamReader isr = new InputStreamReader(exchange.getRequestBody());
                Map body = new Gson().fromJson(isr, Map.class);
                if (body != null && body.containsKey("valve")) {
                    Object val = body.get("valve");
                    if (val instanceof Number) CUS.state.valveOpening = ((Number) val).intValue();
                }
                exchange.sendResponseHeaders(200, 0);
            }
            exchange.close();
        });

        server.start();
        System.out.println("HTTP: Server running on port " + port);
    }
}
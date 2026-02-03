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

public final class HttpServerTask {

    private static final int OK_CODE = 200;
    private static final int NOT_FOUND_CODE = 404;
    private static final int METHOD_NOT_ALLOWED_CODE = 405;

    public void startServer(final int port) throws IOException {
        final HttpServer server = HttpServer.create(new InetSocketAddress(port), 0);

        server.createContext("/", exchange -> {
            if (!exchange.getRequestMethod().equalsIgnoreCase("GET")) {
                exchange.sendResponseHeaders(METHOD_NOT_ALLOWED_CODE, -1); 
                return;
            }
            File f = new File("../dbs/index.html");
            if (!f.exists()) {
                f = new File("dbs/index.html");
            }
            if (f.exists()) {
                final InputStream is = new FileInputStream(f);
                final byte[] bytes = is.readAllBytes();
                exchange.sendResponseHeaders(OK_CODE, bytes.length);
                exchange.getResponseBody().write(bytes);
                is.close();
            } else {
                System.err.println("ERRORE: Dashboard non trovata in " + f.getAbsolutePath());
                final String r = "404 - Dashboard Not Found (Check 'dbs' folder)";
                exchange.sendResponseHeaders(NOT_FOUND_CODE, r.length());
                exchange.getResponseBody().write(r.getBytes());
            }
            exchange.close();
        });

        // API DATA
        server.createContext("/api/data", exchange -> {
            final String json = new Gson().toJson(CUS.state);
            exchange.getResponseHeaders().add("Content-Type", "application/json");
            exchange.sendResponseHeaders(OK_CODE, json.length());
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
                exchange.sendResponseHeaders(OK_CODE, 0);
            }
            exchange.close();
        });

        // API VALVE
        server.createContext("/api/valve", exchange -> {
            if ("POST".equalsIgnoreCase(exchange.getRequestMethod()) && CUS.state.mode.equals("MANUAL")) {
                final InputStreamReader isr = new InputStreamReader(exchange.getRequestBody());
                final Map body = new Gson().fromJson(isr, Map.class);
                if (body != null && body.containsKey("valve")) {
                    final Object val = body.get("valve");
                    if (val instanceof Number) {
                        CUS.state.valveOpening = ((Number) val).intValue();
                    }
                }
                exchange.sendResponseHeaders(OK_CODE, 0);
            }
            exchange.close();
        });

        server.start();
        System.out.println("HTTP: Server running on port " + port);
    }
}
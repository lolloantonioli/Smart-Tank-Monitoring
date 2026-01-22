from flask import Flask, render_template, jsonify, request
import random
import time

app = Flask(__name__)

# STATO DEL SISTEMA
system_data = {
    "state": "AUTOMATIC", 
    "level": 50.0,        # Partiamo da mezzo serbatoio
    "valve": 0
}

# CONFIGURAZIONE SOGLIE (Come da specifica CUS)
L1 = 60.0  # Livello 1
L2 = 85.0  # Livello 2 (Critico)
T1 = 3     # (Opzionale: secondi di attesa sopra L1, qui semplifichiamo per test visivo)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/api/data')
def get_data():
    # --- 1. SIMULAZIONE FISICA (Pioggia vs Valvola) ---
    
    # Entrata: Pioggia costante (random tra 0.5 e 1.5 litri al secondo)
    rain_in = random.uniform(0.5, 1.5)
    
    # Uscita: Dipende dalla valvola (Se aperta al 100%, scarica 3.0 litri al secondo)
    # Nota: L'uscita (3.0) deve essere maggiore della pioggia (1.5) altrimenti non si svuota mai!
    drain_out = (system_data["valve"] / 100.0) * 3.0
    
    # Aggiorna il livello reale
    system_data["level"] += (rain_in - drain_out)
    
    # Limiti fisici (non può andare sotto 0 o sopra 100)
    if system_data["level"] < 0: system_data["level"] = 0
    if system_data["level"] > 100: system_data["level"] = 100
    
    # Arrotondiamo per bellezza
    system_data["level"] = round(system_data["level"], 2)


    # --- 2. LOGICA DI CONTROLLO "CUS" (Il cervello) ---
    if system_data["state"] == "AUTOMATIC":
        # Se siamo sopra L2 -> APERTURA TOTALE (Emergenza)
        if system_data["level"] > L2:
            system_data["valve"] = 100
            
        # Se siamo sopra L1 ma sotto L2 -> APERTURA META'
        elif system_data["level"] > L1:
            system_data["valve"] = 50
            
        # Se siamo sotto L1 -> CHIUDI TUTTO (Lascia riempire)
        else:
            system_data["valve"] = 0

    return jsonify(system_data)

@app.route('/api/mode', methods=['POST'])
def switch_mode():
    if system_data["state"] == "AUTOMATIC":
        system_data["state"] = "MANUAL"
    else:
        system_data["state"] = "AUTOMATIC"
    return jsonify({"status": "ok", "state": system_data["state"]})

@app.route('/api/valve', methods=['POST'])
def set_valve():
    if system_data["state"] == "MANUAL":
        req = request.json
        val = int(req.get("valve", 0))
        system_data["valve"] = val
    return jsonify({"status": "ok", "valve": system_data["valve"]})

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
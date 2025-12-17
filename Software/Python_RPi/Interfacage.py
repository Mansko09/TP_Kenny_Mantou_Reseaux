from flask import Flask, jsonify, request, render_template_string
import serial
import time
import re

app = Flask(__name__)

try:
    ser = serial.Serial("/dev/ttyAMA0", 115200, timeout=1)
except serial.SerialException:
    ser = None

temperatures = []
pressures = []
scales = {}

def read_stm32(cmd: str, arg: str = None) -> str:
    if not ser:
        raise IOError("Serial port is not available.")
    ser.reset_input_buffer()
    ser.reset_output_buffer()
    if arg is not None:
        full_cmd = f"{cmd} {arg}"
    else:
        full_cmd = cmd
    command_bytes = f"{full_cmd}\n".encode('utf-8')
    ser.write(command_bytes)
    ser.write(b"\r")
    time.sleep(0.3)
    raw_bytes = ser.read_all()
    if not raw_bytes:
        return None
    raw_response = raw_bytes.decode('utf-8', errors='replace')
    return raw_response

@app.route("/temp", methods=["POST", "GET"])
def temp_all():
    if request.method == "POST":
        try:
            raw = read_stm32("t")
            if raw is None:
                return jsonify({"error": "No response from STM32"}), 500
            match = re.search(r":\s*([+-]?\d+\.\d+)", raw)
            if not match:
                return jsonify({"error": "Could not parse temperature", "raw_response": raw}), 500
            float_value = float(match.group(1))
            temperatures.append(float_value)
            return jsonify({
                "message": "Temperature stored successfully",
                "temperature_celsius": float_value,
                "all_temperatures": temperatures
            })
        except IOError as e:
            return jsonify({"error": str(e)}), 500
    return jsonify({"temperatures": temperatures})

@app.route("/temp/<int:x>", methods=["GET", "DELETE"])
def temp_one(x):
    if x < 0 or x >= len(temperatures):
        return jsonify({"error": "Index out of range"}), 404
    if request.method == "DELETE":
        removed = temperatures.pop(x)
        return jsonify({"message": "Deleted", "deleted_temperature": removed})
    return jsonify({"temperature": temperatures[x]})

@app.route("/pres", methods=["POST", "GET"])
def pres_all():
    if request.method == "POST":
        try:
            raw = read_stm32("p")
            if raw is None:
                return jsonify({"error": "No response from STM32"}), 500
            match = re.search(r":\s*([+-]?\d+\.\d+)", raw)
            if not match:
                return jsonify({"error": "Could not parse pressure", "raw_response": raw}), 500
            float_value = float(match.group(1))
            pressures.append(float_value)
            return jsonify({
                "message": "Pressure stored successfully",
                "pressure_bar": float_value,
                "all_pressures": pressures
            })
        except IOError as e:
            return jsonify({"error": str(e)}), 500
    return jsonify({"pressures": pressures})

@app.route("/pres/<int:x>", methods=["GET", "DELETE"])
def pres_one(x):
    if x < 0 or x >= len(pressures):
        return jsonify({"error": "Index out of range"}), 404
    if request.method == "DELETE":
        removed = pressures.pop(x)
        return jsonify({"message": "Deleted", "deleted_pressure": removed})
    return jsonify({"pressure": pressures[x]})

@app.route("/scale/<string:index>", methods=["POST", "GET"])
def set_scale(index: str):
    if request.method == "GET":
        return jsonify(scales)
    data = request.json
    if data and "K" in data:
        try:
            k = float(data["K"])
            scales["0"] = k
            return jsonify({"message": "Scale updated via JSON", "K": k, "scales": scales})
        except:
            return jsonify({"error": "Invalid K"}), 400
    cleaned = index.strip()
    if cleaned.startswith('"') and cleaned.endswith('"'):
        try:
            k = float(cleaned[1:-1])
            scales["0"] = k
            return jsonify({"message": "Scale updated via URL", "K": k, "scales": scales})
        except:
            return jsonify({"error": "Value inside quotes must be a float"}), 400
    return jsonify({"error": "Bad format"}), 400

@app.route("/angle", methods=["GET"])
def angle():
    result = []
    for i, t in enumerate(temperatures):
        k = scales.get(str(i), 1.0)
        try:
            k = float(k)
        except:
            k = 1.0
        result.append({"index": i, "temp": t, "scale": k, "angle": t * k})
    return jsonify(result)

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)
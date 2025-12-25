from flask import Flask, request, jsonify
import pickle
import pandas as pd

app = Flask(__name__)

# Load trained ML model
model = pickle.load(open("../ml/model.pkl", "rb"))

@app.route("/predict", methods=["POST"])
def predict():
    data = request.json

    # SAFE feature extraction (no KeyError)
    moisture = data.get("moisture", 0)
    temp = data.get("temp", 0)
    humidity = data.get("humidity", 0)
    rain = data.get("rain", 0)      # default: no rain
    light = data.get("light", 50)   # default: medium light

    # ML model input (same as trained)
    input_df = pd.DataFrame(
        [[moisture, temp, humidity, rain, light]],
        columns=["moisture", "temp", "humidity", "rain", "light"]
    )

    prediction = model.predict(input_df)[0]

    return jsonify({"pump": int(prediction)})

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)

# 🌱 AI-Driven Smart Irrigation System

The **AI-Driven Smart Irrigation System** is an intelligent IoT-based solution designed to optimize water usage in agriculture by combining **machine learning**, **real-time weather data**, and **cloud computing**.  
The system automatically controls irrigation based on soil moisture, weather conditions, and AI predictions, with manual override support.

---

## 📌 Project Objectives

- Reduce water wastage through intelligent irrigation
- Replace fixed threshold logic with AI-based decision making
- Integrate real-time weather information
- Provide manual control with safe AI override
- Demonstrate IoT + Cloud + Machine Learning integration

---

## ⚙️ System Architecture

1. **ESP8266** reads soil moisture data
2. **Weather API** provides temperature, humidity, rain, and cloud cover
3. **Flask Server** processes data and runs the ML model
4. **Machine Learning Model** predicts pump ON/OFF
5. **Pump Control** executes irrigation decisions
6. **Blynk App** allows monitoring and manual override

---

## 🚀 Key Features

- AI-based irrigation decision making
- Real-time weather API integration
- Virtual light intensity sensing (cloud-based LDR)
- Manual override with AI cooldown mechanism
- Cloud-based ML prediction using Flask
- Fault-tolerant and scalable design

---

## 🧠 Technologies Used

### Hardware
- ESP8266 (NodeMCU)
- Soil Moisture Sensor
- Water Pump & Relay Module

### Software & Tools
- Python
- Flask
- scikit-learn
- Pandas
- Arduino IDE
- Blynk IoT Platform
- OpenWeather API

---

## 📊 Machine Learning Details

- Features used:
  - Soil Moisture
  - Temperature
  - Humidity
  - Rain
  - Light Intensity
- Models tested:
  - Decision Tree
  - Random Forest
  - Logistic Regression
- Best accuracy achieved: **~97%**
- Trained model saved as `model.pkl`

---

## ▶️ How to Run the Project

### 1️⃣ Generate Dataset
```bash
python generate_data.py

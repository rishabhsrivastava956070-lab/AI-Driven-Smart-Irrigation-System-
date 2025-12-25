import pandas as pd
import random

rows = []

for _ in range(300):  # 300 rows
    moisture = random.randint(0, 100)     # %
    temp = random.randint(20, 40)          # °C
    humidity = random.randint(30, 90)      # %
    rain = random.choice([0, 1])           # 0 = No rain, 1 = Rain
    light = random.randint(0, 100)          # Virtual LDR (0–100)

    # ================= RULE-BASED LABELING =================
    # Ground truth logic (human knowledge)
    if moisture < 30 and rain == 0:
        pump = 1   # ON
    else:
        pump = 0   # OFF

    rows.append([moisture, temp, humidity, rain, light, pump])

# Create DataFrame
df = pd.DataFrame(
    rows,
    columns=["moisture", "temp", "humidity", "rain", "light", "pump"]
)

# Save dataset
df.to_csv("data.csv", index=False)

print("✅ data.csv generated with 300 rows (with rain & light)")

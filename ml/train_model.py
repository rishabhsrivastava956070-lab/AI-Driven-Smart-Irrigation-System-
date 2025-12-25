import pandas as pd
import pickle

from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score

from sklearn.tree import DecisionTreeClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.linear_model import LogisticRegression

# ================= LOAD DATASET =================
data = pd.read_csv("data.csv")

# ================= FEATURES & LABEL =================
X = data[["moisture", "temp", "humidity", "rain", "light"]]
y = data["pump"]

# ================= TRAIN-TEST SPLIT =================
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.2, random_state=42
)

# ================= MODELS =================
models = {
    "DecisionTree": DecisionTreeClassifier(random_state=42),
    "RandomForest": RandomForestClassifier(n_estimators=100, random_state=42),
    "LogisticRegression": LogisticRegression(max_iter=1000)
}

best_model = None
best_accuracy = 0

# ================= TRAIN & COMPARE =================
for name, model in models.items():
    model.fit(X_train, y_train)
    preds = model.predict(X_test)
    acc = accuracy_score(y_test, preds)

    print(f"{name} Accuracy: {acc:.2f}")

    if acc > best_accuracy:
        best_accuracy = acc
        best_model = model

# ================= SAVE MODEL =================
pickle.dump(best_model, open("model.pkl", "wb"))

print("\n✅ Best Model Saved with Accuracy:", best_accuracy)

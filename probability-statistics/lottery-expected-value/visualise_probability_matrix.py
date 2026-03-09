import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("prob_matrix.csv")
M = df.drop(columns=["i\\k"]).to_numpy()

plt.figure(figsize=(8,6))
plt.imshow(M, aspect="auto", origin="lower")
plt.colorbar()
plt.title("P(i at position j)")
plt.xlabel("position j (1..r)")
plt.ylabel("element i (1..n)")
plt.show()
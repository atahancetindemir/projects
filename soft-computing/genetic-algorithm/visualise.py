import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("output.csv")

plt.figure(figsize=(10, 6))
plt.plot(df["Generation"], df["Average"], label="Average")
plt.plot(df["Generation"], df["Best"], label="Best")

plt.title("Results (Chromosome: 20, Generation: 100, Crossover: 0.7, Mutation: 0.001)")
plt.xlabel("Generation")
plt.ylabel("Fitness")
plt.legend(loc="lower right")
plt.grid(True)
plt.tight_layout()

plt.savefig("plot.png", dpi=150)

plt.show()

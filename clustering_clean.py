"""
Agglomerative (Hierarchical) Clustering on IoT Air Quality Sensor Data
Analyses MQ6, MQ135, and dust density readings to classify air quality levels.
"""

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.preprocessing import normalize
from sklearn.cluster import AgglomerativeClustering
import scipy.cluster.hierarchy as shc
import os

# Load data (relative path — works when run from repo root or analysis/ folder)
data_path = os.path.join(os.path.dirname(__file__), '..', 'data', 'airquality_dataset.csv')
data = pd.read_csv(data_path)

# Normalise features
data_scaled = normalize(data)
data_scaled = pd.DataFrame(data_scaled, columns=data.columns)
print("Scaled data sample:")
print(data_scaled.head())

# Drop ID columns for clustering
data_scale = data_scaled.drop(columns=['id_num', 'id', 'id1'])

# --- Dendrograms ---
plt.figure(figsize=(10, 7))
dend = shc.dendrogram(shc.linkage(data_scale, method='ward'))
plt.title('Dendrogram — Ward Linkage')
plt.xlabel("Sample Index")
plt.ylabel("Euclidean Distance")
plt.savefig('dendrogram_ward.jpg')
plt.show()

plt.figure(figsize=(10, 7))
dend = shc.dendrogram(shc.linkage(data_scaled, method='ward'))
plt.axhline(y=1.8, color='r', linestyle='--')
plt.title('Dendrogram — 2 Clusters (threshold = 1.8)')
plt.xlabel("Sample Index")
plt.ylabel("Euclidean Distance")
plt.savefig('dendrogram_2clusters.jpg')
plt.show()

# --- Agglomerative Clustering (k=2: clean vs polluted) ---
cluster = AgglomerativeClustering(n_clusters=2, metric='euclidean', linkage='ward')
labels = cluster.fit_predict(data_scale)

# --- Cluster visualisation per sensor ---
sensors = {
    'mq6': 'MQ6 Gas Level',
    'mq135': 'MQ135 Air Quality Level',
    'dust_density': 'Dust Density Level'
}

for col, label in sensors.items():
    plt.figure(figsize=(10, 7))
    plt.scatter(data_scaled[col], data_scaled['id'], c=labels, cmap='viridis')
    plt.title(f'Clusters: {label} vs Sample ID')
    plt.xlabel(label)
    plt.ylabel("Sample ID")
    plt.colorbar(label='Cluster')
    plt.savefig(f'cluster_{col}.jpg')
    plt.show()

print(f"\nClustering complete. {len(set(labels))} clusters identified across {len(labels)} samples.")

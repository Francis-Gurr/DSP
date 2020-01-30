import pandas as pd
import matplotlib
matplotlib.use('tkagg')
import matplotlib.pyplot as plt

x = []
for chunk in pd.read_csv('fir_sum.csv', chunksize=208125):
    x.extend(chunk)
plt.plot(x);
plt.show()

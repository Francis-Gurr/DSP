import pandas as pd 
import matplotlib
matplotlib.use('tkagg')
import matplotlib.pyplot as plt

#x = pd.Series([0.0,0.0,0.0]) 
for chunk in pd.read_csv('fir_sum.csv', squeeze='True', dtype='float', chunksize=10000000):
    #x = x.append(chunk)
    chunk.plot()
#x.plot()
plt.show()

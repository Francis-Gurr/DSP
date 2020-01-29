import matplotlib.pyplot as plt
import csv

x = []

f = open('fir_sum.csv', 'rb')
reader=csv.reader(f)
x =[]
for row in reader:
    for i in range(350000,400000):
        x.append(row[i])
plt.plot(x);
plt.show()

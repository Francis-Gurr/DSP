import matplotlib.pyplot as plt
import csv

x = []

with open('fir_sum.csv', 'r') as file:
    rows = csv.reader(file, delimiter=',')
    for row in rows:
        x.append(float(row[0]));

plt.plot(x);
plt.show()

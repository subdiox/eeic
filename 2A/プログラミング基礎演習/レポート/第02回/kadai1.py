import numpy as np
import matplotlib.pyplot as plt
from ica import ICA

data1 = np.loadtxt('report2_kadai1_data1/dat1.txt')
data2 = np.loadtxt('report2_kadai1_data1/dat2.txt')
data = np.matrix([data1, data2])

y = ICA(data).start()

for y_i in y:
    plt.plot(y_i.tolist())    
plt.show()

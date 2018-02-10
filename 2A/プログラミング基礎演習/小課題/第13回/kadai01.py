import numpy as np
import matplotlib.pyplot as plt

def zscore(x, axis = None):
    xmean = x.mean(axis=axis, keepdims=True)
    xstd = np.std(x, axis=axis, keepdims=True)
    xdim = np.shape(x)[0]
    zscore = (x - xmean) / xstd / np.sqrt(xdim)
    return zscore

n = 1000
arr = zscore(np.random.randn(n, n))
m = np.asmatrix(arr)
eig, vec = np.linalg.eig(m)
x = list(map(lambda x:x.real, eig.tolist()))
y = list(map(lambda x:x.imag, eig.tolist()))
plt.scatter(x, y)
plt.show()

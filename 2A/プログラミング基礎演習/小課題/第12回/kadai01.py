import mylib.myio as io
import numpy as np

m1 = io.import_matrix('test.txt')
print(m1)

m2 = np.matrix([[1,2,3],[4,5,6]])
io.export_matrix('test2.txt', m2)

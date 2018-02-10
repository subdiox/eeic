import numpy as np
import imageio as im
from ica import ICA

data1 = im.imread('report2_kadai3_data1/image1.png').astype(float)
data2 = im.imread('report2_kadai3_data1/image2.png').astype(float)

y = ICA([data1.flatten(), data2.flatten()]).start()
y = [(y_i * 127 / max(np.absolute(y_i).tolist()) + 128).astype(np.uint8) for y_i in y]
y = [y_i.reshape(data1.shape) for y_i in y]

im.imwrite('report2_kadai3_data1/image3.png', y[0])
im.imwrite('report2_kadai3_data1/image4.png', y[1])

import numpy as np
import scipy.io.wavfile as wf
from ica import ICA

data1 = wf.read('report2_kadai2_data2/speechB1.wav')[1].astype(float)
data2 = wf.read('report2_kadai2_data2/speechB2.wav')[1].astype(float)
data3 = wf.read('report2_kadai2_data2/speechB3.wav')[1].astype(float)

y = ICA([data1, data2, data3]).start()
y = [(y_i * 32767 / max(np.absolute(y_i).tolist())).astype(np.int16) for y_i in y]

wf.write('report2_kadai2_data2/speechB4.wav', 8000, y[0])
wf.write('report2_kadai2_data2/speechB5.wav', 8000, y[1])
wf.write('report2_kadai2_data2/speechB6.wav', 8000, y[2])

import numpy as np
import scipy.io.wavfile as wf
from ica import ICA

data1 = wf.read('report2_kadai2_data1/speechA1.wav')[1].astype(float)
data2 = wf.read('report2_kadai2_data1/speechA2.wav')[1].astype(float)

y = ICA([data1, data2]).start()
y = [(y_i * 32767 / max(np.absolute(y_i).tolist())).astype(np.int16) for y_i in y]

wf.write('report2_kadai2_data1/speechA3.wav', 8000, y[0])
wf.write('report2_kadai2_data1/speechA4.wav', 8000, y[1])

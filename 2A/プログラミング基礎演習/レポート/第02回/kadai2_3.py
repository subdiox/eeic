import numpy as np
import scipy.io.wavfile as wf
from ica import ICA

data1 = wf.read('report2_kadai2_data3/music1.wav')[1].astype(float)
data2 = wf.read('report2_kadai2_data3/music2.wav')[1].astype(float)

y = ICA([data1, data2]).start()
y = [(y_i * 32767 / max(np.absolute(y_i).tolist())).astype(np.int16) for y_i in y]

wf.write('report2_kadai2_data3/music3.wav', 44100, y[0])
wf.write('report2_kadai2_data3/music4.wav', 44100, y[1])

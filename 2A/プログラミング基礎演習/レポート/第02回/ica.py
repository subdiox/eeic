import numpy as np

W_STOP = 1e-5

class ICA:
    def __init__(self, x):
        self.x = np.matrix(x)

    def start(self):
        self.fit()
        z = self.whiten()
        y = self.analyze(z)
        return y

    def normalize(self, x):
        if x.sum() < 0:
            x *= -1
        return x / np.linalg.norm(x)

    def fit(self):
        self.x -= self.x.mean(axis=1)
        
    def whiten(self):
        sigma = np.cov(self.x, rowvar=True, bias=True)
        la, E = np.linalg.eigh(sigma)
        E = np.asmatrix(E)
        Dh = np.diag(np.array(la) ** (-1/2))
        V = E * Dh * E.T
        z = V * self.x
        return z

    def analyze(self, z):
        c, r = self.x.shape
        W = np.empty((0, c))
        for _ in range(c):
            w = np.random.normal(0, 1, (c, 1))
            w = self.normalize(w)
            while True:
                w_prev = w
                w = np.asmatrix((np.asarray(z) * np.asarray(w.T * z) ** 3).mean(axis=1)).T - 3 * w
                w = self.normalize(np.linalg.qr(np.asmatrix(np.concatenate((W, w.T), axis=0)).T)[0].T[-1].T)
                if np.linalg.norm(w - w_prev) < W_STOP:
                    W = np.concatenate((W, w.T), axis=0)
                    break
        y = W * z
        y = [np.asarray(y_i).flatten() for y_i in y]
        return y
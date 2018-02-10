import pandas as pd
import numpy as np

def import_matrix(filename):
    df = pd.read_csv(filename, delimiter = ',', header = None)
    return np.matrix(df)

def export_matrix(filename, matrix):
    df = pd.DataFrame(matrix.tolist())
    df.to_csv(filename, sep = ',', header = False, index = False)

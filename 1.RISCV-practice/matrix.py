import numpy as np 

m1 = np.array([
    [-3, 0, 3, -1],
    [5, 2, 0, -4],
    [0, 1, -5, 0],
    [2, 0 ,-4, 1],
    [-1, 0, 0, 1]
])

m2 = np.array ([
    [1, -2, 0, 3, 0, 4, 2, 4],
    [0, -2, 0, 2, -2, 0, 1, -1],
    [-1, 0, -4, 1, 4, 3, 0, -1],
    [-4, -1, 5, -2, 0, 0, -1, -1]
])

result = m1@m2

print(f'result of numpy dot product: \n {result}')

#simulating input stationary flow with assembly
#all matrices in assembly are one dimensional
m3 = np.zeros(40)
m1 = m1.flatten()
m2 = m2.flatten()


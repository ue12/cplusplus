import numpy as np 

import numpy_wrapper as npw

d = np.empty(10)
npw.fill_array(d)

print(f"d = {d}")
fc = npw.PyFakeComputer(d)
print(f"d = {d}")
fc.compute()
print(f"d = {d}")
d[1] = 10.
print(f"d = {d}")
fc.compute()
print(f"d = {d}")


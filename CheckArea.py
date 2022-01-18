import math

N = 1500
act = 30
L = 1200
per = 0.7142857142857143

density = (N * (math.pi * (act ** 2)) / 4) / (L ** 2)
print("")
print("")
print("density = ", density)
inh = act / per
print("inh = ", inh)
print("act = ", act)

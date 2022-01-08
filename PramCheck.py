Dv = 0.40
Du = 0.08
Cv = 0.0000
Cu = 0.0001
a = 0.010
b = -0.010
c = 0.010
d = -0.010

print(" ")

print(a + d, a + d < 0)
print(a * d - b * c, a * d - b * c > 0)
print((Du * b - Dv * a) ** 2 - (4 * Du * Dv * (a * b - b * c)),
      (Du * b - Dv * a) ** 2 - (4 * Du * Dv * (a * b - b * c)) > 0)

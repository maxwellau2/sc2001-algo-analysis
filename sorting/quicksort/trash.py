# arr = [2.1,2.2, 2.2, 2.3, 2.3, 2.4, 2.5, 2.5, 2.5, 2.8,
# 2.9,2.9, 2.9, 3.0, 3.1, 3.1, 3.2, 3.3, 3.3, 3.3,
# 3.4,3.5, 3.6, 3.6, 3.6, 3.7, 3.7, 4.0, 4.2, 4.5,
# 4.9,5.1, 5.2, 5.3, 5.7, 6.0, 6.1, 7.1, 7.8, 7.9,
# 8.9 ]


# print(sum(arr)/len(arr))
# print(len(arr))
import math

x = [2.5 ,3.4 ,5.6 ,6.7 ,7.9 ]
y = [10.2, 11.8, 13.7, 19.7, 20.6 ]

mean_x = sum(x)/len(x)
mean_y = sum(y)/len(y)

numerator = 0
for i in range(len(x)):
    numerator += (x[i] - mean_x) * (y[i] - mean_y)

denominator = 0
denom_x = 0
denom_y = 0
for i in range(len(x)):
    denom_x += (x[i] - mean_x)**2
    denom_y += (y[i] - mean_y)**2

# numerator /= len(x)
denominator = math.sqrt(denom_x * denom_y)

print(numerator)
print(denominator)

print(numerator/denominator)
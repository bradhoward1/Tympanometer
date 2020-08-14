import numpy as np
import matplotlib.pyplot as plt
from statistics import mean

voltage = np.array([0.227, 0.23, 0.234, 0.248, 0.274, 0.63, 1.38, 1.904, 2.4, 2.6, 3.09, 3.59, 4.31, 4.85], dtype=np.float64)
pressure = np.array([-400, -350, -300, -250, -200, -150, -100, -50, 0, 10, 50, 100, 150, 200], dtype=np.float64)

plt.plot(voltage, pressure)
plt.ylabel("Pressure (daPa)")
plt.xlabel("Voltage (V)")
plt.title("Pressure over Voltage")
plt.show()

def best_fit_slope_and_intercept(x,y):
    m = (((mean(x)*mean(y)) - mean(x*y)) /
         ((mean(x)*mean(x)) - mean(x*x)))

    b = mean(y) - m*mean(x)

    return m, b

voltage1 = voltage[0:5]
pressure1 = pressure[0:5]
voltage2 = voltage[4:]
pressure2 = pressure[4:]

# After observing the graph we need two different lines of best best fit

m1, b1 = best_fit_slope_and_intercept(voltage1, pressure1)
m2, b2 = best_fit_slope_and_intercept(voltage2, pressure2)

print("m1 is {} and b1 is {}".format(m1,b1))
print("m2 is {} and b2 is {}".format(m2,b2))

regression_line1 = [(m1*x)+b1 for x in voltage1]
regression_line2 = [(m2*x)+b2 for x in voltage2]

plt.plot(voltage1, regression_line1)
plt.plot(voltage2, regression_line2)
plt.ylabel("Pressure (daPa)")
plt.xlabel("Voltage (V)")
plt.title("Pressure over Voltage")
plt.show()

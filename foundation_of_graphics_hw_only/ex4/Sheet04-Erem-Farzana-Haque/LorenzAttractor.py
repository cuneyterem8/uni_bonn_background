import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from scipy.integrate import odeint

# Cüneyt Erem, Md Emtazul Haque, Sheik Mastura Farzana

def LorenzAttractor(t, X):
    """
	Computes the force at a time t and a position X
	t: the current time. In this case, the function is constant in t
	X: the position as [x, y, z] array
	"""
    x = np.zeros((3))
    sigma = 10
    p = 28
    beta = 8/3
    x_t, y_t, z_t = X[0], X[1], X[2]
    x_dot = sigma*(y_t - x_t)
    y_dot = x_t*(p - z_t) - y_t
    z_dot = x_t*y_t -beta*z_t
    x = np.array([x_dot, y_dot, z_dot])
    return x

def Euler(f, y0, stepsize, steps):
    """
	Compute positions using the Euler integration scheme.
	f:			The function that is being integrated
	y0:			The start position
	stepsize:	Delta for each step
	steps:		Amount of integration steps
	"""
    x = np.zeros((3, steps))
    x[0:3, 0] = y0
    for i in range(0, steps-1):
        x[0:3, i+1] = x[0:3, i] + stepsize*f(i+1, x[0:3, i])
    return x

def RungeKutta4thOrder(f, y0, stepsize, steps):
	"""
	Compute positions using the Runge-Kutta 4th Order integration scheme.
	f:			The function that is being integrated
	y0:			The start position
	stepsize:	Delta for each step
	steps:		Amount of integration steps
	"""
	x = np.zeros((3, steps))
	x[0:3, 0] = y0
	stepsize_xyz = np.array([stepsize, stepsize, stepsize])
	for i in range(0, steps-1):
		k1=f(i, x[0:3, i])
		k2=f(i+stepsize/2, x[0:3, i]+stepsize/2*k1)
		k3=f(i+stepsize/2, x[0:3, i]+stepsize/2*k2)
		k4=f(i+stepsize, x[0:3, i]+stepsize*k3)
		x[0:3,i+1] = x[0:3, i]+stepsize*(1/6)*(k1+2*k2+2*k3+k4)
	return x
	
# Compute Values
y0 = [-1, 3, 4]
euler = Euler(LorenzAttractor, y0, 0.025, 3000)
rungekutta = RungeKutta4thOrder(LorenzAttractor, y0, 0.025, 3000)
ref = odeint(lambda y, t : LorenzAttractor(t, y), y0, np.arange(0, 0.025*3000, 0.025)).T


# Plot Everything
plt.figure()
ax = plt.gcf().add_subplot(111, projection='3d')
ax.set_title("Euler Integration")
ax.plot(euler[0, :], euler[1, :], euler[2, :])

plt.figure()
ax = plt.gcf().add_subplot(111, projection='3d')
ax.set_title("Runge Kutta Integration")
ax.plot(rungekutta[0, :], rungekutta[1, :], rungekutta[2, :])

plt.figure()
ax = plt.gcf().add_subplot(111, projection='3d')
ax.set_title("ODE Integrate")
ax.plot(ref[0, :], ref[1, :], ref[2, :])

plt.figure()
ax = plt.gcf().add_subplot(111, projection='3d')
ax.set_title("Comparison")
ax.plot(euler[0, :], euler[1, :], euler[2, :], label="Euler")
ax.plot(rungekutta[0, :], rungekutta[1, :], rungekutta[2, :], label="Runge Kutta")
ax.plot(ref[0, :], ref[1, :], ref[2, :], label="ODE Integrate")
plt.legend()

plt.show()

#Authors: Sheikh Mastura Farzana, Md. Emtazul Haque, Cuneyt Erem
import numpy as np
import matplotlib.pyplot as plt
from math import exp, sqrt, pi


# f: target function, which we want to integrate
# ...
def f_func(x):
	return np.power(x,0.9) * np.exp(-1 * (np.power(x,2)/2))

# p_n: normal distribution for samples
sigma2 = 0.3
mu = 1.2



# the uniform distribution is expected to be the worst performing of the 
# three distributions, as it approximates the original function rather
# badly
def p_uniform(x, a=0, b=4):
	return np.select([(x >= a)*( x <=b), (x<a) + (x>b)], [1/(b-a), 0])

# the normal distribution is expected to perform better than the uniform 
# distribution but will be outperformed by the polynomial distribution. 
# Reason being that the normal distribution approximates the original function
# better than the uniform function but the approximation is inferior to the
# polynomial distribution.
def p_normal(x):
	left = 1/np.sqrt(2 * np.pi * sigma2)
	right = np.exp(-1 * (np.power((mu-x),2)/(2*sigma2)))
	return left * right


# p_p: polynomial fit of the target function
# ...
# (make sure, the fitting is computed only once and not on every invocation of p_p !
x_ar = np.array([0,1,2,3,4])
y_ar = f_func(x_ar)
polyf = np.polyfit(x_ar, y_ar, 4)

# the polynomial function is expected to be the most precise out of 
# the three distributions, as it is approximates the original function
# better than the other two distributions.
def p_polynomial(x):
	return np.polyval(polyf,x)


# plot the function graphs:
plt.figure("Functions")
# ...
x = np.linspace(0,4, 100)
plt.plot(x, f_func(x))
plt.plot(x, p_normal(x), "g")
plt.plot(x, p_polynomial(x), "r")

plt.legend(("f(x)","p_normal", "p_uniform"))

"""
Uses rejection sampling to generate samples
n: amount of samples to generate
d: distribution to draw samples from
max: maximum y value that is used to generate the samples
returns: x and y values of the samples in the shape (n, 2)
"""
def GenSamples(n, d, max):
	s = np.zeros((n, 2))
	accepted_samples = 0
	# ...
	while (accepted_samples < n):
		random_x = np.random.uniform(0,4)
		random_y = np.random.uniform(0,max)
		if d(random_x) < random_y:
			continue

		s[accepted_samples, 0] = random_x
		s[accepted_samples, 1] = random_y
		accepted_samples += 1		
	return s

# Plot results of GenSamples()

# Hint: 0.8 is a reasonable value for the max parameter of GenSamples
plt.figure("Normal Distribution")
samples = GenSamples(200, p_normal, 0.8)
plt.scatter(samples[:,0] , samples[:,1] )
x = np.linspace(0,4,100)
plt.plot(x, p_normal(x))


plt.figure("Polynomial")
samples = GenSamples(200, p_polynomial, 0.8)
plt.scatter(samples[:,0] , samples[:,1] )
x = np.linspace(0,4,100)
plt.plot(x, p_polynomial(x))


"""
p: the function to integrate
samples: array with the sample positions
weights: function to compute the weight of each sample
"""
def Integrate(p, samples, weights):
	# ...
	sum = 0
	num_of_samples = len(samples[:,0])
	for i in range(0, num_of_samples):
		sum = sum + p(samples[i][0])/weights(samples[i][0])
	sum = sum/num_of_samples
	return sum


maximumSamples = 500
id = np.zeros(maximumSamples)
norm = np.zeros(maximumSamples)
poly = np.zeros(maximumSamples)

# As expected the polynomial function is able to easily outperfrom the 
# other two distributions in terms of requiring less samples to converge quicker.
# In the graph we can also see, that the integration with the 
# polynomial distribution suffers significantly less from fluctuation 
# compared to the integration with the other distributions.
# However, the polynomial converges to a value, that is not really near the exact value of the integral.
for i in range(0, maximumSamples):
	id[i] = Integrate(f_func, GenSamples(i+1, p_uniform, 0.25), p_uniform)
	norm[i] = Integrate(f_func, GenSamples(i+1, p_normal, 0.8), p_normal)
	poly[i] = Integrate(f_func, GenSamples(i+1, p_polynomial, 0.8), p_polynomial)
	if i%10 == 0: # print progress
		print(i)

#plot results
plt.figure("Convergence")
# ...

plt.plot(np.linspace(1, maximumSamples, maximumSamples).astype(int) , poly+1.5, "g")
plt.plot(np.linspace(1, maximumSamples, maximumSamples).astype(int) , norm + 0.75 , "orange")
plt.plot(np.linspace(1, maximumSamples, maximumSamples).astype(int) , id)

plt.yticks([])
plt.legend(("poly","normal", "uniform"))


plt.show()

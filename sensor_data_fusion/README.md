# Sensor Data Fusion


Most of the exercises include heavily theoretical math and some practical coding


## Content

Gaussian probability density functions, Kalman filter, Multi-Hypothesis-Trackier, Interacting Multiple Model Filter, Retrodiction, Smoothing, Maneuver Modeling


### Python example from task

```python
import numpy as np
import matplotlib.pyplot as plt

class KalmanFilter(object):
    def __init__(self, F = 0, H = 0, D = 0, R = 0, P = 0, x0 = 0, v = 0, W = 0):
        self.F = F
        self.H = H
        self.D = np.eye(F.shape[1]) if D is 0 else D
        self.R = np.eye(F.shape[1]) if R is 0 else R
        self.P = np.eye(F.shape[1]) if P is 0 else P
        self.x = np.zeros((F.shape[1], 1)) if x0 is 0 else x0
        self.v = np.eye(F.shape[1]) if v is 0 else v
        self.W = np.eye(F.shape[1]) if W is 0 else W

    def prediction(self):
        self.x = np.dot(self.F, self.x)
        self.P = np.dot(np.dot(self.F, self.P), self.F.T) + self.D
        return self.x

    def filtering(self, z):
        self.v = z - np.dot(self.H, self.x)
        S = np.dot(self.H, np.dot(self.P, self.H.T)) + self.R
        self.W = np.dot(np.dot(self.P, self.H.T), np.linalg.inv(S))
        self.x = self.x + np.dot(self.W, self.v)
        self.P = self.P - np.dot(np.dot(self.W, S), self.W.T)

        #for the l = k-1
    def retrodiction(self, v, W, P_k_k1):
        P_k1_k1 = np.dot(np.dot(self.F, (P_k_k1 - self.D)), self.F.T) 
        W_k1_k = np.dot(np.dot(P_k1_k1, self.F.T), np.linalg.inv(P_k_k1))
        x_k_k1 = self.x - np.dot(W, v)
        x_k1_k1 = np.dot(np.linalg.inv(self.F), x_k_k1)
        
        self.x = x_k1_k1 + np.dot(W_k1_k, np.dot(W, v) )
        self.P = P_k1_k1 + np.dot(np.dot(W_k1_k, (self.P - P_k_k1)), W_k1_k.T)
        
def run(F, H, D, R):
	x = np.linspace(-10, 10, 100)
	measurements = x*x + np.random.normal(0, 2, 100)

	kalman = KalmanFilter(F = F, H = H, D = D, R = R)
	predictions = []
	filterings = []
	pred_p = []
	retros = []
	
	for z in measurements:
		predictions.append(np.dot(H,  kalman.prediction())[0])
		pred_p = kalman.P
		kalman.filtering(z)
		filterings.append(kalman.x[0])
		kalman.retrodiction(kalman.v, kalman.W, pred_p)
		retros.append(kalman.x[0])
	
	plt.plot(range(len(measurements)), measurements, label = 'measurements')
	plt.plot(range(len(predictions)), np.array(predictions), label = 'kalman filter predictions')
	plt.plot(range(len(filterings)), np.array(filterings), label = 'kalman filter filterings')
	plt.plot(range(len(retros)), np.array(retros), label = 'kalman filter retrodictions', color='black')
	plt.legend()
	plt.show()

if __name__ == '__main__':
	delta_t = 2.0/60
	F = np.array([[1, delta_t, 1/2*delta_t**2], 
                  [0, 1, delta_t], 
                  [0, 0, 1]])
	H = np.array([1, 0, 0]).reshape(1, 3)
	D = np.array([[1/4*delta_t**4, 1/2*delta_t**3, 1/2*delta_t**2], 
                  [1/2*delta_t**3, delta_t**2, delta_t], 
                  [1/2*delta_t**2, delta_t, 1]])
	R = np.array([0.5]).reshape(1, 1)
	run(F, H, D, R)
```

<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/sensor_data_fusion/output.png?raw=true" width="60%" height="60%">

> output


### Theory example from task

<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/sensor_data_fusion/image.png?raw=true" width="60%" height="60%">

> Example from exercise

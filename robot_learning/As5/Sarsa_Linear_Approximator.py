import numpy as np
import matplotlib.pyplot as mp
import BeCareful
import itertools



class CoarseCoding:
	def __init__(self):
		self.dealer = [(3,6),(6,9),(9,12)]
		self.player = [(1, 6), (4, 9), (7, 12), (10, 15), (13, 18), (16, 21)]
		self.phiSet = np.zeros((BeCareful.states[0],BeCareful.states[1],BeCareful.NoOfActions,36))
		for i in range(BeCareful.states[0]):
			for j in range(BeCareful.states[1]):
				for k in range(BeCareful.NoOfActions):
					self.phiSet[i,j,k,:] = self.CreateFeature(i,j,k)
					#print(self.phiSet[(i,j),k,:])
		
	def CreateFeature(self,x,y,z):
		I = []
		J = []
		feature = np.zeros((36,))
		for i in range(0,len(self.dealer)):
			if self.dealer[i][0] <= x and x <= self.dealer[i][1]:
				I.append(i) 
		for i in range(0,len(self.player)):
			if self.player[i][0] <= y and y <= self.player[i][1]:
				J.append(i)
		#print('new feature-',x,y,z,feature)
		for i in itertools.product(I,J,[0,1]):
			#print(i,i[0],i[1],i[2],(12*i[0])+(2*i[1])+i[2],feature[ (12*i[0])+(2*i[1])+i[2] ])
			feature[ (12*i[0])+(2*i[1])+i[2] ] = 1.0
			#print(feature[ (12*i[0])+(2*i[1])+i[2] ])
			#print(feature,feature.shape)
			
		#print('old feature-',feature,feature.shape)
		return feature.copy()

	def Qaction(self,s,theta):
		Q = np.zeros((BeCareful.NoOfActions,))
		for a in range(BeCareful.NoOfActions):
			#print(s,a,self.phiSet[s,a,:],self.phiSet[s,a,:].shape)
			p = np.reshape(self.phiSet[s[0],s[1],a],(1,36))
			q = np.reshape(theta,(1,36))
			#print('~~~~~~~~~',p,p.shape,q,q.shape,np.dot(p.transpose(),q))
			Q[a] = np.dot(q,p.transpose())
		return Q

	def Features(self,s,a):
		return self.phiSet[s[0],s[1],a,:].copy()

def SarsaLinearApproximatorExplore(lambda_):
	theta = np.zeros((1,36))
	discount = 1.0
	alpha = 0.05
	epsilon = 0.1
	R = 0.0
	functionApproximator = CoarseCoding()
	for episode in range(1000):
		#print(episode,theta)
		#Initialize e(s,a) for each episode, to assure Markov independence	
		e = np.zeros((1,36))			
		game = BeCareful.Game()	
		s = game.current_state
		#print(s)
		Qa = functionApproximator.Qaction(s,theta.copy())
		if np.random.rand() > epsilon:
			a = np.argmax(Qa)
		else:
			a = np.random.choice([0,1], p = [0.5, 0.5])
		while True:
			e = discount*lambda_*e
			#Accumulating the traces 
			e = e + functionApproximator.Features(s,a) #Features are binary. Incrementing e by 1 for the features in s,a
			s_ , r = game.advance(s,a)
			R += r
			
			Qa_ = functionApproximator.Qaction(s_,theta.copy())
			#print(s_,r,a,game.next_action,Qa_)
			if np.random.rand() > epsilon:
				a_ = np.argmax(Qa_)
			else:
				a_ = np.random.choice([0,1], p = [0.5, 0.5])
			delta = r + (discount*Qa_[a_]) - Qa[a]
			theta += (alpha*delta*e)
			a = a_
			if game.terminalstate == True:
				break
	return theta,R
	

def Greedy(theta): #greedy policy, without exploration, accumulating rewards
	discount = 1.0
	alpha = 0.05
	epsilon = 0.1
	R = 0.0
	functionApproximator = CoarseCoding()
	for episode in range(100):
		#print(episode,theta)
		game = BeCareful.Game()	
		s = game.current_state
		#print(s)
		Qa = functionApproximator.Qaction(s,theta.copy())
		a = np.argmax(Qa)
		while True:
			s_ , r = game.advance(s,a)
			R += r			
			a_ = game.next_action 
			a = a_
			if game.terminalstate == True:
				break
	return R
	
def Experiment(lambda_):
	trained_theta,r = SarsaLinearApproximatorExplore(lambda_)
	R = Greedy(trained_theta)
	#print(R)
	return R


l = np.arange(0,1.1,0.1)
r =[]
for i in l:
	r.append(Experiment(i))
	#print(i,Experiment(i) )
print(l,r)
mp.plot(l,r)
mp.show()


		

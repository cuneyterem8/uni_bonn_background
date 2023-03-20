import numpy as np
import matplotlib.pyplot as mp
import BeCareful

def Sarsa(lambda_):
	#Initialize Q(s,a) 
	Q = np.zeros((BeCareful.states[0],BeCareful.states[1],BeCareful.NoOfActions))
	N = np.zeros((BeCareful.states[0],BeCareful.states[1],BeCareful.NoOfActions))
	discount = 1.0
	R = 0.0
	N0 = 10
	for episode in range(1000):
		#print(episode)
		#Initialize e(s,a) for each episode, to assure Markov independence	
		e = np.zeros((BeCareful.states[0],BeCareful.states[1],BeCareful.NoOfActions))			
		game = BeCareful.Game()	
		s = game.current_state
		a = game.next_action
		while True:
			#print(s)
			N[s[0],s[1],a] += 1
			#Take action a, and get the reward r and new state S_
			s_ , r = game.advance(s,a)
			#Apply epsilon-greedy to choose new action
			epsilon = N0 / ( N0 + (np.sum(N,axis=2))[s[0],s[1]] )
			if np.random.rand() > epsilon: #epsilon greedy strategy
				a_ = np.argmax(Q[s_[0],s_[1],:])
			else:
				a_ = game.next_action
			#Compute delta and e(s,a) 
			delta = r + (discount * Q[s_[0],s_[1],a_]) - Q[s[0],s[1],a]
			# Update Q and e for all s and a
			e[s[0],s[1],a] += 1
			alpha = np.zeros((BeCareful.states[0],BeCareful.states[1],BeCareful.NoOfActions))
			#calculating alpha based on N[s,a], for each step
			for i in range(BeCareful.states[0]):
				for j in range(BeCareful.states[1]):
					for action in range(BeCareful.NoOfActions):
						if N[i,j,action] == 0:
							alpha[i,j,action] = 0
						else:
							alpha[i,j,action] = 1/N[i,j,action]
			Q += ( (alpha*delta)*e )
			e = discount*lambda_*e
			#Change s and a to s_ and a_
			s = s_
			a = a_
			if game.terminalstate == True:
				break
			
	return Q.copy(),R

def Greedy(Q): #greedy policy, without exploration, accumulating rewards
	R = 0.0
	for episode in range(100):
		#print(episode,theta)
		game = BeCareful.Game()	
		s = game.current_state
		#print(s)
		a = np.argmax(Q[s[0],s[1],:])
		while True:
			s_ , r = game.advance(s,a)
			R += r			
			a_ = game.next_action 
			a = a_
			if game.terminalstate == True:
				break
	return R
	
def Experiment(lambda_):
	Q,r = Sarsa(lambda_)
	R = Greedy(Q)
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



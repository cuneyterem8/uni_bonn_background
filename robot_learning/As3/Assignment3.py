
import numpy as np
import matplotlib.pyplot as plt
from itertools import product

from Grid import GridWorld

#################################################################Function for question 3.1
def iterative_policy_evaluation():
	""" Do iterative value evaluation V(s) for each state """
	probabilities = [0.125, 0.125, 0.125, 0.625] #probabilities for the 4 actions ( 0,1,2,3) , i.e, moving in the 4 directions
	gridworld = GridWorld()
	V = np.zeros((9, 9))
	theta = 0.00001
	discount = 0.9
	count = 1
	while True:
		print(" Iteration No : ",count)
		delta = 0.0
		V_new = np.zeros((9,9))
		for x,y in product(np.arange(9), repeat=2):
			rewards , move = gridworld.GetRewards4Action(x,y)
			Vs = 0.0 
			# calculating expectation 
			for i in np.arange(4):
				p,q = gridworld.action4(i)
				value = 0.0				
				if move[i] == True:
					value = V[x+p][y+q]
				Vs +=  probabilities[i] * ( rewards[i] + (discount * value) ) #expected value of policy
			V_new[x,y] = Vs
		#print(V_new, V)
		count = count + 1
		if np.all( np.abs( V_new - V ) ) < theta :
			return V_new
		else:
			V = V_new.copy()
	
################################################################Functions for question 3.2
# It can take all 4 actions ( left, right, top, left ) with the equal probabilities  
def policy_evaluation(gridworld,V,policy):
	theta = 0.00001
	discount = 0.9
	#count = 1
	while True:
		#print(" Iteration No : ",count)
		V_new = np.zeros((9,9))
		for x,y in product(np.arange(9), repeat=2):
			rewards , move = gridworld.GetRewards4Action(x,y)
			# calculating V(s) according to previous policy(s) 
			i = int(policy[x,y]) 
			p,q = gridworld.action4(i)
			v = 0.0				
			if move[i] == True:
				v = V[x+p,y+q]
			V_new[x,y] =  ( rewards[i] + (discount * v) )
		#print(V_new, V)
		if np.all( np.abs( V_new - V ) ) < theta :
			return V_new
		else:
			V = V_new.copy()

def policy_improvement(gridworld,policy,V):
	discount = 0.9
	policy_stable = True
	policy_new = np.zeros((9,9))
	for x,y in product(np.arange(9), repeat=2):
		rewards , move = gridworld.GetRewards4Action(x,y)
		valuesofPolicies = [0.0,0.0,0.0,0.0] 
		for i in np.arange(4): #iterating over all 4 actions, and getting the action which gives the maximum policy
			p,q = gridworld.action4(i)
			v = 0.0				
			if move[i] == True:
				v = V[x+p,y+q]
			valuesofPolicies[i] =  ( rewards[i] + (discount * v) )
		policy_new[x,y] = np.argmax(valuesofPolicies)
		if policy[x,y] != policy_new[x,y]:
			policy_stable = False	
	return policy_stable,policy_new

def policy_iteration_algorithm():
	
	gridworld = GridWorld()
	V = np.zeros((9, 9)) #Setting arbitary value
	policy = np.zeros((9, 9)) #setting arbitary policy
	count = 1
	while True:
		V = policy_evaluation(gridworld,V,policy)
		p,policy_new = policy_improvement(gridworld,policy,V)
		#print("Policy after step ",count,"::::",policy_new)
		count = count + 1
		if p == True:
			return policy_new,V
		else:
			policy = policy_new.copy()

###########################################Functions for 3.3

def value_evaluation(gridworld,V):
	theta = 0.00001
	discount = 0.9
	#count = 1
	while True:
		#print(" Iteration No : ",count)
		V_new = np.zeros((9,9))
		for x,y in product(np.arange(9), repeat=2):
			rewards , move = gridworld.GetRewards8Action(x,y)
			values = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] 
			for i in np.arange(8):
				p,q = gridworld.action8(i)
				v = 0.0				
				if move[i] == True:
					v = V[x+p,y+q]
				values[i] = ( rewards[i] + (discount * v) )
			V_new[x,y] = np.max(values)
		#print(V_new, V)
		if np.all( np.abs( V_new - V ) ) < theta :
			return V_new
		else:
			V = V_new.copy()

def get_deterministic_policy(gridworld,V):
	discount = 0.9
	policy = np.zeros((9,9))
	for x,y in product(np.arange(9), repeat=2):
		rewards , move = gridworld.GetRewards8Action(x,y)
		valuesofPolicies = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] 
		for i in np.arange(8):
			p,q = gridworld.action8(i)
			v = 0.0				
			if move[i] == True:
				v = V[x+p,y+q]
				valuesofPolicies[i] =  ( rewards[i] + (discount * v) )
		policy[x,y] = np.argmax(valuesofPolicies)		
	return policy

def value_iteration():
	gridworld = GridWorld()
	V = np.zeros((9, 9))
	V = value_evaluation(gridworld,V)
	policy= get_deterministic_policy(gridworld,V)
	return policy,V	


###########################################Functions for 3.4
# This has the same 4 direction action set. But each policy associated with each action is stochastic, which takes to states with probabilities [0.15,0.70,0.15]
def non_deterministic_value_evaluation(gridworld,V):
	theta = 0.00001
	discount = 0.9
	#count = 1
	prob = [ 0.15,0.70,0.15]
	while True:
		#print(" Iteration No : ",count)
		V_new = np.zeros((9,9))
		for x,y in product(np.arange(9), repeat=2):
			values = [0.0, 0.0, 0.0, 0.0] 
			for i in np.arange(4):
				ndrewards,m = gridworld.non_deterministic_rewards(x,y,i)
				s = 0.0				
				for j in np.arange(2):
					v = 0.0
					if m[j] == True:
						p,q = gridworld.non_deterministic_action(i,j)
						v=V[x+p,y+q]
					s += prob[j] * (ndrewards[j] + discount*v)
				values[i] = s
			V_new[x,y] = np.max(values)
		#print(V_new, V)
		if np.all( np.abs( V_new - V ) ) < theta :
			return V_new
		else:
			V = V_new.copy()

def get_non_deterministic_policy(gridworld,V):
	discount = 0.9
	policy = np.zeros((9,9))
	prob = [ 0.15,0.70,0.15]
	for x,y in product(np.arange(9), repeat=2):
			values = [0.0, 0.0, 0.0, 0.0] 
			for i in np.arange(4):
				ndrewards,m = gridworld.non_deterministic_rewards(x,y,i)
				s = 0.0				
				for j in np.arange(2):
					v = 0.0
					if m[j] == True:
						p,q = gridworld.non_deterministic_action(i,j)
						v=V[x+p,y+q]
					s += prob[j] * (ndrewards[j] + discount*v)
				values[i] = s
			policy[x,y] = np.argmax(values)		
	return policy

def value_iteration_non_deterministic():
	gridworld = GridWorld()
	V = np.zeros((9, 9))
	V = non_deterministic_value_evaluation(gridworld,V)
	policy= get_non_deterministic_policy(gridworld,V)
	return policy,V	






###########matplotlib plot arrows
def plot4arrows(pol):
	plt.xlim(-2,10)
	plt.ylim(-2,10)
	for i in np.arange(9):
		for j in np.arange(9):
			if pol[i,j] ==0:
				plt.arrow(i+0.5, j-0.5, -0.3, 0,head_width=0.2)
			if pol[i,j] ==1:
				plt.arrow(i+0.5, j-0.5, 0, +0.3,head_width=0.2)
			if pol[i,j] ==2:
				plt.arrow(i+0.5, j-0.5, -0.3, 0,head_width=0.2)
			if pol[i,j] ==3:
				plt.arrow(i+0.5, j-0.5, 0, -0.3,head_width=0.2)
	plt.show()


def plot8arrows(pol):
	plt.xlim(-2,10)
	plt.ylim(-2,10)
	for i in np.arange(9):
		for j in np.arange(9):
			if pol[i,j] ==0:
				plt.arrow(i+0.5, j-0.5, -0.3, -0.3,head_width=0.2)
			if pol[i,j] ==1:
				plt.arrow(i+0.5, j-0.5, -0.3, 0,head_width=0.2)
			if pol[i,j] ==2:
				plt.arrow(i+0.5, j-0.5, -0.3, +0.3,head_width=0.2)
			if pol[i,j] ==3:
				plt.arrow(i+0.5, j-0.5, 0, 0.3,head_width=0.2)
			if pol[i,j] ==4:
				plt.arrow(i+0.5, j-0.5, 0.3, 0.3,head_width=0.2)
			if pol[i,j] ==5:
				plt.arrow(i+0.5, j-0.5, 0.3, 0,head_width=0.2)
			if pol[i,j] ==6:
				plt.arrow(i+0.5, j-0.5, 0.3, -0.3,head_width=0.2)
			if pol[i,j] ==7:
				plt.arrow(i+0.5, j-0.5, 0, -0.3,head_width=0.2)
	plt.show()







#########################################################################################################
# Question 3.1 
V_expected = iterative_policy_evaluation()
print("Expected Value of all cells :::",V_expected)
print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~")
#Question 3.2
policy,V = policy_iteration_algorithm()
print("Policy Iteration .. Value of each cell :::")
print(V)
print("Policy ",policy)
plot4arrows(policy)
print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~")
#Question 3.3
policy,V = value_iteration()
print("Value Iteration .. Value of each cell :::")
print(V)
print("Policy ",policy)
plot8arrows(policy)
print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~")
#Question 3.4
policy,V = value_iteration_non_deterministic()
print("Value Iteration non-deterministic .. Value of each cell :::")
print(V)
print("Policy ",policy)
plot4arrows(policy)






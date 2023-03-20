# Robot Learning


Most of the exercises include heavily practical coding and some theoretical math


## Content

Reinforcement learning, Markov decision processes, dynamic programming, Monte Carlo methods, temporal-difference methods, function approximation, liear quadratic regulation, differential dynamic programming, partially observable MDPs, policy gradient methods, inverse reinforcement learning, imitation learning, learning kinematic models, perceiving and handling of objects


### Python

```python
# Calculating expectation , assuming that he can attempt tasks in any order and m
def expectedReturn(policy,attempts):
	if len(policy) > 0 and attempts <= 10 : # Max number of attempts is 10
		# we take the next task and increase attempts
		task = policy[0]

		second_attempt_pass = task[1] + expectedReturn(policy[1:], attempts+2)
		second_attempt_fail = 0 + expectedReturn(policy[1:], attempts+2)
		first_attempt_pass = task[2] * ( task[1] + expectedReturn(poli
		first_attempt_fail = (1-task[2]) * ( 0 + (task[2]/2)*second_

		return first_attempt_pass + first_attempt_fail
		
	else:
		return 0

#Task 2.2
# Find expectations of Policy A and B
tasks = [(1, 12, 0.25), (2, 4, 0.4), (3, 10, 0.35), (4, 5, 0.6), (5, 7, 0.45), (6
import itertools
maxer = 0

for policy in list(itertools.permutations(tasks)) :
	#print("Current Policy :",policy)
	er = expectedReturn(policy,0)
	if er > maxer :
		maxer = er
		policyC = policy
		#print("Expected return of current policy :",er )
print("Best policy: ",policyC)
print("Expected Return of best policy: ",maxer)

# Explanation
# The function expectedReturn models the expectation of this scenario. A student
#( following this policy )
# Probability of passing : Expectation greater than 50%
```

```python
# Expectation Calculation
# It is assuming that only the first non solved task will be attempted again. aft
def expectedReturnFirstNonSolvedTask(policy,attempts,first_task_nonsolved):
	if len(policy) > 0 and attempts <= 8 : # Max number of attempts is 8
	# we take the next task and increase attempts
	task = policy[0]
	if first_task_nonsolved == False:
		second_attempt_pass = task[1] + expectedReturnFirstNonSolvedTask(poli
		second_attempt_fail = 0 + expectedReturnFirstNonSolvedTask(policy[1:
		first_attempt_pass = task[2] * ( task[1] + expectedReturnF
		first_attempt_fail = (1-task[2]) * ( 0 + (task[2]/2)*sec

		return first_attempt_pass + first_attempt_fail

	if first_task_nonsolved == True : #This means that once task has already
		first_attempt_pass = task[2] * ( task[1] + expectedReturnF
		first_attempt_fail = (1-task[2]) * ( 0 + expectedReturnF

		return first_attempt_pass + first_attempt_fail

	else:
		return 0

# Find expectations of Policy A and B 
tasks = [(1, 12, 0.25), (2, 4, 0.4), (3, 10, 0.35), (4, 5, 0.6), (5, 7, 0.45), (6, 3, 0.5), (7, 50, 0.15)]

policyA = tasks #Policy A, sequential order
policyB = sorted(tasks, key=lambda task: task[2], reverse=True) # Policy B, order of increasing difficulty
print("Policy A: ",policyA)
print("Expected return of policy A: ",expectedReturnFirstNonSolvedTask(policyA,0,False))

print("Policy B: ",policyB)
print("Expected return of policy B: ",expectedReturnFirstNonSolvedTask(policyB,0,False))
```

```python
# Randomly shuffle the order of tasks to generate all policies. Check the expected return of each policy and find the best 
# Simulatıon of the iteratıon algorithms . Generating a shuffle of all polıcıes and runnıng ıt on the functıon to fınd the best
import itertools

maxer = 0

for policy in list(itertools.permutations(tasks)) :
    #print("Current Policy :",policy)
    er = expectedReturnFirstNonSolvedTask(policy,0,False)
    if er > maxer :
        maxer = er
        policyC = policy

print("Improved policy C: ",policyC)
print("Expected Return of policy C: ",maxer)
```

Example from exercise
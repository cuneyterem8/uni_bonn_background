#Class to manage and simulate teh grid and its actions
import numpy as np

class GridWorld(object):
	def __init__(self):
		self.grid = np.array([['*', '*', '*', ' ', '*', '*', ' ', '*', '*'],
                               	      [' ', ' ', ' ', ' ', ' ', 'X', ' ', ' ', '*'],
                                      [' ', 'X', 'X', 'X', 'X', 'X', 'X', ' ', ' '],
                                      [' ', ' ', ' ', '*', '*', '*', ' ', 'X', ' '],
                                      [' ', 'X', 'X', 'X', 'X', 'X', 'X', ' ', ' '],
                                      [' ', ' ', ' ', ' ', ' ', ' ', ' ', 'X', ' '],
                                      [' ', 'X', 'X', 'X', 'X', 'X', 'X', 'G', ' '],
                                      [' ', '*', '*', '*', 'X', '*', '*', ' ', 'X'],
                                      [' ', '*', '*', '*', ' ', '*', '*', ' ', 'X']])

	def action4(self,i):
		if i==0:
			return 0,-1
		if i==1:
			return -1,0
		if i==2:
			return 0,1
		if i==3:
			return 1,0


	def GetRewards4Action(self,x,y):
		rewards = [0,0,0,0]
		move = [False,False,False,False]
		if self.grid[x][y] == 'G' or self.grid[x][y] == 'X':
			return rewards,move 		
		for i in np.arange(4):
			p,q = self.action4(i)		
			#print (x,y,p,q)		
			if x+p >=0 and x+p <9 and y+q >=0 and y+q <9:
				#print (x,y,x+p,y+q)
				if self.grid[x+p][y+q] == '*':
					rewards[i] = 5.0
					move[i] = True
				if self.grid[x+p][y+q] == 'G':
					rewards[i] = 100.0
					move[i] = False
				if self.grid[x+p][y+q] == ' ':
					rewards[i] = -1.0
					move[i] = True
				if self.grid[x+p][y+q] == 'X':
					rewards[i] = -20.0
					move[i] = False
			else:
				rewards[i] = -5.0
				move[i] = False
		#print (x,y,rewards,move)
		return rewards,move

	def action8(self,i):
		if i==0:
			return -1,-1
		if i==1:
			return -1,0
		if i==2:
			return -1,1
		if i==3:
			return 0,1
		if i==4:
			return 1,1
		if i==5:
			return 1,0
		if i==6:
			return 1,-1
		if i==7:
			return 0,-1
			


	def GetRewards8Action(self,x,y):
		rewards = [0,0,0,0,0,0,0,0]
		move = [False,False,False,False,False,False,False,False]
		if self.grid[x][y] == 'G' or self.grid[x][y] == 'X':
			return rewards,move 		
		for i in np.arange(8):
			p,q = self.action8(i)		
			#print (x,y,p,q)		
			if x+p >=0 and x+p <9 and y+q >=0 and y+q <9:
				#print (x,y,x+p,y+q)
				if self.grid[x+p][y+q] == '*':
					rewards[i] = 5.0
					move[i] = True
				if self.grid[x+p][y+q] == 'G':
					rewards[i] = 100.0
					move[i] = False
				if self.grid[x+p][y+q] == ' ':
					rewards[i] = -1.0
					move[i] = True
				if self.grid[x+p][y+q] == 'X':
					rewards[i] = -20.0
					move[i] = False
			else:
				rewards[i] = -5.0
				move[i] = False
		#print (x,y,rewards,move)
		return rewards,move

	def non_deterministic_action(self,i,j):
		if i==0:
			if j==0:
				return -1,-1
			if j==1:
				return -1,0
			if j==2:
				return -1,+1
		if i==1:
			if j==0:
				return -1,+1
			if j==1:
				return 0,1
			if j==2:
				return +1,+1
		if i==2:
			if j==0:
				return +1,+1
			if j==1:
				return 1,0
			if j==2:
				return 1,-1
		if i==3:
			if j==0:
				return 1,-1
			if j==1:
				return 0,-1
			if j==2:
				return -1,-1

	def non_deterministic_rewards(self,x,y,i):
		rewards = [0,0,0]
		move = [False,False,False]
		if self.grid[x][y] == 'G' or self.grid[x][y] == 'X':
			return rewards,move 		
		for j in np.arange(2):
			p,q = self.non_deterministic_action(i,j)		
			#print (x,y,p,q)		
			if x+p >=0 and x+p <9 and y+q >=0 and y+q <9:
				#print (x,y,x+p,y+q)
				if self.grid[x+p][y+q] == '*':
					rewards[j] = 5.0
					move[j] = True
				if self.grid[x+p][y+q] == 'G':
					rewards[j] = 100.0
					move[j] = False
				if self.grid[x+p][y+q] == ' ':
					rewards[j] = -1.0
					move[j] = True
				if self.grid[x+p][y+q] == 'X':
					rewards[j] = -20.0
					move[j] = False
			else:
				rewards[j] = -5.0
				move[j] = False
		return rewards,move
		
		
									
			
	

		
	

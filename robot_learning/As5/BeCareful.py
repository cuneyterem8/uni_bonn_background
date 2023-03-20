import numpy as np

NoOfActions = 2
states = (13,45)

class Game:
	
	#Constructor
	def __init__(self):
		#Both player and dealer draw a black card, so its value will be added to current value
		self.current_state = (np.random.random_integers(3,12) ,np.random.random_integers(3,12) +11)
		if self.current_state[0] < self.current_state[1]:
			self.next_action = 1
		else:
			self.next_action = 0
		self.terminalstate = False
		
	#draw a card
	def draw(self):
		cardvalue = np.random.random_integers(3,12)
		cardcolor = np.random.choice([-1,+1], p = [0.3, 0.7])
		#print(cardcolor*cardvalue)
		return (cardcolor*cardvalue)

	# function advance is written w.r.t the player, since dealer is a part of the environment
	# There are only 2 actions 
	# actions : 0 - hit, 1 - stick
	# If action is a stick, then dealer will continue playing 
	def advance(self,state,action):
		if action==1: #player has declaed a stick, so dealer will play 
			dealer = state[0]
			playersum = state[1]-11
			#print('dealer hits...')
			while True:	
				#print(dealer)		
				if dealer < 1:
					r = 1 #dealer loses game, goes bust
					self.terminalstate = True
					break
				if dealer >= 1 and dealer < 15: #dealer continues to hit
					dealer += self.draw()
					continue
				if dealer >= 15 and dealer <= 21: #dealer sticks
					if dealer > playersum: #player loses
						r = -1
					if dealer == playersum:#game draw
						r = 0
					if dealer < playersum: #player wins
						r = 1
					self.terminalstate = True				
					break
				if dealer > 21:
					r = 1 #dealer loses game, goes bust
					self.terminalstate = True
					break				
			return state,r
		if action==0: #player has declared a hit, so player will play 
			dealer = state[0]			
			cardsum = state[1]-11
			#print('Player hits..',dealer,cardsum)
			while True:
				#print(cardsum)				
				if cardsum < 1:
					r = -1 #player loses game
					self.terminalstate = True
					break
				if cardsum >= 1 and cardsum <= 21: #player continues to play. He chooses to hit or stick
					if cardsum < dealer:
						cardsum += self.draw()
						#print(cardsum,dealer)
						continue
					else:
						r = 0 #reward of 0 since this is a nonterminal state
						self.next_action = 1	#change action
						self.terminalstate = False	
						#print(cardsum,dealer,'tie')	
						break		
				if cardsum > 21:
					r = -1 #player loses game
					self.terminalstate = True
					break
				
				
			self.current_state = (dealer,cardsum+11)	
			return self.current_state,r
		



public class NextState {
	
	public char nextState(char currentState, int[] neighbourTypesCount, double p, double f, double q) {
		
		char nextState = currentState;
		
		switch (nextState) {
		// Decide the next state based on the empty/ashes
		case 'A':
			// Check if next state can be converted to tree on the basis of spontaneous growth probability.
			nextState = Math.random() < p ? 'T': 'A';
			// Check if it has a neighbour tree.
			boolean hasTreeNeighbour = neighbourTypesCount[1] > 0;
			
			// Check if it has a neighbour and induced growth probability is satisfied then produce a tree.
			if (hasTreeNeighbour && Math.random() < q) {
				nextState = 'T';
			}
			
			break;
			
		// On the basis of Tree as current state. Decide next state.
		case 'T':
			// Check if it has burning neighbour.
			boolean hasBurningNeighbour = neighbourTypesCount[2] > 0;
			nextState = hasBurningNeighbour ? 'F': 'T';
			
			// Decide the next state as Fire or not on the basis of spontaneous fire probability.
			if (Math.random() < f) {
				nextState = 'F';
			}
			
			break;
		
		// On the basis of Fire as current state. Decide next state.
		case 'F':
			// if fire set next state to Ashes.
			nextState = 'A';
			break;
		}
	
		
		return nextState;
	}
	
}

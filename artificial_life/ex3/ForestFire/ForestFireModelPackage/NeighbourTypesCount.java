

public class NeighbourTypesCount {
	public  int[] neighbourTypesCount(char[][] rectangularGrid, int positionX, int positionY) {
		
		TorusNeighbor torusNeighbor = new TorusNeighbor();
		

		int[] neighbourTypesCount = new int[3];
		neighbourTypesCount[0] = 0;
		neighbourTypesCount[1] = 0;
		neighbourTypesCount[2] = 0;
		
		// Von Neumann Neighbourhood.
		
		int left = positionY - 1;
		int right = positionY + 1;
		int up = positionX - 1;
		int down = positionX + 1;
		
		
		char leftNeighbour = torusNeighbor.torusNeighbour(rectangularGrid, positionX, left);
			
		char rightNeighbour = torusNeighbor.torusNeighbour(rectangularGrid, positionX, right);
				
		char TopNeighbour = torusNeighbor.torusNeighbour(rectangularGrid, up, positionY);
			
		char downNeighbour = torusNeighbor.torusNeighbour(rectangularGrid, down, positionY);
	
		
		neighbourTypesCount = increaseNeighbourTypeCount(leftNeighbour, neighbourTypesCount);
		neighbourTypesCount = increaseNeighbourTypeCount(rightNeighbour, neighbourTypesCount);
		neighbourTypesCount = increaseNeighbourTypeCount(TopNeighbour, neighbourTypesCount);
		neighbourTypesCount = increaseNeighbourTypeCount(downNeighbour, neighbourTypesCount);
		
		return neighbourTypesCount;
	}
	
	public static int[] increaseNeighbourTypeCount(char cell, int[] counts) {
		if (cell == 'A') {
			counts[0] += 1;
		} else if (cell == 'T') {
			counts[1] += 1;
		} else if (cell == 'F') {
			counts[2] += 1;
		}
		
		return counts;
	}
 	
	public  int[] TypeStateCounts(char[][] rectangularGrid) {
		
		int[] count = new int[3];
		
		// Initialize all types of states to 0s.
		for (int i=0; i<count.length; i++) {
			count[i] = 0;
		}
		
		// Count all types of states. Ashes, Fire and burning.
		for (int i=0; i<rectangularGrid.length; i++) {
			for (int j=0; j<rectangularGrid[i].length; j++) {
				char cell = rectangularGrid[i][j];
				
				if (cell == 'A') {
					count[0] += 1;
				} else if (cell == 'T') {
					count[1] += 1;
				} else if (cell == 'F') {
					count[2] += 1;
				}
			}
		}
		
		return count;
	}
	
}

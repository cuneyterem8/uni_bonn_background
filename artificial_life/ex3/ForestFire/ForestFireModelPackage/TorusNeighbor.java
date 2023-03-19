

public class TorusNeighbor {
public  char torusNeighbour(char[][] rectangularGrid, int positionX, int positionY) {
		
		
		int rows_rectangularrectangularGrid = 101;
		int columns_rectangularrectangularGrid = 82;
		
		int torusPositionX = ((positionX % rows_rectangularrectangularGrid) + rows_rectangularrectangularGrid) % rows_rectangularrectangularGrid;
		int torusPositionY = ((positionY % columns_rectangularrectangularGrid) + columns_rectangularrectangularGrid) % columns_rectangularrectangularGrid;
		
		return rectangularGrid[torusPositionX][torusPositionY];
	}
}

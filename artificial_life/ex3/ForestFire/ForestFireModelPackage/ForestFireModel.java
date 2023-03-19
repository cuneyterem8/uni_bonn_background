import java.io.IOException;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.util.Scanner;

public class ForestFireModel {

	public static void main(String[] args) {
		
		// Create a Scanner object to read inputs
		Scanner myObj = new Scanner(System.in); 
		
		// Creating Objects
		NextState nextState = new NextState();
		NeighbourTypesCount neighbourTypesCount = new NeighbourTypesCount();
		
		
		// 101 x 82 sized
		int rows_rectangularrectangularGrid = 101;
		int columns_rectangularrectangularGrid = 82;
		
		// Rectangular grid of 101 x 82 sized
		char[][] rectangularGrid = new char[rows_rectangularrectangularGrid][columns_rectangularrectangularGrid];
		
		// Getting Input from the user for spontaneous growth as the rate. From AL slides : p = rate of spontaneous growth 
		System.out.println("Please enter p, the rate of spontaneous growth:");
		double p =  Double.parseDouble(myObj.nextLine());  // Read user input
		 
		// Getting Input from the user for spontaneous fire as the rate. From AL slides : f = rate of spontaneous fire 
		System.out.print("Please enter f, the rate of spontaneous fire: ");
		double f = Double.parseDouble(myObj.nextLine());
		
		// Getting Input from the user for induced growth  as the rate. From AL slides : q = rate of induced growth 
		System.out.print("Please enter q, the rate of induced growth :  ");
		double q = Double.parseDouble(myObj.nextLine());
		
		// We will write the FinalResults of first 30 generations in the FinalFinalResults.txt file via bufferedwriter.
		BufferedWriter bufferedWriter = null;
		
		try {
			File file = new File("FinalResults.txt");
			if (!file.exists()) {
			     file.createNewFile();
			  }
			FileWriter fw = new FileWriter(file);
			bufferedWriter = new BufferedWriter(fw);
		} catch (IOException e) {
			System.out.println("We found an error in creation of FinalResults.txt file");
		}
		
		// Initialize with empty/Ashes ; Fire turns into ashes 
		for (int i=0; i<rectangularGrid.length; i++) {
			for (int j=0; j<rectangularGrid[i].length; j++) {
				rectangularGrid[i][j] = 'A';
			}
		}
		
		// Here, I run 30 generations, but you can change it to run as much as you can
		for (int g=0; g<30; g++) {
			
			// Create a new array to produce the next generation.
			char[][] nextGeneration = new char[rows_rectangularrectangularGrid][columns_rectangularrectangularGrid];
			
			// Iterate the current generation.
			for (int i=0; i<rectangularGrid.length; i++) {
				for (int j=0; j<rectangularGrid[i].length; j++) {
					
					// Here all types of Neighbours will be counted
					int[] neighbourTypesCount1 = neighbourTypesCount.neighbourTypesCount(rectangularGrid, i, j);
							
					char currentState = rectangularGrid[i][j];
					
					// Prediction of nextstate1 based on currentState, neighbourTypesCount1, p, f, q
					char nextState1 = nextState.nextState(currentState, neighbourTypesCount1, p, f, q);
					nextGeneration[i][j] = nextState1;
				}
			}
			
			rectangularGrid = nextGeneration;
			
			// we have to count number of Trees, fire and ashes in next geneation  
			int[] TypeStateCounts = neighbourTypesCount.TypeStateCounts(rectangularGrid);
	
			
			// Write to a file.
			if (bufferedWriter != null) {
				// Also print the counts for the next generation.
				String FinalResults = TypeStateCounts[0] + " " + TypeStateCounts[1] + " " + TypeStateCounts[2];
				System.out.println("FinalResults are : " + FinalResults);
				try {
					bufferedWriter.write(FinalResults);
					bufferedWriter.newLine();
				} catch (IOException e) {
					System.out.println("There is an error writing into FinalResults file");
				}
			}
			
		}
		
		try {
			bufferedWriter.close();
			myObj.close();
			System.out.println("Successfull !!! Check the Project's Root Folder, FinalResults.txt file will be placed ");
		} catch (IOException e) {
			System.out.println("There is an error closing FinalResults file");
		}
	}
	


	
	
	

}

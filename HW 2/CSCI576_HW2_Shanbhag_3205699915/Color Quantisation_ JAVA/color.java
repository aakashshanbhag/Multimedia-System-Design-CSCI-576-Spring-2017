package colorquant;

// K means algorithm is dependent on initial guess, hence may need more runs for expected answer.
import java.io.*;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;


public class color
{
	//Initialization of all variables.
	private byte [] bytes_original; 
	private byte [] bytes_output;           
	private int height;private int width;
	private int N; // Number of k maps= Clustering parameter. 
	private double [][] leftvalue_input;   private double [][] rightvalue_input;            
	private double [][] leftindex_output;  private double [][] rightvalue_output;       
	private int [] index1;   private int [] index2;                       
	private int [] index1_final;  private int [] index2_final;              
	private double [][] centroid1;     private double [][] centroid2;   
	private double [][] code_book_1;   private double [][] code_book_2;    
	private int [] mapping;                   
	private int [] freq;           
	private int [] freq_sorted;    
	private int [] mapping_code;

	// Passing initial values to the parameters. 
	// Default constructor to the class.

	private color(String fileName )
	{
		//Input data evaluation as given in the starter code.
		try {
			//Checking for input data.
			File file = new File(fileName);
			InputStream is = new FileInputStream(file);
			long len = file.length();
			bytes_original = new byte[(int)len];
			int offset = 0;
			int numRead = 0;
			while (offset < bytes_original.length && (numRead=is.read(bytes_original, offset, bytes_original.length-offset)) >= 0) {
				offset += numRead;
			}
		}
		catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}

		// Default values for all	 images in the assignment.

		width  = 352;
		height = 288;
		leftvalue_input = new double[width*height/2][3];
		rightvalue_input = new double[width*height/2][3];

		// Extracting the odd and even RGB image coordinates to create input tuples to form left and the right sides respectively of the tuple.
		// A tuple is [leftvalue rightvalue].

		int ind = 0;
		for (int x = 0 ; x < width*height/2 ; x++) {
			leftvalue_input[x][0] = 0xFF & bytes_original[ind];
			leftvalue_input[x][1] = 0xFF & bytes_original[ind + width*height];
			leftvalue_input[x][2] = 0xFF & bytes_original[ind + 2*width*height];

			rightvalue_input[x][0] = 0xFF & bytes_original[ind+1];
			rightvalue_input[x][1] = 0xFF & bytes_original[(ind+1) + width*height];
			rightvalue_input[x][2] = 0xFF & bytes_original[(ind+1) + 2*width*height];
			ind = ind + 2;

		}
		// Initialisation to reduce the large sample set to only desired values using highest likelihood logic.
		mapping = new int [(width*height/2)];
		for (int i = 0 ; i < (width*height/2) ; i++) {
			mapping[i] = 0;
		}

	}
	// Calculation of the k means of the left and the right RGB pixels individually and combining them to form tuple. 
	private void clusterdata(int n, int runs, double [][] centroid)
	{
		N = n;

		if (centroid !=null) {
			
			centroid1 = centroid;
			centroid2 = centroid;
			
		}
		else{
			
			//Random value assignment for initial guess.
			centroid1 = new double[N][];
			centroid2 = new double[N][];

			ArrayList idx = new ArrayList();
			
			for (int i = 0; i < n; i++){
				int c;
				do{
					c = (int) (Math.random()*(width*height/2));
				}while(idx.contains(c)); 
				idx.add(c);

			
				centroid1[i] = new double[3];
				centroid2[i] = new double[3];

				for (int j=0; j<3; j++) {
					centroid1[i][j] = leftvalue_input[c][j];
					centroid2[i][j] = rightvalue_input[c][j];
				}

			}
		}

		// Clustering for even or left located pixels of the tuples
		
		double [][] centroid_1 = centroid1;// First guess passed on to calculate further values.
		double threshold = 0.0001;
		int iteration=0;

		while (true){
			
			centroid1 = centroid_1;

			
			index1 = new int[(width*height/2)];
			for (int i=0; i<(width*height/2); i++)
			{
				index1[i] = nearest_centroid(leftvalue_input[i],centroid1);    
			}

			centroid_1 = updateCentroids(index1,leftvalue_input);
			iteration ++;
			if ((runs >0 && iteration >=runs) || converge(centroid1, centroid_1, threshold))
				break;
		}

		System.out.println("Left pixel converges at iteration:  " + iteration);


		// Clustering for odd or right located pixels of the tuples
		double [][] centroid_2 = centroid2;
		iteration=0;

		while (true){
			
			centroid2 = centroid_2;

				index2 = new int[(width*height/2)];
			for (int i=0; i<(width*height/2); i++){
				index2[i] = nearest_centroid(rightvalue_input[i],centroid2);
			}

		
			centroid_2 = updateCentroids(index2,rightvalue_input);
			iteration ++;
			if ((runs >0 && iteration >=runs) || converge(centroid2, centroid_2, threshold))
				break;
		}

		System.out.println("Right pixel converges at iteration: " + iteration);
		System.out.println();
	}

	// Find the nearest_centroid to be assigned to the current pixel.
	private int nearest_centroid(double [] rgb , double[][] centroid){
		double min_euclidean = euclidean(rgb, centroid[0]);
		int index =0;
		for (int i=1; i<N; i++){
			double t = euclidean(rgb, centroid[i]);
			if (min_euclidean>t){
				min_euclidean = t;
				index = i;
			}
		}
		return index;
	}

	// Evaluate the Euclidean distance for checking which is the closest pixel
	private double euclidean(double [] rgb1, double [] rgb2){
		double sum=0;
		for (int i=0; i<3; i++){
			double d = rgb1[i]-rgb2[i];
			sum += d*d;
		}
		return Math.sqrt(sum);
	}

	//Update the centroids after each completed clustered operation on the RGB pixel
	private double [][] updateCentroids(int [] index_location , double[][] img){

		double [][] newc = new double [N][]; 
		int [] counts = new int[N]; 

		
		for (int i=0; i<N; i++){
			counts[i] =0;
			newc[i] = new double [3];
			for (int j=0; j<3; j++)
				newc[i][j] =0;
		}

		for (int i=0; i<(width*height/2); i++){
			int newcount = index_location[i]; 
			for (int j=0; j<3; j++){
				newc[newcount][j] += img[i][j]; 
			}
			counts[newcount]++;
		}

		// Next value to be assigned.
		for (int i=0; i< N; i++){
			for (int j=0; j<3; j++){
				newc[i][j]/= counts[i];
			}
		}

		return newc;// Finally updated new count.
	}

	// Check convergence
	private boolean converge(double [][] centroid_1, double [][] centroid_2, double threshold){
		
		double max = 0;
		for (int i=0; i< N; i++){
			double d= euclidean(centroid_1[i], centroid_2[i]);
			if (max<d)
				max = d;
		}

		if (max < threshold)
			return true;
		else
			return false;

	}

	// Finding the top N symbols (This will be our code book)
	private void reduce_combinations (int N) {

		

		// Map the vector to scalar for all possible combinations of the N*N*N values of a pixel tuple
		for (int i = 0 ; i < (width*height/2) ; i++) {
			int shift = (int) (Math.log(N) / Math.log(2));
			mapping[i] = (index1[i] << shift) + index2[i];
		}

		// Counting number of occurrences (histogram)
		freq = new int[N*N];
		for (int i = 0 ; i < N*N ; i++) {
			int count = 0;
			for (int j = 0 ; j < (width*height/2) ; j++) {
				if(mapping[j] == i) count++;
			}
			freq[i] = count;
		}

		
		int [] freq_temp = new int[N*N];
		for (int i = 0 ; i < N*N ; i++) {
			freq_temp[i] = freq[i];
		}

		Arrays.sort(freq_temp);
		
		for (int i = 0 ; i < freq_temp.length/2 ; i++) {
			int temp = freq_temp[i];
			freq_temp[i] = freq_temp[freq_temp.length - (i+1)];
			freq_temp[freq_temp.length - (i+1)] = temp;
		}

		// Obtaining the most frequent N codewords from the histogram generated.		
		freq_sorted = new int[N];
		for (int i = 0 ; i < N ; i++) {
			freq_sorted[i] = freq_temp[i];
		}

		// Find the scalar value corresponding to those frequencies
		mapping_code = new int [N];
		for (int i = 0 ; i < N ; i++) {
			mapping_code[i] = find(freq , freq_sorted[i]);
		}

		// Get the corresponding index code
		index1_final = new int [N];
		index2_final = new int [N];
		for (int i = 0 ; i < N ; i++) {
			int shift = (int) (Math.log(N) / Math.log(2));
			index1_final[i] = (mapping_code[i] >> shift) & (N-1);
			index2_final[i] = mapping_code[i] & (N-1);
		}

		// Creating the code book
		code_book_1 = new double[N][3];
		code_book_2 = new double[N][3];
		for (int i = 0 ; i < N ; i++) {
			for (int j = 0 ; j < 3 ; j++) {
				code_book_1[i][j] = centroid1[index1_final[i]][j];
				code_book_2[i][j] = centroid2[index2_final[i]][j];
			}
		}

		


	}

	private int find (int[] array , int value) {
		int num = -1;
		for (int i = 0 ; i < array.length ; i++) {
			if (array[i] == value) {
				num = i;
				break;
			}
		}
		return num;
	}
	// Final codeword set to image to reduce to colors in the image by choosing the most frequent K combinations of the RGB tuples.
	private void vector_quant () {

		
		leftindex_output = new double[(width*height/2)][3];
		rightvalue_output = new double[(width*height/2)][3];


		for (int count = 0 ; count < (width*height/2) ; count++) {

			for (int i = 0 ; i < N ; i++) {
				int [] array1 = {index1[count] , index2[count]};
				int [] array2 = {index1_final[i] , index2_final[i]};
				if(Arrays.equals(array1,array2)) {



					//Red
					leftindex_output[count][0] = centroid1[index1[count]][0];
					rightvalue_output[count][0] = centroid2[index2[count]][0];

					//Green
					leftindex_output[count][1] = centroid1[index1[count]][1];
					rightvalue_output[count][1] = centroid2[index2[count]][1];

					//Blue
					leftindex_output[count][2] = centroid1[index1[count]][2];
					rightvalue_output[count][2] = centroid2[index2[count]][2];

					break;

				} else {

					double [] rgb1 = new double[3];
					double [] rgb2 = new double[3];

					for (int j = 0 ; j < 3 ; j++) {
						rgb1[j] = leftvalue_input[count][j];
						rgb2[j] = rightvalue_input[count][j];
					}

					double min = 2 * Math.pow(255,3); 
					int index = -1;

					for (int j = 0 ; j < N ; j++) {
						double error = Math.pow(euclidean(rgb1,code_book_1[j]) , 2) + Math.pow(euclidean(rgb2,code_book_2[j]),2);
						if (error < min ) {
							min = error;
							index = j;
						}
					}

					//Red
					leftindex_output[count][0] = centroid1[index1_final[index]][0];
					rightvalue_output[count][0] = centroid2[index2_final[index]][0];

					//Green
					leftindex_output[count][1] = centroid1[index1_final[index]][1];
					rightvalue_output[count][1] = centroid2[index2_final[index]][1];

					//Blue
					leftindex_output[count][2] = centroid1[index1_final[index]][2];
					rightvalue_output[count][2] = centroid2[index2_final[index]][2];

				}

			}

		}

		
		
	}
	// Converting back to Interlaced RGB format.
	private void byteformat () {

		int id = 0;
		bytes_output = new byte [3*width*height];
		for (int i = 0 ; i < (width*height/2) ; i++) {
			bytes_output[id]                       = (byte) (int) leftindex_output[i][0];
			bytes_output[id + width*height]        = (byte) (int) leftindex_output[i][1];
			bytes_output[id + 2*width*height]      = (byte) (int) leftindex_output[i][2];

			bytes_output[id+1]                     = (byte) (int) rightvalue_output[i][0];
			bytes_output[(id+1) + width*height]    = (byte) (int) rightvalue_output[i][1];
			bytes_output[(id+1) + 2*width*height]  = (byte) (int) rightvalue_output[i][2];

			id = id + 2;
		}
		
	}

	//Main function with steps mentioned.
	public static void main( String[] args )
	{

		int N = Integer.parseInt(args[1]); //Number of clusters
		//Constructor call for initialisation.
		color image = new color(args[0]);

		//Display the input image
		imageReader input_image = new imageReader();
		input_image.title       = "Original Image";
		input_image.showIms(image.width,image.height,image.bytes_original);
		System.out.println("Input image displayed");
		System.out.println();

		//Perform k-Means Clustering
		image.clusterdata( N , 250, null);
		System.out.println("K means application carried out on pixel RGB tuples successfuly");
		System.out.println();

		//Highest occurring tuple pair choice to be taken for codebook final assignment.
		image.reduce_combinations(N);
		System.out.println("Reduction of the large RGB tupled k means combinations "+N*N*N+" to "+N+" codewords");
		System.out.println();

		//Final vector quantised output.
		image.vector_quant();

		//Interlaced RGB for display.
		image.byteformat();

		//Display Vector Quantized Output Image
		imageReader output_image = new imageReader();
		output_image.title = "Vector Quantized Image";
		output_image.showIms(image.width,image.height,image.bytes_output);
		System.out.println("Output image displayed");

	}
}



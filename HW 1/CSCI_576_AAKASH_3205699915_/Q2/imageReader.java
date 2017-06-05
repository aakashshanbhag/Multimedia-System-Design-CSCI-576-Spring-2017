package hw1;
import java.awt.*;
import java.awt.image.*;
import java.io.*;
import javax.swing.*;
import java.util.ArrayList;


public class imageReader {

	JFrame frame;
	JLabel lbIm1;
	JLabel lbIm2;
	BufferedImage img;
	private InputStream is;

	public void showIms(String[] args)
	{
		// Input arguments from command line .
		int width = Integer.parseInt(args[1]);
		int height = Integer.parseInt(args[2]);
		long fps = Long.parseLong(args[3]);

		// Use labels to display the images
		frame = new JFrame();
		GridBagLayout gLayout = new GridBagLayout();
		frame.getContentPane().setLayout(gLayout);
		JLabel lbText1 = new JLabel("Video height"+height+" width"+ width);
		lbText1.setHorizontalAlignment(SwingConstants.CENTER);
		
		//Creation of array list for obtaining pixel values.
		ArrayList<ImageIcon> images = new ArrayList<ImageIcon>();
		
		//Generation of the GUI for display.
		GridBagConstraints c = new GridBagConstraints();
		c.fill = GridBagConstraints.HORIZONTAL;
		c.anchor = GridBagConstraints.CENTER;
		c.weightx = 0.5;
		c.gridx = 0;
		c.gridy = 0;
		frame.getContentPane().add(lbText1, c);
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridx = 0;
		c.gridy = 1;

		// Taking input parameters for display.
		try {
			File file = new File(args[0]);
			is = new FileInputStream(file);

			long len = file.length();
			byte[] bytes = new byte[(int)len];

			int offset = 0;
			int numRead = 0;
			while (offset < bytes.length && (numRead=is.read(bytes, offset, bytes.length-offset)) >= 0)
			{
				offset += numRead;
			}

			// Processing code begins here.
			img = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
			while(true){
				frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
				for (int i=0;i<(len/(3.0*width*height));i++)
				{
					
					long lStartTime = System.currentTimeMillis();

					int ind = 3*width*height*i;
					for(int y = 0; y < height; y++)
					{

						for(int x = 0; x < width; x++)
						{
							byte r = bytes[ind];
							byte g = bytes[ind+height*width];
							byte b = bytes[ind+height*width*2]; 

							int pix = 0xff000000 | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
							img.setRGB(x,y,pix);// set the value to the location.	
							ind++;
							
						}
					}
					ImageIcon image = new ImageIcon(img);
					images.add(image);// Addition of pixel values to the frames created.
					long lEndTime = System.currentTimeMillis();// Calculation of the amount of time taken for processing inside for loop.
					long output = lEndTime - lStartTime;
					displayFrame(image, c);// Display per frame.

					//Debug purpose 
					//System.out.println("Elapsed time in milliseconds inside loop: " + output);

					try {
						long delay = (long) ((1000.0/fps) - output);
						Thread.sleep((delay>0)?delay:0);
						}
					catch(InterruptedException ex) 
					{
						Thread.currentThread().interrupt();
					}

				}
			}

		} 
		catch (FileNotFoundException e)
		{
			e.printStackTrace();
		} catch (IOException e) 
		{
			e.printStackTrace();
		}
	}

// Display per frame.
	private void displayFrame(ImageIcon image, GridBagConstraints c){
		lbIm1 = new JLabel(image);
		frame.getContentPane().add(lbIm1, c);
		frame.pack();
		frame.setVisible(true);
	}
// Main function.
	public static void main(String[] args) {
		imageReader ren = new imageReader();
		ren.showIms(args);
	}

}
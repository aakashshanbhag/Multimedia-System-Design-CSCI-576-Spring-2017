package colorquant;

import java.awt.*;
import java.awt.image.*;
import java.io.*;
import javax.swing.*;
import java.util.ArrayList;

public class imageReader {
	JFrame frame;
	JLabel lbIm1;
	//JLabel lbIm2;
	BufferedImage img;
	String title;

	public void showIms(int width,int height,byte [] bytes) {

		img = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);

		int ind = 0;
		int pix = 0;

		for(int y = 0; y < height; y++){

			for(int x = 0; x < width; x++){

				byte a = 0;
				

					byte r = bytes[ind];
					byte g = bytes[ind+height*width];
					byte b = bytes[ind+height*width*2];
					pix = 0xff000000 | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
					//debug: System.out.println("Image is COLOR RGB");


				img.setRGB(x,y,pix);
				ind++;
			}
		}

		// Use labels to display the images
		frame = new JFrame();
		GridBagLayout gLayout = new GridBagLayout();
		frame.getContentPane().setLayout(gLayout);
		String result = String.format("Height: %d, Width: %d , %s", height, width,title);
		JLabel lbText1 = new JLabel(result);
		lbText1.setHorizontalAlignment(SwingConstants.CENTER);
		
		lbIm1 = new JLabel(new ImageIcon(img));
		
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
		frame.getContentPane().add(lbIm1, c);
		
		frame.pack();
		frame.setVisible(true);
	}

	public static void main(String[] args) {

		int width  = Integer.parseInt(args[1]);
		int height = Integer.parseInt(args[2]);
	

		byte[] bytes = new byte[width*height];

		try {
			File file = new File(args[0]);
			InputStream is = new FileInputStream(file);

			long len = file.length();
		

			int offset = 0;
			int numRead = 0;
			while (offset < bytes.length && (numRead=is.read(bytes, offset, bytes.length-offset)) >= 0) {
				offset += numRead;
			}

		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}

		imageReader ren = new imageReader();
		ren.showIms(width,height,bytes);
	}


}

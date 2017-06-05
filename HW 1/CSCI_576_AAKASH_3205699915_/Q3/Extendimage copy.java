package hw1;
import java.awt.image.*;
import java.io.*;
import javax.swing.*;

public class Extendimage
{
	JFrame frame;
	JLabel lbIm1;
	JLabel lbIm2;
	BufferedImage img;
	private InputStream is;
	private OutputStream stream;
	private byte[][][] extended_imgs;
	
	// Processing method.
	public void showIms(String[] args) throws IOException
	{
		int width1 =0 ;
		int height1 = 0;
		int width=0;
		int height=0;
		int flag=0;
		int flag1=0;

		//Input parameters taken here from command line arguments.
		File file = new File(args[0]);
		is = new FileInputStream(file);
		File file1 = new File(args[1]);
		stream= new FileOutputStream(file1);
		int n=3;
		long len=0;
		String type=(args [2]);
		int alias=Integer.parseInt(args [3]);
		String sd2hd= new String("sd2hd");
		String SD2HD=new String("SD2HD");

		// Taking the input length
		len = file.length();
		byte[] bytes = new byte[(int)len];
		System.out.println("File read success");

		// Setting up according sd2hd or hd2sd and sizes correspondingly		
		if(type.equals(SD2HD) || type.equals(sd2hd) )
		{
			width =176 ;
			height = 144;
			width1=960;
			height1=540;
			flag=1;
		}
		else
		{
			width1 =176 ;
			height1 = 144;
			width=960;
			height=540;
			flag1=1;
		}


		// Declarations needed.
		byte[][][] rgbin=new byte[height][width][3];
		byte[][][] rgbin1=new byte[height][width][3];
		byte[][][] rgbout=new byte[height1][width1][3];
		byte[][][] rgbim=new byte[height+(n-1)][width+(n-1)][3];

		img = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
		
		// Reading input parameters.
		int framecount=  (int) (len/(3.0*width*height));
		byte[] op = new byte[(int)(width1*height1*3*framecount)];
		int offset = 0;
		int numRead = 0;
		byte [][][][] net=new byte[height1][width1][3][framecount];
		while (offset < bytes.length && (numRead=is.read(bytes, offset, bytes.length-offset)) >= 0)
		{
			offset += numRead;
		}

		// Processing code begins here.
		for (int i=0;i<(framecount);i++)
		{
			//per frame processing begins here
			int ind = 3*width*height*i;

			rgbin=inputtake(bytes,width,height,ind);


			// In case of sd2hd without anti-aliasing filter.


			if(type.equals(SD2HD) || type.equals(sd2hd) && alias==0 && flag==1 && flag1==0)
			{

				// Interpolation by nearest neighbor algorithm.

				double widthratio=(double)width/width1;
				double heightratio=(double)height/height1;
				for (int i1 = 0; i1 < height1; i1++) 
				{
					for (int j = 0; j < width1; j++) 
					{
						rgbout[i1][j][0]=rgbin[(int) Math.floor(heightratio*i1)][(int) (Math.floor(widthratio*j))][0];
						rgbout[i1][j][1]=rgbin[(int) (Math.floor(heightratio*i1))][(int) (Math.floor(widthratio*j))][1];
						rgbout[i1][j][2]=rgbin[(int) (Math.floor(heightratio*i1))][(int) (Math.floor(widthratio*j))][2];

						net[i1][j][0][i]=rgbout[i1][j][0];
						net[i1][j][1][i]=rgbout[i1][j][1];
						net[i1][j][2][i]=rgbout[i1][j][2];

						op[(i*3*width1*height1)+(i1*width1)+j]=net[i1][j][0][i];
						op[(i*3*width1*height1)+(i1*width1)+j+(width1*height1)]=net[i1][j][1][i];
						op[(i*3*width1*height1)+(i1*width1)+j+(2*width1*height1)]=net[i1][j][2][i];
					}
				}
			}

			// In case of sd2hd with anti-aliasing post filter.


			else if (type.equals(SD2HD) || type.equals(sd2hd) && alias==1 && flag==1 && flag1==0)
			{

				// Interpolation by nearest neighbor algorithm.

				double widthratio=(double)width/width1;
				double heightratio=(double)height/height1;
				for (int i1 = 0; i1 < height1; i1++) 
				{
					for (int j = 0; j < width1; j++) 
					{
						rgbout[i1][j][0]=rgbin[(int) Math.floor(heightratio*i1)][(int) (Math.floor(widthratio*j))][0];
						rgbout[i1][j][1]=rgbin[(int) (Math.floor(heightratio*i1))][(int) (Math.floor(widthratio*j))][1];
						rgbout[i1][j][2]=rgbin[(int) (Math.floor(heightratio*i1))][(int) (Math.floor(widthratio*j))][2];

					}
				}


				// Post-filtering process.

				extended_imgs = extend1(rgbout,height1,width1,n);
				rgbim=meanfilter(extended_imgs,n,width1,height1);
				rgbin1=mapback(rgbim,width1,height1,n);

				for (int i1 = 0; i1 < height1; i1++) 
				{
					for (int j = 0; j < width1; j++) 
					{

						net[i1][j][0][i]=rgbin1[i1][j][0];
						net[i1][j][1][i]=rgbin1[i1][j][1];
						net[i1][j][2][i]=rgbin1[i1][j][2];


						op[(i*3*width1*height1)+(i1*width1)+j]=net[i1][j][0][i];
						op[(i*3*width1*height1)+(i1*width1)+j+(width1*height1)]=net[i1][j][1][i];
						op[(i*3*width1*height1)+(i1*width1)+j+(2*width1*height1)]=net[i1][j][2][i];
					}
				}



			}

			// In case of hd2sd without anti-aliasing filter.

			else if	(alias==0 && flag==0 && flag1==1)
			{
				// Interpolation by nearest neighbor algorithm.
				int widthratio=(int)((width<<16)/width1)+1;
				int heightratio=(int)((height<<16)/height1)+1;
				for (int i1 = 0; i1 < height1; i1++) 
				{
					for (int j = 0; j < width1; j++) 
					{
						rgbout[i1][j][0]=rgbin[((i1 * heightratio) >> 16)][((j * widthratio) >> 16)][0];
						rgbout[i1][j][1]=rgbin[((i1 * heightratio) >> 16)][((j * widthratio) >> 16)][1];
						rgbout[i1][j][2]=rgbin[((i1 * heightratio) >> 16)][((j * widthratio) >> 16)][2];

						net[i1][j][0][i]=rgbout[i1][j][0];
						net[i1][j][1][i]=rgbout[i1][j][1];
						net[i1][j][2][i]=rgbout[i1][j][2];

						op[(i*3*width1*height1)+(i1*width1)+j]=net[i1][j][0][i];
						op[(i*3*width1*height1)+(i1*width1)+j+(width1*height1)]=net[i1][j][1][i];
						op[(i*3*width1*height1)+(i1*width1)+j+(2*width1*height1)]=net[i1][j][2][i];
					}
				}	
			}

			// In case of hd2sd with anti-aliasing pre-filter.

			else{
				// Pre-filtering process.
				extended_imgs = extend1(rgbin,height,width,n);
				rgbim=meanfilter(extended_imgs,n,width,height);
				rgbin1=mapback(rgbim,width,height,n);
				// Interpolation by nearest neighbor algorithm.
				int widthratio=(int)((width<<16)/width1)+1;
				int heightratio=(int)((height<<16)/height1)+1;
				for (int i1 = 0; i1 < height1; i1++) 
				{
					for (int j = 0; j < width1; j++) 
					{
						rgbout[i1][j][0]=rgbin1[((i1 * heightratio) >> 16)][((j * widthratio) >> 16)][0];
						rgbout[i1][j][1]=rgbin1[((i1 * heightratio) >> 16)][((j * widthratio) >> 16)][1];
						rgbout[i1][j][2]=rgbin1[((i1 * heightratio) >> 16)][((j * widthratio) >> 16)][2];

						net[i1][j][0][i]=rgbout[i1][j][0];
						net[i1][j][1][i]=rgbout[i1][j][1];
						net[i1][j][2][i]=rgbout[i1][j][2];


						op[(i*3*width1*height1)+(i1*width1)+j]=net[i1][j][0][i];
						op[(i*3*width1*height1)+(i1*width1)+j+(width1*height1)]=net[i1][j][1][i];
						op[(i*3*width1*height1)+(i1*width1)+j+(2*width1*height1)]=net[i1][j][2][i];

					}
				}	

			}	

		}

		// writing to output file.
		stream.write(op);
		System.out.println("File write success");
		// End of processing here.

	}
	// Main function
	public static void main(String[] args) throws IOException 
	{
		Extendimage ren = new Extendimage();
		ren.showIms(args);
	}

	// Extension of boundary pixels for appropriate increase filtering. 
	public byte[][][] extend1(byte[][][] inputimg, int h,int w,int n)
	{

		byte[][][]extended_img=new byte[h+(n-1)][w+(n-1)][3];

		//top and down and right and left
		for(int i1=0;i1<(n>>1);i1++)
		{
			for(int j=0;j<w;j++)
			{
				extended_img[i1][j+(n>>1)][0]=inputimg[0][j][0];
				extended_img[i1][j+(n>>1)][1]=inputimg[0][j][1];
				extended_img[i1][j+(n>>1)][2]=inputimg[0][j][2];


				extended_img[i1+h+(n>>1)][j+(n>>1)][0]=inputimg[h-1][j][0];
				extended_img[i1+h+(n>>1)][j+(n>>1)][1]=inputimg[h-1][j][1];
				extended_img[i1+h+(n>>1)][j+(n>>1)][2]=inputimg[h-1][j][2];
			}
		}

		for(int i1=0;i1<h;i1++)
		{	
			for(int j=0;j<w;j++)
			{
				extended_img[i1+(n>>1)][j+(n>>1)][0]=inputimg[i1][j][0];
				extended_img[i1+(n>>1)][j+(n>>1)][1]=inputimg[i1][j][1];
				extended_img[i1+(n>>1)][j+(n>>1)][2]=inputimg[i1][j][2];	
			}
		}
		for(int k=0;k<h;k++)
		{
			for(int  i1=0;i1<(n>>1);i1++)

			{

				extended_img[k+(n>>1)][i1][0]=inputimg[k][0][0];
				extended_img[k+(n>>1)][i1][1]=inputimg[k][0][1];
				extended_img[k+(n>>1)][i1][2]=inputimg[k][0][2];


				extended_img[k+(n>>1)][i1+w+(n>>1)][0]=inputimg[k][w-1][0];
				extended_img[k+(n>>1)][i1+w+(n>>1)][1]=inputimg[k][w-1][1];
				extended_img[k+(n>>1)][i1+w+(n>>1)][2]=inputimg[k][w-1][2];

			}

		}

		//corner.
		for(int i1=0;i1<(n>>1);i1++)
		{
			for(int j=0;j<(n>>1);j++)
			{
				extended_img[i1][j][0]=inputimg[0][0][0];
				extended_img[i1][j][1]=inputimg[0][0][1];
				extended_img[i1][j][2]=inputimg[0][0][2];

				extended_img[i1][j+w+(n>>1)][0]=inputimg[0][w-1][0];
				extended_img[i1][j+w+(n>>1)][1]=inputimg[0][w-1][1];
				extended_img[i1][j+w+(n>>1)][2]=inputimg[0][w-1][2];

				extended_img[i1+h+(n>>1)][j][0]=inputimg[h-1][0][0];
				extended_img[i1+h+(n>>1)][j][1]=inputimg[h-1][0][1];
				extended_img[i1+h+(n>>1)][j][2]=inputimg[h-1][0][2];

				extended_img[i1+h+(n>>1)][j+w+(n>>1)][0]=inputimg[h-1][w-1][0];
				extended_img[i1+h+(n>>1)][j+w+(n>>1)][1]=inputimg[h-1][w-1][1];
				extended_img[i1+h+(n>>1)][j+w+(n>>1)][2]=inputimg[h-1][w-1][2];

			}


		}


		return extended_img;
	}

	// Conversion from 1d array to 3d array.
	public byte[][][] oned23d(byte [ ] bytes, int h,int w,int ind)
	{
		byte[][][] rgbin=new byte[h][w][3];

		for(int y = 0; y < h; y++)
		{

			for(int x = 0; x < w; x++)
			{


				rgbin[y][x][0] = bytes[ind];
				rgbin[y][x][1] = bytes[ind+h*w];
				rgbin[y][x][2] = bytes[ind+h*w*2]; 

				ind++;
			}
		}
		return rgbin;
	}

	// Mean or averaging algorithm. 
	public byte[][][]meanfilter(byte[][][]img1,int n,int Size_w,int Size_h)
	{byte[] Imagedata1r=new byte[n*n];
	byte[] Imagedata1g=new byte[n*n];
	byte[] Imagedata1b=new byte[n*n];

	for(int i=0;i<Size_h-(1);i++)
	{
		for(int j=0;j<Size_w-(1);j++)
		{
			for(int k=0;k<n;k++)
			{
				for(int l=0;l<n;l++)
				{
					Imagedata1r[k*n+l]=img1[i+k][j+l][0];
					Imagedata1g[k*n+l]=img1[i+k][j+l][1];
					Imagedata1b[k*n+l]=img1[i+k][j+l][2];
				}

			}
			img1[i+(n>>1)][j+(n>>1)][0]=(byte) meantake(Imagedata1r,n*n);
			img1[i+(n>>1)][j+(n>>1)][1]=(byte) meantake(Imagedata1g,n*n);
			img1[i+(n>>1)][j+(n>>1)][2]=(byte) meantake(Imagedata1b,n*n);

		}
	}
	return img1;
	}
	// Method to take mean of 1d arrays.

	public int meantake(byte[] a,int n)
	{
		int mean=0;
		for(int i=0;i<n;i++)
		{
			mean=Byte.toUnsignedInt(a[i])+ mean;
		}
		mean=Integer.divideUnsigned(mean, n);
		return mean;
	}
	// Conversion of boundary extended image to original size image.

	public byte[][][] mapback(byte[][][] img1,int Size_w,int Size_h,int n)
	{
		byte[][][] Imgdata=new byte[Size_h][Size_w][3];
		for(int i=0;i<Size_h;i++)
		{	
			for(int j=0;j<Size_w;j++)
			{
				Imgdata[i][j][0]=img1[i+(n>>1)][j+(n>>1)][0];
				Imgdata[i][j][1]=img1[i+(n>>1)][j+(n>>1)][1];
				Imgdata[i][j][2]=img1[i+(n>>1)][j+(n>>1)][2];
			}
		}
		return Imgdata;
	}

	// Nearest neighbor algorithm for conversion from hd2sd.
	public byte[][][][] hd2sd (byte[][][] rgbin, int i, int width, int width1, int height, int height1,int framecount)
	{
		byte[][][] rgbout=new byte[height1][width1][3];
		byte[][][][] net=new byte[height1][width1][3][framecount];
		double widthratio=(double)width/width1;
		double heightratio=(double)height/height1;

		for (int i1 = 0; i1 < height1; i1++) 
		{
			for (int j = 0; j < width1; j++) 
			{
				rgbout[i1][j][0]=rgbin[(int) Math.floor(heightratio*i1)][(int) (Math.floor(widthratio*j))][0];
				rgbout[i1][j][1]=rgbin[(int) (Math.floor(heightratio*i1))][(int) (Math.floor(widthratio*j))][1];
				rgbout[i1][j][2]=rgbin[(int) (Math.floor(heightratio*i1))][(int) (Math.floor(widthratio*j))][2];

				net[i1][j][0][i]=rgbout[i1][j][0];
				net[i1][j][1][i]=rgbout[i1][j][1];
				net[i1][j][2][i]=rgbout[i1][j][2];

			}
		}
		return net;
	}

	// Input conversion from 1d array to 3d array for easier manipulation. 
	public byte[][][] inputtake(byte[] bytes, int width, int height, int ind)
	{
		byte[][][] rgbin=new byte[height][width][3];
		for(int y = 0; y < height; y++)
		{
			for(int x = 0; x < width; x++)
			{
				rgbin[y][x][0] = bytes[ind];
				rgbin[y][x][1] = bytes[ind+height*width];
				rgbin[y][x][2] = bytes[ind+height*width*2]; 
				ind++;
			}
		}
		return rgbin;
	}

}
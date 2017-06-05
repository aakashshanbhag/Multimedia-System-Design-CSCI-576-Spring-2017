#include <iostream>
#include<stdlib.h>
#include "stdafx.h"
#include <math.h>
#include <fstream>
#include<string>
using namespace std;

// Memory allocation for 2d array for char.
char **alloc_mem2char(char **Image,int h,int w)
{ Image=new char *[h];	
	for(int i=0;i<h;i++)
	{
 		Image[i]=new char[w];
	}
return Image;
}
// Allocation of heap memory for 2d array.
unsigned char ***alloc_mem(unsigned char ***Image,int h,int w,int BytesPerPixel)
{ Image=new unsigned char **[h];	
	for(int i=0;i<h;i++)
	{
 		Image[i]=new unsigned char*[w];
		
		for(int j=0;j<w;j++)
		{
			Image[i][j]=new unsigned char[BytesPerPixel];
		
			for(int k=0;k<BytesPerPixel;k++)
			{
				Image[i][j][k]=0;
			}


		}

		
	}
return Image;
}

// Memory allocation for 2d array for  unsigned char.
unsigned char **alloc_mem2(unsigned char **Image,int h,int w)
{ Image=new unsigned char *[h];	
	for(int i=0;i<h;i++)
	{
 		Image[i]=new unsigned char[w];
	}
return Image;
}
// Deallocation of memory from the heap to avoid dangling pointers.
void dealloc_mem(unsigned char *** Img,int h,int w)
{
	for(int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
		{ delete [] Img[i][j];
		}
		delete [] Img[i];
	}
delete [] Img;
}
// Deallocation of memory from the heap to avoid dangling pointers.
void dealloc_mem2(unsigned char ** Img,int h,int w)
{
	for(int i=0;i<h;i++)
	{
		 delete [] Img[i];
	}
	delete [] Img;
}
// Conversion from 1d to 3d array.
unsigned char ***oned23d(unsigned char ***Imgdata,unsigned char *image,int h ,int w, int BytesPerPixel)
{
	for(int i=0;i<h;i++)
	{
 	
		for(int j=0;j<w;j++)
		{
		
			for(int k=0;k<BytesPerPixel;k++)
			{
			Imgdata[i][j][k]=image[3*
					i*w+3*j+k]; 


			}
		}
		
	}
return Imgdata;
}
// Conversion from 1d to 2d array.
unsigned char **oned22d(unsigned char **Imgdata,unsigned char *image,int h ,int w)
{
	for(int i=0;i<h;i++)
	{
 	
		for(int j=0;j<w;j++)
		{
			Imgdata[i][j]=image[1*
					i*w+1*j];
		}
		
	}
return Imgdata;
}
// Conversion from 3d to 1d array.
unsigned char *threed21d(unsigned char *Imagedata1,unsigned char ***img1,int h,int w)
{
	
	for (int i=0;i<h;i++)
	{
		
		for(int j=0;j<w;j++)
		{
			
			Imagedata1[i*w*3+3*j]=img1[i][j][0];
			Imagedata1[i*w*3+3*j+1]=img1[i][j][1];
			Imagedata1[i*w*3+3*j+2]=img1[i][j][2];

		}
	}
	return Imagedata1;
}
// Conversion from 2d to 1d array.
unsigned char *twod21d(unsigned char *Imagedata1,unsigned char **img1,int h,int w)
{
	
	for (int i=0;i<h;i++)
	{
		
		for(int j=0;j<w;j++)
		{
			
			Imagedata1[i*w*1+1*j]=img1[i][j];
	
		}
	}
	return Imagedata1;
}
// File write logic for output.
void filewrite(FILE *file,char* argv,unsigned char *Image,int Size_h,int Size_w,int BytesPerPixel)
{	errno_t err1;
	if ((err1 = fopen_s(&file, argv, "wb"))!=0)
	{
		cout << "Cannot open file: " << argv << endl;
		exit(1);
	}


	// Write image data (filename specified by second argument) from image data matrix
	fwrite(Image, sizeof(unsigned char), Size_h*Size_w*BytesPerPixel, file);
	fclose(file);
	cout << "File write successful" << endl;

}
// File read logic for input.
void fileread(FILE *file,char* argv,unsigned char *Image,int Size_h,int Size_w,int BytesPerPixel)
{
	errno_t err;
	if ((err = fopen_s(&file, argv, "rb"))!=0)
	{
		cout << "Cannot open file: " << argv << endl;
		exit(1);
	}

	fread(Image, sizeof(unsigned char), Size_w*Size_h*BytesPerPixel, file);
	fclose(file);
	cout << "File read successful" << endl;
}
// Extension of boundaries by replication of sides.
unsigned char*** extend(unsigned char ***inputimg, int h,int w,int n)
{
	unsigned char*** extended_img=NULL;
	extended_img=alloc_mem(extended_img,(h+(n-1)),(w+(n-1)),3);
	
	//mirror
	
	
	//top and down and right and left
	for(int i=0;i<(n>>1);i++)
	{
		for(int j=0;j<w;j++)
		{
			extended_img[i][j+(n>>1)][0]=inputimg[0][j][0];
			extended_img[i][j+(n>>1)][1]=inputimg[0][j][1];
			extended_img[i][j+(n>>1)][2]=inputimg[0][j][2];
			
					
			extended_img[i+h+(n>>1)][j+(n>>1)][0]=inputimg[h-1][j][0];
			extended_img[i+h+(n>>1)][j+(n>>1)][1]=inputimg[h-1][j][1];
			extended_img[i+h+(n>>1)][j+(n>>1)][2]=inputimg[h-1][j][2];
		}
	}

	for(int i=0;i<h;i++)
	{	
		for(int j=0;j<w;j++)
		{
			extended_img[i+(n>>1)][j+(n>>1)][0]=inputimg[i][j][0];
			extended_img[i+(n>>1)][j+(n>>1)][1]=inputimg[i][j][1];
			extended_img[i+(n>>1)][j+(n>>1)][2]=inputimg[i][j][2];	
 		}
	}
	for(int k=0;k<h;k++)
	{
		for(int  i=0;i<(n>>1);i++)
	
		{

			extended_img[k+(n>>1)][i][0]=inputimg[k][0][0];
			extended_img[k+(n>>1)][i][1]=inputimg[k][0][1];
			extended_img[k+(n>>1)][i][2]=inputimg[k][0][2];


			extended_img[k+(n>>1)][i+w+(n>>1)][0]=inputimg[k][w-1][0];
			extended_img[k+(n>>1)][i+w+(n>>1)][1]=inputimg[k][w-1][1];
			extended_img[k+(n>>1)][i+w+(n>>1)][2]=inputimg[k][w-1][2];
	
		}

	}
		
	//corner.
	for(int i=0;i<(n>>1);i++)
		{
			for(int j=0;j<(n>>1);j++)
			{
				extended_img[i][j][0]=inputimg[0][0][0];
				extended_img[i][j][1]=inputimg[0][0][1];
				extended_img[i][j][2]=inputimg[0][0][2];

				extended_img[i][j+w+(n>>1)][0]=inputimg[0][w-1][0];
				extended_img[i][j+w+(n>>1)][1]=inputimg[0][w-1][1];
				extended_img[i][j+w+(n>>1)][2]=inputimg[0][w-1][2];

				extended_img[i+h+(n>>1)][j][0]=inputimg[h-1][0][0];
				extended_img[i+h+(n>>1)][j][1]=inputimg[h-1][0][1];
				extended_img[i+h+(n>>1)][j][2]=inputimg[h-1][0][2];

				extended_img[i+h+(n>>1)][j+w+(n>>1)][0]=inputimg[h-1][w-1][0];
				extended_img[i+h+(n>>1)][j+w+(n>>1)][1]=inputimg[h-1][w-1][1];
				extended_img[i+h+(n>>1)][j+w+(n>>1)][2]=inputimg[h-1][w-1][2];

				}


		}


return extended_img;
}

// Mapping back from extended image matrix to original size.
unsigned char *** mapback(unsigned char*** img1,int Size_w,int Size_h,int n,unsigned char *** Imgdata)
{
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

// Creation of Bayer matrix for index 2
unsigned char** indexmatrix2(unsigned char **IndexMatrix)
{
	
IndexMatrix[0][0]=0;
IndexMatrix[0][1]=2;
IndexMatrix[1][0]=3;
IndexMatrix[1][1]=1;
	
return IndexMatrix;
}

// Creation of the threshold matrix.
unsigned char ** thresholdtake(unsigned char **indmat2,int n,unsigned char** threshold)
{
for(int i=0;i<n;i++)
	{
	for(int j=0;j<n;j++)
		{
		threshold[i][j]=(unsigned char)((indmat2[i][j]+0.5)/(1.0*n*n)*255.0);
		cout<<(int)threshold[i][j]<<"\t";
		}
	cout<<endl;}
return threshold;
}


// Creation of Bayer matrix for index 8
unsigned char** matrixcreate8(unsigned char ** IndexMatrix,unsigned char **IndexMatrix4,unsigned char **IndexMatrix8)
{	
	// Generating 4x4 matrix.
for(int i=0;i<4;i=i+2)
{
	for(int j=0;j<4;j=j+2)
	{

		if(i==0 && j==0)
		{
			for ( int k=0;k<2;k++)
			{
				for(int l=0;l<2;l++)
				{
					IndexMatrix4[i+k][j+l]=4*IndexMatrix[k][l];
				}
			}
		}
		else if(i==0 && j==2)
		{
			for ( int k=0;k<2;k++)
			{
				for(int l=0;l<2;l++)
				{
					IndexMatrix4[i+k][j+l]=4*IndexMatrix[k][l]+2;
				}
			}
		}
		 else if(i==2 && j==0)
		{
			for ( int k=0;k<2;k++)
			{
				for(int l=0;l<2;l++)
				{
					IndexMatrix4[i+k][j+l]=4*IndexMatrix[k][l]+3;
				}
			}
		}
		 else	{
			for ( int k=0;k<2;k++)
			{
				for(int l=0;l<2;l++)
				{
					IndexMatrix4[i+k][j+l]=4*IndexMatrix[k][l]+1;
				}
			}
		}	

	}
}

	// Generating 8x8 matrix.
for(int i=0;i<8;i=i+4)
{
	for(int j=0;j<8;j=j+4)
	{

		if(i==0 && j==0)
		{
			for ( int k=0;k<4;k++)
			{
				for(int l=0;l<4;l++)
				{
					IndexMatrix8[i+k][j+l]=4*IndexMatrix4[k][l];
				}
			}
		}
		else if(i==0 && j==4)
		{
			for ( int k=0;k<4;k++)
			{
				for(int l=0;l<4;l++)
				{
					IndexMatrix8[i+k][j+l]=4*IndexMatrix4[k][l]+2;
				}
			}
		}
		 else if(i==4 && j==0)
		{
			for ( int k=0;k<4;k++)
			{
				for(int l=0;l<4;l++)
				{
					IndexMatrix8[i+k][j+l]=4*IndexMatrix4[k][l]+3;
				}
			}
		}
		 else	{
			for ( int k=0;k<4;k++)
			{
				for(int l=0;l<4;l++)
				{
					IndexMatrix8[i+k][j+l]=4*IndexMatrix4[k][l]+1;
				}
			}
		}	

	}
}


return IndexMatrix8;
}
// Creation of Bayer matrix for index 4
unsigned char** matrixcreate4(unsigned char ** IndexMatrix,unsigned char **IndexMatrix4)
{	
	// Generating 4x4 matrix.
for(int i=0;i<4;i=i+2)
{
	for(int j=0;j<4;j=j+2)
	{

		if(i==0 && j==0)
		{
			for ( int k=0;k<2;k++)
			{
				for(int l=0;l<2;l++)
				{
					IndexMatrix4[i+k][j+l]=4*IndexMatrix[k][l];
				}
			}
		}
		else if(i==0 && j==2)
		{
			for ( int k=0;k<2;k++)
			{
				for(int l=0;l<2;l++)
				{
					IndexMatrix4[i+k][j+l]=4*IndexMatrix[k][l]+2;
				}
			}
		}
		 else if(i==2 && j==0)
		{
			for ( int k=0;k<2;k++)
			{
				for(int l=0;l<2;l++)
				{
					IndexMatrix4[i+k][j+l]=4*IndexMatrix[k][l]+3;
				}
			}
		}
		 else	{
			for ( int k=0;k<2;k++)
			{
				for(int l=0;l<2;l++)
				{
					IndexMatrix4[i+k][j+l]=4*IndexMatrix[k][l]+1;
				}
			}
		}	

	}
}
return IndexMatrix4;
}

// Dithering employeed on the image.
unsigned char** Dither(unsigned char** Imgdatac,unsigned char ** Imgdatam,unsigned char **threshold,int Size_w,int Size_h, int n)
{
	for(int i=0;i<Size_h;i++)
	{
		for(int j=0;j<Size_w;j++)
		{

					if((Imgdatac[i][j])>threshold[i%n][j%n])
					{
						Imgdatam[i][j]=255;
					}
					else
					{
						Imgdatam[i][j]=0;
					}

		}
	}

return Imgdatam;
}
// The different 4 matrix to check threhold condition-A4.
unsigned char ** different4(unsigned char** diff4)
{
	diff4[0][0]=14;
	diff4[0][1]=10;
	diff4[0][2]=11;
	diff4[0][3]=15;
	diff4[1][0]=9;
	diff4[1][1]=3;
	diff4[1][2]=0;
	diff4[1][3]=4;
	diff4[2][0]=8;
	diff4[2][1]=2;
	diff4[2][2]=1;
	diff4[2][3]=5;
	diff4[3][0]=13;
	diff4[3][1]=7;
	diff4[3][2]=6;
	diff4[3][3]=12;
	
	return diff4;
}

// Extension by replication of boundary for 2 d array..
unsigned char** extend2d(unsigned char **inputimg, int h,int w,int n)
{
	unsigned char** extended_img=NULL;
	extended_img=alloc_mem2(extended_img,(h+(n-1)),(w+(n-1)));
	
	//top and down and right and left
	for(int i=0;i<(n>>1);i++)
	{
		for(int j=0;j<w;j++)
		{
			extended_img[i][j+(n>>1)]=inputimg[0][j];	
			extended_img[i+h+(n>>1)][j+(n>>1)]=inputimg[h-1][j];
		}
	}

	for(int i=0;i<h;i++)
	{	
		for(int j=0;j<w;j++)
		{
			extended_img[i+(n>>1)][j+(n>>1)]=inputimg[i][j];
		}
	}
	for(int k=0;k<h;k++)
	{
		for(int  i=0;i<(n>>1);i++)
	
		{
			extended_img[k+(n>>1)][i]=inputimg[k][0];
			extended_img[k+(n>>1)][i+w+(n>>1)]=inputimg[k][w-1];
				
		}

	}
		
	//corner.
	for(int i=0;i<(n>>1);i++)
		{
			for(int j=0;j<(n>>1);j++)
			{
				extended_img[i][j]=inputimg[0][0];
				extended_img[i][j+w+(n>>1)]=inputimg[0][w-1];
				extended_img[i+h+(n>>1)][j]=inputimg[h-1][0];
				extended_img[i+h+(n>>1)][j+w+(n>>1)]=inputimg[h-1][w-1];
				
			}

		}
return extended_img;
}
// Mapping back from extended image matrix to original size.
unsigned char ** mapback2d(unsigned char** img1,int Size_w,int Size_h,int n,unsigned char ** Imgdata)
{
	for(int i=0;i<Size_h;i++)
	{	
		for(int j=0;j<Size_w;j++)
		{
			Imgdata[i][j]=img1[i+(n>>1)][j+(n>>1)];
 		}
	}
return Imgdata;
}
// Allocation of 2d array.
double **alloc_mem2double(double **Image,int h,int w)
{ 
	Image=new double *[h];	
	for(int i=0;i<h;i++)
	{
 		Image[i]=new double[w];
		for(int j=0;j<w;j++)
		{
			Image[i][j]=0.0;
		}
	}
return Image;
}
// Floyd Steinberg matrix
double ** floydsteinberg(double** fsd)
{
fsd[0][0]=0;
fsd[0][1]=0;
fsd[0][2]=0;
fsd[1][0]=0;
fsd[1][1]=0;
fsd[1][2]=7;
fsd[2][0]=3;
fsd[2][1]=5;
fsd[2][2]=1;



return fsd;
}
//Floyd Steinberg matrix reverse matrix for right to left movement.
double ** fsdreverse(double** fsd)
{
fsd[0][0]=0;
fsd[0][1]=0;
fsd[0][2]=0;
fsd[1][0]=7;
fsd[1][1]=0;
fsd[1][2]=0;
fsd[2][0]=1;
fsd[2][1]=5;
fsd[2][2]=3;
return fsd;
}

// JJN matrix.
double **jjn(double** fsd)
{
fsd[0][0]=0;
fsd[0][1]=0;
fsd[0][2]=0;
fsd[0][3]=0;
fsd[0][4]=0;
fsd[1][0]=0;
fsd[1][1]=0;
fsd[1][2]=0;
fsd[1][3]=0;
fsd[1][4]=0;
fsd[2][0]=0;
fsd[2][1]=0;
fsd[2][2]=0;
fsd[2][3]=7;
fsd[2][4]=5;
fsd[3][0]=3;
fsd[3][1]=5;
fsd[3][2]=7;
fsd[3][3]=5;
fsd[3][4]=3;
fsd[4][0]=1;
fsd[4][1]=3;
fsd[4][2]=5;
fsd[4][3]=3;
fsd[4][4]=1;


return fsd;


}

// JJN reverse matrix for right to left movement.
double **jjnreverse(double** fsd)
{
fsd[0][0]=0;
fsd[0][1]=0;
fsd[0][2]=0;
fsd[0][3]=0;
fsd[0][4]=0;
fsd[1][0]=0;
fsd[1][1]=0;
fsd[1][2]=0;
fsd[1][3]=0;
fsd[1][4]=0;
fsd[2][0]=7;
fsd[2][1]=5;
fsd[2][2]=0;
fsd[2][3]=0;
fsd[2][4]=0;
fsd[3][0]=3;
fsd[3][1]=5;
fsd[3][2]=7;
fsd[3][3]=5;
fsd[3][4]=3;
fsd[4][0]=1;
fsd[4][1]=3;
fsd[4][2]=5;
fsd[4][3]=3;
fsd[4][4]=1;


return fsd;


}

// Stucki matrix
double **stucki(double** fsd)
{
fsd[0][0]=0;
fsd[0][1]=0;
fsd[0][2]=0;
fsd[0][3]=0;
fsd[0][4]=0;
fsd[1][0]=0;
fsd[1][1]=0;
fsd[1][2]=0;
fsd[1][3]=0;
fsd[1][4]=0;
fsd[2][0]=0;
fsd[2][1]=0;
fsd[2][2]=0;
fsd[2][3]=8;
fsd[2][4]=4;
fsd[3][0]=2;
fsd[3][1]=4;
fsd[3][2]=8;
fsd[3][3]=4;
fsd[3][4]=2;
fsd[4][0]=1;
fsd[4][1]=2;
fsd[4][2]=4;
fsd[4][3]=2;
fsd[4][4]=1;


return fsd;


}
// Reverse Stucki matrix.
double **stuckirev(double** fsd)
{
fsd[0][0]=0;
fsd[0][1]=0;
fsd[0][2]=0;
fsd[0][3]=0;
fsd[0][4]=0;
fsd[1][0]=0;
fsd[1][1]=0;
fsd[1][2]=0;
fsd[1][3]=0;
fsd[1][4]=0;
fsd[2][0]=8;
fsd[2][1]=4;
fsd[2][2]=0;
fsd[2][3]=0;
fsd[2][4]=0;
fsd[3][0]=2;
fsd[3][1]=4;
fsd[3][2]=8;
fsd[3][3]=4;
fsd[3][4]=2;
fsd[4][0]=1;
fsd[4][1]=2;
fsd[4][2]=4;
fsd[4][3]=2;
fsd[4][4]=1;


return fsd;


}
// The 1st phase of Shrinking.
bool shrinkingtable(unsigned char * valuecheck)
{ 
	string compare1[58];
	string s;
	
	
	// S 1
	compare1[0]="001010000";
	compare1[1]="100010000";
	compare1[2]="000010100";
	compare1[3]="000010001";

	// S 2

	compare1[4]="000011000";
	compare1[5]="010010000";
	compare1[6]="000110000";
	compare1[7]="000010010";

	// S 3
	compare1[8]="001011000";
	compare1[9]="011010000";
	compare1[10]="110010000";
	compare1[11]="100110000";
	compare1[12]="000110100";
	compare1[13]="000010110";
	compare1[14]="000010011";
	compare1[15]="000011001";
	
	// STK 4
	compare1[16]="001011001";
	compare1[17]="111010000";
	compare1[18]="100110100";
	compare1[19]="000010111";
	
	// ST 5
	compare1[20]="110011000";
	compare1[21]="010011001";
	compare1[22]="011110000";
	compare1[23]="001011010";
	compare1[24]="011011000";
	compare1[25]="110110000";
	compare1[26]="000110110";
	compare1[27]="000011011";

	// ST 6
	compare1[28]="110011001";
	compare1[29]="011110100";

	// STK 6
	compare1[30]="111011000";
	compare1[31]="011011001";
	compare1[32]="111110000";
	compare1[33]="110110100";
	compare1[34]="000110111";
	compare1[35]="000011111";
	compare1[36]="001011011";
	compare1[37]="100110110";

	// STK 7
	compare1[38]="111011001";
	compare1[39]="111110100";
	compare1[40]="100110111";
	compare1[41]="001011111";

	// STK 8
	compare1[42]="011011011";
	compare1[43]="111111000";
	compare1[44]="110110110";
	compare1[45]="000111111";

	//STK 9 
	compare1[46]="111011011";
	compare1[47]="011011111";
	compare1[48]="111111100";
	compare1[49]="111111001";
	compare1[50]="111110110";
	compare1[51]="110110111";
	compare1[52]="100111111";
	compare1[53]="001111111";

	//STK 10
	compare1[54]="111011111";
	compare1[55]="111111101";
	compare1[56]="111110111";
	compare1[57]="101111111";
	
	// if anything matches break the loop and set the centre value to 0.
	for(int i=0;i<58;i++)
	{
		int count=0;
		s=compare1[i];


				for(int j=0;j<9;j++)
				{
					
					if (((int)s[j]-int('0'))==valuecheck[j])
					{
						
					count++;
					}
			
				}
	if(count==9)
			{
				return (true);
				
			}
	}
	
return (false);
}

// The 1st phase of Thinning.
bool thinningtable( unsigned char * valuecheck)
{
	string compare1[46];
	string s;
	

	// STK 4
	compare1[0]="001011001";
	compare1[1]="111010000";
	compare1[2]="100110100";
	compare1[3]="000010111";
	
	// ST 5
	compare1[4]="110011000";
	compare1[5]="010011001";
	compare1[6]="011110000";
	compare1[7]="001011010";
	compare1[8]="011011000";
	compare1[9]="110110000";
	compare1[10]="000110110";
	compare1[11]="000011011";

	// ST 6
	compare1[12]="110011001";
	compare1[13]="011110100";

	// STK 6
	compare1[14]="111011000";
	compare1[15]="011011001";
	compare1[16]="111110000";
	compare1[17]="110110100";
	compare1[18]="000110111";
	compare1[19]="000011111";
	compare1[20]="001011011";
	compare1[21]="100110110";

	// STK 7
	compare1[22]="111011001";
	compare1[23]="111110100";
	compare1[24]="100110111";
	compare1[25]="001011111";

	// STK 8
	compare1[26]="011011011";
	compare1[27]="111111000";
	compare1[28]="110110110";
	compare1[29]="000111111";

	//STK 9 
	compare1[30]="111011011";
	compare1[31]="011011111";
	compare1[32]="111111100";
	compare1[33]="111111001";
	compare1[34]="111110110";
	compare1[35]="110110111";
	compare1[36]="100111111";
	compare1[37]="001111111";

	//STK 10
	compare1[38]="111011111";
	compare1[39]="111111101";
	compare1[40]="111110111";
	compare1[41]="101111111";

	//TK 4

	compare1[42]="010011000";
	compare1[43]="010011000";
	compare1[44]="000110010";
	compare1[45]="000011010";

	// if anything matches break the loop and set the centre value to 0.
	for(int i=0;i<46;i++)
	{
		int count=0;
		s=compare1[i];
				for(int j=0;j<9;j++)
				{
					if  (((int)s[j]-int('0'))==valuecheck[j])
					{
					count++;
					}
			
				}
	if(count==9)
			{
				return (true);
			}
	}
return (false);

}
// The 1st phase of Skeletonizing.
bool skeletontable(unsigned char * valuecheck)
{string s;
	string compare1[40];
	
	// STK 4
	compare1[0]="001011001";
	compare1[1]="111010000";
	compare1[2]="100110100";
	compare1[3]="000010111";
	
	
	// STK 6
	compare1[04]="111011000";
	compare1[05]="011011001";
	compare1[06]="111110000";
	compare1[7]="110110100";
	compare1[8]="000110111";
	compare1[9]="000011111";
	compare1[10]="001011011";
	compare1[11]="100110110";

	// STK 7
	compare1[12]="111011001";
	compare1[13]="111110100";
	compare1[14]="100110111";
	compare1[15]="001011111";

	// STK 8
	compare1[16]="011011011";
	compare1[17]="111111000";
	compare1[18]="110110110";
	compare1[19]="000111111";


	//STK 9 
	compare1[20]="111011011";
	compare1[21]="011011111";
	compare1[22]="111111100";
	compare1[23]="111111001";
	compare1[24]="111110110";
	compare1[25]="110110111";
	compare1[26]="100111111";
	compare1[27]="001111111";

	//STK 10
	compare1[28]="111011111";
	compare1[29]="111111101";
	compare1[30]="111110111";
	compare1[31]="101111111";

	//TK 4

	compare1[32]="010011000";
	compare1[33]="010011000";
	compare1[34]="000110010";
	compare1[35]="000011010";

	// K 11
	compare1[36]="111111011";
	compare1[37]="111111110";
	compare1[38]="110111111";
	compare1[39]="011111111";

	for(int i=0;i<40;i++)
	{
		int count=0;
		s=compare1[i];
				for(int j=0;j<9;j++)
				{
					if (((int)s[j]-int('0'))==valuecheck[j])
					{
					count++;
					}
			
				}
	if(count==9)
			{
				return (true);
			}
	}
return (false);
	
}

// Shrinking and thinning @nd phase filters
bool shrink_thin2phase(unsigned char * valuecheck)
{ 
unsigned char** value=NULL;
value=alloc_mem2(value,3,3);

// converting to 2d array for easier comparsion.
for(int i=0;i<3;i++)
{
	for (int j=0;j<3;j++)
	{
		value[i][j]= valuecheck[3*i+j];
		
	}
}
//spur
if( value[0][0]==0 && value[0][1]==0 && value[1][0]==0 && value[1][2]==0 && value[2][0]==0 && value[2][1]==0 && value[2][2]==0 && value[0][2]==1 && value[1][1]==1)
{ 
	return (true);
}
 if( value[0][0]==1 && value[0][1]==0 && value[1][0]==0 && value[1][2]==0 && value[2][0]==0 && value[2][1]==0 && value[2][2]==0 && value[0][2]==0 && value[1][1]==1)
{ 
		return (true);
}
//single
 if( value[0][0]==0 && value[0][1]==0 && value[1][0]==0 && value[1][2]==0 && value[2][0]==0 && value[2][1]==1 && value[2][2]==0 && value[0][2]==0 && value[1][1]==1)
{ 
		return (true);
}
 if( value[0][0]==0 && value[0][1]==0 && value[1][0]==0 && value[1][2]==1 && value[2][0]==0 && value[2][1]==0 && value[2][2]==0 && value[0][2]==0 && value[1][1]==1)
{ 
		return (true);
}
//l cluster
 if( value[0][0]==0 && value[0][1]==0 && value[1][0]==0 && value[1][2]==1 && value[2][0]==0 && value[2][1]==0 && value[2][2]==0 && value[0][2]==1 && value[1][1]==1)
{
		return (true);
}
 if( value[0][0]==0 && value[0][1]==1 && value[1][0]==0 && value[1][2]==0 && value[2][0]==0 && value[2][1]==0 && value[2][2]==0 && value[0][2]==1 && value[1][1]==1)
{ 
		return (true);
}
 if( value[0][0]==1 && value[0][1]==1 && value[1][0]==0 && value[1][2]==0 && value[2][0]==0 && value[2][1]==0 && value[2][2]==0 && value[0][2]==0 && value[1][1]==1)
{ 
		return (true);
}
 if( value[0][0]==1 && value[0][1]==0 && value[1][0]==1 && value[1][2]==0 && value[2][0]==0 && value[2][1]==0 && value[2][2]==0 && value[0][2]==0 && value[1][1]==1)
{ 
		return (true);
}
//

 if( value[0][0]==0 && value[0][1]==0 && value[1][0]==1 && value[1][2]==0 && value[2][0]==1 && value[2][1]==0 && value[2][2]==0 && value[0][2]==0 && value[1][1]==1)
{ 
		return (true);
}
 if( value[0][0]==0 && value[0][1]==0 && value[1][0]==0 && value[1][2]==0 && value[2][0]==1 && value[2][1]==1 && value[2][2]==0 && value[0][2]==0 && value[1][1]==1)
{ 
		return (true);
}
 if( value[0][0]==0 && value[0][1]==0 && value[1][0]==0 && value[1][2]==0 && value[2][0]==0 && value[2][1]==1 && value[2][2]==1 && value[0][2]==0 && value[1][1]==1)
{ 
		return (true);
}
 if( value[0][0]==0 && value[0][1]==0 && value[1][0]==0 && value[1][2]==1 && value[2][0]==0 && value[2][1]==0 && value[2][2]==1 && value[0][2]==0 && value[1][1]==1)
{ 
		return (true);
}
//4
 if( value[0][0]==0 && value[0][1]==1 && value[1][0]==1 && value[1][2]==0 && value[2][0]==0 && value[2][1]==0 && value[2][2]==0 && value[0][2]==1 && value[1][1]==1)
{ 
		return (true);
}
 if( value[0][0]==1 && value[0][1]==1 && value[1][0]==0 && value[1][2]==1 && value[2][0]==0 && value[2][1]==0 && value[2][2]==0 && value[0][2]==0 && value[1][1]==1)
{ 
		return (true);
}
 if( value[0][0]==0 && value[0][1]==1 && value[1][0]==0 && value[1][2]==1 && value[2][0]==0 && value[2][1]==0 && value[2][2]==1 && value[0][2]==0 && value[1][1]==1)
{ 
		return (true);
}
 if( value[0][0]==0 && value[0][1]==0 && value[1][0]==0 && value[1][2]==1 && value[2][0]==0 && value[2][1]==1 && value[2][2]==0 && value[0][2]==1 && value[1][1]==1)
{ 
		return (true);
}
//spur
 if( value[0][0]==0 && (value[0][1]==1||value[1][2]==1) && value[1][0]==0 && value[2][0]==1 && value[2][1]==0 && value[2][2]==0 && value[0][2]==1 && value[1][1]==1)
{ 
		return (true);
}
 if( value[1][2]==0 && (value[0][1]||value[1][0]==1) && value[0][0]==1 && value[2][0]==0 && value[2][1]==0 && value[2][2]==1 && value[0][2]==0 && value[1][1]==1)
{ 
		return (true);
}
 if( value[0][0]==0 && (value[2][1]||value[1][0]==1) && value[0][1]==0 && value[2][0]==1 && value[0][2]==1 && value[2][2]==0 && value[1][2]==0 && value[1][1]==1)
{ 
		return (true);
}
 if( value[0][0]==1 && (value[2][1]||value[1][2]==1) && value[1][0]==0 && value[2][0]==0 && value[0][1]==0 && value[2][2]==1 && value[0][2]==0 && value[1][1]==1)
{ 
		return (true);
}
//cc
 if( value[0][0]==1 && value[0][1]==1 && value[1][1]==1 && value[1][0]==1)
{ 
		return (true);
}
//tee
 if( value[1][2]==1 && value[0][1]==1 && value[1][1]==1 && value[1][0]==1 && value[0][2]==0 && value[2][2]==0 && value[2][1]==0)
{ 
		return (true);
}
 if( value[1][2]==1 && value[0][1]==1 && value[1][1]==1 && value[1][0]==1 && value[0][0]==0 && value[2][0]==0 && value[2][1]==0)
{ 
		return (true);
}
 if( value[1][2]==1 && value[0][1]==0 && value[1][1]==1 && value[1][0]==1 && value[2][1]==1 && value[0][0]==0 && value[2][0]==0)
{ 
		return (true);
}
 if( value[1][2]==1 && value[0][1]==0 && value[1][1]==1 && value[1][0]==1 && value[0][2]==0 && value[2][1]==1 && value[2][2]==0)
{ 
		return (true);
}
//
 if( value[1][2]==0 && value[0][1]==1 && value[1][1]==1 && value[1][0]==1 && value[2][2]==0 && value[2][0]==0 && value[2][1]==1)
{ 
		return (true);
}
 if( value[0][2]==0 && value[0][1]==1 && value[1][1]==1 && value[1][0]==1 && value[0][0]==0 && value[2][1]==1 && value[1][2]==0)
{ 
		return (true);
}
 if( value[1][2]==1 && value[0][1]==1 && value[1][1]==1 && value[1][0]==0 && value[2][1]==1 && value[0][2]==0 && value[0][0]==0)
{ 
		return (true);
}
 if( value[0][1]==1 && value[1][0]==0 && value[1][1]==1 && value[1][2]==1 && value[2][1]==1 && value[2][2]==0 && value[2][0]==0)
{ 
		return (true);
}

//vee
 if( value[0][0]==1 && value[0][2]==1 && value[1][1]==1 && (value[2][0]||value[2][1]||value[2][2]==1) )
{ 
		return (true);
}
 if( value[0][0]==1 && value[2][0]==1 && value[1][1]==1 && (value[0][2]||value[1][2]||value[2][2]==1) )
{ 
		return (true);
} if( value[2][2]==1 && value[2][0]==1 && value[1][1]==1 && (value[0][0]||value[0][1]||value[0][2]==1) )
{ 
		return (true);
} if( value[0][2]==1 && value[2][2]==1 && value[1][1]==1 && (value[0][0]||value[1][0]||value[2][0]==1 ))
{ 
		return (true);
}
//diagonal branch.
 if( value[0][1]==1 && value[0][2]==0 && value[1][0]==0 && value[1][1]==1 && value[1][2]==1 && value[2][0]==1 && value[2][1]==0)
{ 
		return (true);
}
 if( value[0][0]==0 && value[0][1]==1 && value[1][0]==1 && value[1][1]==1 && value[1][2]==0 && value[2][1]==0 && value[2][2]==1)
{ 
		return (true);
} 
 if( value[0][1]==0 && value[0][2]==1 && value[1][0]==1 && value[1][1]==1 && value[1][2]==0 && value[2][0]==0 && value[2][1]==1)
{ 
		return (true);
}
 if( value[0][0]==1 && value[0][1]==0 && value[1][0]==0 && value[1][1]==1 && value[1][2]==1 && value[2][1]==1 && value[2][2]==0)
{ 
		return (true);
}

return (false);
}

// 2nd phase of Skeletonizing.
bool skeloton2phase(unsigned char * valuecheck)
{
unsigned char** value=NULL;
value=alloc_mem2(value,3,3);

// converting to 2d array for easier comparsion.
for(int i=0;i<3;i++)
{
	for (int j=0;j<3;j++)
	{
		value[i][j]= valuecheck[3*i+j];
		
	}
}

if( value[0][0]==0 && value[0][1]==0 && value[1][0]==0 && value[1][2]==0 && value[2][0]==0 && value[2][1]==0 && value[2][2]==1 && value[0][2]==0 && value[1][1]==1)
{
	return (true);
}
 if( value[0][0]==0 && value[0][1]==0 && value[1][0]==0 && value[1][2]==0 && value[2][0]==1 && value[2][1]==0 && value[2][2]==0 && value[0][2]==0 && value[1][1]==1)
{ 
		return (true);
}
 if( value[0][0]==0 && value[0][1]==0 && value[1][0]==0 && value[1][2]==0 && value[2][0]==1 && value[2][1]==0 && value[2][2]==0 && value[0][2]==0 && value[1][1]==1)
{ 
		return (true);
}
 if( value[0][0]==0 && value[0][1]==0 && value[1][0]==0 && value[1][2]==0 && value[2][0]==1 && value[2][1]==0 && value[2][2]==0 && value[0][2]==1 && value[1][1]==1)
{ 
		return (true);
}
 if( value[0][0]==1 && value[0][1]==0 && value[1][0]==0 && value[1][2]==0 && value[2][0]==0 && value[2][1]==0 && value[2][2]==0 && value[0][2]==1 && value[1][1]==1)
{ 
		return (true);
}//2nd
 if( value[0][0]==0 && value[0][1]==0 && value[1][0]==0 && value[1][2]==0 && value[2][0]==0 && value[2][1]==1 && value[2][2]==0 && value[0][2]==1 && value[1][1]==1)
{ 
		return (true);
}
 if( value[0][0]==0 && value[0][1]==0 && value[1][0]==0 && value[1][2]==1 && value[2][0]==0 && value[2][1]==0 && value[2][2]==0 && value[0][2]==1 && value[1][1]==1)
{ 
		return (true);
}
 if( value[0][0]==0 && value[0][1]==0 && value[1][0]==1 && value[1][2]==0 && value[2][0]==0 && value[2][1]==0 && value[2][2]==0 && value[0][2]==1 && value[1][1]==1)
{ 
		return (true);
}
 if( value[0][0]==0 && value[0][1]==1 && value[1][0]==0 && value[1][2]==0 && value[2][0]==0 && value[2][1]==0 && value[2][2]==0 && value[0][2]==1 && value[1][1]==1)
{ 
		return (true);
}//3rd
	
 if( value[0][0]==0 && value[0][1]==1 && value[1][0]==0 && value[1][2]==1 && value[2][0]==0 && value[2][1]==1 && value[2][2]==0 && value[0][2]==0 && value[1][1]==1)
{ 
		return (true);
}
 if( value[0][0]==0 && value[0][1]==1 && value[1][0]==1 && value[1][2]==0 && value[2][0]==0 && value[2][1]==0 && value[2][2]==0 && value[0][2]==0 && value[1][1]==1)
{ 
		return (true);
}
 if( value[0][0]==0 && value[0][1]==0 && value[1][0]==1 && value[1][2]==1 && value[2][0]==0 && value[2][1]==1 && value[2][2]==0 && value[0][2]==0 && value[1][1]==1)
{ 
		return (true);
}
 if( value[0][0]==0 && value[0][1]==1 && value[1][0]==1 && value[1][2]==0 && value[2][0]==0 && value[2][1]==1 && value[2][2]==0 && value[0][2]==0 && value[1][1]==1)
{ 
		return (true);
}//4rd
 if( value[0][0]==1 && value[0][1]==1 && value[1][1]==1 && value[1][0]==1)
{ 
		return (true);
}
 if( value[2][2]==1 && value[2][1]==1 && value[1][1]==1 && value[1][2]==1)
{ 
		return (true);
}
//5th
 if( value[1][2]==1 && value[0][1]==1 && value[1][1]==1 && value[1][0]==1)
{ 
		return (true);
}
 if( value[0][1]==1 && value[2][1]==1 && value[1][1]==1 && value[1][0]==1)
{ 
		return (true);
}
 if( value[1][2]==1 && value[2][1]==1 && value[1][1]==1 && value[1][0]==1)
{ 
		return (true);
}
 if( value[0][1]==1 && value[2][1]==1 && value[1][1]==1 && value[1][2]==1)
{ 
		return (true);
}
//6th
 if( value[0][0]==1 && value[0][2]==1 && value[1][1]==1 && (value[2][0]||value[2][1]||value[2][2]==1) )
{ 
		return (true);
}
 if( value[0][0]==1 && value[2][0]==1 && value[1][1]==1 && (value[0][2]||value[1][2]||value[2][2]==1) )
{ 
		return (true);
} if( value[2][2]==1 && value[2][0]==1 && value[1][1]==1 && (value[0][0]||value[0][1]||value[0][2]==1) )
{ 
		return (true);
}
 if( value[0][2]==1 && value[2][2]==1 && value[1][1]==1 && (value[0][0]||value[1][0]||value[2][0]==1 ))
{ 
		return (true);
}
if( value[0][1]==1 && value[0][2]==0 && value[1][0]==0 && value[1][1]==1 && value[1][2]==1 && value[2][0]==1 && value[2][1]==0)
{ 
		return (true);
}
 if( value[0][0]==0 && value[0][1]==1 && value[1][0]==1 && value[1][1]==1 && value[1][2]==0 && value[2][1]==0 && value[2][2]==1)
{ 
		return (true);
}
 if( value[0][1]==0 && value[0][2]==1 && value[1][0]==1 && value[1][1]==1 && value[1][2]==0 && value[2][0]==0 && value[2][1]==1)
{ 
		return (true);
}
 if( value[0][0]==1 && value[0][1]==0 && value[1][0]==0 && value[1][1]==1 && value[1][2]==1 && value[2][1]==1 && value[2][2]==0)
{ 
		return (true);
}
return (false);
}

// Floodfill recursive approach.
void  floodfill(unsigned char **img, int i, int j)
{

	if (img[i][j]==255)
	{
		return;
	}

	if(img[i][j]==0)
	{
		img[i][j]=255;
		floodfill(img,i-1,j);
		floodfill(img,i+1,j);
		floodfill(img,i,j-1);
		floodfill(img,i,j+1);

	}

}



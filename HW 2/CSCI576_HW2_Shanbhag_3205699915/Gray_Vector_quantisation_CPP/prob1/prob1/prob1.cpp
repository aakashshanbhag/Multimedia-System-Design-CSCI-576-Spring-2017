// Inclusion of all headers
#include "stdafx.h"
#include <iostream>
#include "Header1.h"

using namespace std;
#include <fstream>

int main(int argc, char* argv[])
{
	// Define file pointer and variables
	FILE* file=NULL;
	int BytesPerPixel1=1;
	if (argc < 3)
	{
		cout << "Syntax Error - Incorrect parameter usage:" << endl;
		cout<<  "program_name input_image.raw output_image.raw"<<endl;
		return 0;
	}
	
	 int Size_h=288;// Width of the image used.
	 int Size_w=352;// Height of the image used.
	int n=atoi(argv[3]);

	//	Define 1 d array fr image printing

	unsigned char *Imagedatac;
	unsigned char *Imagedatam;
	unsigned char *leftindex;
	unsigned char *rightindex;
	leftindex= new unsigned char[Size_w*Size_h/2];
	rightindex=new unsigned char[Size_w*Size_h/2];
	Imagedatac= new unsigned char [Size_w*Size_h*BytesPerPixel1];// Input image matrix
	Imagedatam= new unsigned char [Size_w*Size_h*BytesPerPixel1];// Output image matrix
	
	//	Reading input image.

	fileread(file,argv[1],Imagedatac,Size_h,Size_w,BytesPerPixel1);
	
	// Allocation of multi-dimensional array for easier handling of pixels.

	unsigned char **Imgdatac=NULL;
	Imgdatac=alloc_mem2(Imgdatac,Size_h,Size_w);
	unsigned char **Imgdatam=NULL;
	Imgdatam=alloc_mem2(Imgdatam,Size_h,Size_w);
	
	unsigned char **img=NULL;
	img=alloc_mem2(img,Size_h*Size_w/2.0,2);
	
	unsigned char **img1=NULL;
	img1=alloc_mem2(img1,Size_h*Size_w/2.0,2);

	unsigned char **centroid=NULL;
	centroid=alloc_mem2(centroid,n,2);// 16 case.
	
	unsigned char **centroid1=NULL;
	centroid1=alloc_mem2(centroid1,n,2);// 16 case.
	
	double **distance=NULL;
	distance=alloc_mem2double(distance,Size_w*Size_h/2.0,n);// 16 case.

	unsigned char ***cluster=NULL;
	cluster=alloc_mem(cluster,Size_w*Size_h/2.0,2,n);// 16 case.



//----------------------------------------------------------------------------------------------------------
	//	Conversion from 1 dimensional array to Multi dimensional array for easier manipulation.

	Imgdatac=oned22d(Imgdatac,Imagedatac,Size_h,Size_w);//input.
	Imgdatam=oned22d(Imgdatam,Imagedatam,Size_h,Size_w);// output
	

	// Taking the input set of 2pixel batches and setting up of scatter plot for input.
	int k=0;
	for(int i=0;i<Size_h;i++)
	{
		for(int j=0;j<Size_w-1;j=j+2)
		{

			leftindex[k]=Imgdatac[i][j];
			k++;
		}
	}
	// Right index
	int l=0;
	for(int i=0;i<Size_h;i++)
	{
		for(int j=1;j<Size_w;j=j+2)
		{

			rightindex[l]=Imgdatac[i][j];
			l++;
		}
	}
	// Output of the scatterdata to xls sheet.
	ofstream file1;
	file1.open("scatterdata1.xls",ios::out);
	for(int i=0;i<(Size_w*Size_h/2);i++)
	{
		file1<<i+1<<" \t     "<<(leftindex[i])<<" \t     "<<(rightindex[i])<<endl;
	}
	file1.close();
//----------------------------------------------------------------------------------------------------------	
	// Store into 2 dimensional array

	for(int i=0;i<(Size_w*Size_h/2);i++)
	{
		img[i][0]=leftindex[i];
		img[i][1]=rightindex[i];
		//cout<<i<<"   "<< (int)img[i][0]<< "   "<< (int)img[i][1]<< endl;
	}
	

	// Applying K means algorithm to setup N gray levels.
	// Randomly select N points.

	cout<<"Initial value of centroid:";
	int *index1=new int [n];
	for(int i=0;i<n;i++)
	{
		index1[i]=rand()%(Size_w*Size_h/2);
		centroid[i][0]=img[index1[i]][0];
		centroid[i][1]=img[index1[i]][1];
		cout<<(int)centroid[i][0]<<"   "<<(int)centroid[i][1]<<endl;
	}



//----------------------------------------------------------------------------------------------------------

	int counter=0;
	int counter1=0;
//----------------------------------------------------------------------------------------------------------
	while (counter!=n)
	{counter1++;
	
	for(int k=0;k<n;k++)
	{
		centroid1[k][0]=centroid[k][0];
		centroid1[k][1]=centroid[k][1];
	}

	// Calculate the distance of each point wrt N points. 
	
	for(int i=0;i<(Size_w*Size_h/2.0);i++)
	{
		for(int r=0;r<n;r++)
		{
			 distance[i][r]= sqrt(pow(int(centroid[r][0]-img[i][0]),2.0)+pow(int(centroid[r][1]-img[i][1]),2.0));
		//	cout<<i<< "  "<<(int)distance[i][r]<<endl;
		}
	//cout<< "   "<<endl;
	}

	// Minimum value is obtained in this case.
	for(int i=0;i<(Size_w*Size_h/2.0);i++)
	{	int index=0;
		int min=(int)(distance[i][0]);

		for(int r=0;r<n;r++)
		{//	cout<<i<< "  "<<(int)distance[i][r]<<endl;
			if(min>(int)distance[i][r])
			{
				min=(int)distance[i][r];
				index=r;
			}
			
		}
	//cout<<index<<endl;
	cluster[i][0][index]=img[i][0];
	cluster[i][1][index]=img[i][1];
	 //cout<< (int)cluster[i][0][index]<<  "   "<<(int)cluster[i][1][index]<<endl;
	 //cout<<endl;
	}

	// New mean calculation.
	// Recalculate the centre of the cluster setup.
	for(int r=0;r<n;r++)
		{	
			int count=0;
			int sumx=0;
			int sumy=0;
			
			for(int i=0;i<(Size_w*Size_h/2.0);i++)
			{
				if (cluster[i][0][r]!=0 || cluster[i][1][r]!=0)
				
				{	count++;
					sumx=sumx+cluster[i][0][r];
					sumy=sumy+cluster[i][1][r];

				
				}

			}
			// Calculate the new centroid
			centroid[r][0]=sumx/(1.0*count);
			centroid[r][1]=sumy/(1.0*count);
			
	//		cout <<"the new centroid "<< (int)centroid[r][0] <<"   "<<(int)centroid[r][1]<<endl;
			
			//cout<<"cluster"<<r<<"contains"<<count<<endl;
	}

	cout<<"Iterations completed:"<<counter1<<endl;


	counter=0;
	for(int k=0;k<n;k++)
	{
	if(centroid1[k][0]==centroid[k][0] && centroid1[k][1]==centroid[k][1])
	{
	counter++;
	}
	}
	}



	// Find minimum distance for each point and put that point inside that cluster.

	cout <<"the new centroid "<<endl;
	for(int r=0;r<n;r++)
		{
			cout << (int)centroid[r][0] <<"   "<<(int)centroid[r][1]<<endl;
		}


	for(int i=0;i<(Size_w*Size_h/2.0);i++)
	{
		for(int r=0;r<n;r++)
		{
			if(cluster[i][0][r]!=0 || cluster[i][1][r]!=0)
			{
			cluster[i][0][r]=centroid[r][0];
			cluster[i][1][r]=centroid[r][1];

			img1[i][0]=cluster[i][0][r];
			img1[i][1]=cluster[i][1][r];
			}//cout<<i<<"   "<< (int)img1[i][0]<< "   "<< (int)img1[i][1]<< endl;
		}

	}



	k=0;
	for(int i=0;i<Size_h;i++)
	{
		for(int j=0;j<Size_w-1;j=j+2)
		{

			Imgdatam[i][j]=img1[k][0];
			//cout<<(int)img1[k][0]<<endl;
			k++;
			
		}
	}
	// Right index
	 l=0;
	for(int i=0;i<Size_h;i++)
	{
		for(int j=1;j<Size_w;j=j+2)
		{

		Imgdatam[i][j]=img1[l][1];
			l++;
		}
	}
	
	// Conversion back to 1 dimensional array.

	Imagedatam=twod21d(Imagedatam,Imgdatam,Size_h,Size_w);
	filewrite(file,argv[2],Imagedatam,Size_h,Size_w,BytesPerPixel1);
	
	//	De-allocation of memory after processing.
	dealloc_mem2(Imgdatac,Size_h,Size_w);
	delete[] Imagedatac;
	dealloc_mem2(Imgdatam,Size_h,Size_w);
	delete[] Imagedatam;

	return 0;
}



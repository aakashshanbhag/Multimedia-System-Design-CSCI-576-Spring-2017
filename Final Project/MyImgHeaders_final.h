#include <stdio.h>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>
#include <string>
#include <math.h>
#include <ctime>

using namespace std;
using namespace cv;

/* different functions used */
unsigned char** AllocHeap2d(unsigned char**, int, int);
unsigned char*** AllocHeap3d(unsigned char***, int, int, int);
void DeallocHeap2d(unsigned char**, int);
void DeallocHeap3d(unsigned char***, int, int);
unsigned char*** Img1dTo3d(unsigned char*, int, int, int);
unsigned char* Img3dTo1d(unsigned char***, int, int, int);
unsigned char** Img1dTo2d(unsigned char*, int, int);
unsigned char* Img2dTo1d(unsigned char**, int, int);
void FileRead(char*, unsigned char*, long int);
int FileSize(char*);

/* Allocate 2D memory in heap*/
unsigned char** AllocHeap2d(unsigned char** Img2d, int ImgHeight, int ImgWidth)
{
	Img2d = new unsigned char *[ImgHeight]();
	for (int i = 0;i < ImgHeight;i++)
	{
		Img2d[i] = new unsigned char[ImgWidth]();
		for (int j = 0;j < ImgWidth;j++)
		{
			Img2d[i][j] = 0;
		}
	}
	return Img2d;
}

/* Allocate double 2d */
double** AllocHeapDouble2d(double** Img2d, int ImgHeight, int ImgWidth)
{
	Img2d = new double *[ImgHeight]();
	for (int i = 0;i < ImgHeight;i++)
	{
		Img2d[i] = new double[ImgWidth]();
		for (int j = 0;j < ImgWidth;j++)
		{
			Img2d[i][j] = 0.0;
		}
	}
	return Img2d;
}

float** AllocHeapFloat2d(float** Img2d, int ImgHeight, int ImgWidth)
{
	Img2d = new float *[ImgHeight]();
	for (int i = 0; i < ImgHeight; i++)
	{
		Img2d[i] = new float[ImgWidth]();
		for (int j = 0; j < ImgWidth; j++)
		{
			Img2d[i][j] = 0.0;
		}
	}
	return Img2d;
}

/* Allocate short int 2d */
short int** AllocHeapSInt2d(short int** Img2d, int ImgHeight, int ImgWidth)
{
	Img2d = new short int *[ImgHeight]();
	for (int i = 0; i < ImgHeight; i++)
	{
		Img2d[i] = new short int[ImgWidth]();
		for (int j = 0; j < ImgWidth; j++)
		{
			Img2d[i][j] = 0;
		}
	}
	return Img2d;
}

/* Deallocate 2D double in heap*/
void DeallocHeapDouble2d(double** Img2d, int ImgHeight)
{
	for (int i = 0; i < ImgHeight; i++)
	{
		delete[] Img2d[i];
	}
	delete[] Img2d;
}

/* Allocate 3d memory in heap*/
unsigned char*** AllocHeap3d(unsigned char*** Img3d, int ImgHeight, int ImgWidth, int BytesPerPixel)
{
	Img3d = new unsigned char **[ImgHeight]();
	for (int i = 0;i < ImgHeight;i++)
	{
		Img3d[i] = new unsigned char *[ImgWidth]();
		for (int j = 0;j < ImgWidth;j++)
		{
			Img3d[i][j] = new unsigned char[BytesPerPixel]();
			for (int k = 0; k < BytesPerPixel;k++)
			{
				Img3d[i][j][k] = 0;
			}
		}
	}
	return Img3d;
}

/* Allocate 3d memory in heap double */
double*** AllocHeapDouble3d(double*** Img3d, int ImgHeight, int ImgWidth, int TSize)
{
	Img3d = new double **[ImgHeight]();
	for (int i = 0;i < ImgHeight;i++)
	{
		Img3d[i] = new double *[ImgWidth]();
		for (int j = 0;j < ImgWidth;j++)
		{
			Img3d[i][j] = new double[TSize]();
			for (int k = 0; k < TSize;k++)
			{
				Img3d[i][j][k] = 0;
			}
		}
	}
	return Img3d;
}

/* Allocate 3d memory in heap float */
float*** AllocHeapFloat3d(float*** Img3d, int ImgHeight, int ImgWidth, int TSize)
{
	Img3d = new float **[ImgHeight]();
	for (int i = 0; i < ImgHeight; i++)
	{
		Img3d[i] = new float *[ImgWidth]();
		for (int j = 0; j < ImgWidth; j++)
		{
			Img3d[i][j] = new float[TSize]();
			for (int k = 0; k < TSize; k++)
			{
				Img3d[i][j][k] = 0;
			}
		}
	}
	return Img3d;
}

/* Convert 1D image data to 2D */
unsigned char** Img1dTo2d(unsigned char* Img1d, int ImgHeight, int ImgWidth)
{
	unsigned char** Img2d = new unsigned char *[ImgHeight]();
	for (int i = 0;i < ImgHeight;i++)
	{
		Img2d[i] = new unsigned char [ImgWidth]();
		for (int j = 0;j < ImgWidth;j++)
		{
			Img2d[i][j] = 0;
		}
	}

	for (int i = 0; i < ImgHeight; i++)
	{
		for (int j = 0; j < ImgWidth; j++)
		{
			Img2d[i][j] = Img1d[i * ImgWidth + j];
		}
	}
	return(Img2d);
}

/* Convert 1D image data to 3D */
unsigned char*** Img1dTo3d(unsigned char* Img1d, int ImgHeight, int ImgWidth, int BytesPerPixel)
{
	unsigned char*** Img3d =  new unsigned char **[ImgHeight]();
	for (int i = 0;i < ImgHeight;i++)
	{
		Img3d[i] = new unsigned char *[ImgWidth]();
		for (int j = 0;j < ImgWidth;j++)
		{
			Img3d[i][j] = new unsigned char[BytesPerPixel]();
			for (int k = 0; k < BytesPerPixel;k++)
			{
				Img3d[i][j][k] = 0;
			}
		}
	}

	for (int i = 0; i < ImgHeight; i++)
	{
		for (int j = 0; j < ImgWidth; j++)
		{
			for (int k = 0; k < BytesPerPixel; k++)
			{
				Img3d[i][j][k] = Img1d[i * ImgWidth * BytesPerPixel + j * BytesPerPixel + k];
			}
		}
	}
	return(Img3d);
}

/* Convert 3D image data to 1D */
unsigned char* Img3dTo1d(unsigned char*** Img3d, int ImgHeight, int ImgWidth, int BytesPerPixel)
{
	unsigned char* Img1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();
	int m = 0;
	for (int i = 0; i < ImgHeight; i++)
	{
		for (int j = 0; j < ImgWidth; j++)
		{
			Img1d[m] = Img3d[i][j][0];
			Img1d[m + 1] = Img3d[i][j][1];
			Img1d[m + 2] = Img3d[i][j][2];
			m = m + 3;
		}
	}
	return(Img1d);
}

/* Convert 2D image data to 1D */
unsigned char* Img2dTo1d(unsigned char** Img2d, int ImgHeight, int ImgWidth)
{
	unsigned char* Img1d = new unsigned char[ImgHeight*ImgWidth]();
	for (int i = 0; i < ImgHeight; i++)
	{
		for (int j = 0; j < ImgWidth; j++)
		{
			Img1d[i * ImgWidth + j] = Img2d[i][j];
		}
	}
	return(Img1d);
}

/* Convert 2D image data to 1D Short Int */
short int* SI2dTo1d(short int** Img2d, int ImgHeight, int ImgWidth)
{
	short int* Img1d = new short int[ImgHeight*ImgWidth]();
	for (int i = 0; i < ImgHeight; i++)
	{
		for (int j = 0; j < ImgWidth; j++)
		{
			Img1d[i * ImgWidth + j] = Img2d[i][j];
		}
	}
	return(Img1d);
}

/* read images from file */
void FileRead(char* FileName, unsigned char* Data, long int DataSize)
{
	FILE* file;
	errno_t err;

	if ((err = fopen_s(&file, FileName, "rb")) != 0)	{		cout << "Cannot open file " << FileName << endl;		exit(1);	}	fread(Data, sizeof(unsigned char), DataSize, file);	fclose(file);
}

/* size of file */
int FileSize(char* FileName)
{
	FILE* file;
	errno_t err;

	if ((err = fopen_s(&file, FileName, "rb")) != 0)	{		cout << "Cannot open file " << FileName << endl;		exit(1);	}	fseek(file, 0, SEEK_END);
	int size = ftell(file);	fclose(file);
	cout << "File " << FileName << " read successfully !!!" << endl;
	return size;
}

void RunVideo_Color(string filename, Mat Vid_RGB)
{
	Mat Vid_BGR;
	cvtColor(Vid_RGB, Vid_BGR, CV_RGB2BGR);
	cout << Vid_BGR.size() << endl;
	imshow(filename, Vid_BGR);
}

void RunVideo_Gray(string filename, Mat Vid_Gray)
{
	imshow(filename, Vid_Gray);
}

//-----calculate costs using MAD------//
float costFuncMAD(Mat currentBlk, Mat refBlk, int mbSize)
{
	Mat m;
	absdiff(currentBlk, refBlk, m);
	Scalar sum1 = cv::sum(m);
	m.release();
	float cost = (1.0 * sum1[0]) / (mbSize * mbSize);
	return cost;
}

//-------extract window----------//
Mat MatExtractWindow(Mat Frame_In, int start_h, int end_h, int start_w, int end_w)
{
	Mat Frame_Out = Mat(end_h - start_h, end_w - start_w, DataType<float>::type);
	Rect rect1(start_w, start_h, end_w - start_w, end_h - start_h);
	Frame_Out = Frame_In(rect1);
	return Frame_Out;
}

//-----------------------------------------------------------------------------
// Input
// imgP : The image for which we want to find motion vectors (Frame_Now)
// imgI : The reference image (Frame_Prev)
// mbSize : Size of the macroblock
// p : Search parameter(read literature to find what this means)
//
// Ouput
// motionVect : the motion vectors for each integral macroblock in imgP
// SESTSScomputations : The average number of points searched for a macroblock
//------------------------------------------------------------------------------

Mat motion_EffTSS(Mat Frame_Now, Mat Frame_Prev, int mbSize, int p, Mat Metric_binary, Mat Dilate_final, unsigned char** Vid_binary_2d, unsigned char* Vid_binary_1d)
{
	Metric_binary.convertTo(Metric_binary, DataType<float>::type);
	Mat MetricX = Mat(33, 60, DataType<float>::type);
	Mat MetricY = Mat(33, 60, DataType<float>::type);
	Mat Metric_dx;
	Mat Metric_dy;

	int height = 528;
	int width = 960;

	// we now take effectively log to the base 2 of p
	// this will give us the number of steps required
	int L = floor(log10(p + 1) / log10(2)); // 3
	//cout << L << endl;
	int stepMax = pow(2, (L-1)); // 4
	//cout << stepMax << endl;
	float costs[6] = { 65535, 65535, 65535, 65535, 65535, 65535 };
	int computations = 0;


	// we start off from the top left of the image
	// we will walk in steps of mbSize
	// for every marcoblock that we look at we will look for
	// a close match p pixels on the left, right, top and bottom of it

	//int mbCount = 1;
	int refBlkVerPointA = 0;
	int refBlkHorPointA = 0;
	int refBlkVerPointB = 0;
	int refBlkHorPointB = 0;
	int refBlkVerPointC = 0;
	int refBlkHorPointC = 0;
	int refBlkVerPointD = 0;
	int refBlkHorPointD = 0;
	int refBlkVerPointE = 0;
	int refBlkHorPointE = 0;
	int refBlkVerPointF = 0;
	int refBlkHorPointF = 0;

	for (int i = 0; i < height; i = i + mbSize)
	{
		for (int j = 0; j < width; j = j + mbSize)
		{
			// the Simple and Efficient three step search starts here
			//
			// each step is divided into two phases
			// in the first phase we evaluate the cost in two ortogonal
			// directions at a distance of stepSize away
			// based on a certain relationship between the three points costs
			// we select the remaining TSS points in the second phase
			// At the end of the second phase, which ever point has the least
			// cost becomes the root for the next step.
			// Please read the paper to find out more detailed information
			
			int stepSize = stepMax;
			int x = j;
			int y = i;
			
			Mat F = MatExtractWindow(Frame_Now, i, i + mbSize, j, j + mbSize);

			while (stepSize >= 1)
			{		
				refBlkVerPointA = y;
				refBlkHorPointA = x;

				refBlkVerPointB = y;
				refBlkHorPointB = x + stepSize;

				refBlkVerPointC = y + stepSize;
				refBlkHorPointC = x;

				// if condition 1
				if (refBlkVerPointA < 0 || refBlkVerPointA + mbSize > height || refBlkHorPointA < 0 || refBlkHorPointA + mbSize > width)
				{
					//cout << "if A" << endl;
					// do nothing
				}	
				else
				{
					costs[0] = costFuncMAD(F,
											MatExtractWindow(Frame_Prev, refBlkVerPointA, refBlkVerPointA + mbSize , refBlkHorPointA, refBlkHorPointA + mbSize), 
											mbSize);
					computations = computations + 1;
				}

				// if condition 2
				if (refBlkVerPointB < 0 || refBlkVerPointB + mbSize > height || refBlkHorPointB < 0 || refBlkHorPointB + mbSize > width)
				{
					//cout << "if B" << endl;
					// do nothing
				}
				else
				{
					costs[1] = costFuncMAD(F,
						MatExtractWindow(Frame_Prev, refBlkVerPointB, refBlkVerPointB + mbSize, refBlkHorPointB, refBlkHorPointB + mbSize),
						mbSize);
					computations = computations + 1;
				}

				// if condition 3
				if (refBlkVerPointC < 0 || refBlkVerPointC + mbSize > height || refBlkHorPointC < 0 || refBlkHorPointC + mbSize > width)
				{
					//cout << "if C" << endl;
					// do nothing
				}
				else
				{
					costs[1] = costFuncMAD(F,
						MatExtractWindow(Frame_Prev, refBlkVerPointC, refBlkVerPointC + mbSize, refBlkHorPointC, refBlkHorPointC + mbSize),
						mbSize);
					computations = computations + 1;
				}
			
				int quadrant = 0;
				if (costs[0] >= costs[1] && costs[0] >= costs[2])
					quadrant = 4;
				else if(costs[0] >= costs[1] && costs[0] < costs[2])
					quadrant = 1;
				else if(costs[0] < costs[1] && costs[0] < costs[2])
					quadrant = 2;
				else if(costs[0] < costs[1] && costs[0] >= costs[2])
					quadrant = 3;
				
				switch (quadrant)
				{
				case 1:
					refBlkVerPointD = y - stepSize;
					refBlkHorPointD = x;

					refBlkVerPointE = y - stepSize;
					refBlkHorPointE = x + stepSize;

					// for D
					if (refBlkVerPointD < 0 || refBlkVerPointD + mbSize  > height
						|| refBlkHorPointD < 0 || refBlkHorPointD + mbSize > width)
					{
						// do nothing
					}
					else
					{
						costs[3] = costFuncMAD(F,
							MatExtractWindow(Frame_Prev, refBlkVerPointD, refBlkVerPointD + mbSize, refBlkHorPointD, refBlkHorPointD + mbSize),
							mbSize);
						computations = computations + 1;
					}
						
					// for ED
					if (refBlkVerPointE < 0 || refBlkVerPointE + mbSize  > height
						|| refBlkHorPointE < 0 || refBlkHorPointE + mbSize - 1 > width)
					{
						// do nothing
					}
					else
					{
						costs[4] = costFuncMAD(F,
							MatExtractWindow(Frame_Prev, refBlkVerPointD, refBlkVerPointD + mbSize, refBlkHorPointD, refBlkHorPointD + mbSize),
							mbSize);
						computations = computations + 1;
					}
					break;
				case 2:
					refBlkVerPointD = y - stepSize;
					refBlkHorPointD = x;

					refBlkVerPointE = y - stepSize;
					refBlkHorPointE = x - stepSize;

					refBlkVerPointF = y;
					refBlkHorPointF = x - stepSize;

					// for D
					if (refBlkVerPointD < 0 || refBlkVerPointD + mbSize  > height
						|| refBlkHorPointD < 0 || refBlkHorPointD + mbSize > width)
					{
						// do nothing
					}
					else
					{
						costs[3] = costFuncMAD(F,
							MatExtractWindow(Frame_Prev, refBlkVerPointD, refBlkVerPointD + mbSize, refBlkHorPointD, refBlkHorPointD + mbSize),
							mbSize);
						computations = computations + 1;
					}

					// for E
					if (refBlkVerPointE < 0 || refBlkVerPointE + mbSize  > height
						|| refBlkHorPointE < 0 || refBlkHorPointE + mbSize > width)
					{
						// do nothing
					}
					else
					{
						costs[4] = costFuncMAD(F,
							MatExtractWindow(Frame_Prev, refBlkVerPointE, refBlkVerPointE + mbSize, refBlkHorPointE, refBlkHorPointE + mbSize),
							mbSize);
						computations = computations + 1;
					}

					// for F
					if (refBlkVerPointF < 0 || refBlkVerPointF + mbSize  > height || refBlkHorPointF < 0 || refBlkHorPointF + mbSize > width)
					{
						// do nothing
					}
					else
					{
						costs[5] = costFuncMAD(F,
							MatExtractWindow(Frame_Prev, refBlkVerPointF, refBlkVerPointF + mbSize, refBlkHorPointF, refBlkHorPointF + mbSize),
							mbSize);
						computations = computations + 1;
					}
					break;
				case 3:
					refBlkVerPointD = y;
					refBlkHorPointD = x - stepSize;

					refBlkVerPointE = y - stepSize;
					refBlkHorPointE = x - stepSize;

					// for D
					if (refBlkVerPointD < 0 || refBlkVerPointD + mbSize  > height
						|| refBlkHorPointD < 0 || refBlkHorPointD + mbSize > width)
					{
						// do nothing
					}
					else
					{
						costs[3] = costFuncMAD(F,
							MatExtractWindow(Frame_Prev, refBlkVerPointD, refBlkVerPointD + mbSize, refBlkHorPointD, refBlkHorPointD + mbSize),
							mbSize);
						computations = computations + 1;
					}

					// for ED
					if (refBlkVerPointE < 0 || refBlkVerPointE + mbSize  > height
						|| refBlkHorPointE < 0 || refBlkHorPointE + mbSize > width)
					{
						// do nothing
					}
					else
					{
						costs[4] = costFuncMAD(F,
							MatExtractWindow(Frame_Prev, refBlkVerPointD, refBlkVerPointD + mbSize, refBlkHorPointD, refBlkHorPointD + mbSize),
							mbSize);
						computations = computations + 1;
					}
					break;
				case 4:
					refBlkVerPointD = y + stepSize;
					refBlkHorPointD = x + stepSize;

					// for D
					if (refBlkVerPointD < 0 || refBlkVerPointD + mbSize  > height
						|| refBlkHorPointD < 0 || refBlkHorPointD + mbSize > width)
					{
						// do nothing
					}
					else
					{
						costs[3] = costFuncMAD(F,
							MatExtractWindow(Frame_Prev, refBlkVerPointD, refBlkVerPointD + mbSize, refBlkHorPointD, refBlkHorPointD + mbSize),
							mbSize);
						computations = computations + 1;
					}
					break;
				default:
					break;
				} // end of switch case
				
				// Now we find the vector where the cost is minimum
				// and store it ... this is what will be passed back.
				// we use the matlab function min() in this case and not the one
				// that is written by author : minCosts()
				int min = 65535;
				int dxy = 0;
				for (int ii = 0; ii < 5; ii++)
				{
					if (costs[ii] < min)
					{
						min = costs[ii];
						dxy = ii;
					}
				}
				
				switch (dxy)
				{
				case 0:
					break;
				case 1:
					x = refBlkHorPointB;
					y = refBlkVerPointB;
					break;
				case 2:
					x = refBlkHorPointC;
					y = refBlkVerPointC;
					break;
				case 3:
					x = refBlkHorPointD;
					y = refBlkVerPointD;
					break;
				case 4:
					x = refBlkHorPointE;
					y = refBlkVerPointE;
					break;
				case 5:
					x = refBlkHorPointF;
					y = refBlkVerPointF;
					break;
				default:
					break;
				}// end of switch case for dxy

				for (int c = 0; c < 6; c++)
				{
					costs[c] = 65535;
				}
				stepSize = stepSize / 2;
			} // end of while for stepSize
			
			MetricX.at<float>(i/16, j/16) = x - j;
			MetricY.at<float>(i/16, j/16) = y - i;
			// mbCount not used
		} // end of for loop for width
	} // end of for loop for height

	//cout << computations / (1980) << endl; 
	  //------apply Sobel for calculating gradients--------//
	Sobel(MetricX, Metric_dx, -1, 2, 2, 3, 1, 0, BORDER_REPLICATE);
	Sobel(MetricY, Metric_dy, -1, 2, 2, 3, 1, 0, BORDER_REPLICATE);

	//------find max-----//
	int max = 0;
	for (int i = 0; i < height/16; i++)
	{
		for (int j = 0; j < width/16; j++)
		{
			if (((MetricX.at<float>(i, j) * Metric_dx.at<float>(i, j)) + (MetricY.at<float>(i, j) * Metric_dy.at<float>(i, j))) > max)
				max = ((MetricX.at<float>(i, j) * Metric_dx.at<float>(i, j)) + (MetricY.at<float>(i, j) * Metric_dy.at<float>(i, j)));
		}
	}

	//cout << max << endl;
	//----loop for Metric-----//
	for (int i = 0; i < height / 16; i++)
	{
		for (int j = 0; j < width / 16; j++)
		{
			Rect rect3(j * 16, i * 16, 16, 16);
			//if (((MetricX.at<float>(i, j) * Metric_dx.at<float>(i, j)) + (MetricY.at<float>(i, j) * Metric_dy.at<float>(i, j))) == 0)
			if (((MetricX.at<float>(i, j) * Metric_dx.at<float>(i, j)) + (MetricY.at<float>(i, j) * Metric_dy.at<float>(i, j))) > (max / 2.0))
				Metric_binary(rect3).setTo(255);
			else
				Metric_binary(rect3).setTo(0);
		}
	}
	Metric_binary.convertTo(Metric_binary, CV_8UC1);

	MetricX.release();
	MetricY.release();
	Metric_dx.release();
	Metric_dy.release();
	/*------------dilate---------------*/
	int dilation_size = 1;

	Mat Dilate_Out1;
	Mat Dilate_Out2;
	Mat Dilate_Out3;
	Mat Dilate_Out4;
	Mat Dilate_Out5;
	Mat Dilate_Out6;
	Mat Dilate_Out7;
	Mat Dilate_Out8;
	Mat Dilate_Out9;
	Mat Dilate_Out10;
	Mat Dilate_Out11;
	Mat Dilate_Out12;
	Mat Dilate_Out13;
	Mat Dilate_Out14;
	Mat Dilate_Out15;
	Mat Dilate_Out16;
	Mat Dilate_Out17;
	Mat Dilate_Out18;
	Mat Dilate_Out19;
	Mat Dilate_Out20;
	Mat Dilate_Out21;
	Mat Dilate_Out22;
	Mat Dilate_Out23;
	Mat Dilate_Out24;
	Mat Dilate_Out25;

	Mat element = getStructuringElement(MORPH_RECT,
		Size(2 * dilation_size + 1, 2 * dilation_size + 1),
		Point(dilation_size, dilation_size));

	dilate(Metric_binary, Dilate_Out1, element);
	dilate(Dilate_Out1, Dilate_Out2, element);
	Dilate_Out1.release();
	dilate(Dilate_Out2, Dilate_Out3, element);
	Dilate_Out2.release();
	dilate(Dilate_Out3, Dilate_Out4, element);
	Dilate_Out3.release();
	dilate(Dilate_Out4, Dilate_Out5, element);
	Dilate_Out4.release();
	dilate(Dilate_Out5, Dilate_Out6, element);
	Dilate_Out5.release();
	dilate(Dilate_Out6, Dilate_Out7, element);
	Dilate_Out6.release();
	dilate(Dilate_Out7, Dilate_Out8, element);
	Dilate_Out7.release();
	dilate(Dilate_Out8, Dilate_Out9, element);
	Dilate_Out8.release();
	dilate(Dilate_Out9, Dilate_Out10, element);
	Dilate_Out9.release();
	dilate(Dilate_Out10, Dilate_Out11, element);
	Dilate_Out10.release();
	dilate(Dilate_Out11, Dilate_Out12, element);
	Dilate_Out11.release();
	dilate(Dilate_Out12, Dilate_Out13, element);
	Dilate_Out12.release();
	dilate(Dilate_Out13, Dilate_Out14, element);
	Dilate_Out13.release();
	dilate(Dilate_Out14, Dilate_Out15, element);
	Dilate_Out14.release();
	dilate(Dilate_Out15, Dilate_Out16, element);
	Dilate_Out15.release();
	dilate(Dilate_Out16, Dilate_Out17, element);
	Dilate_Out16.release();
	dilate(Dilate_Out17, Dilate_Out18, element);
	Dilate_Out17.release();
	dilate(Dilate_Out18, Dilate_Out19, element);
	Dilate_Out18.release();
	dilate(Dilate_Out19, Dilate_Out20, element);
	Dilate_Out19.release();
	dilate(Dilate_Out20, Dilate_Out21, element);
	Dilate_Out20.release();
	dilate(Dilate_Out21, Dilate_Out22, element);
	Dilate_Out21.release();
	dilate(Dilate_Out22, Dilate_Out23, element);
	Dilate_Out22.release();
	dilate(Dilate_Out23, Dilate_Out24, element);
	Dilate_Out23.release();
	dilate(Dilate_Out24, Dilate_Out25, element);
	Dilate_Out24.release();
	dilate(Dilate_Out25, Dilate_final, element);
	Dilate_Out25.release();
	element.release();

	/*---------bounding box-----------*/
	//later

	/*---------Run Video to see segmented output-----------*/
	//RunVideo_Gray("x", Dilate_final);

	return Dilate_final;
}

Mat MotionVector(Mat Frame_Now, Mat Frame_Prev, Mat F_Out, Mat Search_Prev, Mat Search_Now, Mat Metric_binary, Mat Dilate_final, unsigned char** Vid_binary_2d, unsigned char* Vid_binary_1d, int ImgHeight, int ImgWidth, int blockSize, int searchSize, int frame_count)
{
	Metric_binary.convertTo(Metric_binary, DataType<float>::type);
	int block_h = ImgHeight / 16; // 33
	int block_w = ImgWidth / 16; // 60
	Mat MetricX = Mat(block_h, block_w, DataType<float>::type);
	Mat MetricY = Mat(block_h, block_w, DataType<float>::type);
	Mat Metric_dx;
	Mat Metric_dy;

	//------this complete loop needs to be replaced by the function above-----//
	for (int i = 0; i < block_h; i++)
	{
		for (int j = 0; j < block_w; j++)
		{
			/*----------16x16 block in current frame---------------*/
			Rect rect1(j * 16, i * 16, 16, 16);
			Search_Now = Frame_Now(rect1);

			/*----------64x64 search window in previous frame---------------*/
			int min = 65535;
			int min_loc_x = 0;
			int min_loc_y = 0;
			float X = 0.0, Y = 0.0;

			int track_loc = 0; // for keeping track of the minimum 16x16 block
			int ii = 0;
			int jj = 0;
			for (int i1 = 0; i1 < 4; i1++)
			{
				for (int j1 = 0; j1 < 4; j1++)
				{
					/*---------16x16 block inside the 64x64 search window---------*/					
					if (i <= block_h - 4 && j <= block_w - 4)
					{
						Rect rect2(j * 16 + j1 * 16, i * 16 + i1 * 16, 16, 16);
						Search_Prev = Frame_Prev(rect2);
					}			
					else
					{
						ii = i;
						jj = j;
						if (i > block_h - 4)
							ii = block_h - 4;
						if (j > block_w - 4)
							jj = block_w - 4;
						Rect rect2(jj * 16 + j1 * 16, ii * 16 + i1 * 16, 16, 16);
						Search_Prev = Frame_Prev(rect2);
					}	

					/*-----------find SAD---------------*/
					int Sum_SAD = 0;

					Mat m;
					absdiff(Search_Prev, Search_Now, m);
					Scalar sum1 = cv::sum(m);
					m.release();
					Sum_SAD = sum1[0];

					if (i <= block_h - 4 && j <= block_w - 4)
					{
						/*------------find mimimum--------------*/
						if (Sum_SAD < min)
						{
							min = Sum_SAD;
							min_loc_y = i * 16 + (track_loc / 4) * 16;
							min_loc_x = j * 16 + (track_loc % 4) * 16;
						}

						X = min_loc_x - j * 16;
						Y = min_loc_y - i * 16;
					}
					else
					{
						ii = i;
						jj = j;
						if (i > block_h - 4)
							ii = block_h - 4;
						if (j > block_w - 4)
							jj = block_w - 4;

						/*------------find mimimum--------------*/
						if (Sum_SAD < min)
						{
							min = Sum_SAD;
							min_loc_y = ii * 16 + (track_loc / 4) * 16;
							min_loc_x = jj * 16 + (track_loc % 4) * 16;
						}

						/*--------gradient-------*/
						X = min_loc_x - jj * 16;
						Y = min_loc_y - ii * 16;
					}
						
					/*--------gradient-------*/
					MetricX.at<float>(i, j) = X;
					MetricY.at<float>(i, j) = Y;

					track_loc++;
				}
			}
		} // end of width loop image
	} //  end of height loop image

	//------apply Sobel for calculating gradients--------//
	Sobel(MetricX, Metric_dx, -1, 2, 2, 3, 1, 0, BORDER_REPLICATE);
	Sobel(MetricY, Metric_dy, -1, 2, 2, 3, 1, 0, BORDER_REPLICATE);

	//------find max-----//
	/*int max = 0;
	for (int i = 0; i < block_h; i++)
	{
		for (int j = 0; j < block_w; j++)
		{
			if (((MetricX.at<float>(i, j) * Metric_dx.at<float>(i, j)) + (MetricY.at<float>(i, j) * Metric_dy.at<float>(i, j))) > max)
				max = ((MetricX.at<float>(i, j) * Metric_dx.at<float>(i, j)) + (MetricY.at<float>(i, j) * Metric_dy.at<float>(i, j)));
		}
	}*/

	//----loop for Metric-----//
	for (int i = 0; i < block_h; i++)
	{
		for (int j = 0; j < block_w; j++)
		{
			Rect rect3(j * 16, i * 16, 16, 16);
			if (((MetricX.at<float>(i, j) * Metric_dx.at<float>(i, j)) + (MetricY.at<float>(i, j) * Metric_dy.at<float>(i, j))) == 0)
			//if (((MetricX.at<float>(i, j) * Metric_dx.at<float>(i, j)) + (MetricY.at<float>(i, j) * Metric_dy.at<float>(i, j))) >(max / 2.0))
				Metric_binary(rect3).setTo(0);
			else
				Metric_binary(rect3).setTo(255);
		}
	}
	Metric_binary.convertTo(Metric_binary, CV_8UC1);

	MetricX.release();
	MetricY.release();
	Metric_dx.release();
	Metric_dy.release();
	
	/*------------dilate---------------*/
	int dilation_size = 1;

	Mat Dilate_Out1;
	Mat Dilate_Out2;
	Mat Dilate_Out3;
	Mat Dilate_Out4;
	Mat Dilate_Out5;
	Mat Dilate_Out6;
	Mat Dilate_Out7;
	Mat Dilate_Out8;
	Mat Dilate_Out9;
	Mat Dilate_Out10;
	Mat Dilate_Out11;
	Mat Dilate_Out12;
	Mat Dilate_Out13;
	Mat Dilate_Out14;
	Mat Dilate_Out15;
	Mat Dilate_Out16;
	Mat Dilate_Out17;
	Mat Dilate_Out18;
	Mat Dilate_Out19;
	Mat Dilate_Out20;
	Mat Dilate_Out21;
	Mat Dilate_Out22;
	Mat Dilate_Out23;
	Mat Dilate_Out24;
	Mat Dilate_Out25;

	Mat element = getStructuringElement(MORPH_RECT,
		Size(2 * dilation_size + 1, 2 * dilation_size + 1),
		Point(dilation_size, dilation_size));

	dilate(Metric_binary, Dilate_Out1, element);
	dilate(Dilate_Out1, Dilate_Out2, element);
	Dilate_Out1.release();
	dilate(Dilate_Out2, Dilate_Out3, element);
	Dilate_Out2.release();
	dilate(Dilate_Out3, Dilate_Out4, element);
	Dilate_Out3.release();
	dilate(Dilate_Out4, Dilate_Out5, element);
	Dilate_Out4.release();
	dilate(Dilate_Out5, Dilate_Out6, element);
	Dilate_Out5.release();
	dilate(Dilate_Out6, Dilate_Out7, element);
	Dilate_Out6.release();
	dilate(Dilate_Out7, Dilate_Out8, element);
	Dilate_Out7.release();
	dilate(Dilate_Out8, Dilate_Out9, element);
	Dilate_Out8.release();
	dilate(Dilate_Out9, Dilate_Out10, element);
	Dilate_Out9.release();
	dilate(Dilate_Out10, Dilate_Out11, element);
	Dilate_Out10.release();
	dilate(Dilate_Out11, Dilate_Out12, element);
	Dilate_Out11.release();
	dilate(Dilate_Out12, Dilate_Out13, element);
	Dilate_Out12.release();
	dilate(Dilate_Out13, Dilate_Out14, element);
	Dilate_Out13.release();
	dilate(Dilate_Out14, Dilate_Out15, element);
	Dilate_Out14.release();
	dilate(Dilate_Out15, Dilate_Out16, element);
	Dilate_Out15.release();
	dilate(Dilate_Out16, Dilate_Out17, element);
	Dilate_Out16.release();
	dilate(Dilate_Out17, Dilate_Out18, element);
	Dilate_Out17.release();
	dilate(Dilate_Out18, Dilate_Out19, element);
	Dilate_Out18.release();
	dilate(Dilate_Out19, Dilate_Out20, element);
	Dilate_Out19.release();
	dilate(Dilate_Out20, Dilate_Out21, element);
	Dilate_Out20.release();
	dilate(Dilate_Out21, Dilate_Out22, element);
	Dilate_Out21.release();
	dilate(Dilate_Out22, Dilate_Out23, element);
	Dilate_Out22.release();
	dilate(Dilate_Out23, Dilate_Out24, element);
	Dilate_Out23.release();
	dilate(Dilate_Out24, Dilate_Out25, element);
	Dilate_Out24.release();
	dilate(Dilate_Out25, Dilate_final, element);
	Dilate_Out25.release();
	element.release();

	/*---------Run Video to see segmented output-----------*/
	//RunVideo_Gray("x", Dilate_final);

	return Dilate_final;
}

Mat Stage1(float* Vid_R, float* Vid_G, float* Vid_B, float** Vid_RD, float** Vid_GD, float** Vid_BD, Mat Q_tab, float** Vid_RD_final, float** Vid_GD_final, float** Vid_BD_final, unsigned char*** Vid_final_3d, unsigned char* Vid_final_1d, int height, int width, int blockSize)
{
	int w_new = width / blockSize;
	int h_new = height / blockSize;

	for (int i = 0; i < h_new; i++)
	{
		for (int j = 0; j < w_new; j++)
		{
			for (int m = 0; m < blockSize; m++)
			{
				for (int n = 0; n < blockSize; n++)
				{
					Vid_R[m * blockSize + n] = Vid_RD[i * blockSize + m][j * blockSize + n];
					Vid_G[m * blockSize + n] = Vid_GD[i * blockSize + m][j * blockSize + n];
					Vid_B[m * blockSize + n] = Vid_BD[i * blockSize + m][j * blockSize + n];
				}
			}

			//====== DCT blockSizexblockSize ==========//
			Mat R = Mat(blockSize, blockSize, DataType<float>::type, Vid_R);
			Mat G = Mat(blockSize, blockSize, DataType<float>::type, Vid_G);
			Mat B = Mat(blockSize, blockSize, DataType<float>::type, Vid_B);

			R.convertTo(R, DataType<float>::type);
			G.convertTo(G, DataType<float>::type);
			B.convertTo(B, DataType<float>::type);
			Mat Vid_DCTR = Mat(blockSize, blockSize, DataType<float>::type);
			Mat Vid_DCTG = Mat(blockSize, blockSize, DataType<float>::type);
			Mat Vid_DCTB = Mat(blockSize, blockSize, DataType<float>::type);
			dct(R, Vid_DCTR);
			dct(G, Vid_DCTG);
			dct(B, Vid_DCTB);
			//cout << Vid_DCTR << endl << endl;

			//========= Quantization =========//
			double N1 = 2.0;
			Mat DCTQuantised_R;
			Mat DCTQuantised_G;
			Mat DCTQuantised_B;
			divide(Vid_DCTR, Q_tab * N1, DCTQuantised_R);
			divide(Vid_DCTG, Q_tab * N1, DCTQuantised_G);
			divide(Vid_DCTB, Q_tab * N1, DCTQuantised_B);
			DCTQuantised_R.convertTo(DCTQuantised_R, DataType<int>::type);
			DCTQuantised_G.convertTo(DCTQuantised_G, DataType<int>::type);
			DCTQuantised_B.convertTo(DCTQuantised_B, DataType<int>::type);

			//======== DeQuantization ===========//
			DCTQuantised_R.convertTo(DCTQuantised_R, DataType<float>::type);
			DCTQuantised_G.convertTo(DCTQuantised_G, DataType<float>::type);
			DCTQuantised_B.convertTo(DCTQuantised_B, DataType<float>::type);
			Mat DCTDeQuantised_R;
			Mat DCTDeQuantised_G;
			Mat DCTDeQuantised_B;
			multiply(N1 * DCTQuantised_R, Q_tab, DCTDeQuantised_R);
			multiply(N1 * DCTQuantised_G, Q_tab, DCTDeQuantised_G);
			multiply(N1 * DCTQuantised_B, Q_tab, DCTDeQuantised_B);

			//========== IDCT ================//
			Mat Vid_IDCTR;
			Mat Vid_IDCTG;
			Mat Vid_IDCTB;
			idct(DCTDeQuantised_R, Vid_IDCTR);
			idct(DCTDeQuantised_G, Vid_IDCTG);
			idct(DCTDeQuantised_B, Vid_IDCTB);
			//Vid_IDCTR.convertTo(Vid_IDCTR, DataType<int>::type);
			//Vid_IDCTG.convertTo(Vid_IDCTG, DataType<int>::type);
			//Vid_IDCTB.convertTo(Vid_IDCTB, DataType<int>::type);
			//cout << Vid_IDCTR << endl << endl;
			//Vid_IDCTR.convertTo(Vid_IDCTR, DataType<float>::type);

			//========== Convert 2D-Mat image to 2D-double image for easy manipulation ================//
			for (int s = 0; s < blockSize; s++)
			{
				for (int t = 0; t < blockSize; t++)
				{
					Vid_RD_final[s][t] = Vid_IDCTR.at<float>(s, t);
					Vid_GD_final[s][t] = Vid_IDCTG.at<float>(s, t);
					Vid_BD_final[s][t] = Vid_IDCTB.at<float>(s, t);
				}
			}

			//========== Convert 2D image to 3D for easier manipulation ================//
			for (int m = 0; m < blockSize; m++)
			{
				for (int n = 0; n < blockSize; n++)
				{
					if (abs(Vid_RD_final[m][n] - int(Vid_RD_final[m][n])) > 0.5)
						Vid_final_3d[i * blockSize + m][j * blockSize + n][0] = ceil(Vid_RD_final[m][n]);
					else
						Vid_final_3d[i * blockSize + m][j * blockSize + n][0] = floor(Vid_RD_final[m][n]);

					if (abs(Vid_GD_final[m][n] - int(Vid_GD_final[m][n])) > 0.5)
						Vid_final_3d[i * blockSize + m][j * blockSize + n][1] = ceil(Vid_GD_final[m][n]);
					else
						Vid_final_3d[i * blockSize + m][j * blockSize + n][1] = floor(Vid_GD_final[m][n]);

					if (abs(Vid_BD_final[m][n] - int(Vid_BD_final[m][n])) > 0.5)
						Vid_final_3d[i * blockSize + m][j * blockSize + n][2] = ceil(Vid_BD_final[m][n]);
					else
						Vid_final_3d[i * blockSize + m][j * blockSize + n][2] = floor(Vid_BD_final[m][n]);
				}
			}
		}
	}
	//========== Convert to 1D fo rconversion to Mat ================//
	Vid_final_1d = Img3dTo1d(Vid_final_3d, h_new * blockSize, w_new * blockSize, 3);
	Mat Vid_Out = Mat(h_new * blockSize, w_new * blockSize, CV_8UC3);
	Vid_Out.data = Vid_final_1d;
	
	return Vid_Out;
}

Mat DCT_channel(float* Vid_R, float** Vid_RD, Mat Vid_DCTR, int height, int width, int blockSize = 8)
{
	//Mat Vid_DCTR1 = Mat(8, 8, DataType<float>::type);;
	int w_new = width / blockSize;
	int h_new = height / blockSize;

	for (int i = 0; i < h_new; i++)
	{
		for (int j = 0; j < w_new; j++)
		{
			for (int m = 0; m < blockSize; m++)
			{
				for (int n = 0; n < blockSize; n++)
				{
					Vid_R[m * blockSize + n] = Vid_RD[i * blockSize + m][j * blockSize + n];
				}
			}

			//====== DCT blockSizexblockSize ==========//
			Mat R = Mat(blockSize, blockSize, DataType<float>::type, Vid_R);
			R.convertTo(R, DataType<float>::type);
			Mat Vid_DCT = Mat(blockSize, blockSize, DataType<float>::type);
			dct(R, Vid_DCT);
			//cout << Vid_DCT << endl << endl;
			R.release();

			//========== Convert 2D image to 3D for easier manipulation ================//
			Rect rect1(j * blockSize, i * blockSize, blockSize, blockSize);
			Vid_DCT.copyTo(Vid_DCTR(rect1));
			Vid_DCT.release();
		}
	}
	return Vid_DCTR;
}
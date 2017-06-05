// Encoder code for Team Rookies

#include <stdio.h>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "MyImgHeaders_final.h"
#include <vector>
#include <ctime>
#include <thread>
#include <future>
#include <fstream>
#include <algorithm>

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;


/** @function main */
int main(int argc, char** argv)
{
	int width = 960;
	int height = 540;
	int fr = 60;
	int delay = int((1.0 / fr) * 1000);
	int temp = 1;
	string windowName = argv[1];
	clock_t start, start1;
	double duration;
	int blockSize = 8;
	int w_new = width / blockSize;
	int h_new = height / blockSize;
	int fileSize = FileSize(argv[1]);
	const int frameCount = fileSize / (height * width * 3);
	cout << "\nTotal number of frames in the video is: " << frameCount << endl;
	//cout << "CMP_before" << endl;

	//----------Define variable for cmp file creation-----------//
	const int h_cmp = ((height / 16) * 16) / blockSize; // for 540 not being a multiple of 8 or 16
	//cout << h_cmp << endl;
	const int w_cmp = width / blockSize;
	//cout << w_cmp << endl;
	//short int** CMP = NULL;
	//CMP = AllocHeapSInt2d(CMP,h_cmp * w_cmp, 193);
	//short int* CMP_1d = new short int[h_cmp * w_cmp * 193]();
	//float CMP[7920][193];
	static short int CMP[7920][193];

	//cout << "CMP" << endl;

	//========== Read the input rgb video ==========//	
	unsigned char* InpVid = new unsigned char[height * width * 3 * frameCount]();
	unsigned char* Vid1d = new unsigned char[height * width * 3]();
	unsigned char* VidR_1d = new unsigned char[height * width]();
	float* VidR_Double = new float[height * width]();
	unsigned char* VidG_1d = new unsigned char[height * width]();
	unsigned char* VidB_1d = new unsigned char[height * width]();
	unsigned char*** Vid3d = NULL;
	Vid3d = AllocHeap3d(Vid3d, height, width, 3);
	
	float** Vid_RD = NULL;
	Vid_RD = AllocHeapFloat2d(Vid_RD, height, width);
	float** Vid_GD = NULL;
	Vid_GD = AllocHeapFloat2d(Vid_GD, height, width);
	float** Vid_BD = NULL;
	Vid_BD = AllocHeapFloat2d(Vid_BD, height, width);

	float** Vid_RD_final = NULL;
	Vid_RD_final = AllocHeapFloat2d(Vid_RD_final, height, width);
	float** Vid_GD_final = NULL;
	Vid_GD_final = AllocHeapFloat2d(Vid_GD_final, height, width);
	float** Vid_BD_final = NULL;
	Vid_BD_final = AllocHeapFloat2d(Vid_BD_final, height, width);
	
	float* Vid_R = new float[blockSize * blockSize]();
	float* Vid_G = new float[blockSize * blockSize]();
	float* Vid_B = new float[blockSize * blockSize]();

	unsigned char* Vid_motionE_1d = new unsigned char[height * width * 3]();

	unsigned char* Vid_final_1d = new unsigned char[w_new * blockSize * h_new * blockSize]();
	unsigned char*** Vid_final_3d = NULL;
	Vid_final_3d = AllocHeap3d(Vid_final_3d, h_new * blockSize, w_new * blockSize, 3);

	//cout << "cleared" << endl;

	Mat Q_tab = (Mat_<float> (blockSize, blockSize) << 16, 11, 10, 16, 24, 42, 51, 61, 12, 12, 14, 19, 26, 58, 60, 55, 14, 13, 16, 24, 40, 57, 69, 56, 14, 17, 22, 29, 51, 87, 80, 62, 18, 22, 37, 58, 68, 109, 123, 77, 24, 35, 55, 64, 81, 104, 113, 92, 49, 64, 78, 87, 103, 121, 120, 101, 72, 92, 95, 98, 122, 100, 103, 99);
	//Q_tab.convertTo(Q_tab, CV_32FC1);
	//cout << Q_tab << endl;

	FileRead(argv[1], InpVid, height * width * 3 * frameCount);

	int ind = 0;
	int videoLoopCount = 0;
	Mat Frame_Prev, Frame_Now;
	Mat Vid_DCTR = Mat(536, 960, DataType<float>::type);
	Mat Vid_DCTG = Mat(536, 960, DataType<float>::type);
	Mat Vid_DCTB = Mat(536, 960, DataType<float>::type);

	Mat MotionE_Inp = Mat(height, width, CV_8UC3);
	Mat Vid_Gray;
	Mat Frame_MV_out;
	//int searchSize = 64;
	//int blockSize = 16;
	Mat F_Out = Mat(height, width, DataType<float>::type);
	Mat Search_Prev = Mat(64, 64, DataType<float>::type);
	Mat Search_Now = Mat(16, 16, DataType<float>::type);

	Mat Metric_binary = Mat((height/16) * 16, (width/16) * 16, CV_8UC1);
	Mat Dilate_final;

	/*-----------for binary image--------------*/
	//Mat Metric_binary = Mat(block_h * 16, block_w * 16, DataType<int>::type);
	unsigned char** Vid_binary_2d = NULL;
	Vid_binary_2d = AllocHeap2d(Vid_binary_2d, (height / 16) * 16, (width / 16) * 16);
	unsigned char* Vid_binary_1d = new unsigned char[(height / 16) * 16 * (width / 16) * 16]();

	ind = 0;
	start1 = clock();

	/*-----for every frame-------*/
	for (int i1 = 0; i1 < frameCount; i1++)
	{	
		/*----------for CMP file write----------*/
		ostringstream ss2;
		ss2 << "CMP_" << i1 + 1 << ".txt";
		ofstream out2(ss2.str().c_str(), ios::out | ios::binary);

		start = clock();			
		ind = height * width * 3 * i1;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				Vid3d[i][j][0] = InpVid[ind];
				Vid3d[i][j][1] = InpVid[ind + height * width];
				Vid3d[i][j][2] = InpVid[ind + height * width * 2];
;
				Vid_RD[i][j] = float(Vid3d[i][j][0]);
				Vid_GD[i][j] = float(Vid3d[i][j][1]);
				Vid_BD[i][j] = float(Vid3d[i][j][2]);

				ind++;
			}
		}

		//============ perform motion estimation ===============//
		Vid_motionE_1d = Img3dTo1d(Vid3d, height, width, 3);
		//MotionE_Inp = Mat(height, width, CV_8UC3);
		MotionE_Inp.data = Vid_motionE_1d;

		//Mat Vid_Gray;
		cvtColor(MotionE_Inp, Vid_Gray, CV_RGB2GRAY);
		Vid_Gray.convertTo(Vid_Gray, DataType<float>::type);

		if (i1 == 0)
			Frame_Prev = Vid_Gray;
		else
		{
			
			/*-----------frame binarized--------------*/
			Frame_Now = Vid_Gray;
			//Frame_MV_out = MotionVector(Frame_Now, Frame_Prev, F_Out, Search_Prev, Search_Now, Metric_binary, Dilate_final, Vid_binary_2d, Vid_binary_1d, height, width, 16, 64, i1 + 1);
			Frame_MV_out = motion_EffTSS(Frame_Now, Frame_Prev, 16, 3, Metric_binary, Dilate_final, Vid_binary_2d, Vid_binary_1d);
			Frame_MV_out.convertTo(Frame_MV_out, DataType<float>::type);
			Frame_Prev = Frame_Now;

			//clock_t starts = clock();
			/*----------DCT on each channel----------*/
			Vid_DCTR = DCT_channel(Vid_R, Vid_RD, Vid_DCTR, height, width, blockSize);
			Vid_DCTG = DCT_channel(Vid_G, Vid_GD, Vid_DCTG, height, width, blockSize);
			Vid_DCTB = DCT_channel(Vid_B, Vid_BD, Vid_DCTB, height, width, blockSize);
			//cout << (clock() - starts) / ((double)CLOCKS_PER_SEC) << " seconds !!!\n";
			/*------assigning values for CMP file creation----------*/
			//clock_t s = clock();
			for (int ii = 0; ii < 66; ii++) // 66
			{
				for (int jj = 0; jj < 120; jj++)  // 120
				{	
					// for 0th index (binary)
					CMP[ii * (960 / 8) + jj][0] = Frame_MV_out.at<float>(ii * 8, jj * 8);

					// for R, G, B DCT pixel values
					for (int m = 0; m < blockSize; m++)
					{
						for (int n = 0; n < blockSize; n++)
						{
							CMP[ii * (960 / 8) + jj][m * blockSize + n + 1] = Vid_DCTR.at<float>(ii * blockSize + m, jj * blockSize + n);
							CMP[ii * (960 / 8) + jj][m * blockSize + n + 65] = Vid_DCTG.at<float>(ii * blockSize + m, jj * blockSize + n);
							CMP[ii * (960 / 8) + jj][m * blockSize + n + 129] = Vid_DCTB.at<float>(ii * blockSize + m, jj * blockSize + n);
						}
					}
				} // end of 120
			} // end of 66
			
			out2.write((char *) &CMP, sizeof CMP);
			out2.close();
		}

		//---------File write status-----------//
		cout << "\nFile CMP_" << i1+1 << ".txt written successfully !!!" << endl;
												
		//============ do DCT-IDCT by creating macroblock blockSizexblockSize ===============//	
		//Mat Vid_Out = Stage1(Vid_R, Vid_G, Vid_B, Vid_RD, Vid_GD, Vid_BD, Q_tab, Vid_RD_final, Vid_GD_final, Vid_BD_final, Vid_final_3d, Vid_final_1d, height, width, blockSize);
			
		cout << "Time-taken for processing Frame-" << i1+1 << " is: " << (clock() - start) / ((double)CLOCKS_PER_SEC) << " seconds !!!\n";

		//========== Display the Color Video ================//			
		//RunVideo_Color(windowName, Vid_Out);

		//========== Display the Gray Video ================//	
		/*Mat Vid_Out1;
		cvtColor(Vid_Out, Vid_Out1, CV_RGB2GRAY);
		RunVideo_Gray(windowName, Vid_Out1);*/

		if (waitKey(delay) == 27) // press Esc to exit and close window
		{
			temp = 0;
			cout << endl;
			break;
		}	
	}
	if (temp == 0)
	{
		cout << endl;
		//break;
	}
	//========== Time taken to process the whole video once ================//
	cout << "\nTime-taken processing the whole video " << videoLoopCount + 1 << " time(s) is: " << (clock() - start1) / (double)CLOCKS_PER_SEC << " seconds !!!\n\n";
	videoLoopCount++;
	
	return 0;
}


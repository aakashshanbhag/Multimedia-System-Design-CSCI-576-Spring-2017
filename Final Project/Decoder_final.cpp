//Decoder code by Team Rookies

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "MyImgHeaders_final.h"

using namespace std;
using namespace cv;

//Global Variable
bool playVideo = true; // play, pause, close and restart
float N1 = 50.0, N2 = 1.0;
int height_orig = 540; // original video height
int height = 528; // video height
int width = 960; // video width
const int blockSize = 8; // the block size for performing compression/decompression
static short int CMP[7920][193]; // for reading data from file
clock_t start, start1; // for capturing time
int videoLoopCount = 0; // count video loop

int fr = 60; // frame rate

//----convert frame rate to delay------//
int delay = int((1.0 / fr) * 1000); //
//int delay = 2;
//----access intermediate channel pixels-------//
float** Output_R2d =  AllocHeapFloat2d(Output_R2d, height, width);
float ** Output_G2d = AllocHeapFloat2d(Output_G2d, height, width);
float ** Output_B2d = AllocHeapFloat2d(Output_B2d, height, width);

//----for storing the ouput frame for displaying video----//
unsigned char* Output = new unsigned char[960 * 528 * 3];

//------Quantization matrix-----//
Mat Q_tab = (Mat_ < float >(8, 8) << 16, 11, 10, 16, 24, 42, 51, 61, 12, 12, 14, 19, 26, 58, 60, 55, 14, 13, 16, 24, 40, 57, 69, 56, 14, 17, 22, 29, 51, 87, 80, 62, 18, 22, 37, 58, 68, 109, 123, 77, 24, 35, 55, 64, 81, 104, 113, 92, 49, 64, 78, 87, 103, 121, 120, 101, 72, 92, 95, 98, 122, 100, 103, 99);

//------Frame Creation matrices-----//
float R_Frame[64];
float G_Frame[64];
float B_Frame[64];

//-----final IDCT values-----//
Mat Vid_IDCTR;
Mat Vid_IDCTG;
Mat Vid_IDCTB;

//-------Capturing Mouse Pointer first-----//
Point points;

//--------------mouse callback--------------//
void CallBackFunc(int event, int x, int y, int flags, void * userdata) {

	if (event == EVENT_LBUTTONDOWN) {
		Point * p = (Point *)userdata;
		p->x = x;
		p->y = y;
		//cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if (event == EVENT_RBUTTONDOWN) {
		Point * p = (Point *)userdata;
		p->x = x;
		p->y = y;
		//cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if (event == EVENT_MBUTTONDOWN) {
		Point * p = (Point *)userdata;
		p->x = x;
		p->y = y;
		//cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if (event == EVENT_MOUSEMOVE) {
		Point * p = (Point *)userdata;
		p->x = x;
		p->y = y;
		//cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
	}

}

//Display And Gaze Control
void showGaze(int f) 
{
	//-----continuously keep track of the mouse pointer-----//
	setMouseCallback("try", CallBackFunc, (void *)(&points));

	//-----for reading data from file-----//
	ostringstream ss2;
	ss2 << "CMP_" << f << ".txt";
	ifstream in(ss2.str().c_str(), ios::in | ios::binary); 
	in.read((char *)& CMP, sizeof CMP);

	//-----convert (x, y) Coordinates of the mouse pointer to representative blocks of 8x8-----//
	int X_Coordinate = points.x / blockSize;
	int Y_Coordinate = points.y / blockSize;

	int block_number[blockSize][blockSize]; //Identifying the box where mouse pointer is present
	bool flag = true; // flag to control the gaze

	//----capture block numbers where gaze needs to be present----//
	for (int i = 0; i < blockSize; i++) {
		for (int j = 0; j < blockSize; j++) {
			block_number[i][j] = Y_Coordinate * (width / blockSize) + X_Coordinate + j + (width / blockSize) * i;
		}
	}

	//----take data from compressed file----//
	for (int i = 0; i < ((height / blockSize) * (width / blockSize)); i++) 
	{
		//----for dequatisation-----//
		Mat DCTDeQuantised_R;
		Mat DCTDeQuantised_G;
		Mat DCTDeQuantised_B;
		
		flag = true; // reset flag to true for every frame
					 //------raise flag to indicate whether to differentiate between gaze and non gaze region------//
		for (int iii = 0; iii < blockSize; iii++) 
		{
			for (int jjj = 0; jjj < blockSize; jjj++) 
			{
				if (i == block_number[iii][jjj])
					flag = false;
			}
		}

		//------background with N1 quantization------//
		if (CMP[i][0] == 0 && flag == true) 
		{
			for (int ii = 0; ii < blockSize * blockSize; ii++) 
			{
				R_Frame[ii] = CMP[i][ii + 1];
				G_Frame[ii] = CMP[i][ii + (blockSize * blockSize) + 1];
				B_Frame[ii] = CMP[i][ii + (blockSize * blockSize * 2) + 1];
			}

			//----------R, G, B Channel------------------//
			//---------- Quantization-----------//
			Mat Vid_R = Mat(blockSize, blockSize, DataType < float > ::type, R_Frame);
			Mat Vid_G = Mat(blockSize, blockSize, DataType < float > ::type, G_Frame);
			Mat Vid_B = Mat(blockSize, blockSize, DataType < float > ::type, B_Frame);

			Mat DCTQuantised_R;
			Mat DCTQuantised_G;
			Mat DCTQuantised_B;
			divide(Vid_R, Q_tab * N1, DCTQuantised_R);
			divide(Vid_G, Q_tab * N1, DCTQuantised_G);
			divide(Vid_B, Q_tab * N1, DCTQuantised_B);
			DCTQuantised_R.convertTo(DCTQuantised_R, DataType < char > ::type);
			DCTQuantised_G.convertTo(DCTQuantised_G, DataType < char > ::type);
			DCTQuantised_B.convertTo(DCTQuantised_B, DataType < char > ::type);
			Vid_R.release();
			Vid_G.release();
			Vid_B.release();

			//----------- DeQuantization ------------//
			DCTQuantised_R.convertTo(DCTQuantised_R, DataType < float > ::type);
			DCTQuantised_G.convertTo(DCTQuantised_G, DataType < float > ::type);
			DCTQuantised_B.convertTo(DCTQuantised_B, DataType < float > ::type);

			multiply(N1 * DCTQuantised_R, Q_tab, DCTDeQuantised_R);
			multiply(N1 * DCTQuantised_G, Q_tab, DCTDeQuantised_G);
			multiply(N1 * DCTQuantised_B, Q_tab, DCTDeQuantised_B);
			DCTQuantised_R.release();
			DCTQuantised_G.release();
			DCTQuantised_B.release();
			
		} // End of if for background and beginning of foreground.

		//----foreground with N2 quantization-----//
		else if (CMP[i][0] == 255 && flag == true) 
		{
			for (int ii = 0; ii < blockSize * blockSize; ii++) 
			{
				R_Frame[ii] = CMP[i][ii + 1];
				G_Frame[ii] = CMP[i][ii + (blockSize * blockSize) + 1];
				B_Frame[ii] = CMP[i][ii + (blockSize * blockSize * 2) + 1];
			}

			//----------R, G, B Channel------------------//
			//---------- Quantization-----------//
			Mat Vid_R = Mat(blockSize, blockSize, DataType < float > ::type, R_Frame);
			Mat Vid_G = Mat(blockSize, blockSize, DataType < float > ::type, G_Frame);
			Mat Vid_B = Mat(blockSize, blockSize, DataType < float > ::type, B_Frame);

			Mat DCTQuantised_R;
			Mat DCTQuantised_G;
			Mat DCTQuantised_B;
			divide(Vid_R, Q_tab * N2, DCTQuantised_R);
			divide(Vid_G, Q_tab * N2, DCTQuantised_G);
			divide(Vid_B, Q_tab * N2, DCTQuantised_B);
			DCTQuantised_R.convertTo(DCTQuantised_R, DataType < char > ::type);
			DCTQuantised_G.convertTo(DCTQuantised_G, DataType < char > ::type);
			DCTQuantised_B.convertTo(DCTQuantised_B, DataType < char > ::type);
			Vid_R.release();
			Vid_G.release();
			Vid_B.release();

			//----------- DeQuantization ------------//
			DCTQuantised_R.convertTo(DCTQuantised_R, DataType < float > ::type);
			DCTQuantised_G.convertTo(DCTQuantised_G, DataType < float > ::type);
			DCTQuantised_B.convertTo(DCTQuantised_B, DataType < float > ::type);

			multiply(N2 * DCTQuantised_R, Q_tab, DCTDeQuantised_R);
			multiply(N2 * DCTQuantised_G, Q_tab, DCTDeQuantised_G);
			multiply(N2 * DCTQuantised_B, Q_tab, DCTDeQuantised_B);
			DCTQuantised_R.release();
			DCTQuantised_G.release();
			DCTQuantised_B.release();
		}

		//------for gaze no quantization just IDCT-------//
		else if (flag == false) 
		{
			for (int ii = 0; ii < blockSize * blockSize; ii++) 
			{
				R_Frame[ii] = CMP[i][ii + 1];
				G_Frame[ii] = CMP[i][ii + (blockSize * blockSize) + 1];
				B_Frame[ii] = CMP[i][ii + (blockSize * blockSize * 2) + 1];
			}

			//----------R, G, B Channel------------------//
			//----------data that needs to be passed for IDCT-----------//
			DCTDeQuantised_R = Mat(blockSize, blockSize, DataType < float > ::type, R_Frame);
			DCTDeQuantised_G = Mat(blockSize, blockSize, DataType < float > ::type, G_Frame);
			DCTDeQuantised_B = Mat(blockSize, blockSize, DataType < float > ::type, B_Frame);
		}

		//========== IDCT ================//				
		idct(DCTDeQuantised_R, Vid_IDCTR);
		idct(DCTDeQuantised_G, Vid_IDCTG);
		idct(DCTDeQuantised_B, Vid_IDCTB);
		DCTDeQuantised_R.release();
		DCTDeQuantised_G.release();
		DCTDeQuantised_B.release();
		Vid_IDCTR.convertTo(Vid_IDCTR, DataType < unsigned char > ::type);
		Vid_IDCTG.convertTo(Vid_IDCTG, DataType < unsigned char > ::type);
		Vid_IDCTB.convertTo(Vid_IDCTB, DataType < unsigned char > ::type);

		//------Converting the Mat result to 1d array of R, G, B-----//
		int h_new = (i / (width / blockSize)) * blockSize; // capture the block number along height for 8x8 block size
		int w_new = (i % (width / blockSize)) * blockSize; // capture the block number along width for 8x8 block size

		for (int ii = 0; ii < blockSize; ii++) 
		{
			for (int jj = 0; jj < blockSize; jj++) 
			{
				Output_R2d[h_new + ii][w_new + jj] = Vid_IDCTR.at < unsigned char >(ii, jj); // [64 * i + ii * 8 + jj]
				Output_G2d[h_new + ii][w_new + jj] = Vid_IDCTG.at < unsigned char >(ii, jj); // [64 * i + ii * 8 + jj]
				Output_B2d[h_new + ii][w_new + jj] = Vid_IDCTB.at < unsigned char >(ii, jj); // [64 * i + ii * 8 + jj]
			}
		}
	}
	// Stack them for display.
	if (f != 1) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				Output[3 * i * width + 3 * j] = Output_R2d[i][j];
				Output[3 * i * width + 3 * j + 1] = Output_G2d[i][j];
				Output[3 * i * width + 3 * j + 2] = Output_B2d[i][j];
			}
		}
	}

	Mat Vid_Out = Mat(height, width, CV_8UC3);
	Vid_Out.data = Output;
	Mat Vid_BGR;
	cvtColor(Vid_Out, Vid_BGR, CV_RGB2BGR);
	imshow("try", Vid_BGR);
	in.close();
}

int main(int argc, char** argv) 
{
	//-----determine the size of the file-----//
	int fileSize = FileSize(argv[1]);
	const int frameCount = fileSize / (height_orig * width * 3);
	cout << "\nTotal number of frames in the video is: " << frameCount << endl;

	while (1) // infinite loop for playing video in loop
	{
		start1 = clock(); // start of each video
		int temp = 1, f = 1;

		//-----read compressed files representing each frame-----//
		while (f <= frameCount) 
		{
			//Paused Gaze Control
			if (playVideo == false)
			{
				showGaze(f);
			}

			//Pausing the Video
			if (playVideo == false && waitKey(delay) == 'x') {
				cout << "Playing Video" << endl;
				playVideo = true;
			}

			//Restart Video in Pause Mode
			if (playVideo == false && waitKey(delay) == 'r') {
				cout << "Restarting Video" << endl;
				playVideo = true;
				break;
			}

			//Exiting the Video by pressing E in Paused
			if (waitKey(delay) == 'e') {
				temp = 0;
				break;
			}

			if (playVideo) 
			{
				start = clock(); // start of each frame

				showGaze(f);

				if (waitKey(delay) == 'e') // press E to exit and close window
				{
					temp = 0;
					break;
				}
				else if (waitKey(delay) == 'r') // press R to restart video
				{
					cout << "Restarting Video" << endl;
					break;
				}

				if (waitKey(delay) == 'p') { //press P to pause video
					cout << "Pausing Video" << endl;
					playVideo = false;
				}

				f++;
				cout << "Time-taken for processing Frame-" << f << " is: " << (clock() - start) / (double)CLOCKS_PER_SEC << " seconds !!!\n";
			} // end of if
		} // end of for loop
		if (temp == 0) {
			destroyWindow("try");
			break;
		}
		cout << "\nTime-taken processing the whole video " << videoLoopCount + 1 << " time(s) is: " << (clock() - start1) / (double)CLOCKS_PER_SEC << " seconds !!!\n\n";
		videoLoopCount++;
	} //end of while loop 
	
	return 0;
}
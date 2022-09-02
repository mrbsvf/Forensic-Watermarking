#pragma once
#include "stdafx.h"
#include "Headers.h"
#define m 38
#define n 107
#define D 106

void ExtractWatermark(Mat& marked_img)
{
	Extract(marked_img);
}

/* DWT-DCT-CRT*/
float Extract_CRT(Mat blocks)
{
	int Z = blocks.at<float>(0, 0);
	if (Z == 40 || Z == 44) {
		Z += 60;
	}

	int p = Z % m;
	int q = Z % n;

	int d = abs(p - q);
	int b = p + q;

	if (b >= (D + 35) / 2) // watermark bit 1 
	{
		return 255;
	}
	else // watermark bit 0
	{
		return 0;
	}
}

void Extract(Mat& Marked_Image)
{
	//imshow("Marked_Image", Marked_Image);
	

	Mat yuv_arr[3];
	cvtColor(Marked_Image, Marked_Image, COLOR_RGB2YCrCb);   // RGB to YCrCb
	split(Marked_Image, yuv_arr);
	Mat Marked_Y_channel = Mat(Marked_Image.cols, Marked_Image.rows, Marked_Image.type());
	int QRcodeSize;

	yuv_arr[0].copyTo(Marked_Y_channel); 
	yuv_arr[0].convertTo(Marked_Y_channel, CV_32F);    //uchar -> float

	//Mat HH_recoverd_QRcode_Pixel = Mat(32, 32, CV_8UC1);
	//Mat HL_recoverd_QRcode_Pixel = Mat(32, 32, CV_8UC1);
	Mat LH_recoverd_QRcode_Pixel = Mat(32, 32, CV_8UC1);

	Mat WT_result;
	yuv_arr[0].convertTo(Marked_Y_channel, CV_32F);      //uchar -> float
	WT(Marked_Y_channel, WT_result, 1); 
	//imshow("Extracted_Image_WT", WT_result);

    //Mat HH_subband = Mat(WT_result.cols / 2, WT_result.rows / 2, WT_result.type());
   // Mat HL_subband = Mat(WT_result.cols / 2, WT_result.rows / 2, WT_result.type());
	Mat LH_subband = Mat(WT_result.cols / 2, WT_result.rows / 2, WT_result.type());

	//HH_subband = WT_result(Rect(WT_result.cols / 2, WT_result.rows / 2, WT_result.cols / 2, WT_result.rows / 2)); // HH
	//HL_subband = WT_result(Rect(WT_result.cols / 2, 0, WT_result.cols / 2, WT_result.rows / 2));
	LH_subband = WT_result(Rect(0, WT_result.rows / 2, WT_result.cols / 2, WT_result.rows / 2));
	//LL_subband = WT_result(Rect(0, 0, WT_result.rows / 2, WT_result.cols / 2)); // LL

	Size blockSize(8, 8);
	//vector<Mat> HH_blocks;
	//vector<Mat> HL_blocks;
	vector<Mat> LH_blocks;

	for (int y = 0; y < 256; y += blockSize.height)
	{
		for (int x = 0; x < 256; x += blockSize.width)
		{
			Rect rect = Rect(x, y, blockSize.width, blockSize.height);
			//HH_blocks.push_back(Mat(HH_subband, rect));
			//HL_blocks.push_back(Mat(HL_subband, rect));
			LH_blocks.push_back(Mat(LH_subband, rect));
		}
	}

	int x = 0, y = 0;
	//   cout << "EXTRACTED" << endl;
	int enter = 0;

	for (int i = 0; i < 1024; i++)
	{
		//dct(HH_blocks[i], HH_blocks[i]);
		//dct(HL_blocks[i], HL_blocks[i]);
		dct(LH_blocks[i], LH_blocks[i]);
		enter++;
		if (enter == 32) {
			enter = 0;
		}
	}

	for (int i = 0; i < 1024; i++)
	{
		//HH_recoverd_QRcode_Pixel.at<uchar>((int)(y), (int)(x)) = Extract_CRT(HH_blocks[i]);
		//HL_recoverd_QRcode_Pixel.at<uchar>((int)(y), (int)(x)) = Extract_CRT(HL_blocks[i]);
		LH_recoverd_QRcode_Pixel.at<uchar>((int)(y), (int)(x++)) = Extract_CRT(LH_blocks[i]);

		if (x == 32)
		{
			y++;
			x = 0;
		}
	}

	//QRcodeSize = HL_recoverd_QRcode_Pixel.rows;
	QRcodeSize = LH_recoverd_QRcode_Pixel.rows;
	//Mat QR_HH(QRcodeSize + 2, QRcodeSize + 2, HH_recoverd_QRcode_Pixel.type(), Scalar(255));
	//Mat QR_HL(QRcodeSize + 2, QRcodeSize + 2, HL_recoverd_QRcode_Pixel.type(), Scalar(255));
	Mat QR_LH(QRcodeSize + 2, QRcodeSize + 2, LH_recoverd_QRcode_Pixel.type(), Scalar(255));

	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			//QR_HH.at<uchar>(i + 1, j + 1) = HH_recoverd_QRcode_Pixel.at<uchar>(i, j);
			//QR_HL.at<uchar>(i + 1, j + 1) = HL_recoverd_QRcode_Pixel.at<uchar>(i, j);
			QR_LH.at<uchar>(i + 1, j + 1) = LH_recoverd_QRcode_Pixel.at<uchar>(i, j);
		}
	}

    //Mat BIG_QR_HH(100, 100, HH_recoverd_QRcode_Pixel.type(), Scalar(255));
    //Mat BIG_QR_HL(100, 100, HL_recoverd_QRcode_Pixel.type(), Scalar(255));
	Mat BIG_QR_LH(100, 100, LH_recoverd_QRcode_Pixel.type(), Scalar(255));
	int nn = 0;
	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			//BIG_QR_HH.at<uchar>(nn, 3 * j) = QR_HH.at<uchar>(i, j);
			//BIG_QR_HH.at<uchar>(nn, 3 * j + 1) = QR_HH.at<uchar>(i, j);
			//BIG_QR_HH.at<uchar>(nn, 3 * j + 2) = QR_HH.at<uchar>(i, j);
			//BIG_QR_HH.at<uchar>(nn + 1, 3 * j) = QR_HH.at<uchar>(i, j);
			//BIG_QR_HH.at<uchar>(nn + 1, 3 * j + 1) = QR_HH.at<uchar>(i, j);
			//BIG_QR_HH.at<uchar>(nn + 1, 3 * j + 2) = QR_HH.at<uchar>(i, j);
			//BIG_QR_HH.at<uchar>(nn+ 2, 3 * j) = QR_HH.at<uchar>(i, j);
			//BIG_QR_HH.at<uchar>(nn + 2, 3 * j + 1) = QR_HH.at<uchar>(i, j);
			//BIG_QR_HH.at<uchar>(nn + 2, 3 * j + 2) = QR_HH.at<uchar>(i, j);

			//BIG_QR_HL.at<uchar>(nn, 3 * j) = QR_HL.at<uchar>(i, j);
			//BIG_QR_HL.at<uchar>(nn, 3 * j + 1) = QR_HL.at<uchar>(i, j);
			//BIG_QR_HL.at<uchar>(nn, 3 * j + 2) = QR_HL.at<uchar>(i, j);
			//BIG_QR_HL.at<uchar>(nn + 1, 3 * j) = QR_HL.at<uchar>(i, j);
			//BIG_QR_HL.at<uchar>(nn + 1, 3 * j + 1) = QR_HL.at<uchar>(i, j);
			//BIG_QR_HL.at<uchar>(nn + 1, 3 * j + 2) = QR_HL.at<uchar>(i, j);
			//BIG_QR_HL.at<uchar>(nn + 2, 3 * j) = QR_HL.at<uchar>(i, j);
			//BIG_QR_HL.at<uchar>(nn + 2, 3 * j + 1) = QR_HL.at<uchar>(i, j);
			//BIG_QR_HL.at<uchar>(nn + 2, 3 * j + 2) = QR_HL.at<uchar>(i, j);

			BIG_QR_LH.at<uchar>(nn, 3 * j) = QR_LH.at<uchar>(i, j);
			BIG_QR_LH.at<uchar>(nn, 3 * j + 1) = QR_LH.at<uchar>(i, j);
			BIG_QR_LH.at<uchar>(nn, 3 * j + 2) = QR_LH.at<uchar>(i, j);
			BIG_QR_LH.at<uchar>(nn + 1, 3 * j) = QR_LH.at<uchar>(i, j);
			BIG_QR_LH.at<uchar>(nn + 1, 3 * j + 1) = QR_LH.at<uchar>(i, j);
			BIG_QR_LH.at<uchar>(nn + 1, 3 * j + 2) = QR_LH.at<uchar>(i, j);
			BIG_QR_LH.at<uchar>(nn + 2, 3 * j) = QR_LH.at<uchar>(i, j);
			BIG_QR_LH.at<uchar>(nn + 2, 3 * j + 1) = QR_LH.at<uchar>(i, j);
			BIG_QR_LH.at<uchar>(nn + 2, 3 * j + 2) = QR_LH.at<uchar>(i, j);
		}
		nn += 3;
	}

	//imshow("HH_Extracted_QRcode", BIG_QR_HH);
	imshow("LH_Extracted_QRcode", BIG_QR_LH);
	//imshow("HL_Extracted_QRcode", BIG_QR_HL);

	//imwrite("HH_Extracted_QRcode.png", BIG_QR_HH);
	imwrite("LH_Extracted_QRcode.png", BIG_QR_LH);
	//imwrite("HL_Extracted_QRcode.png", BIG_QR_HL);
	

	cout << "**********DWT-DCT-CRT**********" << endl;
	cvtColor(Marked_Image, Marked_Image, COLOR_YCrCb2RGB);   // YCrCb to RGB
	getPSNR(Marked_Image);
	getNCC();
}
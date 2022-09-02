#include "stdafx.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "BitBuffer.hpp"
#include "QrCode.hpp"

#include "QRmain.h"

using std::uint8_t;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;
using namespace std;
using namespace cv;

/* Creates a single QR Code, then prints it to the console. */
Mat doBasicDemo(CString id)
{

	int len = id.GetLength();
	char* text = new char[len+1];
	LPCWCH idCH = CT2CW(id.GetString());
	WideCharToMultiByte(CP_ACP, 0, idCH, -1, text, len + 1, NULL, NULL);
	

	const QrCode::Ecc errCorLvl = QrCode::Ecc::LOW;  // Error correction level

	// Make and print the QR Code symbol
	const QrCode qr = QrCode::encodeText(text, errCorLvl);

	Mat QR;
	QR = printQr(qr, 32); // qrcode size를 32로 줌

	return QR;
}

// Prints the given QrCode object to the console.
Mat printQr(const QrCode &qr, int size) {
   int border = (32 - qr.getSize()) / 2;
   //Mat QRCODE(size, size, CV_8UC3, Scalar(255, 255, 255)); // SVD 방식 삽입/추출 사용 시
   Mat QRCODE(size, size, CV_8UC1, Scalar(255));
   int intW = 1;

   int mv = (32 - qr.getSize()) / 2;
   int a = 0;
   int b = (32 - qr.getSize()) / 2;
   int re = 1;
   for (int y = (-border); y < (qr.getSize() + border); y++) {
      int f = 0;
      a = 0;
      for (int x = (-border); x < (qr.getSize() + border); x++) {
         if (qr.getModule(x, y) == true) { // black
            for (int i = 0; i < re; i++) {
               //line(QRCODE, Point(a + i, b), Point(a + i, b + re), Scalar(0, 0, 0), 1);
			   line(QRCODE, Point(a + i, b), Point(a + i, b + re), Scalar(0), 1);
            }
            f = 1;
         }
         else { // white
            for (int i = 0; i < re; i++) {
               //line(QRCODE, Point(a + i, b), Point(a + i, b + re), Scalar(255, 255, 255), 1);
			   line(QRCODE, Point(a + i, b), Point(a + i, b + re), Scalar(255), 1);
            }
         }
         a += re;
      }
      if (f == 1) { b += re; }
   }
   const char* qrcodeName = "QRCODE.jpg";
  // imwrite(qrcodeName, QRCODE);
   
   
   Mat QR(34, 34, QRCODE.type(), Scalar(255));
   //cout << qr.getSize() << "  " << qr.getSize() + 2 << endl;
   for (int i = 0; i < 32; i++)
   {
	   for (int j = 0; j < 32; j++)
	   {
		   QR.at<uchar>(i + 1, j + 1) = QRCODE.at<uchar>(i, j);
	   }
   }

   Mat QRCODE_100(100, 100, QRCODE.type(), Scalar(255));

   int n = 0;
   for (int i = 0; i < 32; i++)
   {
	   for (int j = 0; j < 32; j++)
	   {
		   QRCODE_100.at<uchar>(n, 3 * j) = QR.at<uchar>(i, j);
		   QRCODE_100.at<uchar>(n, 3 * j + 1) = QR.at<uchar>(i, j);
		   QRCODE_100.at<uchar>(n, 3 * j + 2) = QR.at<uchar>(i, j);
		   QRCODE_100.at<uchar>(n + 1, 3 * j) = QR.at<uchar>(i, j);
		   QRCODE_100.at<uchar>(n + 1, 3 * j + 1) = QR.at<uchar>(i, j);
		   QRCODE_100.at<uchar>(n + 1, 3 * j + 2) = QR.at<uchar>(i, j);
		   QRCODE_100.at<uchar>(n + 2, 3 * j) = QR.at<uchar>(i, j);
		   QRCODE_100.at<uchar>(n + 2, 3 * j + 1) = QR.at<uchar>(i, j);
		   QRCODE_100.at<uchar>(n + 2, 3 * j + 2) = QR.at<uchar>(i, j);
	   }
	   n += 3;
   }
   imshow("Original_Qrcode", QRCODE_100);
   imwrite("Original_QRCODE.png", QRCODE_100);
   
   return QRCODE;
}

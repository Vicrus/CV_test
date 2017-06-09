#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <vector>

#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\imgproc\imgproc.hpp>


using namespace std;
using namespace cv;

void eye_detect(Mat frame, CascadeClassifier cascade_eyes, Rect face);
void smile_detect(Mat frame, CascadeClassifier cascade_smile, Rect face, int* smile_counter);
int proof_point(int x, int y, Rect face);
void glass_print(int x, int y, Mat frame, Mat glasses_file, int width);
Mat glasses_file;

int main() {
	VideoCapture cap(0);
	//CvCapture* cap;
	RNG rng(12345);
	int count = 0;
	CascadeClassifier cascade_face;
	CascadeClassifier cascade_eyes;
	CascadeClassifier cascade_smile;
	glasses_file = imread("glasses.png", CV_LOAD_IMAGE_COLOR);
	if (!cascade_face.load("cascade\\haarcascade_frontalface_alt.xml")) {
		cout << "error_face_Loading" << endl;
	}
	if (!cascade_eyes.load("cascade\\haarcascade_eye.xml")) {
		cout << "error_eye_loading" << endl;
	}
	if (!cascade_smile.load("cascade\\haarcascade_smile.xml")) {
		cout << "error_smile_loading" << endl;
	}
	

	int smileCounter = 0;
	/*if (!cap.isOpened()) {
	printf("dont opened!\n");
	return 0;
	}*/
	//cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT, 960);
	//cap.set(CAP_PROP_FPS, 10);

	Mat frame;
	Mat frame_gray;
	vector<Rect> faces;
	
	//cap = cvCaptureFromCAM(-1);
	while (true) {
		cap >> frame; //-- çàõâàòûâàåì î÷åðåäíîé êàäð
					  //frame = cvQueryFrame(cap);
		cvtColor(frame, frame_gray, CV_RGB2GRAY);
		cascade_face.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
		
		
		//buff = frame * glasses;
		//cascade_smile.detectMultiScale(frame_gray, smiles, 1, 1, 0 | CV_HAAR_SCALE_IMAGE, Size(10, 10));
		for (int i = 0; i < (int)faces.size(); i++)
		{
			Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
			int radius = cvRound((faces[i].width + faces[i].height)*0.25);
			//circle(frame, center, radius, Scalar(255, 0, 255), 4, 8, 0);
			eye_detect(frame, cascade_eyes, faces[i]);
			smile_detect(frame, cascade_smile, faces[i], &smileCounter);

		}
		imshow("Video", frame);
		//imshow("Video_2", buff);

		char c = cvWaitKey(33); //-- åñëè áûëà íàæàòà êëàâèøà, óçíà¸ì å¸ êîä

								/*if (c == 27) {

								string filename = "images/image" + to_string(count) + ".jpg";
								imwrite(filename, frame);
								count++;
								}*/
		if (c == 27) { //-- íàæàòà ESC, ïðåðûâàåì öèêë
			break;
		}


	}


	return 0;
}

void eye_detect(Mat frame, CascadeClassifier cascade_eyes, Rect face) {
	Rect eye_area(0, 0, face.width, face.height / 2);
	Mat face_frame = frame(face);
	face_frame = face_frame(eye_area);
	Mat gray;
	cvtColor(face_frame, gray, CV_RGB2GRAY);
	vector<Rect> eyes;
	cascade_eyes.detectMultiScale(gray, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	/*for (int i = 0; i < eyes.size(); i++) {
		Point center(face.x + eyes[i].x + eyes[i].width*0.5, face.y + eyes[i].y + eyes[i].height*0.5);
		int radius = cvRound((eyes[i].width + eyes[i].height)*0.25);
		circle(frame, center, radius, Scalar(255, 0, 0), 4, 8, 0);
	}*/
	if (eyes.size()>1) {
		if (eyes[0].x < eyes[1].x) {
			glass_print(face.x + eyes[0].x - eyes[0].width*0.3, face.y + eyes[0].y+eyes[0].height*0.5, frame, glasses_file, face.width);
		}
		else
			glass_print(face.x + eyes[1].x - eyes[1].width*0.3, face.y + eyes[1].y + eyes[0].height*0.5, frame, glasses_file, face.width);
	}
	

}

void smile_detect(Mat frame, CascadeClassifier cascade_smile, Rect face, int* smile_counter) {
	Rect smile_area(0, face.height - face.height / 3, face.width, face.height / 3);
	Mat face_frame = frame(face);
	face_frame = face_frame(smile_area);
	Mat gray;
	cvtColor(face_frame, gray, CV_RGB2GRAY);
	vector<Rect> smiles;
	cascade_smile.detectMultiScale(gray, smiles, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	for (int i = 0; i < smiles.size(); i++) {
		//Point center(face.x + eyes[i].x + eyes[i].width*0.5, face.y + eyes[i].y + eyes[i].height*0.5);
		int radius = cvRound((smiles[i].width + smiles[i].height)*0.25);
		//rectangle(frame, center, radius, Scalar(255, 0, 0), 4, 8, 0);
		Rect rect_b(face.x + smiles[i].x, face.y + smiles[i].y + face.height - face.height / 3, smiles[i].width, smiles[i].height);
		rectangle(frame, rect_b, Scalar(0, 255, 0));
	}
	int count_point = 0;
	/*for (int i = 0; i < smiles.size(); i++) {
	int x = smiles[i].x;
	int y = smiles[i].y;
	count_point += proof_point(x, y, face);
	x = smiles[i].x + smiles[i].width;
	y = smiles[i].y;
	count_point += proof_point(x, y, face);

	x = smiles[i].x;
	y = smiles[i].y + smiles[i].height;
	count_point += proof_point(x, y, face);
	x = smiles[i].x + smiles[i].width;
	y = smiles[i].y + smiles[i].height;
	count_point += proof_point(x, y, face);
	if (count_point < 3) {
	smiles.erase(smiles.begin() + i);
	}
	}*/
	if (smiles.size()  == 0) {
		
		cout << "not" << endl;
	}
	else
		cout << "you smile "<< endl;

	/*if (*smile_counter >= 10) {
		cout << "not " <<*smile_counter<< endl;
		*smile_counter = 0;
	}
	else {
		cout << "you smile " << *smile_counter << endl;
		
	}*/

}

int proof_point(int x, int y, Rect face) {
	if ((x - face.x)*(x - face.x) / (face.width*face.width) + (y - face.y)*(y - face.y) / (face.height*face.height) > 1) {
		return 1;
	}
	else {
		return 0;
	}
}

void glass_print(int x, int y, Mat frame, Mat glasses_file, int width) {
	Mat glasses;
	int glasses_x = x;
	int glasses_y = y;
	int d_x = 0;
	int d_y = 0;
	
	float a = 0.9;
	while (glasses_file.cols*a > width) {
		a -= 0.1;
	}
	Size size(glasses_file.cols*a, glasses_file.rows*a);
	resize(glasses_file, glasses, size);
	for (int i = 0; i < frame.rows; i++) {
		for (int j = 0; j < frame.cols; j++) {
			//if (j > glasses_x && i > glasses_y) {
				if (d_x < glasses.rows && d_y < glasses.cols) {
					if (!(glasses.at<Vec3b>(d_x, d_y)[0] == 0 && (glasses.at<Vec3b>(d_x, d_y)[1] >= 50 && glasses.at<Vec3b>(d_x, d_y)[1] <= 255) &&
						glasses.at<Vec3b>(d_x, d_y)[2] == 0)) {
						frame.at<Vec3b>(j + glasses_y, i + glasses_x)[0] = glasses.at<Vec3b>(d_x, d_y)[0];
						frame.at<Vec3b>(j + glasses_y, i + glasses_x)[1] = glasses.at<Vec3b>(d_x, d_y)[1];
						frame.at<Vec3b>(j + glasses_y, i + glasses_x)[2] = glasses.at<Vec3b>(d_x, d_y)[2];
					}
					d_x++;
				}
			//}
		}
		d_y++;
		d_x = 0;
	}
}
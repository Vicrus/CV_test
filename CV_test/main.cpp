#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <opencv2\opencv.hpp>


using namespace std;
using namespace cv;

int main() {
	VideoCapture cap(0);

	int count = 0;
	CascadeClassifier cascade_face;
	CascadeClassifier cascade_eyes;
	if (!cascade_face.load("cascade\\haarcascade_frontalface_alt.xml")) {
		cout << "error_face_Loading" << endl;
	}
	if (!cascade_eyes.load("cascade\\haarcascade_eye.xml")) {
		cout << "error_eye_loading" << endl;
	}

	if (!cap.isOpened()) {
		printf("dont opened!\n");
		return 0;
	}
	//cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT, 960);
	cap.set(CAP_PROP_FPS, 10);
	
	Mat frame;
	Mat frame_gray;
	Mat* frame_face;
	vector<Rect> faces;
	vector<Rect>* eye;
	

	while (true) {
		cap >> frame; //-- захватываем очередной кадр
		cvtColor(frame, frame_gray, CV_RGB2GRAY);
		cascade_face.detectMultiScale(frame_gray, faces, 1.1, 2,0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
		//cascade_smile.detectMultiScale(frame_gray, smiles, 1, 1, 0 | CV_HAAR_SCALE_IMAGE, Size(10, 10));
		frame_face = new Mat[(int)faces.size()];
		eye = new vector<Rect>[(int)faces.size()];
		for (int i = 0; i < (int)faces.size(); i++)
		{
			Rect eye_area(0, 0, faces[i].width, faces[i].height/2);
			frame_face[i] = frame(faces[i]);
			frame_face[i] = frame_face[i](eye_area);
			cascade_eyes.detectMultiScale(frame_face[i], eye[i], 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
			rectangle(frame, faces[i], Scalar(255, 0, 255), 2, 8);
			int j = 0;
			for (int j = 0; j < (int)eye[i].size(); j++) {
				Rect buf(faces[i].x + eye[i][j].x, faces[i].y + eye[i][j].y, eye[i][j].width, eye[i][j].height);
				rectangle(frame, buf, Scalar(120, 0, 255), 2, 8);
			}
			//rectangle(frame, smiles[i], Scalar(100, 0, 255), 2, 8);
			if ((int)eye[i].size() < 1) {
				string filename = "images/sleep" + to_string(count) + ".jpg";
				imwrite(filename, frame);
				count++;
				//cout << "sleep" << endl;
			}
		}
		imshow("Video", frame); //-- показываем его
		delete[]frame_face;
		delete[]eye;


		char c = cvWaitKey(33); //-- если была нажата клавиша, узнаём её код

		/*if (c == 27) {
			
			string filename = "images/image" + to_string(count) + ".jpg";
			imwrite(filename, frame);
			count++;
		}*/
		if (c == 27) { //-- нажата ESC, прерываем цикл
			break;
		}


	}


	return 0;
}
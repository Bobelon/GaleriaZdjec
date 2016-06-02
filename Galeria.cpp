#include <stdio.h>
#include <opencv2/opencv.hpp>

//using namespace cv;

int main( int argc, char** argv ) {
	double w,h,fps;
	bool continueCapture = true;
	cv::namedWindow( "Okienko Prawie Inteligentne", CV_WINDOW_AUTOSIZE );
	// Tu zagadka dla Ciebie :)
	// cv::VideoCapture cap( "dlugopis.mp4" );
	 cv::VideoCapture cap(0);
	if ( !cap.isOpened() ) return -1;
	w = cap.get( CV_CAP_PROP_FRAME_WIDTH ); //get the width of frames of the video
	h = cap.get( CV_CAP_PROP_FRAME_HEIGHT ); //get the height of frames of the video
	fps = cap.get( CV_CAP_PROP_FPS ); // ile mamy klatek na sekunde?
	std::cout << "Parametry wideo : " << w << " x " << h << " @ " << fps << " fps"<< std::endl;
	if ( fps < 0 ) fps = 30; // tak na wszelki wypadek :)
	while( continueCapture ) {
		int key;
		cv::Mat frame;
// 		cap >> frame; -- mozna tak, jesli wiemy ze nie bedzdie bledow
		if ( cap.read( frame ) ) {
			cv::imshow( "Okienko Prawie Inteligentne", frame );
		} else continueCapture = false;
		//czekaj na klawisz, sprawdz czy to jest 'esc'
		if( cv::waitKey( 1000.0/fps ) == 1048603 ) continueCapture = false;
	}
	return 0;
}

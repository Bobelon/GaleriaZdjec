#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main( int argc, char** argv ) {
	
	double width;
	double height;
	double fps;
	
	namedWindow( "Wspaniała galeria zdjęć", CV_WINDOW_AUTOSIZE ); // Tworzenie nowego okna
	VideoCapture cap(0);
	
	// Błąd dostępu do kamerki
	if (!cap.isOpened())
		return -1;
	
	// Pobieranie parametrów obrazka
	width = cap.get (CV_CAP_PROP_FRAME_WIDTH);
	height = cap.get (CV_CAP_PROP_FRAME_HEIGHT);
	fps = cap.get (CV_CAP_PROP_FPS);
	
	// Mało ważna informacja w terminalu
	cout << "Parametry wideo : " << width<< " x " << height << " @ " << fps << " fps"<< endl;
	
	if (fps < 0)
		fps = 30; // Tak na wszelki wypadek :)
		
		
		
	// Główna pętla programu
	while(true) {
		int key;
		Mat frame;
		Mat frame2 = frame;
		
		if (cap.read(frame)) {
			cvtColor( frame, frame, CV_RGB2HSV); // filtr zmieniający skalę barw
			flip(frame, frame, 1); // Odbicie lustrzane
			imshow( "Wspaniała galeria zdjęć", frame );
		}
		else
			break;
		
		// Czekaj na klawisz, jeżeli jest to 'esc' to kończ pracę
		if(waitKey(1000.0/fps) == 1048603)
			break;
	}
	return 0;
}

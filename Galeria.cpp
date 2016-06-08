#include <stdio.h>
#include <dirent.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/* Funkcka files zapożyczona z 
 * http://cpp0x.pl/artykuly/?id=53
 * */
void files (const char * dirPath ) {
    struct dirent * file;
    DIR * path;
   
    if (path = opendir (dirPath)) {
        while (file = readdir (path))
             puts (file -> d_name);
       
        closedir( path );
    }
    else
         cout << "Nieprawidlowa sciezka" << endl;
}

int main( int argc, char** argv ) {
	
	double width;
	double height;
	double fps;
	
	//files (".");
	
	namedWindow ("Wspaniała galeria zdjęć", CV_WINDOW_AUTOSIZE); // Tworzenie nowego okna
	namedWindow ("Testowe",  CV_WINDOW_AUTOSIZE);
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
		Mat bgr, hsv, mirror, colorDetection, frame;
		Mat frame2;
		
		if (cap.read(bgr)) {
			cvtColor( bgr, hsv, CV_BGR2HSV); // filtr zmieniający skalę barw
			flip(hsv, mirror, 1); // Odbicie lustrzane
			//blur( frame, frame, Size(10,10) );
			//blur( frame, frame, Size(10,10) );
			//erode(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );
			//dilate(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );
			inRange (mirror, Scalar (60, 200, 130), Scalar (100, 255, 255), colorDetection);
			//inRange (frame, Scalar (80, 120, 63), Scalar (120, 240, 83), frame2);
			//inRange (frame, Scalar (0, 100, 100), Scalar (55, 255, 200), frame2);
			dilate(colorDetection, colorDetection, getStructuringElement(MORPH_ELLIPSE, Size(20, 20)) );
			erode(colorDetection, colorDetection, getStructuringElement(MORPH_ELLIPSE, Size(20, 20)) );
			dilate(colorDetection, colorDetection, getStructuringElement(MORPH_ELLIPSE, Size(20, 50)) );
			dilate(colorDetection, colorDetection, getStructuringElement(MORPH_ELLIPSE, Size(20, 50)) );
			
			//Canny( frame2, frame, 3, 3*2, 3 );
			//findContours( frame, frame, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
			imshow ("Wspaniała galeria zdjęć", colorDetection);
			imshow ("Testowe", mirror);
		}
		else
			break;
		
		// Czekaj na klawisz, jeżeli jest to 'esc' to kończ pracę
		if(waitKey(1000.0/fps) == 1048603)
			break;
	}
	return 0;
}

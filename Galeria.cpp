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
		Mat frame;
		Mat frame2;
		
		if (cap.read(frame)) {
			//cvtColor( frame, frame, CV_RGB2HSV); // filtr zmieniający skalę barw
			flip(frame, frame, 1); // Odbicie lustrzane
			//inRange (frame, Scalar (75, 115, 65), Scalar (177, 233, 134), frame2);
			inRange (frame, Scalar (80, 120, 63), Scalar (120, 240, 83), frame2);
			dilate(frame2, frame2, getStructuringElement(MORPH_ELLIPSE, Size(20, 20)) );
			erode(frame2, frame2, getStructuringElement(MORPH_ELLIPSE, Size(20, 20)) );
			dilate(frame2, frame2, getStructuringElement(MORPH_ELLIPSE, Size(20, 50)) );
			
			dilate(frame2, frame2, getStructuringElement(MORPH_ELLIPSE, Size(20, 50)) );
			imshow ("Wspaniała galeria zdjęć", frame2);
			imshow ("Testowe", frame);
		}
		else
			break;
		
		// Czekaj na klawisz, jeżeli jest to 'esc' to kończ pracę
		if(waitKey(1000.0/fps) == 1048603)
			break;
	}
	return 0;
}

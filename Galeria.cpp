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
	
	files (".");
	
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
		
		if (cap.read(frame)) {
			cvtColor( frame, frame, CV_RGB2HSV); // filtr zmieniający skalę barw
			flip(frame, frame, 1); // Odbicie lustrzane
			inRange (frame, Scalar (197, 243, 118), Scalar (166, 238, 44), frame);
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

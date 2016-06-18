#include <stdio.h>
#include <dirent.h>
#include <list>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

list <string> listOfImages;
list<string>::iterator iter;
string name;

// Tworzenie listy zdjęć
void files (const char * dirPath ) {
	
    struct dirent *file;
    DIR *path;
    
    if (path = opendir (dirPath)) {
		
		char *type = (char *) malloc (4 * sizeof (char));
		
		cout << "GALERIA ZDJEC:\n\n";
		
		// Dodawanie do listy tylko plików z rozszezeniem *jpg
		while (file = readdir (path)) {
			
			if (strcmp (file->d_name, ".") != 0 && strcmp (file->d_name, "..") != 0) {
				listOfImages.push_back (file->d_name);
				cout << dirPath << file->d_name << endl;
			}
			
			//cout << "fn: " << file->d_name << " " << strchr (file->d_name, '.') << "\n";
			// NIE DZIAŁA
			/*
			strcpy (type, strchr (file->d_name, '.')); 		
			
			if (strcmp (type, ".jpg") == 0) {
				listOfImages.push_back (file->d_name);
				cout << file->d_name << endl;
			}
			* */
		}
        closedir (path);
        
        listOfImages.sort();
    }    
    else {
		cout << "Nieprawidlowa sciezka" << endl;
	}
}

// Wyświetlanie obrazka
void showPicture (bool next) {
	destroyWindow (*iter);
	
	if (next) {
		iter++;
		if (iter == listOfImages.end()) {
			iter = listOfImages.begin();
		} 
	}
	else {
		if (iter == listOfImages.begin()) {
			iter = listOfImages.end();
		}
		iter--;
	}
	
	Mat image = imread (name + *iter, CV_LOAD_IMAGE_COLOR);
	
	if (image.data) {
		imshow (*iter, image);
		moveWindow (*iter, 0, 0);
	}
}


int main( int argc, char** argv ) {
	
	double width;
	double height;
	double fps;
	clock_t currentTime = clock() / CLOCKS_PER_SEC;

	if (argc > 1) {
		files (argv[1]);
		name = argv[1];
	}
	else {
		name = "CL/";
		files ("CL/");
	}
	
	iter = listOfImages.begin();
	
	
	
	VideoCapture cap(0);
	
	// Błąd dostępu do kamerki
	if (!cap.isOpened())
		return -1;
		
	/*
	int iLowH = 0;
	int iHighH = 255;

	int iLowS = 0; 
	int iHighS = 255;

	int iLowV = 0;
	int iHighV = 255;
	 
	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	cvCreateTrackbar("LowH", "Control", &iLowH, 255); //Hue (0 - 255)
	cvCreateTrackbar("HighH", "Control", &iHighH, 255);

	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &iHighS, 255);

	cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &iHighV, 255);
	*/
	
	// Pobieranie parametrów obrazka
	width = cap.get (CV_CAP_PROP_FRAME_WIDTH);
	height = cap.get (CV_CAP_PROP_FRAME_HEIGHT);
	fps = cap.get (CV_CAP_PROP_FPS);
	
	if (fps < 0)
		fps = 30; // Tak na wszelki wypadek :)
		
		
		
	int key;
	Mat bgr, hsv, mirror, colorDetection, countour;
	
	// Główna pętla programu
	while(true) {		
		
		if (cap.read(bgr)) {
			//blur( bgr, bgr, Size(10,10) );
			cvtColor( bgr, hsv, CV_BGR2HSV); // filtr zmieniający skalę barw
			flip(hsv, mirror, 1); // Odbicie lustrzane
			//inRange(mirror, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), colorDetection); //Threshold the image
			inRange (mirror, Scalar (60, 200, 130), Scalar (100, 255, 255), colorDetection); // znajdowanie koloru
			/*
			dilate(colorDetection, colorDetection, getStructuringElement(MORPH_ELLIPSE, Size(20, 20)) );
			erode(colorDetection, colorDetection, getStructuringElement(MORPH_ELLIPSE, Size(20, 20)) );
			dilate(colorDetection, colorDetection, getStructuringElement(MORPH_ELLIPSE, Size(20, 50)) );
			dilate(colorDetection, colorDetection, getStructuringElement(MORPH_ELLIPSE, Size(20, 50)) );
			* */
			erode(colorDetection, colorDetection, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
			dilate( colorDetection, colorDetection, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
			dilate( colorDetection, colorDetection, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
			erode(colorDetection, colorDetection, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
			
			int jeden, dwa, trzy, cztery;
				jeden = 15;
				dwa = 13;
				trzy = 20;
				cztery = 3;
			Canny( colorDetection, colorDetection, jeden, dwa * trzy, cztery ); // Tworzenie konturu
			
			vector <vector <Point> > contours;
			findContours (colorDetection, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);	
			drawContours (colorDetection, contours, 0, Scalar(255,0,0),5,8);
			
			
			/// Momenty :)
			vector<Moments> mu(contours.size() );
			for( int i = 0; i < contours.size(); i++ ) {
				mu[i] = moments( contours[i], false ); 
			}
        		
			if (mu.size() > 0) {
				Point2f mc; // mass center
				int maxI = 0;
				
				int x, y;
					x = mu[maxI].m10/mu[maxI].m00;
					y = mu[maxI].m01/mu[maxI].m00;
					
				float time, distance;
					distance = abs ((width / 2) - x);
					
					if (distance != 0) {
						time = 50 / distance;
					}
					else {
						time = 10;
					}
				
					cout << ( (double)(clock () - currentTime) / CLOCKS_PER_SEC) << "  ::  " << time <<  endl;
				
				// Kolejne lub poprzednie zdjęcie
				if ((x < width / 2) && ((double)( (clock () - currentTime) / CLOCKS_PER_SEC) >= time)) {
					currentTime = clock();
					showPicture(false);
				}
				else if ((x > width / 2) && (((double) (clock () - currentTime) / CLOCKS_PER_SEC) >= time)) {
					currentTime = clock();						
					showPicture(true);
				}			
			
				mc = Point2f( x , y );
				
				for( int i = 0; i < contours.size(); i++ ) {
					if (contourArea(contours[maxI]) < contourArea(contours[i])) {
						maxI = i;
						mc = Point2f( mu[maxI].m10/mu[maxI].m00, mu[maxI].m01/mu[maxI].m00);					
					} 
				}
			}			
			
			
			namedWindow ("Kształt", CV_WINDOW_AUTOSIZE); // Tworzenie nowego okna
			imshow ("Kształt", colorDetection);
			/*
			namedWindow ("BGR",  CV_WINDOW_AUTOSIZE);			
			imshow ("BGR", mirror);
			*/
		}
		else
			break;
		
		// Czekaj na klawisz, jeżeli jest to 'esc' to kończ pracę
		//if(waitKey(1000.0/fps) == 1048603)
		if(waitKey(1000.0/fps) != -1) {
			break;
		}
	}
	return 0;
}

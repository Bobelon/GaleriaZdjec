#include <stdio.h>
#include <dirent.h>
#include <opencv2/opencv.hpp>

#include <opencv2/highgui/highgui.hpp>
#include <list>
#include <vector>

using namespace cv;
using namespace std;

list <string> listOfImages;
list<string>::iterator iter;


void files (const char * dirPath ) {
	
    struct dirent * file;
    DIR * path;
    
    
    char *rozszezenie = (char *) malloc (1024 * sizeof (char));
	char *pom = (char *) malloc (1024 * sizeof (char));
	
    
    if (path = opendir (dirPath)) {
        while (file = readdir (path)) {
			pom = strchr (file->d_name,'.');
			listOfImages.push_back (file->d_name);
			//strcpy (rozszezenie, pom);
			/*
			if (strcpy (rozszezenie, "jpg") || strcpy (rozszezenie, "png")) {
				cout << file->d_name << endl;
			}*/
		}
       
        closedir( path );
    }
    
    else {
		cout << "Nieprawidlowa sciezka" << endl;
	}
}

void showPicture (bool next) {
	
	
	//cout << "\n\n\nXXDXDXDDD\n\n\n\n";
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
	
	
	
	Mat imageCL;
	string name = "./CL/" + *iter;
	
	imageCL = imread (name, CV_LOAD_IMAGE_COLOR);
	
	if (!imageCL.data) {
		cout << "BŁĄD" << endl;
		cout << name << endl;
	}
	else {
	cout << "showPicture" << endl;
	imshow ("CL", imageCL);
}
}

int main( int argc, char** argv ) {
	
	double width;
	double height;
	double fps;
	int pozPocz = -1;
	clock_t aktualnyCzas = clock() / CLOCKS_PER_SEC;
			//( clock () - begin_time ) /  CLOCKS_PER_SEC;

	
	files ("./CL");
	
	namedWindow ("CL", CV_WINDOW_AUTOSIZE);
	
	
	iter = listOfImages.begin();
	
	//cout << *iter << endl;
	//cout << *++iter << endl;
	//wskaznik++;
	//for (it = listOfImages.begin(); it != listOfImages.end(); ++it) {
	//	cout << *it << endl;
	//}
	//cout << "Ilość znalezionych plików: " << listOfImages.front() << endl;
	//cout << "Ilość znalezionych plików: " << (listOfImages.front() + 1) << endl;
	
	//string imageName = *iter;
	
	//namedWindow (*iter, CV_WINDOW_AUTOSIZE);
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
		
		
		
		int key;
		Mat bgr, hsv, mirror, colorDetection, countour, frame;
		Mat frame2;
	// Główna pętla programu
	while(true) {
		
		
		if (cap.read(bgr)) {
			//blur( bgr, bgr, Size(10,10) );
			cvtColor( bgr, hsv, CV_BGR2HSV); // filtr zmieniający skalę barw
			flip(hsv, mirror, 1); // Odbicie lustrzane
			//
			//blur( mirror, mirror, Size(10,10) );
			//erode(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );
			//dilate(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );
			inRange (mirror, Scalar (60, 200, 130), Scalar (100, 255, 255), colorDetection);
			//inRange (frame, Scalar (80, 120, 63), Scalar (120, 240, 83), frame2);
			//inRange (frame, Scalar (0, 100, 100), Scalar (55, 255, 200), frame2);
			dilate(colorDetection, colorDetection, getStructuringElement(MORPH_ELLIPSE, Size(20, 20)) );
			erode(colorDetection, colorDetection, getStructuringElement(MORPH_ELLIPSE, Size(20, 20)) );
			dilate(colorDetection, colorDetection, getStructuringElement(MORPH_ELLIPSE, Size(20, 50)) );
			dilate(colorDetection, colorDetection, getStructuringElement(MORPH_ELLIPSE, Size(20, 50)) );
			
			int jeden, dwa, trzy, cztery;
				jeden = 15;
				dwa = 13;
				trzy = 20;
				cztery = 3;
			Canny( colorDetection, colorDetection, jeden, dwa * trzy, cztery ); // Tworzenie konturu
			vector <vector <Point> > contours;
			findContours( colorDetection, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);	
			drawContours( colorDetection, contours, 0, Scalar(255,0,0),5,8);
			
			
			/// Momenty :)
		vector<Moments> mu(contours.size() );
		for( int i = 0; i < contours.size(); i++ ) {
			mu[i] = moments( contours[i], false ); 
		}
        
		
		if (mu.size() > 0) {
			Point2f mc; // mass center
			int maxI = 0;
			
			int x, y, przesuniecie;
						x = mu[maxI].m10/mu[maxI].m00;
						y = mu[maxI].m01/mu[maxI].m00;
						przesuniecie = 200;
					//cout << ((int)  aktualnyCzas / 1000000)<< "   " << clock() <<  endl;
					if (pozPocz == -1) {
						pozPocz = x;
					}					
					if ((x < 250) && (((double) (clock () - aktualnyCzas) / CLOCKS_PER_SEC) >= 0.5)) {
						cout << "LEWO" << endl;
						//imshow (*iter, imread (*iter, CV_LOAD_IMAGE_COLOR));
						pozPocz = x;
						aktualnyCzas = clock();
						showPicture(false);
					}
					if ((x > 250) && (((double) (clock () - aktualnyCzas) / CLOCKS_PER_SEC) >= 0.5)) {
						cout << "PRAWO" << endl;
						pozPocz = x;
						aktualnyCzas = clock();
						
						showPicture(true);
					}
			
			
			mc = Point2f( x , y );
			for( int i = 0; i < contours.size(); i++ ) {
				if (contourArea(contours[maxI]) < contourArea(contours[i])) {
					maxI = i;
					mc = Point2f( mu[maxI].m10/mu[maxI].m00, mu[maxI].m01/mu[maxI].m00);
					
					
					
				} 
			}
			//circle( colorDetection, mc, sqrt(contourArea(contours[maxI])), Scalar(255,0,0) );
		}
		
			
			
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

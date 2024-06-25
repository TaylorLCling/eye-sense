#include <opencv2/opencv.hpp> //include opencv  for tools

using namespace std;
using namespace cv; // Declear cv namespace early


int main() {

    VideoCapture cam(0);  // Open the default camera
    if(!cam.isOpened()) {  // Check if we succeeded
        return -1; //return error otherwise
    }


    Mat frame; //mat is the cv array class we will call frame.
    while(true) {
        cam >> frame;  // Get a new frame from camera
    
        if (!frame.empty()) {
            imshow("EyeSense", frame); // Present frame
        }    
        
        if(waitKey(30) >= 0) break;  // Wait for a keystroke then close when done. 
    }
    return 0;
}

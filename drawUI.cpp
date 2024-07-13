#include <opencv2/opencv.hpp>

using namespace cv;

// Global variables
bool isContinuousMode = true;
bool isRunning = false;

void drawUI(Mat& frame) {
    // Draw main areas
    rectangle(frame, Rect(20, 60, 560, 420), Scalar(52, 73, 94), -1);
    rectangle(frame, Rect(600, 60, 180, 420), Scalar(52, 73, 94), -1);
    
    // Draw title bar
    rectangle(frame, Rect(0, 0, 800, 40), Scalar(52, 152, 219), -1);
    putText(frame, "EyeSense", Point(20, 25), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(236, 240, 241), 2);

    // Draw mode switch
    rectangle(frame, Rect(680, 5, 100, 30), Scalar(46, 204, 113), -1);
    circle(frame, Point(695, 20), 10, Scalar(236, 240, 241), -1);
    putText(frame, isContinuousMode ? "Continuous" : "Single-shot", Point(715, 25), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(236, 240, 241), 1);

    // Draw control panel and buttons
    rectangle(frame, Rect(20, 500, 760, 80), Scalar(52, 73, 94), -1);
    rectangle(frame, Rect(40, 520, 100, 40), Scalar(231, 76, 60), -1);
    putText(frame, "Start/Stop", Point(50, 545), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(236, 240, 241), 1);
    // ... (draw other buttons similarly)
}

void mouseCallback(int event, int x, int y, int flags, void* userdata) {
    if (event == EVENT_LBUTTONDOWN) {
        // Handle button clicks and mode switch
        if (x >= 680 && x <= 780 && y >= 5 && y <= 35) {
            isContinuousMode = !isContinuousMode;
        }
        // ... (handle other button clicks)
    }
}

int main() {
    Mat frame(600, 800, CV_8UC3, Scalar(44, 62, 80));
    namedWindow("EyeSense", WINDOW_NORMAL);
    setMouseCallback("EyeSense", mouseCallback, NULL);

    while (true) {
        drawUI(frame);
        imshow("EyeSense", frame);
        if (waitKey(30) >= 0) break;
    }

    return 0;
}

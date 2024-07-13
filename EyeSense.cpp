#include <opencv2/opencv.hpp>
#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/kernels/register.h>
#include <tensorflow/lite/model.h>
#include <tensorflow/lite/optional_debug_tools.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace cv; // Declear cv namespace early


// Global variables for TFLite
unique_ptr<tflite::FlatBufferModel> model;
unique_ptr<tflite::Interpreter> interpreter;


// Function to initialize TensorFlow Lite with Moondream model
bool initializeTFLite(const string& model_path) {
    model = tflite::FlatBufferModel::BuildFromFile(model_path.c_str());
    if (!model) {
        cerr << "Failed to load model" << endl;
        return false;
    }

    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder builder(*model, resolver);
    builder(&interpreter);

    if (!interpreter) {
        cerr << "Failed to construct interpreter" << endl;
        return false;
    }

    if (interpreter->AllocateTensors() != kTfLiteOk) {
        cerr << "Failed to allocate tensors" << endl;
        return false;
    }

    return true;
}



// Function to run the Moondream model
string runLLM(const Mat& image) {
    // Ensure the image is in the correct format (e.g., RGB float32)
    Mat processed;
    image.convertTo(processed, CV_32F, 1.0/255.0);
    cvtColor(processed, processed, COLOR_BGR2RGB);

    // Get input tensor
    int input = interpreter->inputs()[0];
    TfLiteTensor* input_tensor = interpreter->tensor(input);

    // Copy image data to input tensor
    memcpy(input_tensor->data.f, processed.data, processed.total() * processed.elemSize());

    // Run inference
    if (interpreter->Invoke() != kTfLiteOk) {
        return "Error running model";
    }

    // Get output tensor
    int output = interpreter->outputs()[0];
    TfLiteTensor* output_tensor = interpreter->tensor(output);

    // Process the output (this will depend on Moondream's exact output format)
    // For this example, we'll assume it outputs a string directly
    string result(reinterpret_cast<char*>(output_tensor->data.raw));

    return result;
}


int main() {

    // Initialize TensorFlow Lite with Moondream model
    if (!initializeTFLite("path/to/moondream_model.tflite")) {
        return -1;
    }



    VideoCapture cam(0);  // Open the default camera
    if(!cam.isOpened()) {  // Check if we succeeded
        return -1; //return error otherwise
    }
    
    Mat frame; //mat is the cv array class we will call frame.
    while(true) {
        cam >> frame;  // Get a new frame from camera
        if (frame.empty()) {
            cerr << "Error capturing frame" << endl;
            continue;
        }

        // Resize frame to match Moondream's input size (adjust as needed)
        Mat resizedFrame;
        resize(frame, resizedFrame, Size(224, 224));

        // Run LLM on processed frame
        string llmOutput = runLLM(resizedFrame);

        // Display original frame
        imshow("EyeSense", frame); // Present frame

        // Display LLM output
        cout << "LLM Output: " << llmOutput << endl;

        if(waitKey(30) >= 0) break; // Wait for a keystroke then close when done. 
    }

    return 0;
}

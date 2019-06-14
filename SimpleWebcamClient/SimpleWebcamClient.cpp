
#include <RobotRaconteur.h>
#include "robotraconteur_generated.h"

#include <opencv2/highgui/highgui.hpp>

using namespace RobotRaconteur;
using namespace std;
using namespace boost;
using namespace cv;
using namespace ::experimental::createwebcam2;

//Simple client to read images from a Webcam server
//and display the images

//Convert WebcamImage to OpenCV format
Mat WebcamImageToMat(WebcamImagePtr image)
{
	Mat frame2(image->height, image->width, CV_8UC3);
	memcpy(frame2.data,&image->data->at(0),image->data->size());
	return frame2;
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cout << "URL expected as argument" << endl;
		return -1;
	}

	try
	{		
		//Use node setup to help initialize client node
		ClientNodeSetup node_setup(ROBOTRACONTEUR_SERVICE_TYPES);

		WebcamHostPtr c_host=rr_cast<WebcamHost>(RobotRaconteurNode::s()->ConnectService(string(argv[1]),"",RRMapPtr<std::string,RRValue>(),NULL,"experimental.createwebcam2.WebcamHost"));

		//Get the Webcam objects from the "Webcams" objref
		WebcamPtr c1=c_host->get_Webcams(0);
		WebcamPtr c2=c_host->get_Webcams(1);

		//Capture an image and convert to OpenCV image type
		Mat frame1=WebcamImageToMat(c1->CaptureFrame());
		Mat frame2=WebcamImageToMat(c2->CaptureFrame());

		//Show image
		imshow("Left",frame1);
		imshow("Right",frame2);

		//Wait for enter key to be pressed
		waitKey();
		
		//Close the image viewers
		destroyAllWindows();
		
		return 0;
	}
	catch (std::exception& e)
	{
		cout << "Error occured in client: " << string(e.what()) << endl;
		return -1;
	}
}


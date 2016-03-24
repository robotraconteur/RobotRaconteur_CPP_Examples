
#include <RobotRaconteur.h>
#include "experimental__createwebcam.h"
#include "experimental__createwebcam_stubskel.h"

#include <opencv2/highgui/highgui.hpp>

using namespace RobotRaconteur;
using namespace std;
using namespace boost;
using namespace cv;
using namespace experimental::createwebcam;

//Simple client to read images from a Webcam server
//and display the images

//Convert WebcamImage to OpenCV format
Mat WebcamImageToMat(boost::shared_ptr<WebcamImage> image)
{
	Mat frame2(image->height, image->width, CV_8UC3);
	memcpy(frame2.data,image->data->ptr(),image->data->Length());
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
		//Register a TCP transport
		boost::shared_ptr<TcpTransport> t=boost::make_shared<TcpTransport>();
		RobotRaconteurNode::s()->RegisterTransport(t);
		 //Register the service type
		RobotRaconteurNode::s()->RegisterServiceType(boost::make_shared<experimental__createwebcamFactory>());

		//Connect to the service
		boost::shared_ptr<WebcamHost> c_host=rr_cast<WebcamHost>(RobotRaconteurNode::s()->ConnectService(string(argv[1]),"",boost::shared_ptr<RRMap<std::string,RRObject> >(),NULL,"experimental.createwebcam.WebcamHost"));

		//Get the Webcam objects from the "Webcams" objref
		boost::shared_ptr<Webcam> c1=c_host->get_Webcams(0);
		boost::shared_ptr<Webcam> c2=c_host->get_Webcams(1);

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

		//Shutdown Robot Raconteur
		RobotRaconteurNode::s()->Shutdown();

		return 0;

	}
	catch (std::exception& e)
	{
		cout << "Error occured in client: " << string(e.what()) << endl;
		return -1;
	}
}


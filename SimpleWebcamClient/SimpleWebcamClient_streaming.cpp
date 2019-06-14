
#include <RobotRaconteur.h>
#include "robotraconteur_generated.h"

#include <opencv2/highgui/highgui.hpp>

using namespace RobotRaconteur;
using namespace std;
using namespace boost;
using namespace cv;
using namespace ::experimental::createwebcam2;

//Simple client to read streaming images from the Webcam pipe to show
//a live view from the cameras

//Convert WebcamImage to OpenCV format
Mat WebcamImageToMat(WebcamImagePtr image)
{
	Mat frame2(image->height, image->width, CV_8UC3);
	memcpy(frame2.data, &image->data->at(0), image->data->size());
	return frame2;
}

Mat current_frame;

//Function to handle when a new frame is received
//This function will be called by a separate thread by
//Robot Raconteur.
void new_frame(PipeEndpointPtr<WebcamImagePtr> pipe_ep)
{
	while (pipe_ep->Available() > 0)
	{
		WebcamImagePtr image=pipe_ep->ReceivePacket();
		if (image->data)
		{
			current_frame = WebcamImageToMat(image);
		}

	}
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

		//Connect to the service
		WebcamHostPtr c_host=rr_cast<WebcamHost>(RobotRaconteurNode::s()->ConnectService(string(argv[1]),"",RRMapPtr<std::string,RRValue>(),NULL,"experimental.createwebcam2.WebcamHost"));

		//Get the webcam object
		WebcamPtr c1=c_host->get_Webcams(0);

		//Connect to the FrameStream pipe and receive a PipeEndpoint
        //PipeEndpoints a symmetric on client and service meaning that
        //you can send and receive on both ends
		PipeEndpointPtr<WebcamImagePtr> p=c1->get_FrameStream()->Connect(-1);
		//Add a callback for when a new pipe packet is received
		p->PacketReceivedEvent.connect([](PipeEndpointPtr<WebcamImagePtr> ep) { new_frame(ep); });

		//Show a named window
		namedWindow("Image");

		try
		{
			//Start streaming image packets
			c1->StartStreaming();
		}
		catch (std::exception&)
		{

		}

		//Loop through and show the new image if available
		while (true)
		{
			if (!current_frame.empty())
			{
				imshow("Image",current_frame);
			}
			//Break the loop if "enter" is pressed on a window
			if (waitKey(50)!=-1)
				break;

		}
		
		//Stop streaming images
		c1->StopStreaming();

		//Close the window
		destroyAllWindows();

		//Close the PipeEndpoint
		p->Close();
		
		return 0;

	}
	catch (std::exception& e)
	{
		cout << "Error occured in client: " << string(e.what()) << endl;
		return -1;
	}
}




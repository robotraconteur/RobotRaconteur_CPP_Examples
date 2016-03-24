
#include <RobotRaconteur.h>
#include "experimental__createwebcam.h"
#include "experimental__createwebcam_stubskel.h"

#include <opencv2/highgui/highgui.hpp>

using namespace RobotRaconteur;
using namespace std;
using namespace boost;
using namespace cv;
using namespace experimental::createwebcam;

//Simple client to read streaming images from the Webcam pipe to show
//a live view from the cameras

//Convert WebcamImage to OpenCV format
Mat WebcamImageToMat(boost::shared_ptr<WebcamImage> image)
{
	Mat frame2(image->height, image->width, CV_8UC3);
	memcpy(frame2.data,image->data->ptr(),image->data->Length());
	return frame2;
}

Mat current_frame;

//Function to handle when a new frame is received
//This function will be called by a separate thread by
//Robot Raconteur.
void new_frame(boost::shared_ptr<PipeEndpoint<boost::shared_ptr<WebcamImage> > > pipe_ep)
{
	while (pipe_ep->Available() > 0)
	{
		boost::shared_ptr<WebcamImage> image=pipe_ep->ReceivePacket();
		current_frame=WebcamImageToMat(image);

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
		//Register the transport
		boost::shared_ptr<TcpTransport> t=boost::make_shared<TcpTransport>();
		RobotRaconteurNode::s()->RegisterTransport(t);

		//Register the service type
		RobotRaconteurNode::s()->RegisterServiceType(boost::make_shared<experimental__createwebcamFactory>());

		//Connect to the service
		boost::shared_ptr<WebcamHost> c_host=rr_cast<WebcamHost>(RobotRaconteurNode::s()->ConnectService(string(argv[1]),"",boost::shared_ptr<RRMap<std::string,RRObject> >(),NULL,"experimental.createwebcam.WebcamHost"));

		//Get the webcam object
		boost::shared_ptr<Webcam> c1=c_host->get_Webcams(0);

		//Connect to the FrameStream pipe and receive a PipeEndpoint
        //PipeEndpoints a symmetric on client and service meaning that
        //you can send and receive on both ends
		boost::shared_ptr<PipeEndpoint<boost::shared_ptr<WebcamImage> > > p=c1->get_FrameStream()->Connect(-1);
		//Add a callback for when a new pipe packet is received
		p->PacketReceivedEvent.connect(boost::bind(new_frame,_1));

		//Show a named window
		namedWindow("Image");

		//Start streaming image packets
		c1->StartStreaming();

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





#include <RobotRaconteur.h>
#include "robotraconteur_generated.h"

#include <opencv2/highgui/highgui.hpp>

using namespace RobotRaconteur;
using namespace std;
using namespace boost;
using namespace cv;
using namespace ::experimental::createwebcam2;

//Simple client to read images from a Webcam server
//and display the image.  This example uses the "memory"
//member type

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

		//Get the webcam
		WebcamPtr c1=c_host->get_Webcams(0);

		//Capture an image to the "buffer" and "multidimbuffer"
		WebcamImage_sizePtr size=c1->CaptureFrameToBuffer();

		//Read the full image from the "buffer" memory
		uint64_t l=c1->get_buffer()->Length();
		RRArrayPtr<uint8_t> data=AllocateRRArray<uint8_t>(l);
		c1->get_buffer()->Read(0,data,0,l);

		//Convert and show the image retrieved from the buffer memory
		Mat frame1(size->height,size->width,CV_8UC3);
		memcpy(frame1.data,&data->at(0),data->size());
		imshow("buffer",frame1);

		//Read the dimensions of the "multidimbuffer" member
		vector<uint64_t> bufsize=c1->get_multidimbuffer()->Dimensions();

		//Retrieve the data from the "multidimbuffer"		
		RRMultiDimArrayPtr<uint8_t> segdata = AllocateEmptyRRMultiDimArray<uint8_t>({ 100,100,1 });

		c1->get_multidimbuffer()->Read({ 10,10,0 }, segdata, { 0,0,0 }, { 100,100,1 });

		//Create a new image and copy data from the WebcamImage
		Mat frame2(100,100,CV_8U);
		memcpy(frame2.data,&segdata->Array->at(0), segdata->Array->size());

		//Take the transpose because of array ordering difference
		Mat frame3=frame2.t();

		//Show the Image
		imshow("multidimbuffer",frame3);
		
		//Wait for user and then close the window
		waitKey();
		destroyAllWindows();
		
		return 0;
	}
	catch (std::exception& e)
	{
		cout << "Error occured in client: " << string(e.what()) << endl;
		return -1;
	}
}


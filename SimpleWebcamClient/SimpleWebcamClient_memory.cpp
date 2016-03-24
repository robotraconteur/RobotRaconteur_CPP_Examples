
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
		//Register the transport
		boost::shared_ptr<TcpTransport> t=boost::make_shared<TcpTransport>();
		RobotRaconteurNode::s()->RegisterTransport(t);
		//Register the service type
		RobotRaconteurNode::s()->RegisterServiceType(boost::make_shared<experimental__createwebcamFactory>());

		//Connect to the service
		boost::shared_ptr<WebcamHost> c_host=rr_cast<WebcamHost>(RobotRaconteurNode::s()->ConnectService(string(argv[1]),"",boost::shared_ptr<RRMap<std::string,RRObject> >(),NULL,"experimental.createwebcam.WebcamHost"));

		//Get the webcam
		boost::shared_ptr<Webcam> c1=c_host->get_Webcams(0);

		//Capture an image to the "buffer" and "multidimbuffer"
		boost::shared_ptr<WebcamImage_size> size=c1->CaptureFrameToBuffer();

		//Read the full image from the "buffer" memory
		uint64_t l=c1->get_buffer()->Length();
		boost::shared_ptr<RRArray<uint8_t> > data=AllocateRRArray<uint8_t>(l);
		c1->get_buffer()->Read(0,data,0,l);

		//Convert and show the image retrieved from the buffer memory
		Mat frame1(size->height,size->width,CV_8UC3);
		memcpy(frame1.data,data->ptr(),data->Length());
		imshow("buffer",frame1);

		//Read the dimensions of the "multidimbuffer" member
		vector<uint64_t> bufsize=c1->get_multidimbuffer()->Dimensions();

		//Retrieve the data from the "multidimbuffer"
		boost::shared_ptr<RRArray<uint8_t> > segdata_bytes=AllocateRRArray<uint8_t>(10000);
		boost::shared_ptr<RRArray<int32_t> > dims=AllocateRRArray<int32_t>(3);
		(*dims)[0]=100;
		(*dims)[1]=100;
		(*dims)[2]=1;
		boost::shared_ptr<RRMultiDimArray<uint8_t> > segdata=boost::make_shared<RRMultiDimArray<uint8_t> >(dims,segdata_bytes);

		vector<uint64_t> memorypos; memorypos.push_back(10); memorypos.push_back(10); memorypos.push_back(0);
		vector<uint64_t> bufferpos; bufferpos.push_back(0); bufferpos.push_back(0); bufferpos.push_back(0);
		vector<uint64_t> count; count.push_back(100); count.push_back(100); count.push_back(1);

		c1->get_multidimbuffer()->Read(memorypos,segdata,bufferpos,count);

		//Create a new image and copy data from the WebcamImage
		Mat frame2(100,100,CV_8U);
		memcpy(frame2.data,segdata_bytes->ptr(), segdata_bytes->Length());

		//Take the transpose because of array ordering difference
		Mat frame3=frame2.t();

		//Show the Image
		imshow("multidimbuffer",frame3);
		
		//Wait for user and then close the window
		waitKey();
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


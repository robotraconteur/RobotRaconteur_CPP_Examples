
#include "SimpleWebcamService.h"

//This program provides a simple Robot Raconteur server for viewing
//multiple webcams


int main(int argc, char* argv[])
{

	//Initialize the host by giving it a vector of names
	vector<Webcam_name> names;
	Webcam_name n1 ={0, "Left"};
	names.push_back(n1) ;
	Webcam_name n2 ={1, "Right"};
	names.push_back(n2) ;

	RR_SHARED_PTR<WebcamHost_impl> webcamhost=RR_MAKE_SHARED<WebcamHost_impl>(names);
	
	//Use node setup to help initialize server node
	ServerNodeSetup node_setup(ROBOTRACONTEUR_SERVICE_TYPES, "experimental.createwebcam2", 2355);
	
	//Register the service
	RobotRaconteurNode::s()->RegisterService("Webcam","experimental.createwebcam2",webcamhost);
	
	//Wait for the user to shutdown the service
	cout << "Webcam server started. Press enter to quit" << endl;
	getchar();
	
	//Shutdown the webcams
	webcamhost->Shutdown();

	return 0;

    
}

//Initialize the webcam and buffer objects
Webcam_impl::Webcam_impl(int cam, string name)
{
	capture=RR_MAKE_SHARED<VideoCapture>( cam );
	if (!capture) throw std::runtime_error("Could not open camera");	
	camopen=true;
		
	capture->set(CV_CAP_PROP_FRAME_WIDTH,320);
	capture->set(CV_CAP_PROP_FRAME_HEIGHT,240);

	m_buffer=AllocateRRArray<uint8_t>(0);
	m_multidimbuffer = AllocateEmptyRRMultiDimArray<uint8_t>({ 0 });
	m_Name=name;
	streaming_timer.reset();
}

//Shutdown the webcams
void Webcam_impl::Shutdown()
{
	if (camopen)
	{		
		camopen=false;
	}
	streaming_timer.reset();
}
Webcam_impl::~Webcam_impl()
{
	Shutdown();
}

//Read the Webcam Name property 
std::string Webcam_impl::get_Name()
{
	return m_Name;
}

//Capture a frame and return a WebcamImage structure
WebcamImagePtr Webcam_impl::CaptureFrame()
{
	boost::recursive_mutex::scoped_lock lock(global_lock);

	WebcamImagePtr image(new WebcamImage());
	Mat mat;
	capture->read(mat);
	image->width = mat.cols;
	image->height = mat.rows;
	image->step = mat.step;
	image->data=AttachRRArrayCopy((uint8_t*)mat.data, mat.step * mat.rows);
	return image;
}

//Start streaming webcam images to provide a live view
void Webcam_impl::StartStreaming()
{
	boost::recursive_mutex::scoped_lock lock(global_lock);
	if (streaming_timer) throw InvalidOperationException("Already streaming");
	RR_WEAK_PTR<Webcam_impl> weak_this = shared_from_this();
	streaming_timer = RobotRaconteurNode::s()->CreateTimer(boost::posix_time::milliseconds(100), 
		[weak_this](TimerEvent ev)
	{
		auto shared_this = weak_this.lock();
		if (!shared_this) return;
		shared_this->send_frame_stream();
	}	
	);
	streaming_timer->Start();	
}

//Stop streaming webcam images
void Webcam_impl::StopStreaming()
{
	boost::recursive_mutex::scoped_lock lock(global_lock);
	if (!streaming_timer) throw InvalidOperationException("Not streaming");
	streaming_timer->Stop();
	streaming_timer.reset();
}

void Webcam_impl::send_frame_stream()
{
	auto frame = CaptureFrame();
	this->rrvar_FrameStream->SendPacket(frame);
}

//Capture a frame and store in the "buffer" and "multidimbuffer" memories
WebcamImage_sizePtr Webcam_impl::CaptureFrameToBuffer()
{

	WebcamImagePtr image=CaptureFrame();

	m_buffer=image->data;
	
	//Rearrange the data into the correct format for MATLAB arrays
	RRMultiDimArrayPtr<uint8_t> mdbuf=AllocateEmptyRRMultiDimArray<uint8_t>({ (uint32_t)image->height, (uint32_t)image->width, 3 });
	auto& mdata = *mdbuf->Array;
	auto& image_data = *image->data;

	for (int channel=0; channel < 3; channel++)
    {
        int channel0 = image->height * image->width * channel;
        for (int x = 0; x < image->width; x++)
        {                        
            for (int y = 0; y < image->height; y++)
            {
                uint8_t value = image_data[(y * image->step + x*3)  + (2-channel)];
                mdata[channel0 + x * image->height + y]=value;
            }
        }
    }
    m_multidimbuffer=mdbuf;

	//Return a WebcamImage_size structure to the client
	WebcamImage_sizePtr size(new WebcamImage_size());
	size->width=image->width;
	size->height=image->height;
	size->step=image->step;
	return size;
}

//Return an ArrayMemory for the "buffer" data containing the image.
ArrayMemoryPtr<uint8_t > Webcam_impl::get_buffer()
{
	//In many cases this ArrayMemory would not be initialized every time,
    //but for this example return a new ArrayMemory
	return RR_MAKE_SHARED<ArrayMemory<uint8_t > >(m_buffer);
}

//Return a MultiDimArray for the "multidimbuffer" data containing the image
MultiDimArrayMemoryPtr<uint8_t > Webcam_impl::get_multidimbuffer()
{ 
	//In many cases this MultiDimArrayMemory would not be initialized every time,
    //but for this example return a new MultiDimArrayMemory
	return RR_MAKE_SHARED<MultiDimArrayMemory<uint8_t > >(m_multidimbuffer);
}

//Override set_FrameStream to set the maximum backlog
void Webcam_impl::set_FrameStream(PipePtr<WebcamImagePtr> value)
{
	Webcam_default_impl::set_FrameStream(value);
	this->rrvar_FrameStream->SetMaximumBacklog(3);
}

//Initialize the webcams
WebcamHost_impl::WebcamHost_impl(vector<Webcam_name> names)
{
	for(auto e : names)
	{
		cameras.push_back(boost::make_shared<Webcam_impl>(e.index,e.name));
	}
}

WebcamHost_impl::~WebcamHost_impl()
{
	Shutdown();
}

//Shutdown all webcams
void WebcamHost_impl::Shutdown()
{
	boost::recursive_mutex::scoped_lock lock(global_lock);

	for(auto& e : cameras)
	{
		e->Shutdown();
	}
}

//Return the indices and names of the available webcams
RRMapPtr<int32_t,RRArray<char> > WebcamHost_impl::get_WebcamNames()
{
	auto o = AllocateEmptyRRMap<int32_t,RRArray<char> >();

	for(size_t i=0; i<cameras.size(); i++)
	{
		auto rr_camname=stringToRRArray(cameras.at(i)->get_Name());
		o->insert(make_pair(i,rr_camname));
	}

	return o;
}

//Function to implement the "Webcams" objref.  Return the
//object for the selected webcam
RR_SHARED_PTR<Webcam > WebcamHost_impl::get_Webcams(int32_t ind)
{
	boost::recursive_mutex::scoped_lock lock(global_lock);

	if (!(ind < cameras.size())) throw InvalidOperationException("Invalid camera");

	return cameras.at(ind);
}

boost::recursive_mutex global_lock;
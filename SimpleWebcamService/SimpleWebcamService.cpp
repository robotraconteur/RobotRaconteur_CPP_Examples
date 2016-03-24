
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

	boost::shared_ptr<WebcamHost_impl> webcamhost=boost::make_shared<WebcamHost_impl>(names);
	
	//Initialize the LocalTransport
	boost::shared_ptr<LocalTransport> t1=boost::make_shared<LocalTransport>();
	t1->StartServerAsNodeName("experimental.createwebcam.WebcamHost");
	RobotRaconteurNode::s()->RegisterTransport(t1);

	//Initialize the transport
	boost::shared_ptr<TcpTransport> t=boost::make_shared<TcpTransport>();
	t->StartServer(2355);

	//Attempt to load a TLS certificate
	try
	{
		t->LoadTlsNodeCertificate();
	}
	catch (std::exception&)
	{
		cout << "warning: could not load TLS certificate" << endl;
	}

	t->EnableNodeAnnounce();
	RobotRaconteurNode::s()->RegisterTransport(t);
	//Register the Webcam_interface service type
	RobotRaconteurNode::s()->RegisterServiceType(boost::make_shared<experimental__createwebcamFactory>());

	//Register the service
	RobotRaconteurNode::s()->RegisterService("Webcam","experimental.createwebcam",webcamhost);
	
	//Wait for the user to shutdown the service
	cout << "Webcam server started. Press enter to quit" << endl;
	getchar();
	
	//Shutdown the webcams
	webcamhost->Shutdown();

	//Shutdown Robot Raconteur
	RobotRaconteurNode::s()->Shutdown();
	
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
	boost::shared_ptr<RRArray<int32_t> > dims=AllocateRRArray<int32_t>(1);
	(*dims)[0]=0;
	m_multidimbuffer=boost::make_shared<RRMultiDimArray<uint8_t> >(dims,AllocateRRArray<uint8_t>(0));
	m_Name=name;
	streaming=false;
}

//Shutdown the webcams
void Webcam_impl::Shutdown()
{
	if (camopen)
	{		
		camopen=false;
	}
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
//Set the Webcam Name property
void Webcam_impl::set_Name(std::string value)
{
	throw std::runtime_error("Read only property");
}

//Capture a frame and return a WebcamImage structure
RR_SHARED_PTR<WebcamImage > Webcam_impl::CaptureFrame()
{
	recursive_mutex::scoped_lock lock(global_lock);

	boost::shared_ptr<WebcamImage> image=boost::make_shared<WebcamImage>();
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
	if (streaming) throw std::runtime_error("Already streaming");
	streaming=true;
	//Start a thread to capture and send frames
	thread(boost::bind(&Webcam_impl::frame_threadfunc,shared_from_this()));
}
//Stop streaming webcam images
void Webcam_impl::StopStreaming()
{
	if (!streaming) throw std::runtime_error("Not streaming");
	streaming=false;
}

//Capture a frame and store in the "buffer" and "multidimbuffer" memories
RR_SHARED_PTR<WebcamImage_size > Webcam_impl::CaptureFrameToBuffer()
{

	boost::shared_ptr<WebcamImage> image=CaptureFrame();

	m_buffer=image->data;

	boost::shared_ptr<RRArray<uint8_t> > mdata=AllocateRRArray<uint8_t>(image->height*image->width*3);

	boost::shared_ptr<RRArray<int32_t> > dims=AllocateRRArray<int32_t>(3);
	(*dims)[0]=image->height;
	(*dims)[1]=image->width;
	(*dims)[2]=3;

	//Rearrange the data into the correct format for MATLAB arrays
	boost::shared_ptr<RRMultiDimArray<uint8_t> > mdbuf=boost::make_shared<RRMultiDimArray<uint8_t> >(dims,mdata);

	for (int channel=0; channel < 3; channel++)
    {
        int channel0 = image->height * image->width * channel;
        for (int x = 0; x < image->width; x++)
        {                        
            for (int y = 0; y < image->height; y++)
            {
                uint8_t value = (*image->data)[(y * image->step + x*3)  + (2-channel)];
                (*mdata)[channel0 + x * image->height + y]=value;
            }
        }
    }
    m_multidimbuffer=mdbuf;

	//Return a WebcamImage_size structure to the client
	boost::shared_ptr<WebcamImage_size> size=boost::make_shared<WebcamImage_size>();
	size->width=image->width;
	size->height=image->height;
	size->step=image->step;
	return size;
}

//Get the WebcamImage Pipe server object
RR_SHARED_PTR<RobotRaconteur::Pipe<RR_SHARED_PTR<WebcamImage > > > Webcam_impl::get_FrameStream()
{
	return m_FrameStream;
}
//Set the WebcamImage Pipe server object
void Webcam_impl::set_FrameStream(RR_SHARED_PTR<RobotRaconteur::Pipe<RR_SHARED_PTR<WebcamImage > > > value)
{
	m_FrameStream=value;
	//Set the callback function for the Pipe to receive connecting PipeEndpoints
	m_FrameStream->SetPipeConnectCallback(boost::bind(&Webcam_impl::FrameStream_pipeconnect,shared_from_this(), _1));
}

//Handles a PipeEndpoint on connection by client
void Webcam_impl::FrameStream_pipeconnect(boost::shared_ptr<PipeEndpoint<boost::shared_ptr<WebcamImage> > > pipe_ep)
{
	recursive_mutex::scoped_lock lock(global_lock);

	//Store the connected PipeEndpoint in nested maps by endpoint and index
	if (framestream_endpoints.count(pipe_ep->GetEndpoint())==0)
	{
		framestream_endpoints.insert(make_pair(pipe_ep->GetEndpoint(),map<int32_t,boost::shared_ptr<PipeEndpoint<boost::shared_ptr<WebcamImage> > > >()));
	}
	map<int32_t,boost::shared_ptr<PipeEndpoint<boost::shared_ptr<WebcamImage> > > >& dict1=framestream_endpoints.at(pipe_ep->GetEndpoint());
	dict1.insert(make_pair(pipe_ep->GetIndex(), pipe_ep));

	pipe_ep->SetPipeEndpointClosedCallback(boost::bind(&Webcam_impl::FrameStream_pipeclosed,shared_from_this(), _1));
}

//Handle a disconnecting PipeEndpoint, remove from the maps
void Webcam_impl::FrameStream_pipeclosed(boost::shared_ptr<PipeEndpoint<boost::shared_ptr<WebcamImage> > > pipe_ep)
{

	recursive_mutex::scoped_lock lock(global_lock);
	try
	{
		map<int32_t,boost::shared_ptr<PipeEndpoint<boost::shared_ptr<WebcamImage> > > >& dict1=framestream_endpoints.at(pipe_ep->GetEndpoint());
		dict1.erase(pipe_ep->GetIndex());
	}
	catch (...) {}
}

//Thread to stream frames by capturing data and sending it to
//all connected PipeEndpoints
void Webcam_impl::frame_threadfunc()
{
	while(streaming)
	{
		boost::shared_ptr<WebcamImage> frame=CaptureFrame();

		//Capture a frame
		recursive_mutex::scoped_lock lock(global_lock);

		
		vector<uint32_t> endpoints;
		for (map<uint32_t,map<int32_t,boost::shared_ptr<PipeEndpoint<boost::shared_ptr<WebcamImage> > > > >::iterator e=framestream_endpoints.begin(); e!=framestream_endpoints.end(); ++e)
		{
			endpoints.push_back(e->first);
		}

		//Loop through all connected PipeEndpoints and send data
		for (vector<uint32_t>::iterator e=endpoints.begin(); e!=endpoints.end(); ++e)
		{
			if (framestream_endpoints.count(*e)==0) continue;

			map<int32_t,boost::shared_ptr<PipeEndpoint<boost::shared_ptr<WebcamImage> > > >& dict1=framestream_endpoints.at(*e);

			vector<int32_t> indices;
			for(map<int32_t,boost::shared_ptr<PipeEndpoint<boost::shared_ptr<WebcamImage> > > >::iterator ee=dict1.begin(); ee!=dict1.end(); ++ee)
			{
				indices.push_back(ee->first);
			}

			for(vector<int32_t>::iterator ee=indices.begin(); ee!=indices.end(); ++ee)
			{
				if (dict1.count(*ee)==0) continue;

				uint32_t endpoint=*e;
				int32_t index=*ee;
				boost::shared_ptr<PipeEndpoint<boost::shared_ptr<WebcamImage> > > pipe_ep;
				try
				{
					pipe_ep=dict1.at(index);

					//Send the data to the connected PipeEndpoint here
					pipe_ep->SendPacket(frame);
				}
				catch (...)
				{
					//If there is an error sending the data to the pipe
					//assume the pipe has been closed.
					FrameStream_pipeclosed(pipe_ep);
				}

				

			}


		}

		this_thread::sleep(posix_time::milliseconds(100));

	}



}

//Return an ArrayMemory for the "buffer" data containing the image.
RR_SHARED_PTR<RobotRaconteur::ArrayMemory<uint8_t > > Webcam_impl::get_buffer()
{
	//In many cases this ArrayMemory would not be initialized every time,
    //but for this example return a new ArrayMemory
	return boost::make_shared<RobotRaconteur::ArrayMemory<uint8_t > >(m_buffer);
}

//Return a MultiDimArray for the "multidimbuffer" data containing the image
RR_SHARED_PTR<RobotRaconteur::MultiDimArrayMemory<uint8_t > > Webcam_impl::get_multidimbuffer()
{ 
	//In many cases this MultiDimArrayMemory would not be initialized every time,
    //but for this example return a new MultiDimArrayMemory
	return boost::make_shared<RobotRaconteur::MultiDimArrayMemory<uint8_t > >(m_multidimbuffer);
}

//Initialize the webcams
WebcamHost_impl::WebcamHost_impl(vector<Webcam_name> names)
{
	for(vector<Webcam_name>::iterator e=names.begin(); e!=names.end(); ++e)
	{
		cameras.push_back(boost::make_shared<Webcam_impl>(e->index,e->name));
	}
}

WebcamHost_impl::~WebcamHost_impl()
{
	Shutdown();
}

//Shutdown all webcams
void WebcamHost_impl::Shutdown()
{
	recursive_mutex::scoped_lock lock(global_lock);

	for(vector<boost::shared_ptr<Webcam_impl> >::iterator e=cameras.begin(); e!=cameras.end(); ++e)
	{
		(*e)->Shutdown();
	}
}

//Return the indices and names of the available webcams
RR_SHARED_PTR<RobotRaconteur::RRMap<int32_t,RobotRaconteur::RRArray<char>  > > WebcamHost_impl::get_WebcamNames()
{
	boost::shared_ptr<RobotRaconteur::RRMap<int32_t,RobotRaconteur::RRArray<char>  > > o=boost::make_shared<RobotRaconteur::RRMap<int32_t,RobotRaconteur::RRArray<char>  > >();

	for(size_t i=0; i<cameras.size(); i++)
	{
		boost::shared_ptr<RRArray<char> > rr_camname=stringToRRArray(cameras.at(i)->get_Name());
		o->map.insert(make_pair(i,rr_camname));
	}

	return o;


}

void WebcamHost_impl::set_WebcamNames(RR_SHARED_PTR<RobotRaconteur::RRMap<int32_t,RobotRaconteur::RRArray<char>  > > value)
{
	throw std::runtime_error("Read only property");
}

//Function to implement the "Webcams" objref.  Return the
//object for the selected webcam
RR_SHARED_PTR<Webcam > WebcamHost_impl::get_Webcams(int32_t ind)
{
	recursive_mutex::scoped_lock lock(global_lock);

	if (!(ind < cameras.size())) throw std::runtime_error("Invalid camera");

	return cameras.at(ind);
}



recursive_mutex global_lock;
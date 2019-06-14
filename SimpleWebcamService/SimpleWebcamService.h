#include <RobotRaconteur.h>
#include "robotraconteur_generated.h"

#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <stdio.h>
#include <boost/enable_shared_from_this.hpp>

#pragma once

using namespace std;
using namespace cv;
using namespace boost;
using namespace RobotRaconteur;
using namespace ::experimental::createwebcam2;

//Class to implement the "Webcam" object abstract class
//and also use "enable_shared_from_this" for shared_ptr support
class Webcam_impl : public Webcam_default_impl, public boost::enable_shared_from_this<Webcam_impl>
{
public:

	Webcam_impl(int cam, string name);
	void Shutdown();
	~Webcam_impl();

	virtual std::string get_Name();	

	virtual WebcamImagePtr CaptureFrame();

	virtual void StartStreaming();

	virtual void StopStreaming();

	virtual WebcamImage_sizePtr CaptureFrameToBuffer();
	
	virtual ArrayMemoryPtr<uint8_t> get_buffer();

	virtual MultiDimArrayMemoryPtr<uint8_t> get_multidimbuffer();

	virtual void set_FrameStream(PipePtr<WebcamImagePtr> value);

private:
	RRArrayPtr<uint8_t> m_buffer;
	RRMultiDimArrayPtr<uint8_t> m_multidimbuffer;
	RR_SHARED_PTR<VideoCapture> capture;
	
	bool camopen;
	string m_Name;	
	TimerPtr streaming_timer;

	void send_frame_stream();
};

class Webcam_name
{
public:
	int index;
	string name;
};

//Class that implemnets the "WebcamHost" object abstract class
class WebcamHost_impl : public WebcamHost
{
public:

	WebcamHost_impl(vector<Webcam_name> names);
	~WebcamHost_impl();
	void Shutdown();

	virtual RRMapPtr<int32_t,RRArray<char> > get_WebcamNames();

	virtual RR_SHARED_PTR<Webcam > get_Webcams(int32_t ind);
private:
	vector<boost::shared_ptr<Webcam_impl> > cameras;
	
};


//Global lock to protect from multi-threaded calls
extern boost::recursive_mutex global_lock;
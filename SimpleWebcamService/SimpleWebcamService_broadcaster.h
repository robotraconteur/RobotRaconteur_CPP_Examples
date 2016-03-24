#include <RobotRaconteur.h>
#include "experimental__createwebcam.h"
#include "experimental__createwebcam_stubskel.h"

#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <stdio.h>
#include <boost/enable_shared_from_this.hpp>

#pragma once

using namespace std;
using namespace cv;
using namespace boost;
using namespace RobotRaconteur;
using namespace experimental::createwebcam;

//Class to implement the "Webcam" object abstract class
//and also use "enable_shared_from_this" for shared_ptr support
class Webcam_impl : public Webcam, public boost::enable_shared_from_this<Webcam_impl>
{
public:

	Webcam_impl(int cam, string name);
	void Shutdown();
	~Webcam_impl();


	virtual std::string get_Name();
	virtual void set_Name(std::string value);

	virtual RR_SHARED_PTR<WebcamImage > CaptureFrame();

	virtual void StartStreaming();

	virtual void StopStreaming();

	virtual RR_SHARED_PTR<WebcamImage_size > CaptureFrameToBuffer();

	virtual RR_SHARED_PTR<RobotRaconteur::Pipe<RR_SHARED_PTR<WebcamImage > > > get_FrameStream();
	virtual void set_FrameStream(RR_SHARED_PTR<RobotRaconteur::Pipe<RR_SHARED_PTR<WebcamImage > > > value);

	virtual RR_SHARED_PTR<RobotRaconteur::ArrayMemory<uint8_t > > get_buffer();

	virtual RR_SHARED_PTR<RobotRaconteur::MultiDimArrayMemory<uint8_t > > get_multidimbuffer();

private:
	boost::shared_ptr<RRArray<uint8_t> > m_buffer;
	boost::shared_ptr<RRMultiDimArray<uint8_t> > m_multidimbuffer;
	RR_SHARED_PTR<VideoCapture> capture;

	RR_SHARED_PTR<RobotRaconteur::Pipe<RR_SHARED_PTR<WebcamImage > > > m_FrameStream;
	RR_SHARED_PTR<RobotRaconteur::PipeBroadcaster<RR_SHARED_PTR<WebcamImage> > > m_FrameStreamBroadcaster;
	bool camopen;
	string m_Name;
	
	
	bool streaming;
	void frame_threadfunc();


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

	virtual RR_SHARED_PTR<RobotRaconteur::RRMap<int32_t,RobotRaconteur::RRArray<char>  > > get_WebcamNames();
	virtual void set_WebcamNames(RR_SHARED_PTR<RobotRaconteur::RRMap<int32_t,RobotRaconteur::RRArray<char>  > > value);

	virtual RR_SHARED_PTR<Webcam > get_Webcams(int32_t ind);
private:
	vector<boost::shared_ptr<Webcam_impl> > cameras;
	
};


//Global lock to protect from multi-threaded calls
extern recursive_mutex global_lock;
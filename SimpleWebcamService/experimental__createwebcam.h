//This file is automatically generated. DO NOT EDIT!

#include <RobotRaconteur.h>
#include <boost/signals2.hpp>
#pragma once

namespace experimental
{
namespace createwebcam
{

class WebcamImage;
class WebcamImage_size;
class Webcam;
class WebcamHost;

class WebcamImage : public RobotRaconteur::RRStructure {
public:
int32_t width;
int32_t height;
int32_t step;
RR_SHARED_PTR<RobotRaconteur::RRArray<uint8_t > > data;

virtual std::string RRType() {return "experimental.createwebcam.WebcamImage";  }
};

class WebcamImage_size : public RobotRaconteur::RRStructure {
public:
int32_t width;
int32_t height;
int32_t step;

virtual std::string RRType() {return "experimental.createwebcam.WebcamImage_size";  }
};

class Webcam : public virtual RobotRaconteur::RRObject
{
public:
virtual std::string get_Name()=0;
virtual void set_Name(std::string value)=0;

virtual RR_SHARED_PTR<WebcamImage > CaptureFrame()=0;

virtual void StartStreaming()=0;

virtual void StopStreaming()=0;

virtual RR_SHARED_PTR<WebcamImage_size > CaptureFrameToBuffer()=0;

virtual RR_SHARED_PTR<RobotRaconteur::Pipe<RR_SHARED_PTR<WebcamImage > > > get_FrameStream()=0;
virtual void set_FrameStream(RR_SHARED_PTR<RobotRaconteur::Pipe<RR_SHARED_PTR<WebcamImage > > > value)=0;

virtual RR_SHARED_PTR<RobotRaconteur::ArrayMemory<uint8_t > > get_buffer()=0;

virtual RR_SHARED_PTR<RobotRaconteur::MultiDimArrayMemory<uint8_t > > get_multidimbuffer()=0;

virtual std::string RRType() {return "experimental.createwebcam.Webcam";  }
};

class WebcamHost : public virtual RobotRaconteur::RRObject
{
public:
virtual RR_SHARED_PTR<RobotRaconteur::RRMap<int32_t,RobotRaconteur::RRArray<char>  > > get_WebcamNames()=0;
virtual void set_WebcamNames(RR_SHARED_PTR<RobotRaconteur::RRMap<int32_t,RobotRaconteur::RRArray<char>  > > value)=0;

virtual RR_SHARED_PTR<Webcam > get_Webcams(int32_t ind)=0;

virtual std::string RRType() {return "experimental.createwebcam.WebcamHost";  }
};

}
}


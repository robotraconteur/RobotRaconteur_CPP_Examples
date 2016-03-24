//This file is automatically generated. DO NOT EDIT!

#include "experimental__createwebcam.h"
#pragma once

namespace experimental
{
namespace createwebcam
{

class experimental__createwebcamFactory : public virtual RobotRaconteur::ServiceFactory
{
public:
virtual std::string GetServiceName();
virtual std::string DefString();
virtual RR_SHARED_PTR<RobotRaconteur::StructureStub> FindStructureStub(std::string s);
virtual RR_SHARED_PTR<RobotRaconteur::MessageElementStructure> PackStructure(RR_SHARED_PTR<RobotRaconteur::RRStructure> structin);
virtual RR_SHARED_PTR<RobotRaconteur::RRObject> UnpackStructure(RR_SHARED_PTR<RobotRaconteur::MessageElementStructure> mstructin);
virtual RR_SHARED_PTR<RobotRaconteur::ServiceStub> CreateStub(std::string objecttype, std::string path, RR_SHARED_PTR<RobotRaconteur::ClientContext> context);
virtual RR_SHARED_PTR<RobotRaconteur::ServiceSkel> CreateSkel(std::string objecttype, std::string path, RR_SHARED_PTR<RobotRaconteur::RRObject> obj, RR_SHARED_PTR<RobotRaconteur::ServerContext> context);
virtual void DownCastAndThrowException(RobotRaconteur::RobotRaconteurException& exp);
virtual RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException> DownCastException(RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException> exp);
};

class WebcamImage_stub : public virtual RobotRaconteur::StructureStub
{
public:
WebcamImage_stub(RR_SHARED_PTR<RobotRaconteur::RobotRaconteurNode> node) : RobotRaconteur::StructureStub(node) {}
virtual RR_SHARED_PTR<RobotRaconteur::MessageElementStructure> PackStructure(RR_SHARED_PTR<RobotRaconteur::RRObject> s);
virtual RR_SHARED_PTR<RobotRaconteur::RRStructure> UnpackStructure(RR_SHARED_PTR<RobotRaconteur::MessageElementStructure> m);
};

class WebcamImage_size_stub : public virtual RobotRaconteur::StructureStub
{
public:
WebcamImage_size_stub(RR_SHARED_PTR<RobotRaconteur::RobotRaconteurNode> node) : RobotRaconteur::StructureStub(node) {}
virtual RR_SHARED_PTR<RobotRaconteur::MessageElementStructure> PackStructure(RR_SHARED_PTR<RobotRaconteur::RRObject> s);
virtual RR_SHARED_PTR<RobotRaconteur::RRStructure> UnpackStructure(RR_SHARED_PTR<RobotRaconteur::MessageElementStructure> m);
};

class async_Webcam
{
public:
virtual void async_get_Name(boost::function<void (std::string,RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > rr_handler, int32_t rr_timeout=RR_TIMEOUT_INFINITE) = 0;
virtual void async_set_Name(std::string value,boost::function<void (RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > rr_handler, int32_t rr_timeout=RR_TIMEOUT_INFINITE) = 0;

#ifdef ROBOTRACONTEUR_USE_ASIO_SPAWN
virtual std::string async_get_Name(boost::asio::yield_context rr_yield, int32_t rr_timeout=RR_TIMEOUT_INFINITE)
{
    return RobotRaconteur::detail::async_wrap_for_spawn<std::string >(boost::bind((void (async_Webcam::*)(boost::function<void (std::string, RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) >, int32_t))&async_Webcam::async_get_Name, this, _1, rr_timeout), rr_yield);
}
virtual void async_set_Name(std::string value, boost::asio::yield_context rr_yield, int32_t rr_timeout=RR_TIMEOUT_INFINITE)
{
    RobotRaconteur::detail::async_wrap_for_spawn_void(boost::bind((void (async_Webcam::*)(std::string, boost::function<void(RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>)>, int32_t))&async_Webcam::async_set_Name, this, boost::ref(value), _1, rr_timeout), rr_yield);
}
#endif

virtual void async_CaptureFrame(boost::function<void (RR_SHARED_PTR<WebcamImage >, RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > rr_handler, int32_t rr_timeout=RR_TIMEOUT_INFINITE) = 0;

#ifdef ROBOTRACONTEUR_USE_ASIO_SPAWN
RR_SHARED_PTR<WebcamImage > async_CaptureFrame(boost::asio::yield_context rr_yield, int32_t rr_timeout=RR_TIMEOUT_INFINITE)
{
    return RobotRaconteur::detail::async_wrap_for_spawn<RR_SHARED_PTR<WebcamImage >>(boost::bind((void (async_Webcam::*)(boost::function<void (RR_SHARED_PTR<WebcamImage >,RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>)>,int32_t))&async_Webcam::async_CaptureFrame, this, _1,rr_timeout), rr_yield);
}
#endif

virtual void async_StartStreaming(boost::function<void (RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > rr_handler, int32_t rr_timeout=RR_TIMEOUT_INFINITE) = 0;

#ifdef ROBOTRACONTEUR_USE_ASIO_SPAWN
virtual void async_StartStreaming(boost::asio::yield_context rr_yield, int32_t rr_timeout=RR_TIMEOUT_INFINITE)
{
    RobotRaconteur::detail::async_wrap_for_spawn_void(boost::bind((void (async_Webcam::*)(boost::function<void (RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>)>,int32_t))&async_Webcam::async_StartStreaming, this, _1,rr_timeout), rr_yield);
}
#endif

virtual void async_StopStreaming(boost::function<void (RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > rr_handler, int32_t rr_timeout=RR_TIMEOUT_INFINITE) = 0;

#ifdef ROBOTRACONTEUR_USE_ASIO_SPAWN
virtual void async_StopStreaming(boost::asio::yield_context rr_yield, int32_t rr_timeout=RR_TIMEOUT_INFINITE)
{
    RobotRaconteur::detail::async_wrap_for_spawn_void(boost::bind((void (async_Webcam::*)(boost::function<void (RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>)>,int32_t))&async_Webcam::async_StopStreaming, this, _1,rr_timeout), rr_yield);
}
#endif

virtual void async_CaptureFrameToBuffer(boost::function<void (RR_SHARED_PTR<WebcamImage_size >, RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > rr_handler, int32_t rr_timeout=RR_TIMEOUT_INFINITE) = 0;

#ifdef ROBOTRACONTEUR_USE_ASIO_SPAWN
RR_SHARED_PTR<WebcamImage_size > async_CaptureFrameToBuffer(boost::asio::yield_context rr_yield, int32_t rr_timeout=RR_TIMEOUT_INFINITE)
{
    return RobotRaconteur::detail::async_wrap_for_spawn<RR_SHARED_PTR<WebcamImage_size >>(boost::bind((void (async_Webcam::*)(boost::function<void (RR_SHARED_PTR<WebcamImage_size >,RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>)>,int32_t))&async_Webcam::async_CaptureFrameToBuffer, this, _1,rr_timeout), rr_yield);
}
#endif

};
class async_WebcamHost
{
public:
virtual void async_get_WebcamNames(boost::function<void (RR_SHARED_PTR<RobotRaconteur::RRMap<int32_t,RobotRaconteur::RRArray<char>  > >,RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > rr_handler, int32_t rr_timeout=RR_TIMEOUT_INFINITE) = 0;
virtual void async_set_WebcamNames(RR_SHARED_PTR<RobotRaconteur::RRMap<int32_t,RobotRaconteur::RRArray<char>  > > value,boost::function<void (RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > rr_handler, int32_t rr_timeout=RR_TIMEOUT_INFINITE) = 0;

#ifdef ROBOTRACONTEUR_USE_ASIO_SPAWN
virtual RR_SHARED_PTR<RobotRaconteur::RRMap<int32_t,RobotRaconteur::RRArray<char>  > > async_get_WebcamNames(boost::asio::yield_context rr_yield, int32_t rr_timeout=RR_TIMEOUT_INFINITE)
{
    return RobotRaconteur::detail::async_wrap_for_spawn<RR_SHARED_PTR<RobotRaconteur::RRMap<int32_t,RobotRaconteur::RRArray<char>  > > >(boost::bind((void (async_WebcamHost::*)(boost::function<void (RR_SHARED_PTR<RobotRaconteur::RRMap<int32_t,RobotRaconteur::RRArray<char>  > >, RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) >, int32_t))&async_WebcamHost::async_get_WebcamNames, this, _1, rr_timeout), rr_yield);
}
virtual void async_set_WebcamNames(RR_SHARED_PTR<RobotRaconteur::RRMap<int32_t,RobotRaconteur::RRArray<char>  > > value, boost::asio::yield_context rr_yield, int32_t rr_timeout=RR_TIMEOUT_INFINITE)
{
    RobotRaconteur::detail::async_wrap_for_spawn_void(boost::bind((void (async_WebcamHost::*)(RR_SHARED_PTR<RobotRaconteur::RRMap<int32_t,RobotRaconteur::RRArray<char>  > >, boost::function<void(RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>)>, int32_t))&async_WebcamHost::async_set_WebcamNames, this, boost::ref(value), _1, rr_timeout), rr_yield);
}
#endif

virtual void async_get_Webcams(int32_t ind, boost::function<void(RR_SHARED_PTR<Webcam>,RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>)> handler, int32_t timeout=RR_TIMEOUT_INFINITE)=0;

#ifdef ROBOTRACONTEUR_USE_ASIO_SPAWN
virtual RR_SHARED_PTR<Webcam> async_get_Webcams(int32_t ind,  boost::asio::yield_context yield, int32_t timeout=RR_TIMEOUT_INFINITE)
{
    return RobotRaconteur::detail::async_wrap_for_spawn<RR_SHARED_PTR<Webcam> >(boost::bind((void (async_WebcamHost::*)(int32_t, boost::function<void (RR_SHARED_PTR<Webcam>, RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) >, int32_t))&async_WebcamHost::async_get_Webcams, this, ind, _1, timeout), yield);
}
#endif
};
class Webcam_stub : public virtual Webcam, public virtual async_Webcam, public virtual RobotRaconteur::ServiceStub
{
public:
Webcam_stub(const std::string& path, RR_SHARED_PTR<RobotRaconteur::ClientContext> c);

virtual void RRInitStub();
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


virtual void DispatchEvent(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m);
virtual void DispatchPipeMessage(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m);
virtual void DispatchWireMessage(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m);
virtual RR_SHARED_PTR<RobotRaconteur::MessageEntry> CallbackCall(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m);
virtual void RRClose();
private:
RR_SHARED_PTR<RobotRaconteur::PipeClient<RR_SHARED_PTR<WebcamImage > > > rrvar_FrameStream;
RR_SHARED_PTR<RobotRaconteur::ArrayMemoryClient<uint8_t > > rrvar_buffer;
RR_SHARED_PTR<RobotRaconteur::MultiDimArrayMemoryClient<uint8_t > > rrvar_multidimbuffer;
virtual void async_get_Name(boost::function<void (std::string,RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > rr_handler, int32_t rr_timeout=RR_TIMEOUT_INFINITE);
virtual void async_set_Name(std::string value,boost::function<void (RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > rr_handler, int32_t rr_timeout=RR_TIMEOUT_INFINITE);

protected:
virtual void rrend_get_Name(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException> err, boost::function< void (std::string ,RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > handler);
virtual void rrend_set_Name(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException> err, boost::function< void (RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > handler);
public:
virtual void async_CaptureFrame(boost::function<void (RR_SHARED_PTR<WebcamImage >, RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > rr_handler, int32_t rr_timeout=RR_TIMEOUT_INFINITE);

protected:
virtual void rrend_CaptureFrame(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException> err, boost::function< void (RR_SHARED_PTR<WebcamImage > ,RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > handler);
public:
virtual void async_StartStreaming(boost::function<void (RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > rr_handler, int32_t rr_timeout=RR_TIMEOUT_INFINITE);

protected:
virtual void rrend_StartStreaming(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException> err, boost::function< void (RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > handler);
public:
virtual void async_StopStreaming(boost::function<void (RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > rr_handler, int32_t rr_timeout=RR_TIMEOUT_INFINITE);

protected:
virtual void rrend_StopStreaming(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException> err, boost::function< void (RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > handler);
public:
virtual void async_CaptureFrameToBuffer(boost::function<void (RR_SHARED_PTR<WebcamImage_size >, RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > rr_handler, int32_t rr_timeout=RR_TIMEOUT_INFINITE);

protected:
virtual void rrend_CaptureFrameToBuffer(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException> err, boost::function< void (RR_SHARED_PTR<WebcamImage_size > ,RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > handler);
public:
virtual std::string RRType();
};

class WebcamHost_stub : public virtual WebcamHost, public virtual async_WebcamHost, public virtual RobotRaconteur::ServiceStub
{
public:
WebcamHost_stub(const std::string& path, RR_SHARED_PTR<RobotRaconteur::ClientContext> c);

virtual void RRInitStub();
virtual RR_SHARED_PTR<RobotRaconteur::RRMap<int32_t,RobotRaconteur::RRArray<char>  > > get_WebcamNames();
virtual void set_WebcamNames(RR_SHARED_PTR<RobotRaconteur::RRMap<int32_t,RobotRaconteur::RRArray<char>  > > value);

virtual RR_SHARED_PTR<Webcam > get_Webcams(int32_t ind);


virtual void DispatchEvent(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m);
virtual void DispatchPipeMessage(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m);
virtual void DispatchWireMessage(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m);
virtual RR_SHARED_PTR<RobotRaconteur::MessageEntry> CallbackCall(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m);
virtual void RRClose();
private:
virtual void async_get_WebcamNames(boost::function<void (RR_SHARED_PTR<RobotRaconteur::RRMap<int32_t,RobotRaconteur::RRArray<char>  > >,RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > rr_handler, int32_t rr_timeout=RR_TIMEOUT_INFINITE);
virtual void async_set_WebcamNames(RR_SHARED_PTR<RobotRaconteur::RRMap<int32_t,RobotRaconteur::RRArray<char>  > > value,boost::function<void (RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > rr_handler, int32_t rr_timeout=RR_TIMEOUT_INFINITE);

protected:
virtual void rrend_get_WebcamNames(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException> err, boost::function< void (RR_SHARED_PTR<RobotRaconteur::RRMap<int32_t,RobotRaconteur::RRArray<char>  > > ,RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > handler);
virtual void rrend_set_WebcamNames(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException> err, boost::function< void (RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>) > handler);
public:
virtual void async_get_Webcams(int32_t ind, boost::function<void(RR_SHARED_PTR<Webcam>,RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException>)> handler, int32_t timeout=RR_TIMEOUT_INFINITE);

virtual std::string RRType();
};


class Webcam_skel : public virtual RobotRaconteur::ServiceSkel
{
public:
virtual void Init(const std::string& path, RR_SHARED_PTR<RobotRaconteur::RRObject> object, RR_SHARED_PTR<RobotRaconteur::ServerContext> context);
virtual RR_SHARED_PTR<RobotRaconteur::MessageEntry> CallGetProperty(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m);

virtual RR_SHARED_PTR<RobotRaconteur::MessageEntry> CallSetProperty(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m);

virtual RR_SHARED_PTR<RobotRaconteur::MessageEntry> CallFunction(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m);

virtual void ReleaseCastObject();

virtual void RegisterEvents(RR_SHARED_PTR<RobotRaconteur::RRObject> rrobj1);

virtual void UnregisterEvents(RR_SHARED_PTR<RobotRaconteur::RRObject> rrobj1);

virtual RR_SHARED_PTR<RobotRaconteur::RRObject> GetSubObj(const std::string &name, const std::string &ind);

virtual void InitPipeServers(RR_SHARED_PTR<RobotRaconteur::RRObject> rrobj1);

virtual void InitWireServers(RR_SHARED_PTR<RobotRaconteur::RRObject> rrobj1);

virtual void DispatchPipeMessage(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, uint32_t e);

virtual void DispatchWireMessage(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, uint32_t e);

virtual void InitCallbackServers(RR_SHARED_PTR<RobotRaconteur::RRObject> o);

virtual RR_SHARED_PTR<RobotRaconteur::MessageEntry> CallPipeFunction(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, uint32_t e);

virtual RR_SHARED_PTR<RobotRaconteur::MessageEntry> CallWireFunction(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, uint32_t e);

virtual RR_SHARED_PTR<void> GetCallbackFunction(uint32_t endpoint, const std::string& membername);

virtual RR_SHARED_PTR<RobotRaconteur::MessageEntry> CallMemoryFunction(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, RR_SHARED_PTR<RobotRaconteur::Endpoint> e);

virtual std::string GetObjectType();
virtual RR_SHARED_PTR<experimental::createwebcam::Webcam > get_obj();

virtual RR_SHARED_PTR<experimental::createwebcam::async_Webcam > get_asyncobj();

protected:
static void rr_get_Name(RR_WEAK_PTR<experimental::createwebcam::Webcam_skel> skel, std::string value, RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException> err, RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, RR_SHARED_PTR<RobotRaconteur::ServerEndpoint> ep);
static void rr_CaptureFrame(RR_WEAK_PTR<experimental::createwebcam::Webcam_skel> skel, RR_SHARED_PTR<WebcamImage > ret, RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException> err, RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, RR_SHARED_PTR<RobotRaconteur::ServerEndpoint> ep);
static void rr_StartStreaming(RR_WEAK_PTR<experimental::createwebcam::Webcam_skel> skel, RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException> err, RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, RR_SHARED_PTR<RobotRaconteur::ServerEndpoint> ep);
static void rr_StopStreaming(RR_WEAK_PTR<experimental::createwebcam::Webcam_skel> skel, RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException> err, RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, RR_SHARED_PTR<RobotRaconteur::ServerEndpoint> ep);
static void rr_CaptureFrameToBuffer(RR_WEAK_PTR<experimental::createwebcam::Webcam_skel> skel, RR_SHARED_PTR<WebcamImage_size > ret, RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException> err, RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, RR_SHARED_PTR<RobotRaconteur::ServerEndpoint> ep);
 public:
protected:bool rr_InitPipeServersRun;
bool rr_InitWireServersRun;
RR_SHARED_PTR<RobotRaconteur::PipeServer<RR_SHARED_PTR<WebcamImage > > > rr_FrameStream_pipe;
public: 
private:
RR_SHARED_PTR<RobotRaconteur::ArrayMemoryServiceSkel<uint8_t > > rr_buffer_mem;
RR_SHARED_PTR<RobotRaconteur::MultiDimArrayMemoryServiceSkel<uint8_t > > rr_multidimbuffer_mem;

};

class WebcamHost_skel : public virtual RobotRaconteur::ServiceSkel
{
public:
virtual void Init(const std::string& path, RR_SHARED_PTR<RobotRaconteur::RRObject> object, RR_SHARED_PTR<RobotRaconteur::ServerContext> context);
virtual RR_SHARED_PTR<RobotRaconteur::MessageEntry> CallGetProperty(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m);

virtual RR_SHARED_PTR<RobotRaconteur::MessageEntry> CallSetProperty(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m);

virtual RR_SHARED_PTR<RobotRaconteur::MessageEntry> CallFunction(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m);

virtual void ReleaseCastObject();

virtual void RegisterEvents(RR_SHARED_PTR<RobotRaconteur::RRObject> rrobj1);

virtual void UnregisterEvents(RR_SHARED_PTR<RobotRaconteur::RRObject> rrobj1);

virtual RR_SHARED_PTR<RobotRaconteur::RRObject> GetSubObj(const std::string &name, const std::string &ind);

virtual void InitPipeServers(RR_SHARED_PTR<RobotRaconteur::RRObject> rrobj1);

virtual void InitWireServers(RR_SHARED_PTR<RobotRaconteur::RRObject> rrobj1);

virtual void DispatchPipeMessage(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, uint32_t e);

virtual void DispatchWireMessage(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, uint32_t e);

virtual void InitCallbackServers(RR_SHARED_PTR<RobotRaconteur::RRObject> o);

virtual RR_SHARED_PTR<RobotRaconteur::MessageEntry> CallPipeFunction(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, uint32_t e);

virtual RR_SHARED_PTR<RobotRaconteur::MessageEntry> CallWireFunction(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, uint32_t e);

virtual RR_SHARED_PTR<void> GetCallbackFunction(uint32_t endpoint, const std::string& membername);

virtual RR_SHARED_PTR<RobotRaconteur::MessageEntry> CallMemoryFunction(RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, RR_SHARED_PTR<RobotRaconteur::Endpoint> e);

virtual std::string GetObjectType();
virtual RR_SHARED_PTR<experimental::createwebcam::WebcamHost > get_obj();

virtual RR_SHARED_PTR<experimental::createwebcam::async_WebcamHost > get_asyncobj();

protected:
static void rr_get_WebcamNames(RR_WEAK_PTR<experimental::createwebcam::WebcamHost_skel> skel, RR_SHARED_PTR<RobotRaconteur::RRMap<int32_t,RobotRaconteur::RRArray<char>  > > value, RR_SHARED_PTR<RobotRaconteur::RobotRaconteurException> err, RR_SHARED_PTR<RobotRaconteur::MessageEntry> m, RR_SHARED_PTR<RobotRaconteur::ServerEndpoint> ep);
 public:
protected:bool rr_InitPipeServersRun;
bool rr_InitWireServersRun;
public: 
private:

};

}
}


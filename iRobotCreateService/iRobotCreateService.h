#include <RobotRaconteur.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include "robotraconteur_generated.h"
#include <boost/asio/serial_port.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <map>

#pragma once

using namespace RobotRaconteur;
using namespace boost;
using namespace std;
using namespace ::experimental::create2;

//Global lock to protect from multi-threaded calls
extern boost::recursive_mutex global_lock;

//Class that implements the "Create" object abstract class
//and also use "enable_shared_from_this" for shared_ptr support
class Create_impl : public Create_default_impl, public boost::enable_shared_from_this<Create_impl>
{
public:

	Create_impl(string port);

	void Shutdown();

	virtual int32_t get_DistanceTraveled();	

	virtual int32_t get_AngleTraveled();	

	virtual uint8_t get_Bumpers();	

	virtual void Drive(int16_t velocity, int16_t radius);

	virtual void StartStreaming();

	virtual void StopStreaming();

	virtual boost::signals2::signal<void ()>& get_Bump();

	virtual CallbackPtr<boost::function<RRArrayPtr<uint8_t >(int32_t, int32_t) > > get_play_callback();
	virtual void set_play_callback(CallbackPtr<boost::function<RRArrayPtr<uint8_t >(int32_t, int32_t)> > value);
		
private:

	boost::signals2::signal<void ()> m_Bump;
	int32_t m_DistanceTraveled;
	int32_t m_AngleTraveled;
	uint8_t m_Bumpers;
	CallbackPtr<boost::function<RRArrayPtr<uint8_t >(int32_t, int32_t) > > m_play_callback;
		
	RR_SHARED_PTR<boost::asio::serial_port> serialPort;

	void recv_thread_func();

	bool streaming;
	void SendSensorPacket(uint8_t id, RRArrayPtr<uint8_t> data);
	bool lastbump;
	void fire_Bump();
	void play();
	bool lastplay;

	uint32_t current_client;	
};
//This file is automatically generated. DO NOT EDIT!

#include <RobotRaconteur.h>
#include <boost/signals2.hpp>
#pragma once

namespace experimental
{
namespace create
{

class SensorPacket;
class Create;

class SensorPacket : public RobotRaconteur::RRStructure {
public:
uint8_t ID;
RR_SHARED_PTR<RobotRaconteur::RRArray<uint8_t > > Data;

virtual std::string RRType() {return "experimental.create.SensorPacket";  }
};

class Create : public virtual RobotRaconteur::RRObject
{
public:
virtual int32_t get_DistanceTraveled()=0;
virtual void set_DistanceTraveled(int32_t value)=0;

virtual int32_t get_AngleTraveled()=0;
virtual void set_AngleTraveled(int32_t value)=0;

virtual uint8_t get_Bumpers()=0;
virtual void set_Bumpers(uint8_t value)=0;

virtual void Drive(int16_t velocity, int16_t radius)=0;

virtual void StartStreaming()=0;

virtual void StopStreaming()=0;

virtual boost::signals2::signal<void ()>& get_Bump()=0;

virtual RR_SHARED_PTR<RobotRaconteur::Callback<boost::function<RR_SHARED_PTR<RobotRaconteur::RRArray<uint8_t > >(int32_t, int32_t) > > > get_play_callback()=0;
virtual void set_play_callback(RR_SHARED_PTR<RobotRaconteur::Callback<boost::function<RR_SHARED_PTR<RobotRaconteur::RRArray<uint8_t > >(int32_t, int32_t)> > > value)=0;

virtual RR_SHARED_PTR<RobotRaconteur::Wire<RR_SHARED_PTR<SensorPacket > > > get_packets()=0;
virtual void set_packets(RR_SHARED_PTR<RobotRaconteur::Wire<RR_SHARED_PTR<SensorPacket > > > value)=0;

virtual std::string RRType() {return "experimental.create.Create";  }
};

namespace experimental__createConstants 
{
    namespace Create
    {
    static const int16_t DRIVE_STRAIGHT=32767;
    static const int16_t SPIN_CLOCKWISE=-1;
    static const int16_t SPIN_COUNTERCLOCKWISE=1;
    }
}
}
}


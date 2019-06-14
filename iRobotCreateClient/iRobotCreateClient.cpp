#include <RobotRaconteur.h>
#include "robotraconteur_generated.h"

using namespace boost;
using namespace std;
using namespace RobotRaconteur;
using namespace ::experimental::create2;

//This program provides a simple client to the iRobotCreate service
//that connects, drives a bit, and then disconnects

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
		CreatePtr c=rr_cast<Create>(RobotRaconteurNode::s()->ConnectService(string(argv[1]),"",RRMapPtr<std::string,RRValue>(),NULL,"experimental.create2.Create"));

		//Set an event handler for the "Bump" event. This can be any valid boost::function, use lambda here
		c->get_Bump().connect(
			[]() {
				cout << "Bump!!" << endl; 
			}		
		);

		//Connect the "packets" wire and add a value changed event handler
		auto wire=c->get_packets()->Connect();
		//Connect the event handler. This can be any valid boost::function, use lambda here
		wire->WireValueChanged.connect(
			[](WireConnectionPtr<SensorPacketPtr> wire_connection, SensorPacketPtr value, TimeSpec time) 
			{
			SensorPacketPtr value2 = wire_connection->GetInValue();

			//Uncomment this line to show the packet start ID when the value changes 
			//cout << ((int)value2->ID) << endl;
			}
		);

		//Set the callback function for the play_callback.  This can be
		//any valid boost::function, use lambda here
		c->get_play_callback()->SetFunction(
			[](int32_t dist, int32_t angle)
			{
				uint8_t notes[] = { 69, 16, 60, 16, 69, 16 };
				return AttachRRArrayCopy(notes, 6);

			}
		);

		//Start streaming data.  Exceptions are passed transparently to the client;
		//if one occurs just ignore it
		try
		{
			c->StartStreaming();
		}
		catch (std::exception& exp1)
		{
			cout << "Warning: " + string(exp1.what()) << endl;
		}

		//Drive the robot a bit
		c->Drive(200,5000);
		boost::this_thread::sleep(posix_time::milliseconds(1000));
		c->Drive(0,0);

		boost::this_thread::sleep(posix_time::milliseconds(20000));

		//Close the wire connection
		wire->Close();

		//Stop streaming data
		c->StopStreaming();
	}
	catch (std::exception& exp)
	{
		//Report an error
		cout << "Error occured in client: " << exp.what() << endl;
		return -1;
	}
}


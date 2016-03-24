#include <RobotRaconteur.h>
#include "experimental__create.h"
#include "experimental__create_stubskel.h"

using namespace boost;
using namespace std;
using namespace RobotRaconteur;
using namespace experimental::create;

//This program provides a simple client to the iRobotCreate service
//that connects, drives a bit, and then disconnects

//Function to handle the "Bump" event
void Bump()
{
	cout << "Bump!!" << endl;
}

//Function to handle when the wire value changes
void wire_changed(boost::shared_ptr<WireConnection<boost::shared_ptr<SensorPacket> > > wire_connection, boost::shared_ptr<SensorPacket> value, TimeSpec time)
{
	boost::shared_ptr<SensorPacket> value2=wire_connection->GetInValue();
	
	//Uncomment this line to show the packet start ID when the value changes 
	//cout << ((int)value2->ID) << endl;
}

//Function that is called by the service as a callback.  This returns
//a few notes to play.
boost::shared_ptr<RRArray<uint8_t> > play_callback(int32_t dist, int32_t angle)
{
	uint8_t notes[]={69, 16, 60, 16, 69, 16};
	return AttachRRArrayCopy(notes,6);

}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cout << "URL expected as argument" << endl;
		return -1;
	}

	try
	{
		//Create and register a Local Transport
		boost::shared_ptr<LocalTransport> t1=boost::make_shared<LocalTransport>();
		RobotRaconteurNode::s()->RegisterTransport(t1);

		//Create and register a TcpTransport
		boost::shared_ptr<TcpTransport> t=boost::make_shared<TcpTransport>();
		RobotRaconteurNode::s()->RegisterTransport(t);
		//Register the Create_interface service type
		RobotRaconteurNode::s()->RegisterServiceType(boost::make_shared<experimental__createFactory>());

		//Connect to the service
		boost::shared_ptr<Create> c=rr_cast<Create>(RobotRaconteurNode::s()->ConnectService(string(argv[1]),"",boost::shared_ptr<RRMap<std::string,RRObject> >(),NULL,"experimental.create.Create"));

		//Set an event handler for the "Bump" event. This can be any valid boost::function (see boost::bind)
		c->get_Bump().connect(boost::bind(&Bump));

		//Connect the "packets" wire and add a value changed event handler
		boost::shared_ptr<WireConnection<boost::shared_ptr<SensorPacket> > > wire=c->get_packets()->Connect();
		//Connect the event handler. This can be any valid boost::function (see boost::bind)
		wire->WireValueChanged.connect(boost::bind(&wire_changed,_1,_2,_3));

		//Set the callback function for the play_callback.  This can be
		//any valid boost::function (see boost::bind)
		c->get_play_callback()->SetFunction(boost::bind(&play_callback,_1,_2));

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
		this_thread::sleep(posix_time::milliseconds(1000));
		c->Drive(0,0);

		this_thread::sleep(posix_time::milliseconds(20000));

		//Close the wire connection
		wire->Close();

		//Stop streaming data
		c->StopStreaming();

		//Shutdown Robot Raconteur.  This must be called at program exit
		RobotRaconteurNode::s()->Shutdown();

	}
	catch (std::exception& exp)
	{
		//Report an error
		cout << "Error occured in client: " << exp.what() << endl;
		return -1;
	}
}


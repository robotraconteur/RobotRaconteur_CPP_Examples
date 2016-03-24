#include <RobotRaconteur.h>
#include "experimental__create.h"
#include "experimental__create_stubskel.h"

using namespace boost;
using namespace std;
using namespace RobotRaconteur;
using namespace experimental::create;

//This program uses the FindServiceByType function to find the iRobot Create service
//using autodiscovery

int main(int argc, char* argv[])
{

	try
	{
		//Create and register a Local Transport
		boost::shared_ptr<LocalTransport> t1=boost::make_shared<LocalTransport>();
		RobotRaconteurNode::s()->RegisterTransport(t1);

		//Create and register a TcpTransport
		boost::shared_ptr<TcpTransport> t=boost::make_shared<TcpTransport>();
		//Enable the TcpChannel to listen for other nodes.  The IPNodeDiscoveryFlags will
        //normally be the same as here
		t->EnableNodeDiscoveryListening();
		//Register the transport
		RobotRaconteurNode::s()->RegisterTransport(t);
		
		//Register the service type
		RobotRaconteurNode::s()->RegisterServiceType(boost::make_shared<experimental__createFactory>());

		//Wait 10 seconds to receive the beacon packets for autodiscovery which are
        //sent every 5 seconds.
		this_thread::sleep(posix_time::milliseconds(10000));

		//Search for the "Create_interface.Create" object type on "tcp" transports
		vector<string> schemes;
		schemes.push_back("rr+tcp");
		schemes.push_back("rrs+tcp");
		schemes.push_back("rr+local");
		vector<ServiceInfo2> res=RobotRaconteurNode::s()->FindServiceByType("experimental.create.Create",schemes);

		if (res.size()==0) 
		{
			cout << "Could not find any service nodes" << endl;
			return -1;
		}

		//Print out the discovered nodes
		for (vector<ServiceInfo2>::iterator e=res.begin(); e!=res.end(); ++e)
		{
			cout << e->NodeName << " " << e->NodeID.ToString() << " " << e->Name << " " << e->ConnectionURL[0] << endl;
		}

		//Connect to the service
		boost::shared_ptr<Create> c=rr_cast<Create>(RobotRaconteurNode::s()->ConnectService(res[0].ConnectionURL,"",boost::shared_ptr<RRMap<std::string,RRObject> >(),NULL,"experimental.create.Create"));
				
		//Drive a bit
		c->Drive(200,5000);
		this_thread::sleep(posix_time::milliseconds(1000));
		c->Drive(0,0);

		//Shutdown
		RobotRaconteurNode::s()->Shutdown();

	}
	catch (std::exception& exp)
	{
		//Report if an error occurs
		cout << "Error occured in client: " << exp.what() << endl;
		return -1;
	}
}


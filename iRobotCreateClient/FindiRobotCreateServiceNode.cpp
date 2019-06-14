#include <RobotRaconteur.h>
#include "robotraconteur_generated.h"

using namespace boost;
using namespace std;
using namespace RobotRaconteur;
using namespace ::experimental::create2;

//This program uses the FindServiceByType function to find the iRobot Create service
//using autodiscovery

int main(int argc, char* argv[])
{

	try
	{
		//Use node setup to help initialize client node
		ClientNodeSetup node_setup(ROBOTRACONTEUR_SERVICE_TYPES);
		
		//Wait for discovery packets
		boost::this_thread::sleep(boost::posix_time::seconds(6));

		//Search for the "Create_interface.Create" object type on "tcp" transports
		vector<ServiceInfo2> res = RobotRaconteurNode::s()->FindServiceByType("experimental.create2.Create", { "rr+tcp", "rrs+tcp", "rr+local" });

		if (res.size()==0) 
		{
			cout << "Could not find any service nodes" << endl;
			return -1;
		}

		//Print out the discovered nodes
		for (auto e : res)
		{
			cout << e.NodeName << " " << e.NodeID.ToString() << " " << e.Name << " " << e.ConnectionURL[0] << endl;
		}

		//Connect to the service
		CreatePtr c=rr_cast<Create>(RobotRaconteurNode::s()->ConnectService(res[0].ConnectionURL,"",RRMapPtr<std::string,RRValue>(),NULL,"experimental.create2.Create"));
				
		//Drive a bit
		c->Drive(200,5000);
		boost::this_thread::sleep(posix_time::milliseconds(1000));
		c->Drive(0,0);
	}
	catch (std::exception& exp)
	{
		//Report if an error occurs
		cout << "Error occured in client: " << exp.what() << endl;
		return -1;
	}
}


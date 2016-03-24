
#include "iRobotCreateService.h"


//This program provides a simple Robot Raconteur server for controlling the iRobot Create.

int main(int argc, char* argv[])
{

	if (argc <2) 
	{
		cout << "Expected 1 parameter of port device" << endl;
		return -1;
	}	

	//Initialize the create robot object
	boost::shared_ptr<Create_impl> create=boost::make_shared<Create_impl>(string(argv[1]));

	//Create local transport
	boost::shared_ptr<LocalTransport> t1=boost::make_shared<LocalTransport>();
	t1->StartServerAsNodeName("experimental.create.Create");
	RobotRaconteurNode::s()->RegisterTransport(t1);

	//Initialize the TCP transport and start listenin on port 2354
	boost::shared_ptr<TcpTransport> t=boost::make_shared<TcpTransport>();
	t->StartServer(2354);

	//Enable auto-discovery announcement
	t->EnableNodeAnnounce(IPNodeDiscoveryFlags_LINK_LOCAL | IPNodeDiscoveryFlags_SITE_LOCAL | IPNodeDiscoveryFlags_NODE_LOCAL);

	//Register the TCP transport
	RobotRaconteurNode::s()->RegisterTransport(t);

	//Register the Create_interface type so that the node can understand the service definition
	RobotRaconteurNode::s()->RegisterServiceType(boost::make_shared<experimental__createFactory>());

	//Register the create object as a service so that it can be connected to
	RobotRaconteurNode::s()->RegisterService("Create","experimental.create",create);
	
	//Stay open until shut down
	cout << "Create server started. Press enter to quit" << endl;
	getchar();
	
	//Shutdown
	create->Shutdown();

	//Shutdown the node. This must be called at program exit
	RobotRaconteurNode::s()->Shutdown();
	
	return 0;
}


//Class for Create_impl

 //Initialize the serial port
Create_impl::Create_impl(string port) : Create(), io_service(), serialPort(io_service, port)
{
	m_DistanceTraveled=0;
	m_AngleTraveled=0;
	m_Bumpers=0;
	streaming=false;
	lastbump=false;
	lastplay=false;

	if (!serialPort.is_open())
	{
		throw std::runtime_error("Could not open port");
	}

	serialPort.set_option(boost::asio::serial_port::baud_rate(57600));
	serialPort.set_option(boost::asio::serial_port::stop_bits());
	serialPort.set_option(boost::asio::serial_port::flow_control());
	serialPort.set_option(boost::asio::serial_port::character_size());

	
	uint8_t command[]={128,132};
	serialPort.write_some(boost::asio::buffer(command,2));

	//Wait a bit for the Create to switch to the correct mode
	this_thread::sleep(posix_time::milliseconds(100));
	
}


//Shutdown the serial port
void Create_impl::Shutdown()
{

	recursive_mutex::scoped_lock lock(global_lock);
	if (streaming)
	{
		StopStreaming();
		this_thread::sleep(posix_time::milliseconds(500));
	}

	uint8_t command[]={128};
	serialPort.write_some(boost::asio::buffer(command,1));
	
	serialPort.close();
}


//Return the DistanceTraveled property
int32_t Create_impl::get_DistanceTraveled()
{
	return m_DistanceTraveled;
}

//Set the DistanceTraveled property
void Create_impl::set_DistanceTraveled(int32_t value)
{
	throw runtime_error("Read only property");
}

//Return the AngleTraveled property
int32_t Create_impl::get_AngleTraveled()
{
	return m_AngleTraveled;
}

//Set the AngleTraveled property
void Create_impl::set_AngleTraveled(int32_t value)
{
	throw runtime_error("Read only property");
}

//Return the Bumpers property
uint8_t Create_impl::get_Bumpers()
{
	return m_Bumpers;
}

//Set the Bumpers property
void Create_impl::set_Bumpers(uint8_t value)
{
	throw runtime_error("Read only property");
}

//Drive the robot at specifiec velocity and radius
void Create_impl::Drive(int16_t velocity, int16_t radius)
{
	recursive_mutex::scoped_lock lock(global_lock);

	uint8_t* vel=(uint8_t*)&velocity;
	uint8_t* rad=(uint8_t*)&radius;
	uint8_t command[5];
	command[0]=137;
	command[1]=vel[1];
	command[2]=vel[0];
	command[3]=rad[1];
	command[4]=rad[0];
	serialPort.write_some(boost::asio::buffer(command,5));
}

//Start streaming data from the robot
void Create_impl::StartStreaming()
{
	recursive_mutex::scoped_lock lock(global_lock);

	if (streaming) throw runtime_error("Already streaming");


	//Save the current connected endpoint.  This uniquely identifies
	//the client
	current_client=ServerEndpoint::GetCurrentEndpoint()->GetLocalEndpoint();
	streaming=true;

	
	//Start the thread that will receive serial data
	boost::thread(boost::bind(&Create_impl::recv_thread_func,shared_from_this()));

	this_thread::sleep(posix_time::milliseconds(100));

	uint8_t command[]={148, 4, 7, 19, 20, 18};
	serialPort.write_some(boost::asio::buffer(command,6));
	
}

//Stop streaming data from the robot
void Create_impl::StopStreaming()
{
	if (!streaming) throw runtime_error("Not streaming");
	recursive_mutex::scoped_lock lock(global_lock);

	uint8_t command[]={150, 0};
	serialPort.write_some(boost::asio::buffer(command,2));
	streaming=false;
}

//Return reference to the Bump event signal object
boost::signals2::signal<void ()>& Create_impl::get_Bump()
{
	return m_Bump;
}

//Return  the play_callback server
RR_SHARED_PTR<RobotRaconteur::Callback<boost::function<RR_SHARED_PTR<RobotRaconteur::RRArray<uint8_t > >(int32_t, int32_t) > > > Create_impl::get_play_callback()
{
	return m_play_callback;
}

//Set the play_callback clint
void Create_impl::set_play_callback(RR_SHARED_PTR<RobotRaconteur::Callback<boost::function<RR_SHARED_PTR<RobotRaconteur::RRArray<uint8_t > >(int32_t, int32_t)> > > value)
{
	m_play_callback=value;
}

//Return the Packets wire server
RR_SHARED_PTR<RobotRaconteur::Wire<RR_SHARED_PTR<SensorPacket > > > Create_impl::get_packets()
{
	return m_packets;
}

//Set the Packets wire server
void Create_impl::set_packets(RR_SHARED_PTR<RobotRaconteur::Wire<RR_SHARED_PTR<SensorPacket > > > value)
{
	m_packets=value;

	//Set the function to call when a WireConnection connects
	value->SetWireConnectCallback(boost::bind(&Create_impl::WireConnectCallbackFunction, shared_from_this(), _1));
}

//Handle a connecting WireConnection
void Create_impl::WireConnectCallbackFunction(RR_SHARED_PTR<RobotRaconteur::WireConnection<RR_SHARED_PTR<SensorPacket > > > wire)
{

	recursive_mutex::scoped_lock lock(global_lock);

	//Store the WireConnection
	wireconnections.insert(make_pair(wire->GetEndpoint(),wire));
	//Set the WireConnectionClosed callback to handle when the wire closes
	wire->SetWireConnectionClosedCallback(boost::bind(&Create_impl::WireDisconnectCallbackFunction,shared_from_this(),_1));
}

//Delete the wire connection from the storage map when closed
void Create_impl::WireDisconnectCallbackFunction(RR_SHARED_PTR<RobotRaconteur::WireConnection<RR_SHARED_PTR<SensorPacket > > > wire)
{
	recursive_mutex::scoped_lock lock(global_lock);

	wireconnections.erase(wire->GetEndpoint());
}


//Function to handle data from the robot.  This is mainly just handling the iRobot Create OI data.
void Create_impl::recv_thread_func()
{
	try
	{
		while(streaming)
		{
			uint8_t seed;
			
			serialPort.read_some(boost::asio::buffer(&seed,1));
			
			if (seed!=19)
			{
				cout << "Error occured in receive thread" << endl;
				return;
			}

			uint8_t nbytes;
			serialPort.read_some(boost::asio::buffer(&nbytes,1));

			boost::shared_ptr<RRArray<uint8_t> > packets=AllocateRRArray<uint8_t>(nbytes+1);
			size_t r=0;
			while (r < packets->Length())
			{
				r+=serialPort.read_some(boost::asio::buffer(packets->ptr()+r,(packets->Length())-r));
			}
			
			SendSensorPacket(seed,packets);
			uint8_t readpos=0;

			while (readpos < nbytes)
			{
				uint8_t id=(*packets)[readpos++];

				switch (id)
				{
					case 7:
                    {
                        uint8_t flags = (*packets)[readpos++];
                        if (((flags & 0x1) != 0) || ((flags & 0x2) != 0))
                        {
                            if (lastbump == false)
                            {
                                fire_Bump();
                            }
                            lastbump = true;
                        }
                        else
                        {
                            lastbump = false;
                        }
                        m_Bumpers = flags;

                    }

                    break;

					case 19:
                    {
                        int16_t a=0;
						uint8_t* a2=(uint8_t*)&a;
						(*(a2+1))=(*packets)[readpos++];
						(*(a2))=(*packets)[readpos++];
						                        
                        m_DistanceTraveled += a;

                    }


                    break;
                case 20:
                    {
						int16_t a=0;
						uint8_t* a2=(uint8_t*)&a;
						(*(a2+1))=(*packets)[readpos++];
						(*(a2))=(*packets)[readpos++];
						                        
                        m_AngleTraveled += a;

                    }
                    break;
                case 18:
                    {
                        uint8_t buttons=(*packets)[readpos++];
                        uint8_t bplay=(uint8_t)(buttons & ((uint8_t)0x1));
                        if (bplay==1)
                        {
                            if (!lastplay)
                            {
                                play();
                            }
                            lastplay=true;
                        }
                        else
                        {
                            lastplay=false;
                        }
                    }
                    break;
                default:

                    readpos++;
                    break;
				}
			}
		}
	}
	catch (std::exception& e)
	{
		//If there is an error report it if streaming is still expected
		if (streaming)
		{
			cout << "Exception occured in receive thread: " << e.what() << endl;
			
		}		
	}
}

//Send a received packet to all connected WireConnections
void Create_impl::SendSensorPacket(uint8_t id, boost::shared_ptr<RRArray<uint8_t> > data)
{
	recursive_mutex::scoped_lock lock(global_lock);

	boost::shared_ptr<SensorPacket> p=boost::make_shared<SensorPacket>();
	p->ID=id;
	p->Data=data;

	//Collect endpoints
	vector<uint32_t> ep;
	for(map<uint32_t, RR_SHARED_PTR<RobotRaconteur::WireConnection<RR_SHARED_PTR<SensorPacket > > > >::iterator e=wireconnections.begin(); e!=wireconnections.end(); ++e)
	{
		ep.push_back(e->first);
	}

	//Iterate through all WireConnections to send data
	for (vector<uint32_t>::iterator e=ep.begin(); e!=ep.end(); ++e)
	{
		RR_SHARED_PTR<RobotRaconteur::WireConnection<RR_SHARED_PTR<SensorPacket > > > wend;
		try
		{
			wend=wireconnections.at(*e);
			//Set the OutValue of the wire
			wend->SetOutValue(p);
		}
		catch (std::exception& ex)
		{
			//If there is an error, close the WireConnection and erase it
			try
			{
				wend->Close();
			}
			catch (std::exception& ex2) {}

			try
			{
				wireconnections.erase(*e);
			}
			catch (std::exception& ex2) {}
		}
	}

}

//Fire the "Bump" event
void Create_impl::fire_Bump()
{
	//Fire the signal
	m_Bump();
}

//Execute the play callback to retrieve notes and play on the robot
void Create_impl::play()
{
	//If there is no current client, return
	if (current_client==0) return;
	
	//Retrieve the play_callback for the "current_client" and execute to retrive
	//an array of notes to play
	boost::shared_ptr<RRArray<uint8_t> > notes=m_play_callback->GetClientFunction(current_client)(m_DistanceTraveled, m_AngleTraveled);

	//Types passed by Robot Raconteur may be null; check that "notes" is not null
	if (!notes) return;
	
	//Construct the command and play on the robot
	uint8_t* command=new uint8_t[notes->Length() + 5];
	command[0]=140;
	command[1]=0;
	command[2]=(uint8_t)notes->Length()/2;
	memcpy(command+3,notes->ptr(),notes->Length());
	command[3+notes->Length()]=141;
	command[4+notes->Length()]=0;

	recursive_mutex::scoped_lock lock(global_lock);
	serialPort.write_some(boost::asio::buffer(command,notes->Length() + 5));
}



recursive_mutex global_lock;
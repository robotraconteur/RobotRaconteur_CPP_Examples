
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
	std::string port = string(argv[1]);
	RR_SHARED_PTR<Create_impl> create=RR_MAKE_SHARED<Create_impl>(port);

	//Use node setup to help initialize server node
	ServerNodeSetup node_setup(ROBOTRACONTEUR_SERVICE_TYPES, "experimental.create2", 2354);
		
	//Register the create object as a service so that it can be connected to
	RobotRaconteurNode::s()->RegisterService("Create","experimental.create2",create);
	
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
Create_impl::Create_impl(string port)
{
	m_DistanceTraveled=0;
	m_AngleTraveled=0;
	m_Bumpers=0;
	streaming=false;
	lastbump=false;
	lastplay=false;

	serialPort.reset(new boost::asio::serial_port(RobotRaconteurNode::s()->GetThreadPool()->get_io_context(), port));

	if (!serialPort->is_open())
	{
		throw std::runtime_error("Could not open port");
	}

	serialPort->set_option(boost::asio::serial_port::baud_rate(57600));
	serialPort->set_option(boost::asio::serial_port::stop_bits());
	serialPort->set_option(boost::asio::serial_port::flow_control());
	serialPort->set_option(boost::asio::serial_port::character_size());

	
	uint8_t command[]={128,132};
	serialPort->write_some(boost::asio::buffer(command,2));

	//Wait a bit for the Create to switch to the correct mode
	boost::this_thread::sleep(posix_time::milliseconds(100));
	
}


//Shutdown the serial port
void Create_impl::Shutdown()
{

	boost::recursive_mutex::scoped_lock lock(global_lock);
	if (streaming)
	{
		StopStreaming();
		boost::this_thread::sleep(posix_time::milliseconds(500));
	}

	uint8_t command[]={128};
	serialPort->write_some(boost::asio::buffer(command,1));
	
	serialPort->close();
}


//Return the DistanceTraveled property
int32_t Create_impl::get_DistanceTraveled()
{
	return m_DistanceTraveled;
}

//Return the AngleTraveled property
int32_t Create_impl::get_AngleTraveled()
{
	return m_AngleTraveled;
}

//Return the Bumpers property
uint8_t Create_impl::get_Bumpers()
{
	return m_Bumpers;
}


//Drive the robot at specifiec velocity and radius
void Create_impl::Drive(int16_t velocity, int16_t radius)
{
	boost::recursive_mutex::scoped_lock lock(global_lock);

	uint8_t* vel=(uint8_t*)&velocity;
	uint8_t* rad=(uint8_t*)&radius;
	uint8_t command[5];
	command[0]=137;
	command[1]=vel[1];
	command[2]=vel[0];
	command[3]=rad[1];
	command[4]=rad[0];
	serialPort->write_some(boost::asio::buffer(command,5));
}

//Start streaming data from the robot
void Create_impl::StartStreaming()
{
	boost::recursive_mutex::scoped_lock lock(global_lock);

	if (streaming) throw runtime_error("Already streaming");


	//Save the current connected endpoint.  This uniquely identifies
	//the client
	current_client=ServerEndpoint::GetCurrentEndpoint()->GetLocalEndpoint();
	streaming=true;

	
	//Start the thread that will receive serial data
	auto shared_this = shared_from_this();
	boost::thread([shared_this]()
	{
		shared_this->recv_thread_func();
	}
	);

	boost::this_thread::sleep(posix_time::milliseconds(100));

	uint8_t command[]={148, 4, 7, 19, 20, 18};
	serialPort->write_some(boost::asio::buffer(command,6));
	
}

//Stop streaming data from the robot
void Create_impl::StopStreaming()
{
	if (!streaming) throw runtime_error("Not streaming");
	boost::recursive_mutex::scoped_lock lock(global_lock);

	uint8_t command[]={150, 0};
	serialPort->write_some(boost::asio::buffer(command,2));
	streaming=false;
}

//Return reference to the Bump event signal object
boost::signals2::signal<void ()>& Create_impl::get_Bump()
{
	return m_Bump;
}

//Return  the play_callback server
CallbackPtr<boost::function<RRArrayPtr<uint8_t >(int32_t, int32_t) > > Create_impl::get_play_callback()
{
	return m_play_callback;
}

//Set the play_callback clint
void Create_impl::set_play_callback(CallbackPtr<boost::function<RRArrayPtr<uint8_t >(int32_t, int32_t) > > value)
{
	m_play_callback=value;
}

//Function to handle data from the robot.  This is mainly just handling the iRobot Create OI data.
void Create_impl::recv_thread_func()
{
	try
	{
		while(streaming)
		{
			uint8_t seed;
			
			serialPort->read_some(boost::asio::buffer(&seed,1));
			
			if (seed!=19)
			{
				cout << "Error occured in receive thread" << endl;
				return;
			}

			uint8_t nbytes;
			serialPort->read_some(boost::asio::buffer(&nbytes,1));

			RRArrayPtr<uint8_t> packets=AllocateRRArray<uint8_t>(nbytes+1);
			size_t r=0;
			while (r < packets->size())
			{
				r+=serialPort->read_some(boost::asio::buffer(&packets->at(0)+r,(packets->size())-r));
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
void Create_impl::SendSensorPacket(uint8_t id, RRArrayPtr<uint8_t> data)
{
	boost::recursive_mutex::scoped_lock lock(global_lock);

	SensorPacketPtr p(new SensorPacket());
	p->ID=id;
	p->Data=data;

	if (rrvar_packets)
	{
		rrvar_packets->SetOutValue(p);
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
	RRArrayPtr<uint8_t> notes=m_play_callback->GetClientFunction(current_client)(m_DistanceTraveled, m_AngleTraveled);

	//Types passed by Robot Raconteur may be null; check that "notes" is not null
	if (!notes) return;
	
	//Construct the command and play on the robot
	boost::shared_array<uint8_t> command(new uint8_t[notes->size() + 5]);
	command[0]=140;
	command[1]=0;
	command[2]=(uint8_t)notes->size()/2;
	memcpy(command.get()+3,&notes->at(0),notes->size());
	command[3+notes->size()]=141;
	command[4+notes->size()]=0;

	boost::recursive_mutex::scoped_lock lock(global_lock);
	serialPort->write_some(boost::asio::buffer(command.get(),notes->size() + 5));
}



boost::recursive_mutex global_lock;
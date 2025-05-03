#include <Server.hpp>

Server::Server(std::pair<int, String> pair) : port(pair.first), password(pair.second)
{
	this->setup();
	this->run();
}

Server::~Server()
{
	if (close(server_fd) < 0)
	{
		std::cerr << "close" << std::endl; // exception
	}
}

void Server::setup()
{
	this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		std::cerr << "socket" << std::endl; // exception
	}
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cerr << "bind" << std::endl; // exception
	}
	if (listen(server_fd, MAX_CONN) < 0)
	{
		std::cerr << "listen" << std::endl; // exception
	}
}

void Server::run()
{
	EventHandler eventHandler(server_fd);
	while (true)
	{
		eventHandler.handleEvents();
	}
	// This is the old run method using select
	// Uncomment the above lines and comment this method to use the new event handler
	// and remove the old select-based implementation.
}

/*
void Server::run()
{
	fd_set master_set, read_set; // declaring 2 fd_set variables
	int max_fd = server_fd;		 // setting max_fd to server_fd for select()

	FD_ZERO(&master_set);			// initializing 0 the master_set
	FD_ZERO(&read_set);				// initializing 0 the read_set
	FD_SET(server_fd, &master_set); // adding server_fd to master_set
	while (true)
	{
		read_set = master_set;

		// Use select to monitor the FDs
		int activity = select(max_fd + 1, &read_set, NULL, NULL, NULL);
		if (activity < 0)
		{
			perror("select");
			break;
		}

		// Check if there's an incoming connection on the server socket
		if (FD_ISSET(server_fd, &read_set))
		{
			// Accept new connection
			int client_fd = accept(server_fd, NULL, NULL);
			if (client_fd < 0)
			{
				perror("accept");
				continue;
			}

			// Add the new client FD to the master set
			FD_SET(client_fd, &master_set);

			// Update max_fd if the new client FD is greater
			if (client_fd > max_fd)
			{
				max_fd = client_fd;
			}

			std::cout << "New client connected: FD " << client_fd << std::endl;
		}
	}
}

*/

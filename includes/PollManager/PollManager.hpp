#ifndef POLLMANAGER_HPP
#define POLLMANAGER_HPP

#include <IRC.hpp>

class PollManager
{
private:
	std::vector<struct pollfd> _fds;

public:
	int wait();
	void addFd(int fd, short events);
	void removeFd(int fd);
	std::vector<struct pollfd> &getPollFds();
};

#endif

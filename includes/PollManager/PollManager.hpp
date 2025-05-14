#ifndef POLL_MANAGER_HPP
#define POLL_MANAGER_HPP

#include <IRC.hpp>

class PollManager
{
private:
	std::vector<struct pollfd> _fds;
	struct pollfd createPollStruct(int fd, short events, short revents);

public:
	int wait();
	void addFd(int fd, short events);
	void removeFd(int fd);
	std::vector<struct pollfd> &getPollFds();
	size_t getPollSize();
};

#endif
// POLL_MANAGER_HPP

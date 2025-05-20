#include <PollManager.hpp>

int PollManager::wait()
{
	return poll(_fds.data(), _fds.size(), -1);
}

void PollManager::addFd(int fd, short events)
{
	_fds.push_back(createPollStruct(fd, events));
}

void PollManager::removeFd(int fd)
{
	for (std::vector<struct pollfd>::iterator it = _fds.begin(); it != _fds.end();)
	{
		if (it->fd == fd)
		{
			close(fd);
			it = _fds.erase(it);
		}
		else
			++it;
	}
}

std::vector<struct pollfd> &PollManager::getPollFds()
{
	return _fds;
}

size_t PollManager::getPollSize()
{
	return _fds.size();
}

struct pollfd PollManager::createPollStruct(int fd, short events)
{
	struct pollfd pollStruct;
	pollStruct.fd = fd;
	pollStruct.events = events;
	pollStruct.revents = 0;
	return pollStruct;
}

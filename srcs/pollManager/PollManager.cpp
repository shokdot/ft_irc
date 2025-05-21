#include <PollManager.hpp>

int PollManager::wait()
{
	return poll(_fds.data(), _fds.size(), -1);
}

void PollManager::addFd(int fd, short events)
{
	_fds.push_back(Utils::createPollStruct(fd, events));
}

void PollManager::removeFd(int fd)
{
	for (std::vector<struct pollfd>::iterator it = _fds.begin(); it != _fds.end();)
	{
		if (it->fd == fd)
		{
			if (close(fd) < 0)
				std::cout << "[ERROR] Close client " << fd << std::endl;
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

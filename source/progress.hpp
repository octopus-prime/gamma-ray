/*
 * progress.hpp
 *
 *  Created on: 28.12.2014
 *      Author: mike_gresens
 */

#pragma once

#include <atomic>
#include <thread>
#include <chrono>

namespace rt {

class progress_t
{
	typedef std::chrono::high_resolution_clock clock_t;

public:
	progress_t(const std::size_t todo, const clock_t::duration wait = std::chrono::seconds(5));
	~progress_t();

	std::atomic_size_t&
	done()
	{
		return _done;
	}

protected:
	void run() const;

private:
	std::size_t _todo;
	clock_t::duration _wait;
	std::atomic_size_t _done;
	clock_t::time_point _time;
	std::thread _thread;
};

}

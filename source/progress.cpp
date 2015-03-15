/*
 * progress.cpp
 *
 *  Created on: 28.12.2014
 *      Author: mike_gresens
 */

#include "progress.hpp"
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/attributes/named_scope.hpp>

namespace rt {

progress_t::progress_t(const std::size_t todo, const clock_t::duration wait)
:
	_todo(todo),
	_wait(wait),
	_done(0),
	_time(clock_t::now()),
	_thread(std::bind(&progress_t::run, this))
{
	_thread.detach();
}

void
progress_t::run() const
{
	BOOST_LOG_NAMED_SCOPE("Rendering");
	for (;;)
	{
		std::this_thread::sleep_for(_wait);

		if (_done >= _todo)
			break;

		const std::size_t progress = 100 * _done / _todo;
		BOOST_LOG_TRIVIAL(info) << boost::format("%3.d") % progress << "% done";
	}
}

}

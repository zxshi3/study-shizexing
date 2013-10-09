/**
 * test boost random library
 */

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

int roll_die(boost::mt19937 & gen)
{
	boost::uniform_int<> dist(1, 6);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(gen, dist);
	return die();
}

int main(int argc, char** argv)
{
	boost::mt19937 gen;
	boost::posix_time::ptime curr_time = boost::posix_time::microsec_clock::local_time();
	gen.seed(curr_time.time_of_day().total_milliseconds());
	for (int i = 0; i < 10; i++)
	{
		std::cout << "r = " << roll_die(gen) << std::endl;
	}

	return 0;
}


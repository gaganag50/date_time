/* A simple example for using a time_zone and a posix_time_zone.
 */

#include "boost/date_time/local_time/local_time.hpp"
#include <iostream>

int
main() 
{
  using namespace boost;
  using namespace local_time;
  using namespace gregorian;
  using posix_time::time_duration;

  /***** time_zone *****/
  
  // create the dependent objects for a time_zone
  time_zone_names tzn("Eastern Standard Time", "EST",
                      "Eastern Daylight Time", "EDT");
  time_duration utc_offset(-5,0,0);
  dst_adjustment_offsets adj_offsets(time_duration(1,0,0), 
                                     time_duration(2,0,0), 
                                     time_duration(2,0,0));
  // rules for this zone are:
  // start on first Sunday of April at 2 am
  // end on last Sunday of October at 2 am
  // so we use a first_last_dst_rule
  first_day_of_the_week_in_month start_rule(Sunday, Apr);
  last_day_of_the_week_in_month    end_rule(Sunday, Oct);
  shared_ptr<dst_calc_rule> nyc_rules(new first_last_dst_rule(start_rule, 
                                                              end_rule));
  // create the time_zone
  time_zone_ptr nyc_1(new time_zone(tzn, utc_offset, adj_offsets, nyc_rules));

  /***** posix_time_zone *****/

  // create a posix_time_zone that is the duplicate of the 
  // time_zone created above. See posix_time_zone documentation 
  // for details on full zone names
  std::string nyc_string;
  nyc_string = "EST-05:00:00EDT+01:00:00,M4.1.0/02:00:00,M10.5.0/02:00:00";
  // nyc_string = "EST-05EDT,M4.1.0,M10.5.0"; // shorter when defaults used
  time_zone_ptr nyc_2(new posix_time_zone(nyc_string));
 

  /***** show the two are equal *****/

  std::cout << "The first zone is in daylight savings from:\n " 
    << nyc_1->dst_local_start_time(2004) << " through "
    << nyc_1->dst_local_end_time(2004) << std::endl;

  std::cout << "The second zone is in daylight savings from:\n " 
    << nyc_2->dst_local_start_time(2004) << " through "
    << nyc_2->dst_local_end_time(2004) << std::endl;

  return 0;
}

/*  Copyright 2001-2005: CrystalClear Software, Inc
 *  http://www.crystalclearsoftware.com
 *
 *  Subject to the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE-1.0 or http://www.boost.org/LICENSE-1.0)
 */


#ifndef GREGORIAN_SERIALIZE_HPP___
#define GREGORIAN_SERIALIZE_HPP___

/* Copyright (c) 2004 CrystalClear Software, Inc.
 * Use, modification and distribution is subject to the 
 * Boost Software License, Version 1.0. (See accompanying
 * file LICENSE-1.0 or http://www.boost.org/LICENSE-1.0)
 * Author: Jeff Garland, Bart Garst
 * $Date$
 */

#include "boost/date_time/gregorian/gregorian_types.hpp"
#include "boost/date_time/gregorian/parsers.hpp"
#include "boost/serialization/split_free.hpp"


// macros to split serialize functions into save & load functions
// These macros must be used outside the namespace
// the expanded version is below for gregorian::date
BOOST_SERIALIZATION_SPLIT_FREE(::boost::gregorian::date_duration)
BOOST_SERIALIZATION_SPLIT_FREE(::boost::gregorian::date_duration::duration_rep)
BOOST_SERIALIZATION_SPLIT_FREE(::boost::gregorian::date_period)
BOOST_SERIALIZATION_SPLIT_FREE(::boost::gregorian::greg_month)
BOOST_SERIALIZATION_SPLIT_FREE(::boost::gregorian::greg_day)
BOOST_SERIALIZATION_SPLIT_FREE(::boost::gregorian::greg_weekday)
BOOST_SERIALIZATION_SPLIT_FREE(::boost::gregorian::partial_date)
BOOST_SERIALIZATION_SPLIT_FREE(::boost::gregorian::nth_kday_of_month)
BOOST_SERIALIZATION_SPLIT_FREE(::boost::gregorian::first_kday_of_month)
BOOST_SERIALIZATION_SPLIT_FREE(::boost::gregorian::last_kday_of_month)
BOOST_SERIALIZATION_SPLIT_FREE(::boost::gregorian::first_kday_before)
BOOST_SERIALIZATION_SPLIT_FREE(::boost::gregorian::first_kday_after)

namespace boost {
namespace serialization {

/*! Method that does serialization for gregorian::date -- splits to load/save
 */
template<class Archive>                         
inline void serialize(Archive & ar,                               
                      ::boost::gregorian::date & d,
                      const unsigned int file_version)
{
  split_free(ar, d, file_version);              
}                                               

//! Function to save gregorian::date objects using serialization lib
/*! Dates are serialized into a string for transport and storage. 
 *  While it would be more efficient to store the internal
 *  integer used to manipulate the dates, it is an unstable solution.  
 */
template<class Archive>
void save(Archive & ar, 
          const ::boost::gregorian::date & d, 
          unsigned int version)
{
  std::string ds = to_iso_string(d);
  ar & make_nvp("date", ds);
}

//! Function to load gregorian::date objects using serialization lib
/*! Dates are serialized into a string for transport and storage. 
 *  While it would be more efficient to store the internal
 *  integer used to manipulate the dates, it is an unstable solution.  
 */
template<class Archive>
void load(Archive & ar, 
          ::boost::gregorian::date & d, 
          unsigned int version)
{
  std::string ds;
  ar & make_nvp("date", ds);
  d = ::boost::gregorian::from_undelimited_string(ds);
}


//!override needed b/c no default constructor
template<class Archive>
inline void load_construct_data(Archive & ar, 
                                ::boost::gregorian::date* dp, 
                                const unsigned int file_version)
{
  // retrieve data from archive required to construct new 
  // invoke inplace constructor to initialize instance of date
  ::new(dp) ::boost::gregorian::date(::boost::gregorian::not_a_date_time);
}

/**** date_duration ****/

//! Function to save gregorian::date_duration objects using serialization lib
template<class Archive>
void save(Archive & ar, const gregorian::date_duration & dd, 
    	  unsigned int version)
{
  typename gregorian::date_duration::duration_rep dr = dd.get_rep();
  ar & make_nvp("date_duration", dr);
}
//! Function to load gregorian::date_duration objects using serialization lib
template<class Archive>
void load(Archive & ar, gregorian::date_duration & dd, unsigned int version)
{
  typename gregorian::date_duration::duration_rep dr(0);
  ar & make_nvp("date_duration", dr);
  dd = gregorian::date_duration(dr);
}
//!override needed b/c no default constructor
template<class Archive>
inline void load_construct_data(Archive & ar, gregorian::date_duration* dd, 
                                const unsigned int file_version)
{
  ::new(dd) gregorian::date_duration(gregorian::not_a_date_time);
}

/**** date_duration::duration_rep (most likely int_adapter) ****/

//! helper unction to save date_duration objects using serialization lib
template<class Archive>
void save(Archive & ar, const gregorian::date_duration::duration_rep & dr, 
    	  unsigned int version)
{
  typename gregorian::date_duration::duration_rep::int_type it = dr.as_number();
  ar & make_nvp("date_duration: duration_rep", it);
}
//! helper function to load date_duration objects using serialization lib
template<class Archive>
void load(Archive & ar, gregorian::date_duration::duration_rep & dr, unsigned int version)
{
  typename gregorian::date_duration::duration_rep::int_type it(0);
  ar & make_nvp("date_duration: duration_rep", it);
  dr = gregorian::date_duration::duration_rep::int_type(it);
}
//!override needed b/c no default constructor
template<class Archive>
inline void load_construct_data(Archive & ar, gregorian::date_duration::duration_rep* dr, 
                                const unsigned int file_version)
{
  ::new(dr) gregorian::date_duration::duration_rep(0);
}

/**** date_period ****/

//! Function to save gregorian::date_period objects using serialization lib
/*! date_period objects are broken down into 2 parts for serialization:
 * the begining date object and the end date object
 */
template<class Archive>
void save(Archive & ar, const gregorian::date_period& dp, 
    	  unsigned int version)
{
  typename gregorian::date d1 = dp.begin();
  typename gregorian::date d2 = dp.end();
  ar & make_nvp("date_period: begin date", d1);
  ar & make_nvp("date_period: end date", d2);
}
//! Function to load gregorian::date_period objects using serialization lib
/*! date_period objects are broken down into 2 parts for serialization:
 * the begining date object and the end date object
 */
template<class Archive>
void load(Archive & ar, gregorian::date_period& dp, unsigned int version)
{
  typename gregorian::date d1(gregorian::not_a_date_time);
  typename gregorian::date d2(gregorian::not_a_date_time);
  ar & make_nvp("date_period: begin date", d1);
  ar & make_nvp("date_period: end date", d2);
  dp = gregorian::date_period(d1,d2);
}
//!override needed b/c no default constructor
template<class Archive>
inline void load_construct_data(Archive & ar, gregorian::date_period* dp, 
                                const unsigned int file_version)
{
  typename gregorian::date d(gregorian::not_a_date_time);
  typename gregorian::date_duration dd(1);
  ::new(dp) gregorian::date_period(d,dd);
}

/**** greg_month ****/

//! Function to save gregorian::greg_month objects using serialization lib
template<class Archive>
void save(Archive & ar, const gregorian::greg_month& gm, 
    	  unsigned int version)
{
  unsigned short us = gm.as_number();
  ar & make_nvp("greg_month", us);
}
//! Function to load gregorian::greg_month objects using serialization lib
template<class Archive>
void load(Archive & ar, gregorian::greg_month& gm, unsigned int version)
{
  unsigned short us;
  ar & make_nvp("greg_month", us);
  gm = gregorian::greg_month(us);
}
//!override needed b/c no default constructor
template<class Archive>
inline void load_construct_data(Archive & ar, gregorian::greg_month* gm, 
                                const unsigned int file_version)
{
  ::new(gm) gregorian::greg_month(1);
}

/**** greg_day ****/

//! Function to save gregorian::greg_day objects using serialization lib
template<class Archive>
void save(Archive & ar, const gregorian::greg_day& gd, 
    	  unsigned int version)
{
  unsigned short us = gd.as_number();
  ar & make_nvp("greg_day", us);
}
//! Function to load gregorian::greg_day objects using serialization lib
template<class Archive>
void load(Archive & ar, gregorian::greg_day& gd, unsigned int version)
{
  unsigned short us;
  ar & make_nvp("greg_day", us);
  gd = gregorian::greg_day(us);
}
//!override needed b/c no default constructor
template<class Archive>
inline void load_construct_data(Archive & ar, gregorian::greg_day* gd, 
                                const unsigned int file_version)
{
  ::new(gd) gregorian::greg_day(1);
}

/**** greg_weekday ****/

//! Function to save gregorian::greg_weekday objects using serialization lib
template<class Archive>
void save(Archive & ar, const gregorian::greg_weekday& gd, 
    	  unsigned int version)
{
  unsigned short us = gd.as_number();
  ar & make_nvp("greg_weekday", us);
}
//! Function to load gregorian::greg_weekday objects using serialization lib
template<class Archive>
void load(Archive & ar, gregorian::greg_weekday& gd, unsigned int version)
{
  unsigned short us;
  ar & make_nvp("greg_weekday", us);
  gd = gregorian::greg_weekday(us);
}
//!override needed b/c no default constructor
template<class Archive>
inline void load_construct_data(Archive & ar, gregorian::greg_weekday* gd, 
                                const unsigned int file_version)
{
  ::new(gd) gregorian::greg_weekday(1);
}

/**** date_generators ****/

/**** partial_date ****/

//! Function to save gregorian::partial_date objects using serialization lib
/*! partial_date objects are broken down into 2 parts for serialization:
 * the day (typically greg_day) and month (typically greg_month) objects
 */
template<class Archive>
void save(Archive & ar, const gregorian::partial_date& pd, 
    	  unsigned int version)
{
  ar & make_nvp("partial_date: day", pd.day());
  ar & make_nvp("partial_date: month", pd.month());
}
//! Function to load gregorian::partial_date objects using serialization lib
/*! partial_date objects are broken down into 2 parts for serialization:
 * the day (greg_day) and month (greg_month) objects
 */
template<class Archive>
void load(Archive & ar, gregorian::partial_date& pd, unsigned int version)
{
  gregorian::greg_day gd(1);
  gregorian::greg_month gm(1);
  ar & make_nvp("partial_date: day", gd);
  ar & make_nvp("partial_date: month", gm);
  pd = gregorian::partial_date(gd,gm);
}
//!override needed b/c no default constructor
template<class Archive>
inline void load_construct_data(Archive & ar, gregorian::partial_date* pd, 
                                const unsigned int file_version)
{
  gregorian::greg_month gm(1);
  gregorian::greg_day gd(1);
  ::new(pd) gregorian::partial_date(gd,gm);
}

/**** nth_kday_of_month ****/

//! Function to save nth_day_of_the_week_in_month objects using serialization lib
/*! nth_day_of_the_week_in_month  objects are broken down into 3 parts for 
 * serialization: the week number, the day of the week, and the month
 */
template<class Archive>
void save(Archive & ar, const gregorian::nth_kday_of_month& nkd, 
    	  unsigned int version)
{
  ar & make_nvp("nth_kday_of_month: week_num", nkd.nth_week());
  ar & make_nvp("nth_kday_of_month: day_of_week", nkd.day_of_week());
  ar & make_nvp("nth_kday_of_month: month", nkd.month());
}
//! Function to load nth_day_of_the_week_in_month objects using serialization lib
/*! nth_day_of_the_week_in_month  objects are broken down into 3 parts for 
 * serialization: the week number, the day of the week, and the month
 */
template<class Archive>
void load(Archive & ar, gregorian::nth_kday_of_month& nkd, unsigned int version)
{
  typename gregorian::nth_kday_of_month::week_num wn(gregorian::nth_kday_of_month::first);
  typename gregorian::nth_kday_of_month::day_of_week_type d(gregorian::Monday);
  typename gregorian::nth_kday_of_month::month_type m(gregorian::Jan);
  ar & make_nvp("nth_kday_of_month: week_num", wn);
  ar & make_nvp("nth_kday_of_month: day_of_week", d);
  ar & make_nvp("nth_kday_of_month: month", m);
  
  nkd = gregorian::nth_kday_of_month(wn,d,m);
}
//!override needed b/c no default constructor
template<class Archive>
inline void load_construct_data(Archive & ar, 
    				gregorian::nth_kday_of_month* nkd, 
                                const unsigned int file_version)
{
  // values used are not significant
  ::new(pd) gregorian::nth_kday_of_month(gregorian::nth_kday_of_month::first,
      					 gregorian::Monday,gregorian::Jan);
}

/**** first_kday_of_month ****/

//! Function to save first_day_of_the_week_in_month objects using serialization lib
/*! first_day_of_the_week_in_month objects are broken down into 2 parts for 
 * serialization: the day of the week, and the month
 */
template<class Archive>
void save(Archive & ar, const gregorian::first_kday_of_month& fkd, 
    	  unsigned int version)
{
  ar & make_nvp("first_kday_of_month: day_of_week", fkd.day_of_week());
  ar & make_nvp("first_kday_of_month: month", fkd.month());
}
//! Function to load first_day_of_the_week_in_month objects using serialization lib
/*! first_day_of_the_week_in_month objects are broken down into 2 parts for 
 * serialization: the day of the week, and the month
 */
template<class Archive>
void load(Archive & ar, gregorian::first_kday_of_month& fkd, unsigned int version)
{
  typename gregorian::first_kday_of_month::day_of_week_type d(gregorian::Monday);
  typename gregorian::first_kday_of_month::month_type m(gregorian::Jan);
  ar & make_nvp("first_kday_of_month: day_of_week", d);
  ar & make_nvp("first_kday_of_month: month", m);
  
  fkd = gregorian::first_kday_of_month(d,m);
}
//!override needed b/c no default constructor
template<class Archive>
inline void load_construct_data(Archive & ar, 
    				gregorian::first_kday_of_month* fkd, 
                                const unsigned int file_version)
{
  // values used are not significant
  ::new(pd) gregorian::first_kday_of_month(gregorian::Monday,gregorian::Jan);
}

/**** last_kday_of_month ****/

//! Function to save last_day_of_the_week_in_month objects using serialization lib
/*! last_day_of_the_week_in_month objects are broken down into 2 parts for 
 * serialization: the day of the week, and the month
 */
template<class Archive>
void save(Archive & ar, const gregorian::last_kday_of_month& lkd, 
    	  unsigned int version)
{
  ar & make_nvp("last_kday_of_month: day_of_week", lkd.day_of_week());
  ar & make_nvp("last_kday_of_month: month", lkd.month());
}
//! Function to load last_day_of_the_week_in_month objects using serialization lib
/*! last_day_of_the_week_in_month objects are broken down into 2 parts for 
 * serialization: the day of the week, and the month
 */
template<class Archive>
void load(Archive & ar, gregorian::last_kday_of_month& lkd, unsigned int version)
{
  typename gregorian::last_kday_of_month::day_of_week_type d(gregorian::Monday);
  typename gregorian::last_kday_of_month::month_type m(gregorian::Jan);
  ar & make_nvp("last_kday_of_month: day_of_week", d);
  ar & make_nvp("last_kday_of_month: month", m);
  
  lkd = gregorian::last_kday_of_month(d,m);
}
//!override needed b/c no default constructor
template<class Archive>
inline void load_construct_data(Archive & ar, 
    				gregorian::last_kday_of_month* lkd, 
                                const unsigned int file_version)
{
  // values used are not significant
  ::new(pd) gregorian::last_kday_of_month(gregorian::Monday,gregorian::Jan);
}

/**** first_kday_before ****/

//! Function to save first_day_of_the_week_before objects using serialization lib
template<class Archive>
void save(Archive & ar, const gregorian::first_kday_before& fkdb, 
    	  unsigned int version)
{
  ar & make_nvp("first_kday_before: day_of_week", fkdb.day_of_week());
}
//! Function to load first_day_of_the_week_before objects using serialization lib
template<class Archive>
void load(Archive & ar, gregorian::first_kday_before& fkdb, unsigned int version)
{
  typename gregorian::first_kday_before::day_of_week_type d(gregorian::Monday);
  ar & make_nvp("first_kday_before: day_of_week", d);
  
  fkdb = gregorian::first_kday_before(d);
}
//!override needed b/c no default constructor
template<class Archive>
inline void load_construct_data(Archive & ar, 
    				gregorian::first_kday_before* fkdb, 
                                const unsigned int file_version)
{
  // values used are not significant
  ::new(pd) gregorian::first_kday_before(gregorian::Monday);
}

/**** first_kday_after ****/

//! Function to save first_day_of_the_week_after objects using serialization lib
template<class Archive>
void save(Archive & ar, const gregorian::first_kday_after& fkda, 
    	  unsigned int version)
{
  ar & make_nvp("first_kday_after: day_of_week", fkda.day_of_week());
}
//! Function to load first_day_of_the_week_after objects using serialization lib
template<class Archive>
void load(Archive & ar, gregorian::first_kday_after& fkda, unsigned int version)
{
  typename gregorian::first_kday_after::day_of_week_type d(gregorian::Monday);
  ar & make_nvp("first_kday_after: day_of_week", d);
  
  fkda = gregorian::first_kday_after(d);
}
//!override needed b/c no default constructor
template<class Archive>
inline void load_construct_data(Archive & ar, 
    				gregorian::first_kday_after* fkda, 
                                const unsigned int file_version)
{
  // values used are not significant
  ::new(pd) gregorian::first_kday_after(gregorian::Monday);
}

} // namespace serialization
} // namespace boost

#endif
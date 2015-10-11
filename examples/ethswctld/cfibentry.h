/*
 * cfibentry.h
 *
 *  Created on: 15.07.2013
 *      Author: andreas
 */

#ifndef CFIBENTRY_H_
#define CFIBENTRY_H_ 1

#include <ostream>
#include <inttypes.h>

#include <rofl/common/cthread.hpp>
#include <rofl/common/caddress.h>
#include <rofl/common/crofbase.h>
#include <rofl/common/crofdpt.h>
#include <rofl/common/logging.h>

namespace rofl {
namespace examples {
namespace ethswctld {

namespace exceptions {

/**
 * @ingroup common_howto_ethswctld
 *
 * @brief	Base class for all exceptions thrown by class cfibtable.
 */
class eFibBase			: public std::runtime_error {
public:
	eFibBase(const std::string& __arg) : std::runtime_error(__arg) {};
};

/**
 * @ingroup common_howto_ethswctld
 *
 * @brief	Invalid parameter specified.
 */
class eFibInval			: public eFibBase {
public:
	eFibInval(const std::string& __arg) : eFibBase(__arg) {};
};

/**
 * @ingroup common_howto_ethswctld
 *
 * @brief	Element not found.
 */
class eFibNotFound		: public eFibBase {
public:
	eFibNotFound(const std::string& __arg) : eFibBase(__arg) {};
};

}; // namespace exceptions


class cfibentry; // forward declaration


/**
 * @ingroup common_howto_ethswctld
 * @interface cfibentry_env
 *
 * @brief	Defines the environment expected by an instance of class cfibentry.
 */
class cfibentry_env {
	friend class cfibentry;
public:

	/**
	 * @brief	cfibentry_env destructor
	 */
	virtual
	~cfibentry_env()
	{};

protected:

	/**
	 * @brief	Called when this FIB entry has expired.
	 */
	virtual void
	fib_timer_expired(
			const rofl::caddress_ll& hwaddr) = 0;

	/**
	 * @brief	Called when this FIB entry's port number has been updated.
	 */
	virtual void
	fib_port_update(
			const cfibentry& entry) = 0;
};



/**
 * @ingroup common_howto_ethswctld
 *
 * @brief	Stores a FIB entry mapping host hwaddr to switch port number.
 *
 * This class stores an entry for a Forwarding Information Base (FIB)
 * and stores a single mapping between a host hardware address used
 * for ethernet based communication and the physical port on the datapath
 * element pointing towards this station. An entry is a soft-state
 * entity running a timer of length 60 seconds. Once expired, a notification
 * method is called in cfibentry's environment to indicate that is entry
 * has become stale. For timer support, cfibentry derives from class
 * rofl::ciosrv.
 */
class cfibentry : public rofl::cthread_env {
public:

	/**
	 * @brief	cfibentry constructor
	 *
	 * @param env environment for this cfibentry instance
	 * @param dptid rofl-common's internal datapath handle
	 * @param hwaddr ethernet hardware address used by station
	 * @param port_no OpenFlow port number of port pointing towards the station
	 */
	cfibentry(
			cfibentry_env *env,
			const rofl::cdptid& dptid,
			const rofl::caddress_ll& hwaddr,
			uint32_t port_no);

	/**
	 * @brief	cfibentry destructor
	 */
	virtual
	~cfibentry();

public:

	/**
	 * @name	Access to class parameters
	 */

	/**@{*/

	/**
	 * @brief	Returns port number stored for this host.
	 *
	 * @return OpenFlow port number of port pointing towards station
	 */
	uint32_t
	get_port_no() const
	{ return port_no; };

	/**
	 * @brief	Update port number stored for this host.
	 *
	 * @param port_no new OpenFlow port number of port pointing towards station
	 */
	void
	set_port_no(uint32_t port_no);

	/**
	 * @brief	Returns ethernet hardware address identifying this host.
	 *
	 * @return host ethernet hardware address
	 */
	const rofl::caddress_ll&
	get_hwaddr() const
	{ return hwaddr; };

	/**@}*/

private:

	/**
	 * @brief	Handle timeout events generated by class rofl::ciosrv.
	 *
	 * This method is overwritten from class rofl::ciosrv and is
	 * called whenever a previously registered timer event has occured.
	 * The 'opaque' and 'data' values are the ones used when calling method
	 * rofl::ciosrv::register_timer().
	 *
	 * @param opaque timer type that can be freely chosen by this class
	 * @param data pointer to an opaque data segment or NULL
	 *
	 * @see rofl::ciosrv
	 */
	virtual void
	handle_timeout(
			cthread& thread, uint32_t timer_id, const std::list<unsigned int>& ttypes);

public:

	/**
	 * @brief	output operator for class cfibentry
	 */
	friend std::ostream&
	operator<< (std::ostream& os, cfibentry const& entry) {
		os << rofl::indent(0) << "<cfibentry portno: "
				<< entry.port_no << " >" << std::endl;
		rofl::indent i(2);
		os << entry.hwaddr;
		return os;
	};

private:

	enum cfibentry_timer_t {
		TIMER_ID_ENTRY_EXPIRED = 1,
	};

	// pointer to class defining the environment for this cfibentry
	cfibentry_env*              env;
	// rofl-common's internal datapath handle
	rofl::cdptid                dptid;

	// OpenFlow port number pointing towards port
	uint32_t                    port_no;
	// station ethernet hardware address
	rofl::caddress_ll           hwaddr;

	// timeout value in seconds for this FIB entry
	time_t                      entry_timeout;
	static const time_t         CFIBENTRY_DEFAULT_TIMEOUT;

	rofl::cthread               thread;
};

}; // namespace ethswctld
}; // namespace examples
}; // namespace rofl

#endif /* CFIBENTRY_H_ */

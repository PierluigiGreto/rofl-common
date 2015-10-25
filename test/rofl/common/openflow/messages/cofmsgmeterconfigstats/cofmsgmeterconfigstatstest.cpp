/*
 * cofmsgmeterconfigstatstest.cpp
 *
 *  Created on: Apr 26, 2015
 *      Author: andi
 */

#include <stdlib.h>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "cofmsgmeterconfigstatstest.hpp"

using namespace rofl::openflow;

CPPUNIT_TEST_SUITE_REGISTRATION( cofmsgmeterconfigstatstest );

void
cofmsgmeterconfigstatstest::setUp()
{}



void
cofmsgmeterconfigstatstest::tearDown()
{}



void
cofmsgmeterconfigstatstest::testRequest13()
{
	testRequest(
			rofl::openflow13::OFP_VERSION,
			rofl::openflow13::OFPT_MULTIPART_REQUEST,
			0xa1a2a3a4,
			rofl::openflow13::OFPMP_METER_CONFIG,
			0xb1b2);
}



void
cofmsgmeterconfigstatstest::testRequest(
		uint8_t version, uint8_t type, uint32_t xid, uint16_t stats_type, uint16_t stats_flags)
{
	rofl::openflow::cofmsg_meter_config_stats_request msg1(version, xid, stats_flags);
	rofl::openflow::cofmsg_meter_config_stats_request msg2;
	rofl::cmemory mem(msg1.length());

	try {
		msg1.pack(mem.somem(), mem.length());
		msg2.unpack(mem.somem(), mem.length());

		CPPUNIT_ASSERT(msg2.get_version() == version);
		CPPUNIT_ASSERT(msg2.get_type() == type);
		CPPUNIT_ASSERT(msg2.get_length() == msg1.length());
		CPPUNIT_ASSERT(msg2.get_xid() == xid);
		CPPUNIT_ASSERT(msg2.get_stats_type() == stats_type);
		CPPUNIT_ASSERT(msg2.get_stats_flags() == stats_flags);
		CPPUNIT_ASSERT(msg2.get_meter_config().get_version() == version);

	} catch (...) {
		std::cerr << ">>> request <<<" << std::endl << msg1;
		std::cerr << ">>> memory <<<" << std::endl << mem;
		std::cerr << ">>> clone <<<" << std::endl << msg2;
		throw;
	}

}



void
cofmsgmeterconfigstatstest::testReply13()
{
	testReply(
			rofl::openflow13::OFP_VERSION,
			rofl::openflow13::OFPT_MULTIPART_REPLY,
			0xa1a2a3a4,
			rofl::openflow13::OFPMP_METER_CONFIG,
			0xb1b2);
}



void
cofmsgmeterconfigstatstest::testReply(
		uint8_t version, uint8_t type, uint32_t xid, uint16_t stats_type, uint16_t stats_flags)
{
	rofl::openflow::cofmsg_meter_config_stats_reply msg1(version, xid, stats_flags);
	rofl::openflow::cofmsg_meter_config_stats_reply msg2;
	rofl::cmemory mem(msg1.length());

	try {
		msg1.pack(mem.somem(), mem.length());
		msg2.unpack(mem.somem(), mem.length());

		CPPUNIT_ASSERT(msg2.get_version() == version);
		CPPUNIT_ASSERT(msg2.get_type() == type);
		CPPUNIT_ASSERT(msg2.get_length() == msg1.length());
		CPPUNIT_ASSERT(msg2.get_xid() == xid);
		CPPUNIT_ASSERT(msg2.get_stats_type() == stats_type);
		CPPUNIT_ASSERT(msg2.get_stats_flags() == stats_flags);
		CPPUNIT_ASSERT(msg2.get_meter_config_array().get_version() == version);

	} catch (...) {
		std::cerr << ">>> request <<<" << std::endl << msg1;
		std::cerr << ">>> memory <<<" << std::endl << mem;
		std::cerr << ">>> clone <<<" << std::endl << msg2;
		throw;
	}
}



void
cofmsgmeterconfigstatstest::testRequestParser13()
{
	uint8_t version = rofl::openflow13::OFP_VERSION;
	uint32_t port_no = 0xd0d1d2d3;

	cofmeter_config_request meterconfig(
			version,
			port_no);

	size_t msglen = sizeof(struct rofl::openflow13::ofp_multipart_request) + meterconfig.length();
	size_t memlen = msglen + /*test overhead*/4;

	rofl::cmemory mem(memlen);
	struct rofl::openflow13::ofp_multipart_request* stats =
			(struct rofl::openflow13::ofp_multipart_request*)(mem.somem());

	stats->header.version = version;
	stats->header.type = rofl::openflow13::OFPT_MULTIPART_REQUEST;
	stats->header.xid = htobe32(0xa0a1a2a3);
	stats->header.length = htobe16(0);
	stats->type = htobe16(rofl::openflow13::OFPMP_METER_CONFIG);
	stats->flags = htobe16(0xb1b2);
	meterconfig.pack(stats->body, meterconfig.length());


	for (unsigned int i = 1; i < msglen; i++) {
		rofl::openflow::cofmsg_meter_config_stats_request msg;
		try {
			stats->header.length = htobe16(i);
			msg.unpack(mem.somem(), i);

			std::cerr << ">>> testing length values (len: " << i << ") <<< " << std::endl;
			std::cerr << "[FAILURE] unpack() no exception seen" << std::endl;
			std::cerr << ">>> request <<<" << std::endl << msg;
			std::cerr << ">>> memory <<<" << std::endl << mem;

			/* unpack() Must yield an exception */
			CPPUNIT_ASSERT(false);

		} catch (rofl::exception& e) {
			CPPUNIT_ASSERT(i < msglen);
		};
	}

	for (unsigned int i = msglen; i == msglen; i++) {
		rofl::openflow::cofmsg_meter_config_stats_request msg;
		try {
			stats->header.length = htobe16(i);
			msg.unpack(mem.somem(), i);

		} catch (rofl::exception& e) {

			std::cerr << ">>> testing length values (len: " << i << ") <<< " << std::endl;
			std::cerr << "[FAILURE] unpack() exception seen" << std::endl;
			std::cerr << ">>> request <<<" << std::endl << msg;
			std::cerr << ">>> memory <<<" << std::endl << mem;

			/* unpack() Must yield an exception */
			CPPUNIT_ASSERT(false);
		}
	}

	for (unsigned int i = msglen + 1; i < memlen; i++) {
		rofl::openflow::cofmsg_meter_config_stats_request msg;
		try {
			stats->header.length = htobe16(i);
			msg.unpack(mem.somem(), i);

		} catch (rofl::eBadRequestBadLen& e) {
			std::cerr << ">>> testing length values (len: " << i << ") <<< " << std::endl;
			std::cerr << "[FAILURE] unpack() exception seen" << std::endl;
			std::cerr << ">>> request <<<" << std::endl << msg;
			std::cerr << ">>> memory <<<" << std::endl << mem;

			CPPUNIT_ASSERT(false);
		}
	}}



void
cofmsgmeterconfigstatstest::testReplyParser13()
{
	uint8_t version = rofl::openflow13::OFP_VERSION;
	uint16_t flags = 0xc5c6;
	uint32_t meter_id = 0xd0d1d2d3;

	cofmeterconfigarray meterconfig(version);
	for (unsigned int i = 0; i < 4; i++) {
		meterconfig.set_meter_config(i).set_flags(flags);
		meterconfig.set_meter_config(i).set_meter_id(meter_id);
	}

	size_t msglen = sizeof(struct rofl::openflow13::ofp_multipart_request);
	size_t memlen = msglen + meterconfig.length() + /*test overhead*/4;

	rofl::cmemory mem(memlen);
	struct rofl::openflow13::ofp_multipart_request* stats =
			(struct rofl::openflow13::ofp_multipart_request*)(mem.somem());

	stats->header.version = version;
	stats->header.type = rofl::openflow13::OFPT_MULTIPART_REPLY;
	stats->header.xid = htobe32(0xa0a1a2a3);
	stats->header.length = htobe16(0);
	stats->type = htobe16(rofl::openflow13::OFPMP_METER_CONFIG);
	stats->flags = htobe16(0xb1b2);
	meterconfig.pack(stats->body, meterconfig.length());


	for (unsigned int i = 1; i < msglen; i++) {
		rofl::openflow::cofmsg_meter_config_stats_reply msg;
		try {
			stats->header.length = htobe16(i);
			msg.unpack(mem.somem(), i);

			std::cerr << ">>> testing length values (len: " << i << ") <<< " << std::endl;
			std::cerr << "[FAILURE] unpack() no exception seen" << std::endl;
			std::cerr << ">>> reply <<<" << std::endl << msg;
			std::cerr << ">>> memory <<<" << std::endl << mem;

			/* unpack() Must yield an axception */
			CPPUNIT_ASSERT(false);

		} catch (rofl::eBadRequestBadLen& e) {
			CPPUNIT_ASSERT(i < msglen);
		}
	}

	for (unsigned int i = msglen; i == msglen; i++) {
		rofl::openflow::cofmsg_meter_config_stats_reply msg;
		try {
			stats->header.length = htobe16(i);
			msg.unpack(mem.somem(), i);

		} catch (rofl::eBadRequestBadLen& e) {

			std::cerr << ">>> testing length values (len: " << i << ") <<< " << std::endl;
			std::cerr << "[FAILURE] unpack() exception seen" << std::endl;
			std::cerr << ">>> reply <<<" << std::endl << msg;
			std::cerr << ">>> memory <<<" << std::endl << mem;

			/* unpack() Must yield an axception */
			CPPUNIT_ASSERT(false);
		}
	}

	for (unsigned int i = msglen + 1; i < memlen; i++) {
		rofl::openflow::cofmsg_meter_config_stats_reply msg;
		try {
			stats->header.length = htobe16(i);
			msg.unpack(mem.somem(), i);

		} catch (rofl::eBadRequestBadLen& e) {
			std::cerr << ">>> testing length values (len: " << i << ") <<< " << std::endl;
			std::cerr << "[FAILURE] unpack() exception seen" << std::endl;
			std::cerr << ">>> reply <<<" << std::endl << msg;
			std::cerr << ">>> memory <<<" << std::endl << mem;

			CPPUNIT_ASSERT(false);
		}
	}
}




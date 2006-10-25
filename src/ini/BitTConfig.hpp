
#pragma once

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

class BitTConfig
{
public:
	BitTConfig() :
		maxConnections(-1),
		maxUploads(-1),
		downRate(-1),
		upRate(-1),
		portFrom(6881),
		portTo(6889)
	{}
	
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{	
		ar & BOOST_SERIALIZATION_NVP(maxConnections);
		ar & BOOST_SERIALIZATION_NVP(maxUploads);
		ar & BOOST_SERIALIZATION_NVP(downRate);
		ar & BOOST_SERIALIZATION_NVP(upRate);
		ar & BOOST_SERIALIZATION_NVP(portFrom);
		ar & BOOST_SERIALIZATION_NVP(portTo);
	}
	
	friend class BitTorrentOptions;
	friend class HaliteWindow;

private:
	int maxConnections;
	int maxUploads;
	
	float downRate;
	float upRate;
	
	int portFrom;
	int portTo;
};

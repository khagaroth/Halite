
//         Copyright E�in O'Callaghan 2006 - 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#define HAL_EVENT_BEGIN 				81000
#define HAL_EVENT_EXP                   HAL_EVENT_BEGIN + 1
#define HAL_EVENT_XML_EXP               HAL_EVENT_BEGIN + 2
#define HAL_EVENT_UNICODE_EXP           HAL_EVENT_BEGIN + 3
#define HAL_EVENT_DEBUG                 HAL_EVENT_BEGIN + 4
#define HAL_EVENT_UNCLASSIFIED          HAL_EVENT_BEGIN + 5
#define HAL_EVENT_PEER                  HAL_EVENT_BEGIN + 6
#define HAL_EVENT_TRACKER               HAL_EVENT_BEGIN + 7
#define HAL_EVENT_TORRENTEXP            HAL_EVENT_BEGIN + 8
#define HAL_EVENT_INVTORRENT            HAL_EVENT_BEGIN + 9

#ifndef RC_INVOKED

#include <string>
#include <vector>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/smart_ptr.hpp>

#include "global/wtl_app.hpp"
#include "global/string_conv.hpp"

#include "halTorrent.hpp"

namespace hal 
{

class Event
{
public:
	enum eventLevel { debug, info, warning, critical, fatal, none };
	
	enum codes {
		noEvent = 0,
		unclassified = HAL_EVENT_UNCLASSIFIED,
		debugEvent = HAL_EVENT_DEBUG,
		invalidTorrent = HAL_EVENT_INVTORRENT,
		torrentException = HAL_EVENT_TORRENTEXP,
		generalException = HAL_EVENT_EXP,
		xmlException = HAL_EVENT_XML_EXP,
		unicodeException = HAL_EVENT_UNICODE_EXP,
		peer = HAL_EVENT_PEER,
		tracker = HAL_EVENT_TRACKER,
		infoCode
	};
	
	static std::wstring eventLevelToStr(eventLevel);	
	void post(boost::shared_ptr<EventDetail> event);
	
	boost::signals::connection attach(boost::function<void (boost::shared_ptr<EventDetail>)> fn)
	{
		return event_signal_.connect(fn);
	}
	
private:
	boost::signal<void (boost::shared_ptr<EventDetail>)> event_signal_;
};

Event& event();

class EventDetail
{
public:
	EventDetail(Event::eventLevel l, boost::posix_time::ptime t, Event::codes c) :
		level_(l),
		timeStamp_(t),
		code_(c)
	{}
	virtual ~EventDetail() 
	{}
	
	virtual std::wstring msg()
	{
		return (boost::wformat(L"Code %1%") % code()).str();
	}

	Event::eventLevel level() { return level_; }
	boost::posix_time::ptime timeStamp() { return timeStamp_; }
	Event::codes code() { return code_; }
	
private:	
	Event::eventLevel level_;
	boost::posix_time::ptime timeStamp_;
	Event::codes code_;
};

class EventLibtorrent : public EventDetail
{
public:
	EventLibtorrent(Event::eventLevel l, boost::posix_time::ptime t, Event::codes c, std::wstring m) :
		EventDetail(l, t, c),
		msg_(m)
	{}
	
	virtual std::wstring msg()
	{
		return (boost::wformat(hal::app().res_wstr(code())) % msg_).str();
	}
	
private:
	std::wstring msg_;
};

class EventGeneral : public EventDetail
{
public:
	EventGeneral(Event::eventLevel l, Event::codes c, std::wstring m) :
		EventDetail(l, boost::posix_time::second_clock::universal_time(), c),
		msg_(m)
	{}
	
	EventGeneral(Event::eventLevel l, boost::posix_time::ptime t, std::wstring m) :
		EventDetail(l, t, Event::noEvent),
		msg_(m)
	{}
	
	template<typename str_t>
	EventGeneral(Event::eventLevel l, str_t m) :
		EventDetail(l, boost::posix_time::second_clock::universal_time(), Event::noEvent),
		msg_(hal::to_wstr_shim(m))
	{}
	
	template<typename str_t>	
	EventGeneral(Event::eventLevel l, boost::posix_time::ptime t, str_t m) :
		EventDetail(l, t, Event::noEvent),
		msg_(hal::to_wstr_shim(m))
	{}
	
	virtual std::wstring msg()
	{
		if (Event::noEvent != code())
			return (boost::wformat(hal::app().res_wstr(code())) % msg_).str();
		else
			return msg_;
	}
	
private:
	std::wstring msg_;
};

class EventMsg : public EventDetail
{
public:
	template<typename str_t>
	EventMsg(str_t m, Event::eventLevel l=Event::debug, 
		boost::posix_time::ptime t=boost::posix_time::second_clock::universal_time(), Event::codes c=Event::noEvent) :
		EventDetail(l, t, c),
		msg_(hal::to_wstr_shim(m))
	{}
	
	virtual std::wstring msg()
	{
		if (Event::noEvent != code())
			return (boost::wformat(hal::app().res_wstr(code())) % msg_).str();
		else
			return msg_;
	}
	
private:
	std::wstring msg_;
};

class EventPeerAlert : public EventDetail
{
public:
	EventPeerAlert(Event::eventLevel l, boost::posix_time::ptime t, std::wstring m) :
		EventDetail(l, t, Event::peer),
		msg_(m)
	{}
	
	virtual std::wstring msg()
	{
		return (boost::wformat(hal::app().res_wstr(code())) % msg_).str();
	}
	
private:
	std::wstring msg_;
};

class EventXmlException : public EventDetail
{
public:
	EventXmlException(std::wstring e, std::wstring m) :
		EventDetail(Event::warning, boost::posix_time::second_clock::universal_time(), Event::xmlException),
		exp_(e),
		msg_(m)
	{}
	
	virtual std::wstring msg()
	{
		return (boost::wformat(hal::app().res_wstr(HAL_EVENT_XML_EXP)) % exp_ % msg_).str();
	}
	
private:
	std::wstring exp_;
	std::wstring msg_;
};

class EventInvalidTorrent : public EventDetail
{
public:
	template<typename t_str, typename f_str>
	EventInvalidTorrent(Event::eventLevel l, Event::codes code, t_str t, f_str f) :
		EventDetail(l, boost::posix_time::second_clock::universal_time(), code),
		torrent_(hal::to_wstr_shim(t)),
		function_(hal::to_wstr_shim(f))
	{}
	
	virtual std::wstring msg()
	{
		return (boost::wformat(hal::app().res_wstr(code())) % torrent_ % function_).str();
	}
	
private:
	std::wstring function_;
	std::wstring torrent_;
	std::wstring exception_;
};

class EventTorrentException : public EventDetail
{
public:
	template<typename e_str, typename t_str, typename f_str>
	EventTorrentException(Event::eventLevel l, Event::codes code, e_str e, t_str t, f_str f) :
		EventDetail(l, boost::posix_time::second_clock::universal_time(), code),
		torrent_(hal::to_wstr_shim(t)),
		function_(hal::to_wstr_shim(f)),
		exception_(hal::to_wstr_shim(e))
	{}
	
	virtual std::wstring msg()
	{
		return (boost::wformat(hal::app().res_wstr(code())) % torrent_ % exception_ % function_).str();
	}
	
private:
	std::wstring torrent_;
	std::wstring function_;
	std::wstring exception_;
};

class EventStdException : public EventDetail
{
public:
	EventStdException(Event::eventLevel l, const std::exception& e, std::wstring from) :
		EventDetail(l, boost::posix_time::second_clock::universal_time(), Event::generalException),
		exception_(hal::from_utf8(e.what())),
		from_(from)
	{}
	
	virtual std::wstring msg()
	{
		return (boost::wformat(hal::app().res_wstr(code())) % exception_ % from_).str();
	}
	
private:
	std::wstring exception_;
	std::wstring from_;
};

class EventDebug : public EventDetail
{
public:
	EventDebug(Event::eventLevel l, std::wstring msg) :
		EventDetail(l, boost::posix_time::second_clock::universal_time(), Event::debugEvent),
		msg_(msg)
	{}
	
	virtual std::wstring msg()
	{
		return (boost::wformat(hal::app().res_wstr(code())) % msg_).str();
	}
	
private:
	std::wstring msg_;
};

class EventInfo : public EventDetail
{
public:
	EventInfo(std::wstring msg) :
		EventDetail(Event::info, boost::posix_time::second_clock::universal_time(), Event::infoCode),
		msg_(msg)
	{}
	
	virtual std::wstring msg() { return msg_; }
	
private:
	std::wstring msg_;
};

class EventSession : public EventDetail
{

};

}// namespace hal

#endif

#ifndef __MPT_MSTREAMS_HPP__
#define __MPT_MSTREAMS_HPP__

#include <sstream>
#include <maya/MString.h>


namespace mpt
{


// Private namespace
// -----------------

namespace mstream
{

class IHandler
{
public:
	IHandler() { }
	virtual ~IHandler() { }
	virtual void fn(std::ostringstream&) = 0;
};

class CmdHandler  : public IHandler { public: virtual void fn(std::ostringstream&); };
class ErrHandler  : public IHandler { public: virtual void fn(std::ostringstream&); };
class InfoHandler : public IHandler { public: virtual void fn(std::ostringstream&); };
class SrcHandler  : public IHandler { public: virtual void fn(std::ostringstream&); };
class WarnHandler : public IHandler { public: virtual void fn(std::ostringstream&); };

class ConvertToStringToken { public: ConvertToStringToken() {} };
class EndLineToken         { public: EndLineToken()         {} };
class EndToken             { public: EndToken()             {} };

} // namespace mstream


// End tokens
// ----------

extern const mstream::ConvertToStringToken ends;
extern const mstream::EndLineToken         endl;
extern const mstream::EndToken             end;


// Stream
// ------

class Stream
{
public:
	Stream(mstream::IHandler *const handler);
	virtual ~Stream();

	template <typename T>
	Stream& operator<<(const T& obj)
	{
		stream_ << obj;
		return *this;
	}

	Stream& operator<<(mstream::EndLineToken);
	Stream& operator<<(mstream::EndToken);

	MString operator<<(mstream::ConvertToStringToken);

private:
	mstream::IHandler *const handler_;
	std::ostringstream       stream_;
};


// Global stream instances
// -----------------------

extern Stream cmd;
extern Stream err;
extern Stream info;
extern Stream source;
extern Stream str;
extern Stream warn;

} // namespace mpt

#endif  // __MPT_MSTREAMS_HPP__

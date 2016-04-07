#include <maya/MGlobal.h>
#include "mstreams.hpp"


namespace
{

// ========================
// Functions
// ========================

/*
std::string prepareStringArg(const MString& m)
{
	return prepareStringArg(std::string(m.asChar()));
}

std::string prepareStringArg(const std::string& s)
{
	//TODO
	//while (1) {
	//	const size_t start = ans.find("\"");
	//	if (start == std::string::npos) {
	//		return ans;
	//	}
	//	ans.replace(start, start + 1, "\\\"");
	//}
	return std::string(s);
}
*/

} // namespace


namespace mpt
{

// ========================
// Handlers
// ========================

namespace mstream
{

/** Execute given command */
void CmdHandler::fn(std::ostringstream& s)
{
	// TODO
	MGlobal::executeCommand(s.str().c_str());
	s.str("");
}

/** Print error message */
void ErrHandler::fn(std::ostringstream& s)
{
	MGlobal::displayError(s.str().c_str());
	s.str("");
}

/** Print info message */
void InfoHandler::fn(std::ostringstream& s)
{
	MGlobal::displayInfo(s.str().c_str());
	s.str("");
}

/** Source file */
void SrcHandler::fn(std::ostringstream&)
{
	// std::ostringstream cmd;
	// cmd << "source \"" << prepareStringArg(stream.str()) << "\";";
	// MGlobal::displayInfo(MString("cmd=") + cmd.str().c_str());
	// MGlobal::executeCommand(cmd.str().c_str());
	// stream.str("");
}

/** Print warning message */
void WarnHandler::fn(std::ostringstream& s)
{
	MGlobal::displayWarning(s.str().c_str());
}

} // mstream


// ========================
// Stream
// ========================

Stream::Stream(mstream::IHandler *const handler)
: handler_(handler)
, stream_()
{
}

Stream::~Stream()
{
	delete handler_;
}

Stream& Stream::operator<<(mstream::EndLineToken)
{
	stream_ << "\n";
	return (*this) << end;
}

Stream& Stream::operator<<(mstream::EndToken)
{
	if (handler_) {
		handler_->fn(stream_);
	}
	return *this;
}

MString Stream::operator<<(mstream::ConvertToStringToken)
{
	const std::string& s = stream_.str();
	const MString ms(s.c_str(), static_cast<int>(s.length()));
	stream_.str("");
	return ms;
}


// ========================
// Externs
// ========================

const mstream::ConvertToStringToken ends;
const mstream::EndLineToken endl;
const mstream::EndToken end;

Stream cmd(new mstream::CmdHandler());
Stream err(new mstream::ErrHandler());
Stream info(new mstream::InfoHandler());
Stream source(new mstream::SrcHandler());
Stream str(NULL);
Stream warn(new mstream::WarnHandler());

}  // namespace mpt

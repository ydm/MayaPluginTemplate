#include <maya/MFnPlugin.h>
#include "default.hpp"


MStatus initializePlugin(MObject obj)
{
	mpt::info << "Hello, world of plugins! :)" << mpt::endl;
	return MStatus::kSuccess;
}


MStatus uninitializePlugin(MObject obj)
{
	mpt::info << "Goodbye, world of plugins! :(" << mpt::endl;
	return MStatus::kSuccess;
}

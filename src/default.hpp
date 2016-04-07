#ifndef __MPT_DEFAULT_HPP__
#define __MPT_DEFAULT_HPP__

#include <maya/MAngle.h>
#include <maya/MArgDatabase.h>
#include <maya/MArgList.h>
#include <maya/MDGModifier.h>
#include <maya/MDagModifier.h>
#include <maya/MDagPath.h>
#include <maya/MDistance.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnRenderLayer.h>
#include <maya/MFnTransform.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MGL.h>
#include <maya/MGlobal.h>
#include <maya/MItDag.h>
#include <maya/MItSelectionList.h>
#include <maya/MObjectArray.h>
#include <maya/MPointArray.h>
#include <maya/MPxCommand.h>
#include <maya/MPxLocatorNode.h>
#include <maya/MPxNode.h>
#include <maya/MPxSurfaceShape.h>
#include <maya/MPxSurfaceShapeUI.h>
#include <maya/MSelectionList.h>
#include <maya/MString.h>
#include <maya/MSyntax.h>
#include <maya/MVector.h>

#include "mstreams.hpp"
#include "override.hpp"


#define mptCheckOpenGL(exp)                                             \
        do {                                                            \
                exp;                                                    \
                GLenum __error__(glGetError());                         \
                if (__error__ != GL_NO_ERROR) {                         \
                        y::warn << "OpenGL exp "                        \
                                << #exp << " failed: "                  \
                                << gluErrorString(__error__) << y::end; \
                }                                                       \
        } while (0)


#define mptComplainIfNot(status)                                        \
        do {                                                            \
                MStatus __status__(status);                             \
                if (__status__ != MStatus::kSuccess) {                  \
                        y::err << "Error/complain: msg="                \
                               << __status__.errorString()              \
                               << ", exp=" << #status << y::end;        \
                }                                                       \
        } while (0)


#define mptContinueIfNot(status)                                        \
        ({                                                              \
                MStatus __status__(status);                             \
                if (__status__ != MStatus::kSuccess) {                  \
                        y::err << "Error/continue: msg="                \
                               << __status__.errorString()              \
                               << ", exp=" << #status << y::end;        \
                        continue;                                       \
                }                                                       \
        })


#define mptReturnIfNot(status)                                          \
        do {                                                            \
                MStatus __status__(status);                             \
                if (__status__ != MStatus::kSuccess) {                  \
                        y::err << "Error/return: msg="                  \
                               << __status__.errorString()              \
                               << ", exp=" << #status << y::end;        \
                        return __status__;                              \
                }                                                       \
        } while (0)

#define mptReturnIfFalse(x)                                             \
        do {                                                            \
                if (!(x)) {                                             \
                        y::err << "Error/return: exp='" << #x           \
                               << "' is true" << y::end;                \
                        return MStatus::kFailure;                       \
                }                                                       \
        } while (0)


#define mptReturnIfTrue(x)                                              \
        do {                                                            \
                if (x) {                                                \
                        y::err << "Error/return: exp='" << #x           \
                               << "' is true" << y::end;                \
                        return MStatus::kFailure;                       \
                }                                                       \
        } while (0)


#endif  // __MPT_DEFAULT_HPP__

#ifndef __MPT_OVERRIDE_HPP__
#define __MPT_OVERRIDE_HPP__

#ifdef __GNUC__
#  if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ <= 6)
#    define override
#  endif
#endif

#endif  // __MPT_OVERRIDE_HPP__

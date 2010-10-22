AUTOMAKE_OPTIONS = subdir-objects


include $(top_srcdir)/filelist.am


noinst_LIBRARIES = libxyto.a libredshift.a
bin_PROGRAMS     = picogen-pure


AM_CXXFLAGS = -fopenmp -D__STDC_LIMIT_MACROS
AM_CCFLAGS  = -fopenmp
AM_LDFLAGS  = -lgomp

redshiftCPPFlags = $(AM_CPPFLAGS) -I./auxiliary/ -I./renderer/ -I./renderer/redshift/include
libredshift_a_CPPFLAGS     = -DPICOGENLIB $(redshiftCPPFlags)
libredshift_a_CXXFLAGS     = $(AM_CXXFLAGS) -O5 `sdl-config --cflags` -I/usr/include/OpenEXR/ -I/mingw/include/OpenEXR/
libredshift_a_SOURCES      = $(redshiftfiles)
libredshift_a_DEPENDENCIES = libxyto.a

picogen_pure_CPPFLAGS     = $(redshiftCPPFlags)
picogen_pure_CXXFLAGS     = $(libredshift_a_CXXFLAGS) 
picogen_pure_SOURCES      = $(redshiftfiles) $(xytofiles)
picogen_pure_DEPENDENCIES = libxyto.a
picogen_pure_LDADD        =  $(picogen_pure_libs) \
                           -lSDL_image -lnoise -lz \
                           -lIlmImf -lImath -lHalf -lIex -lIlmThread \
                           -lboost_program_options \
                           -lboost_filesystem \
                           -lboost_system \
                           libxyto.a

libxyto_a_SOURCES        = $(xytofiles)
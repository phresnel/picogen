AUTOMAKE_OPTIONS = subdir-objects

include $(top_srcdir)/flags.am
include $(top_srcdir)/filelist.am

noinst_LIBRARIES = libxyto.a libredshift.a
bin_PROGRAMS = picogenpure

libxyto_a_SOURCES     = $(xytofiles)
libredshift_a_SOURCES = $(redshiftfiles)

picogenpure_SOURCES   = $(redshiftfiles) $(xytofiles)
picogenpure_DEPENDENCIES = libxyto.a
picogenpure_LDADD    =  $(picogenpure_libs) \
                        -lSDL_image -lnoise -lz \
                         -lIlmImf -lImath -lHalf -lIex -lIlmThread \
                         -lboost_program_options \
                         -lboost_filesystem \
                         -lboost_system \
                         libxyto.a

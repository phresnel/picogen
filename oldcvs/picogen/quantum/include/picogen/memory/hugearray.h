/***************************************************************************
 *            hugearray.h
 *
 *  Copyright  2008  Sebastian Mach
 *  seb@greenhybrid.net
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 3 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <picogen/compile_info.h>


#ifdef POSIX

#include <sys/mman.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <cstdlib>

#include <iostream>
#include <string>
#include <sstream>

namespace picogen {
    namespace mem {

        // Prerequisites:
        //  1) PODs only: 64bit arrays are hardcore, and so shalt be your types :P
        //  2) can calculate size before creation, but at runtime
        //  3) must support >32 bit addresses
        //  4) from 3) most probably follows: must support paging in some file
        //  5) from the limitations of some filesystems follows: must support paging in multiple files


        enum debug_mode {
            no_debug,
            debug
        };

        enum verbose_mode {
            be_quiet,
            be_verbose
        };


        template <typename T, typename I, debug_mode D, verbose_mode V, typename statistics_counter_type=uint32_t> class huge_array {
            public:
                enum mode_t {
                    mode_undefined,
                    mode_read,
                    mode_write
                };

                typedef T type_t;
                typedef I index_t;

                enum { do_debug = D != no_debug };
                enum { be_verbose = V != be_quiet };

                enum {
                    page_size = 4096,
                    file_size = page_size*1024*64 // 256MiB
                };

                enum {
                    elems_per_page = page_size / sizeof (type_t),
                    elems_per_file = file_size / sizeof (type_t),
                    pages_per_file = file_size / page_size
                };

            private:

                index_t current_page;
                index_t current_file;
                mode_t  current_mode;

                int fd;
                type_t *mmap;

                const index_t size; // size in number-of-elements
                const index_t file_count;
                char **filenames;

                mutable statistics_counter_type num_page_changes;
                mutable statistics_counter_type num_file_changes;

            public:

                huge_array (index_t size, std::string tmpfilebasename)
                    : current_page ((index_t)-1)
                    , current_file ((index_t)-1)
                    , current_mode (mode_undefined)
                    , fd (-1)
                    , mmap (0)
                    , size (size)
                    , file_count (1+(size/elems_per_file))
                    , filenames (0)
                    , num_page_changes (0)
                    , num_file_changes (0)
                {
                    using namespace std;

                    // Output some crap.
                    if (be_verbose) {
                        cout << "huge_array::this:" << this << "\n";
                        cout << " page_size:" << page_size << "\n";
                        cout << " file_size:" << file_size << "(" << ((float)file_size/(1024.0f*1024.0f)) << "MiB)"<< "\n";
                        cout << " file_count:" << file_count << "\n";
                        cout << " elems per page:" << elems_per_page << "\n";
                        cout << " elems per file:" << elems_per_file << "\n";
                        cout << " pages per file:" << pages_per_file << "\n";
                        const float total_bytes = size * sizeof (type_t);
                        cout << " total size: " << (total_bytes/1024.0f) << "KiB, "
                            << (total_bytes/(1024.0f*1024.0f)) << "MiB, "
                            << (total_bytes/(1024.0f*1024.0f*1024.0f)) << "GiB \n";
                        cout << flush;
                    }


                    if (be_verbose) cout << " pumping initial data into paging files..." << endl;

                    type_t *d = new type_t [elems_per_page];
                    filenames = new char * [file_count];
                    for (int i=0; i<file_count; ++i) {
                        string s;
                        stringstream ss;
                        ss << tmpfilebasename << "_" << i;
                        ss >> s;
                        filenames [i] = new char [s.size()+1];
                        sprintf (filenames [i], "%s", s.c_str());
                        cout << "  creating '" << filenames [i] << "'" << endl;

                        int tmpfd = open (filenames [i], O_CREAT|O_WRONLY|O_TRUNC|O_APPEND, S_IRUSR|S_IWUSR);
                        if (tmpfd <= 0) {
                            cerr << "error on creating file '" << filenames [i] << "'" << endl;
                            throw;
                        }
                        int b_ = 0;
                        for (int b=0; b<pages_per_file; ++b) {
                            if (b_ > 10000) {
                                b_ = 0;
                                cout << "  " << (100.0f*(static_cast<float>(b)/static_cast<float>(pages_per_file))) << "%   \r" << flush;
                            }
                            b_++;
                            if (-1 == ::write (tmpfd, &d, sizeof (type_t) * elems_per_page)) {
                                cerr << "meh, an error occured while trying to pump initial data into '" << filenames [i] << "': ";
                                if (EBADF == errno) {
                                    cerr << "EBADF";
                                } else if (EIO == errno) {
                                    cerr << "EIO";
                                } else if (EFAULT == errno) {
                                    cerr << "EFAULT";
                                } else if (EINVAL == errno) {
                                    cerr << "EINVAL";
                                } else {
                                    cerr << "unknown:" << errno;
                                }
                                cerr << endl;
                                throw;
                            }
                        }
                        if (be_verbose) cout << "  sync'ing..." << flush;
                        fsync (tmpfd);
                        if (be_verbose) cout << "closing..." << flush;
                        close (tmpfd);
                        if (be_verbose) cout << "next file" << flush;
                        cout << "\n";
                    }
                    delete [] d;
                }

                ~huge_array () {
                    for (int i=0; i<file_count; ++i) {
                        if (0 != filenames [i]) {
                            unlink (filenames [i]);
                            delete [] filenames [i];
                        }
                    }
                    delete [] filenames;
                }

                index_t get_size () const {
                    return size;
                }

                statistics_counter_type get_page_change_count () const {
                    return num_page_changes;
                }

                statistics_counter_type get_file_change_count () const {
                    return num_file_changes;
                }

                void reset_statistics () {
                    num_page_changes = 0;
                    num_file_changes = 0;
                }

                void switch_mode (mode_t mode) {
                    this->mode = mode;
                }

                type_t &operator [] (index_t index) {
                    using namespace std;

                    if (do_debug) {
                        switch (current_mode) {
                            case mode_read:  break;
                            case mode_write: break;
                            case mode_undefined:
                                cerr << "neither in mode_read- nor in mode_write-mode" << endl;
                                throw;
                        };
                    }

                    const index_t page = index / elems_per_page;
                    const index_t trunc_page = page % pages_per_file;
                    const index_t trunc_index = index % elems_per_page;
                    //const index_t file = page  /

                    if (current_page != page) {

                        // Close page.
                        if (0 != mmap) {
                            if (mode_read==current_mode) {
                                msync (reinterpret_cast <void*> (mmap), page_size, 0);
                            }
                            munmap (reinterpret_cast <void*> (mmap), page_size);
                            mmap = 0;
                        }
                        current_page = page;

                        // Close old/Open new file.
                        const index_t file = page / pages_per_file;

                        if (do_debug) {
                            if (file >= file_count) {
                                cerr << "huge_array:file(" << file << ") >= file_count(" << file_count << ")" << endl;
                                throw;
                            }
                            ++ num_page_changes;
                        }

                        if (current_file != file) {
                            if (do_debug) {
                                ++num_file_changes;
                            }
                            if (fd > 0) {
                                close (fd);
                            }

                            fd = open (filenames [file], (mode_read==current_mode)?(O_RDONLY):(O_RDWR));
                            if (fd <= 0) {
                                cerr << "could not open '" << filenames [file] << "'" << endl;
                                throw;
                            }

                            current_file = file;
                            current_page = page;
                        }

                        // Open page.
                        mmap = reinterpret_cast<type_t*> (::mmap (
                            0, // start
                            page_size,  // len
                            (mode_read==current_mode)?(PROT_READ):(PROT_WRITE),  // prot
                            (mode_read==current_mode)?(MAP_PRIVATE):(MAP_SHARED), // flags
                            fd,
                            page_size*trunc_page
                        ));

                        if (MAP_FAILED == reinterpret_cast <void*> (mmap)) {
                            cerr << "mmap failed:";
                            switch (errno) {
                                case EACCES: cerr << "EACCES"; break;
                                case EAGAIN: cerr << "EAGAIN"; break;
                                case EBADF: cerr << "EBADF"; break;
                                case EINVAL: cerr << "EINVAL"; break;
                                case ENFILE: cerr << "ENFILE"; break;
                                case ENODEV: cerr << "ENODEV"; break;
                                case ENOMEM: cerr << "ENOMEM"; break;
                                case EPERM: cerr << "EPERM"; break;
                                case ETXTBSY: cerr << "ETXTBSY"; break;
                                default: cerr << "unknown:"; break;
                            };
                            cerr << endl;
                            throw;
                        }
                    }
                    //cout << "<" << mmap << "[" << trunc_index << "]" << ">" << flush;
                    return mmap [trunc_index];
                }


                void set_mode (mode_t mode) {
                    if (mode != current_mode) {
                        current_mode = mode;
                    }
                }
        };
    }; // namespace mem
}; // namespace picogen

#else // #ifdef POSIX
# error "the template 'hugearray' is currently only supported on POSIX architectures"
#endif // #ifdef POSIX

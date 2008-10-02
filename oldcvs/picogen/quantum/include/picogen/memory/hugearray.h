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
#include <map>


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
                    mode_write,
                    mode_random
                };

                typedef T type_t;
                typedef I index_t;

                enum { do_debug = D != no_debug };
                enum { be_verbose = V != be_quiet };

                /*
                enum {
                    page_size = 4096*16,
                    file_size = 2048UL*1024UL*1024UL
                };

                enum {
                    elems_per_page = page_size / sizeof (type_t),
                    elems_per_file = file_size / sizeof (type_t),
                    pages_per_file = file_size / page_size
                };*/
                const index_t page_size;
                const index_t file_size;
                const index_t elems_per_page;
                const index_t pages_per_file;
                const index_t elems_per_file;

            private:

                mutable index_t current_page;
                mutable index_t current_file;
                mode_t  current_mode;

                mutable int fd;
                mutable char *mmap;

                const index_t size; // size in number-of-elements
                const index_t file_count;//, page_count;
                char **filenames;

                mutable statistics_counter_type num_page_changes;
                mutable statistics_counter_type num_file_changes;
                mutable statistics_counter_type num_pages_cached;

                struct pagekey_t;
                struct pagecache_t {
                    T *data;
                    uint64_t read_count; // large scale read count
                    uint32_t read_count_sub; // fine grain read count, reset to 0 after some increments
                    pagekey_t key;

                    pagecache_t () : data (0), read_count (0), read_count_sub (0) {};
                    void alloc (I size) { data = reinterpret_cast <T*> (malloc (sizeof (T) * size)); }
                    void free () { if (0 != data) ::free (reinterpret_cast <void*> (data)); data = 0; }
                    void free_no_dealloc () { data = 0; }
                    ~pagecache_t () { free(); }
                };
                struct pagekey_t {
                    pagecache_t *p;
                    pagekey_t (pagecache_t *p) : p(p) {}
                    pagekey_t (const pagekey_t &k) : p(k.p) {}
                    pagekey_t () : p(0) {}
                    void operator = (const pagekey_t& k) { p = k.p; }

                    bool operator < (const pagekey_t &k) const {
                        return p->read_count == k.p->read_count
                            ? p > k.p // lower in memory?
                            : p->read_count < k.p->read_count // more reads?
                        ;
                        // this operator ensures that in a map<> the least important element is at map.begin()
                    }
                };

                mutable pagecache_t *pagecache;
                typedef std::map <pagekey_t, pagecache_t*> cachemap_t;
                mutable cachemap_t cachemap;
                const index_t max_pages_cached;
                const bool infinite_cache;
                const index_t cache_update_granularity;

            public:

                huge_array (index_t size, std::string tmpfilebasename, index_t max_cache_size=0, index_t cache_update_granularity=512, bool do_create_files=true)
                    : page_size (getpagesize())
                    , file_size ((2048UL*1024UL*1024UL))
                    //, file_size (page_size*2048)
                    , elems_per_page ((page_size / sizeof (type_t)))
                    , pages_per_file ((file_size / page_size))
                    , elems_per_file (elems_per_page * pages_per_file)

                    , current_page ((index_t)-1)
                    , current_file ((index_t)-1)
                    , current_mode (mode_undefined)
                    , fd (-1)
                    , mmap (0)
                    , size (size)
                    , file_count (1+(size/elems_per_file))
                    //, page_count (1+(size/elems_per_page))
                    , filenames (0)
                    , num_page_changes (0)
                    , num_file_changes (0)
                    , num_pages_cached (0)
                    , pagecache (new pagecache_t [pages_per_file * file_count])
                    , max_pages_cached (max_cache_size/elems_per_page)
                    , infinite_cache (0 == max_cache_size)
                    , cache_update_granularity (cache_update_granularity)
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


                    if (be_verbose) {
                        if (!do_create_files)
                            cout << " do_create_files has not been specified. use with care!" << endl;
                        else
                            cout << " pumping initial data into paging files..." << endl;
                    }

                    type_t *d = new type_t [elems_per_page];
                    filenames = new char * [file_count];
                    for (unsigned int i=0; i<file_count; ++i) {
                        string s;
                        stringstream ss;
                        ss << tmpfilebasename << i;
                        ss >> s;
                        filenames [i] = new char [s.size()+1];
                        sprintf (filenames [i], "%s", s.c_str());
                        if (be_verbose) {
                            if (do_create_files)
                                cout << "  creating '" << filenames [i] << "'" << endl;
                        }

                        if (do_create_files) {
                            int tmpfd = open (filenames [i], O_CREAT|O_WRONLY|O_TRUNC|O_APPEND, S_IRUSR|S_IWUSR);
                            if (tmpfd <= 0) {
                                cerr << "error on creating file '" << filenames [i] << "'" << endl;
                                throw;
                            }
                            index_t b_ = 0;
                            for (index_t b=0; b<pages_per_file; ++b) {
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
                    }
                    delete [] d;
                }

                ~huge_array () {

                    if (false) {
                        using namespace std;
                        typename cachemap_t::iterator it;
                        for (it = cachemap.begin(); it != cachemap.end(); ++it) {
                            cout << "rc=" << it->second->read_count << ":&=" << (uint32_t)it->second << endl;
                        }
                    }

                    // Close page.
                    if (0 != mmap) {
                        close_and_sync_current_page ();
                    }

                    // Close file.
                    if (fd > 0) {
                        close (fd);
                        fd = -1;
                    }

                    for (unsigned int i=0; i<file_count; ++i) {
                        if (0 != filenames [i]) {
                            unlink (filenames [i]);
                            delete [] filenames [i];
                        }
                    }
                    delete [] filenames;

                    delete [] pagecache;
                }

                index_t get_size () const {
                    return size;
                }

                statistics_counter_type get_page_change_count () const {
                    return num_page_changes;
                }

                statistics_counter_type get_cached_pages_count () const {
                    return num_pages_cached;
                }

                statistics_counter_type get_file_change_count () const {
                    return num_file_changes;
                }

                void reset_statistics () {
                    num_page_changes = 0;
                    num_file_changes = 0;
                }


                void flush_cache () {
                    for (index_t i=0; i<pages_per_file*file_count; ++i) {
                        if (0 != pagecache [i].data)
                            pagecache [i].free ();
                    }
                    num_pages_cached = 0;
                    cachemap.clear ();
                }


                type_t &operator [] (index_t index) {
                    using namespace std;

                    if (do_debug) {
                        switch (current_mode) {
                            case mode_read:  break;
                            case mode_write: break;
                            case mode_random: break;
                            case mode_undefined:
                                cerr << "set_mode() has not been called, yet" << endl;
                                throw;
                        };
                    }

                    const index_t page = index / elems_per_page;
                    const index_t trunc_index = index % elems_per_page;
                    const bool page_changed = current_page != page;

                    if (mode_read == current_mode) {
                        ++pagecache [page].read_count_sub;
                        const bool do_inc_read_count = pagecache [page].read_count_sub > cache_update_granularity;
                        if (do_inc_read_count) {
                            pagecache [page].read_count_sub = 0;
                            pagecache [page].read_count++;
                        }
                        // See if we have cached the current page.
                        if (0 != pagecache [page].data) {
                            if (do_inc_read_count) {
                                cachemap.erase (pagecache [page].key);
                                pagekey_t k (&pagecache [page]);
                                pagecache [page].key = k; // <-- update key
                                cachemap [k] = &(pagecache [page]);
                            }
                            return pagecache [page].data [trunc_index];
                        }
                    }

                    const index_t trunc_page = page % pages_per_file;

                    if (page_changed) {

                        // Close page.
                        if (0 != mmap) {
                            close_and_sync_current_page ();
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

                            fd = open (filenames [file],
                                (mode_read==current_mode)?(O_RDONLY)
                                :(mode_write==current_mode)?(O_WRONLY)
                                :(O_RDWR)
                            );
                            if (fd <= 0) {
                                cerr << "could not open '" << filenames [file] << "'" << endl;
                                throw;
                            }

                            current_file = file;
                            current_page = page;
                        }

                        // Open page.
                        mmap = static_cast<char*> (::mmap (
                            0, // start
                            page_size,  // len
                            (mode_read==current_mode)?(PROT_READ)
                            :(mode_write==current_mode)?(PROT_WRITE)
                            :(PROT_READ|PROT_WRITE),  // prot
                            (mode_read==current_mode)?(MAP_PRIVATE):(MAP_SHARED), // flags
                            fd,
                            page_size*trunc_page
                        ));

                        if (MAP_FAILED == static_cast <void*> (mmap)) {
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

                        // mmap now points onto the current page. See what we can do with the pagecache.
                        if (mode_read == current_mode) {
                            bool do_cache = false;
                            pagekey_t k (&pagecache [page]);
                            if (infinite_cache) { // <-- read: cache until malloc()=0
                                pagecache [page].alloc (elems_per_page);
                            }

                            // if not in infinite_cache_size_mode, check if the page is worth to be cached
                            if ((!infinite_cache && cachemap.size () > max_pages_cached) // cache too full
                                || (infinite_cache && 0 == pagecache [page].data) // malloc failed
                            ) {
                                // first see if there is space left in the cachemap
                                if (k < cachemap.begin()->first) {
                                    // swap arrays
                                    pagecache [page].data = cachemap.begin()->second->data;
                                    cachemap.begin()->second->free_no_dealloc (); // free() cache-page but don't free (data)
                                    cachemap.erase (cachemap.begin());
                                    if (do_debug) { --num_pages_cached; }
                                    do_cache = true;
                                }
                            } else if (!infinite_cache || (infinite_cache && 0 != pagecache [page].data)) {
                                do_cache = true;
                            }
                            if (do_cache) {
                                if (0 == pagecache [page].data) { // if not yet allocated
                                    pagecache [page].alloc (elems_per_page);
                                }
                                if (0 != pagecache [page].data) {
                                    pagecache [page].key = k; // <-- update key
                                    cachemap [k] = &(pagecache [page]);

                                    memcpy (pagecache [page].data, mmap, elems_per_page*sizeof (T));
                                    ++num_pages_cached;

                                    // cached. now close mmap.
                                    close_and_sync_current_page ();
                                    return pagecache [page].data [trunc_index];
                                }
                            }
                        }
                    }
                    return * reinterpret_cast <type_t*> (mmap + trunc_index * sizeof (type_t));
                }


                const type_t &operator [] (index_t index) const {
                    using namespace std;

                    if (do_debug) {
                        switch (current_mode) {
                            case mode_read:  break;
                            case mode_write: break;
                            case mode_random: break;
                            case mode_undefined:
                                cerr << "set_mode() has not been called, yet" << endl;
                                throw;
                        };
                    }

                    const index_t page = index / elems_per_page;
                    const index_t trunc_index = index % elems_per_page;
                    const bool page_changed = current_page != page;

                    if (mode_read == current_mode) {
                        ++pagecache [page].read_count_sub;
                        const bool do_inc_read_count = pagecache [page].read_count_sub > cache_update_granularity;
                        if (do_inc_read_count) {
                            pagecache [page].read_count_sub = 0;
                            pagecache [page].read_count++;
                        }
                        // See if we have cached the current page.
                        if (0 != pagecache [page].data) {
                            if (do_inc_read_count) {
                                cachemap.erase (pagecache [page].key);
                                pagekey_t k (&pagecache [page]);
                                pagecache [page].key = k; // <-- update key
                                cachemap [k] = &(pagecache [page]);
                            }
                            return pagecache [page].data [trunc_index];
                        }
                    }

                    const index_t trunc_page = page % pages_per_file;

                    if (page_changed) {

                        // Close page.
                        if (0 != mmap) {
                            close_and_sync_current_page ();
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

                            fd = open (filenames [file],
                                (mode_read==current_mode)?(O_RDONLY)
                                :(mode_write==current_mode)?(O_WRONLY)
                                :(O_RDWR)
                            );
                            if (fd <= 0) {
                                cerr << "could not open '" << filenames [file] << "'" << endl;
                                throw;
                            }

                            current_file = file;
                            current_page = page;
                        }

                        // Open page.
                        mmap = static_cast<char*> (::mmap (
                            0, // start
                            page_size,  // len
                            (mode_read==current_mode)?(PROT_READ)
                            :(mode_write==current_mode)?(PROT_WRITE)
                            :(PROT_READ|PROT_WRITE),  // prot
                            (mode_read==current_mode)?(MAP_PRIVATE):(MAP_SHARED), // flags
                            fd,
                            page_size*trunc_page
                        ));

                        if (MAP_FAILED == static_cast <void*> (mmap)) {
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

                        // mmap now points onto the current page. See what we can do with the pagecache.
                        if (mode_read == current_mode) {
                            bool do_cache = false;
                            pagekey_t k (&pagecache [page]);
                            if (infinite_cache) { // <-- read: cache until malloc()=0
                                pagecache [page].alloc (elems_per_page);
                            }

                            // if not in infinite_cache_size_mode, check if the page is worth to be cached
                            if ((!infinite_cache && cachemap.size () > max_pages_cached) // cache too full
                                || (infinite_cache && 0 == pagecache [page].data) // malloc failed
                            ) {
                                // first see if there is space left in the cachemap
                                if (k < cachemap.begin()->first) {
                                    // swap arrays
                                    pagecache [page].data = cachemap.begin()->second->data;
                                    cachemap.begin()->second->free_no_dealloc (); // free() cache-page but don't free (data)
                                    cachemap.erase (cachemap.begin());
                                    if (do_debug) { --num_pages_cached; }
                                    do_cache = true;
                                }
                            } else if (!infinite_cache || (infinite_cache && 0 != pagecache [page].data)) {
                                do_cache = true;
                            }
                            if (do_cache) {
                                if (0 == pagecache [page].data) { // if not yet allocated
                                    pagecache [page].alloc (elems_per_page);
                                }
                                if (0 != pagecache [page].data) {
                                    pagecache [page].key = k; // <-- update key
                                    cachemap [k] = &(pagecache [page]);

                                    memcpy (pagecache [page].data, mmap, elems_per_page*sizeof (T));
                                    ++num_pages_cached;

                                    // cached. now close mmap.
                                    close_and_sync_current_page ();
                                    return pagecache [page].data [trunc_index];
                                }
                            }
                        }
                    }
                    return * reinterpret_cast <type_t*> (mmap + trunc_index * sizeof (type_t));
                }


            private:
                void close_and_sync_current_page () const {
                    if (mode_read!=current_mode) {
                        msync (static_cast <void*> (mmap), page_size, 0);
                    }
                    munmap (static_cast <void*> (mmap), page_size);
                    mmap = 0;
                    current_page = (index_t)-1;
                }

            public:

                void set_mode (mode_t mode) {
                    if (mode != current_mode) {
                        current_mode = mode;
                        // Close page.
                        if (0 != mmap) {
                            close_and_sync_current_page ();
                        }

                        // Close file.
                        if (fd > 0) {
                            close (fd);
                            fd = -1;
                        }

                        // Invalidate indices and stuff.
                        current_file = (index_t)-1;
                    }
                }
        };
    }; // namespace mem
}; // namespace picogen

#else // #ifdef POSIX
# error "the template 'hugearray' is currently only supported on POSIX architectures"
#endif // #ifdef POSIX

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
// * mail: phresnel/at/gmail/dot/com
// * http://phresnel.org
// * http://picogen.org
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef IARCHIVE_HH_INCLUDED_20090918
#define IARCHIVE_HH_INCLUDED_20090918

#include <stack>

//-----------------------------------------------------------------------------
// IArchive
//-----------------------------------------------------------------------------
namespace actuarius {
class IArchive {
        typedef std::string::iterator iterator_t;
public:
        IArchive (std::istream &in)
        : content("")
        {
                path.push ("");
                
                std::string curr;
                while (getline (in, curr))
                        content += curr + '\n';
                
                doc = detail::parse<iterator_t> (
                 detail::block_match_t<iterator_t> (
                  detail::match_t<iterator_t>(content.begin(),content.begin()),
                  detail::match_t<iterator_t>(content.begin(),content.end())
                ));
                doc.set_as_top_level_node (true);
                
                //doc.dump();
        }
        
        IArchive (IArchive const & oa)        
        : content(oa.content)
        , doc(oa.doc)
        , path(oa.path)
        {}
                
        template <typename T> 
         typename detail::disable_if<
                 detail::has_serialize_function<IArchive,T>, 
                 IArchive
         >::type&
        operator & (nrp<T> val) {
                using namespace detail;
                path.push (path.top() + val.name + "/");
                
                if (detail::value_match_t<iterator_t> 
                      value = doc.take_value (val.name)
                ) {
                        std::stringstream ss;
                        ss << unescape_nonstring_terminal (value.value());
                        ss >> val.value;
                } else {
                        std::cerr << "warning: found nothing for " 
                                  << path.top()
                                  << " (nrp)"
                                  << std::endl;
                }

                path.pop ();
                return *this;
        }
        
        template <typename T> IArchive 
        &operator & (nerp<T> val) {
                using namespace detail;
                path.push (path.top() + val.name + "/");
                if (value_match_t<iterator_t> value=doc.take_value (val.name)){
                        
                        // Extract value (i.e. key for lookup).
                        std::stringstream ss;
                        ss << unescape_nonstring_terminal (value.value());
                        std::string key;
                        ss >> key;
                        
                        // Check if it exists.
                        if (val.enumDesc.exists (key.c_str())) {
                                val.value = val.enumDesc[key.c_str()];
                        } else {
                                std::cerr << "warning: found nothing for " 
                                          << path.top()
                                          << " for value '"
                                          << key
                                          << "' (nerp)"
                                          << std::endl;
                        }
                } else {
                        std::cerr << "warning: found nothing for " 
                                  << path.top()
                                  << " (nerp)"
                                  << std::endl;
                }
                
                path.pop ();
                return *this;
        }
        
        template <typename T> IArchive 
        &operator & (ncrp<T> val) {
                path.push (path.top() + val.name + "/");                
                
                while (detail::block_t<iterator_t> 
                        child = doc.take_child (path.top())
                ) {
                        val.value.push_back (typename T::value_type());

                        IArchive ia (*this, child);
                        ia & make_nrp (val.name, val.value.back());
                }

                path.pop ();
                return *this;
        }
        
        template <typename T> IArchive 
        &operator & (necrp<T> val) {
                path.push (path.top() + val.name + "/");

                while (detail::value_match_t<iterator_t> 
                        value = doc.take_value (val.name)
                ) {
                        using namespace detail;
                        // Extract value (i.e. key for lookup).
                        std::stringstream ss;
                        ss << unescape_nonstring_terminal (value.value());
                        std::string key;
                        ss >> key;
                        
                        // Check if it exists.
                        if (val.enumDesc.exists (key.c_str())) {
                                val.value.push_back(val.enumDesc[key.c_str()]);
                        } else {
                                std::cerr << "warning: found nothing for " 
                                          << path.top()
                                          << " for value '"
                                          << key
                                          << "' (necrp)"
                                          << std::endl;
                        }
                }

                path.pop ();
                return *this;
        }
        
        template <typename T> 
        typename detail::enable_if<
                detail::has_serialize_function<IArchive,T>,
                IArchive
        >::type
        &operator & (nrp<T> val) {
                path.push (path.top() + val.name + "/");
                
                if (detail::block_t<iterator_t> 
                        child = doc.take_child (path.top())
                ) {
                        IArchive ia (*this, child, false);
                        val.value.serialize (ia);
                } else {
                        std::cerr << "warning: found nothing for " 
                                  << path.top() 
                                  << " (nrp rec)" 
                                  << std::endl;
                }

                path.pop ();
                return *this;
        }
        
private:        
        std::string content;
        detail::block_t<iterator_t> doc;
        std::stack<std::string> path;

        IArchive (
                IArchive const & oa, 
                detail::block_t<iterator_t> const &child, 
                bool asChild=true)
        : content(oa.content)
        , doc()
        , path()
        {
                // In case this is a top-level-node we have an
                // anonymous top-level-node.  But if we take a 
                // child,  then  that  child  does not have an
                // anonymous top level node,  hence  we create
                // a new node here, and insert that child into
                // that new node as its child.
                if (asChild)
                        doc.add_child (child);
                else
                        doc = child;
                path.push ("");
        }
};
}



#endif // IARCHIVE_HH_INCLUDED_20090918

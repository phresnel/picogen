//
//    quatsch - a tiny compiler framework for strict functional languages
//
//    Copyright (C) 2009  Sebastian Mach
//
//      email: a@b.c, with a=phresnel, b=gmail, c=com
//        www: http://phresnel.org
//             http://picogen.org
//
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the  Free  Software  Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed  in the hope that it will be useful, but 
//    WITHOUT  ANY  WARRANTY;   without   even  the  implied  warranty  of
//    MERCHANTABILITY  or  FITNESS FOR A PARTICULAR  PURPOSE.  See the GNU 
//    General Public License for more details.
//
//    You should have received a copy  of  the  GNU General Public License
//    along with this program. If not, see <http://www.gnu.org/licenses/>.

#ifndef ESTNODES_CALL_20090124
#define ESTNODES_CALL_20090124
namespace quatsch {  namespace backend {  namespace est {
    // To allow for calls to user defined functions.
    // TODO: templatize for fixed argument counts
    template <typename TYPES> class Call : public TYPES::Function {
            ::boost::weak_ptr <typename TYPES::Function> function; // Must be weak_ptr because boost::shared_ptr does not like circular references, but circles are need to allow for self-recursion.
            const unsigned int argumentCount;
            ::boost::shared_array<typename TYPES::FunctionPtr> arguments;
        

            Call ();
            Call (const Call &);

            Call (const typename TYPES::FunctionPtr &function, const unsigned int argumentCount, 
                const ::boost::shared_array<typename TYPES::FunctionPtr> arguments)
            : function (function), argumentCount (argumentCount), arguments (arguments) {}

            Call (const unsigned int argumentCount, 
                const ::boost::shared_array<typename TYPES::FunctionPtr> arguments)
            : argumentCount (argumentCount), arguments (arguments) {}

        public:
            template <typename argument_vector_t>
            static typename TYPES::FunctionPtr 
            create (const typename TYPES::FunctionPtr &function, 
                const unsigned int argumentCount, const argument_vector_t &arguments_
            ) {
                // Convert from arbitray ::std::vector type to what AccumulatingOp needs.
                ::boost::shared_array <typename TYPES::FunctionPtr> arguments (
                    new typename TYPES::FunctionPtr [argumentCount]
                );
                for (unsigned int i=0; i<argumentCount; ++i) {
                    arguments [i] = arguments_ [i];
                }
                return typename TYPES::FunctionPtr (new Call (function, argumentCount, arguments));
            }

            template <typename argument_vector_t>
            static typename TYPES::FunctionPtr 
            create_incomplete (const unsigned int argumentCount, const argument_vector_t &arguments_) {
                // Convert from arbitray ::std::vector type to what AccumulatingOp needs.
                ::boost::shared_array<typename TYPES::FunctionPtr> arguments (
                    new typename TYPES::FunctionPtr [argumentCount]
                );
                for (unsigned int i=0; i<argumentCount; ++i) {
                    arguments [i] = arguments_ [i];
                }
                return typename TYPES::FunctionPtr (new Call (argumentCount, arguments));                    
            }
            
            virtual ~Call () {}

            void setFunction (const typename TYPES::FunctionPtr &function) {
                if (0 != this->function.lock().get()) {
                    throw operation_not_allowed_exception ();
                }
                this->function = function;
            }
            
            
            /*
            //I was using following code before, which I find more wonderfull. But it relies on forced inlining, which is clearly not portable.
            
            private:
                template <typename T> struct P : public T {};
                
                template <typename T> struct P <T*> {
                    T *m;
                    P (const unsigned int size) __attribute__ ((always_inline)) : m (reinterpret_cast <T*> (alloca (sizeof (T) * size))) {}
                    T & operator [] (int i) { return m [i]; }
                    const T & operator [] (int i) const { return m [i]; }
                    operator T* () { return m; }
                    operator T const * const () const { return m; }
                };

            public:
                scalar_t operator () (const parameters_t &parameters_) const { 
                    P<parameters_t> parameters (this->argumentCount);
                    for (int i=0; i<this->argumentCount; ++i) {
                        parameters [i] = (*this->arguments [i]) (parameters_);
                    }
                    return (*this->function.lock()) (parameters);
                }
            */
        private:
            
            template <typename scalar_t, typename parameters_t> struct DoCall {
                static typename TYPES::scalar_t etHop (const Call &self, 
                    const typename TYPES::parameters_t &parameters_
                ) { 
                    typename TYPES::parameters_t parameters (self.argumentCount);
                    for (int i=0; i<self.argumentCount; ++i) {
                        parameters [i] = (*self.arguments [i]) (parameters_);
                    }
                    return (*self.function.lock()) (parameters);
                }
            };                        

            
            template <typename scalar_t, typename parameters_t> struct DoCall <scalar_t, parameters_t*> {
                static scalar_t etHop (const Call &self, parameters_t * const parameters_) { 
                    typename TYPES::scalar_t parameters [self.argumentCount];
                    for (unsigned int i=0; i<self.argumentCount; ++i) {
                        parameters [i] = (*self.arguments [i]) (parameters_);
                    }
                    return (*self.function.lock()) (parameters);
                }
            };
            
        public:
            typename TYPES::scalar_t operator ()
                (const typename TYPES::parameters_t &parameters_)
            const { 
                return DoCall <typename TYPES::scalar_t, 
                    typename TYPES::parameters_t> :: etHop (*this, parameters_);
            }            
    };
} } }
#endif // ESTNODES_CALL_20090124

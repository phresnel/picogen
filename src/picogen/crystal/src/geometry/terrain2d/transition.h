#ifndef TRANSITION_H_20111123
#define TRANSITION_H_20111123

namespace crystal { namespace geometry { namespace terrain2d {
        class Transition {
        public:
                static Transition Left () { return Transition(1,0,0,0); }
                static Transition Right() { return Transition(0,1,0,0); }
                static Transition Front() { return Transition(0,0,1,0); }
                static Transition Back () { return Transition(0,0,0,1); }
                static Transition None () { return Transition(0,0,0,0); }

                Transition() = delete;

                bool left () const { return left_ ; }
                bool right() const { return right_; }
                bool front() const { return front_; }
                bool back () const { return back_ ; }

                Transition operator| (Transition const& rhs) const {
                        return Transition(left_  || rhs.left_,
                                          right_ || rhs.right_,
                                          front_ || rhs.front_,
                                          back_  || rhs.back_);
                }
        private:
                Transition(bool left, bool right, bool front, bool back)
                        : left_(left), right_(right), front_(front), back_(back)
                {}
                bool left_, right_, front_, back_;
        };
} } }

#endif // TRANSITION_H_20111123

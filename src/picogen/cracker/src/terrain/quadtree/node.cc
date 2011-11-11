
#include "node.h"


namespace picogen { namespace cracker { namespace detail {


void Node::create(int depth,
            NodeDetail detail,
            real left, real right, real front, real back,
            Point cameraPosition,
            std::function<real (real,real)> const & height
) {

        const bool leaf = depth>=detail.deepness(cameraPosition,
                                                 left, right,
                                                 front, back);
        left_  = left;
        right_ = right;
        front_ = front;
        back_  = back;

        if (leaf)
                makeLeaf(left, right,
                         front, back,
                         cameraPosition,
                         height);
        else makeInner(depth, detail,
                       left, right,
                       front, back,
                       cameraPosition,
                       height);

        /*
        for (int i=0; i<depth; ++i)
                std::cout << "  ";
        std::cout << "  {" << depth << "} "
                  << this->min_h_ << ".." << this->max_h_
                  << " [" << left_ << " " << right_ << " "
                  << front_ << " " << back_ << "]"
                  << std::endl;;
        */

}


void Node::makeLeaf (real left, real right,
               real front, real back,
               Point cameraPosition,
               std::function<real (real,real)> const & height)
{
        leaf_ = true;
        patch_ = new Patch (left, right,
                            front, back,
                            cameraPosition,
                            4,4,//TODO: 32 is good
                            height,
                            min_h_, max_h_,
                            Patch::LodSmoothing::None
                            );
}
void Node::makeInner(int depth, NodeDetail detail,
               real left, real right,
               real front, real back,
               Point cameraPosition,
               std::function<real (real,real)> const & height)
{
        leaf_ = false;
        const ChildBoundingQuads childBoxes =
                        child_boxen(left, right, front, back);
        children_ = new Node[4];

        for (size_t i=0; i<4; ++i) {
                children_[i].create (depth+1, detail,
                                     childBoxes[i].left,
                                     childBoxes[i].right,
                                     childBoxes[i].front,
                                     childBoxes[i].back,
                                     cameraPosition,
                                     height);
        }
        refineBoundingBox();
}

// Refinement for inner nodes.
// Pre-condition: * child nodes are refined
//                * has child nodes
void Node::refineBoundingBox () {
        real min_h = children_[0].min_h_;
        min_h = min(min_h, children_[1].min_h_);
        min_h = min(min_h, children_[2].min_h_);
        min_h = min(min_h, children_[3].min_h_);
        real max_h = children_[0].max_h_;
        max_h = max(max_h, children_[1].max_h_);
        max_h = max(max_h, children_[2].max_h_);
        max_h = max(max_h, children_[3].max_h_);

        this->min_h_ = min_h;
        this->max_h_ = max_h;
}




Intersection::Optional Node::new_intersect(Node *root, const Ray &ray,
                                           real root_minT, real root_maxT)
{
        const BoundingBox aabb (Point(root->left_, root->min_h_, root->front_),
                                Point(root->right_, root->max_h_, root->back_));
        const Interval::Optional root_interval = intersect (ray, aabb);
        if (!root_interval) {
                return Intersection::Optional();
        }

        Todo stack[128];
        Todo *top = stack;
        *(top++) = Todo (root_interval.interval().min(),
                         root_interval.interval().max(), root);
        while (top != stack) {
                const Todo curr = *(--top);
                const Node &node = *curr.node;

                //const real center_z

                if (node.leaf_) {
                        const auto i = node.patch_->slow_intersect (ray,
                                                                    curr.minT,
                                                                    curr.maxT);
                        if (i) return i;
                } else {
                        auto push = [&] (int child) {
                                Node * const next = node.children_+child;
                                const BoundingBox aabb (Point(next->left_, next->min_h_, next->front_),
                                                        Point(next->right_, next->max_h_, next->back_));
                                if (auto in = intersect (ray, aabb)) {
                                        *(top++) = Todo (curr.minT, curr.maxT,
                                                         next);
                                }
                        };
                        push (0);
                        push (1);
                        push (2);
                        push (3);
                }
        }

        return Intersection::Optional();
}

} } }

#ifndef INDEXER_H_20120413
#define INDEXER_H_20120413

namespace image { namespace indexer {
        struct Classic
        {
                Classic() = delete;
                Classic(int width, int /*height*/) : width_(width) {}

                int operator() (int x, int y) const { return y*width_ + x; }

        private:
                int width_;
        };
} }


#endif // INDEXER_H_20120413

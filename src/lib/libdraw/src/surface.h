#ifndef SURFACE_H_20120413
#define SURFACE_H_20120413

#include "indexer.h"
#include <vector>
#include <stdexcept>

namespace draw {


template <typename Pixel, typename Indexer=indexer::Classic>
class Surface {
public:
        typedef Pixel pixel_t;


        Surface () = delete;
        Surface (int width, int height) :
          width_(throw_if_negative(width, "Surface::Surface(): width out of range")),
          height_(throw_if_negative (height, "Surface::Surface(): height out of range")),
          data_(width_*height_),
          indexer_ (width_, height_)
        {

        }

        int width () const { return width_;  }
        int height() const { return height_; }

        Pixel& operator() (int x, int y)       { return data_[indexer_(x,y)]; }
        Pixel  operator() (int x, int y) const { return data_[indexer_(x,y)]; }

        Pixel& at (int x, int y) {
                if ((x < 0) | (x>=width_) | (y<0) | (y>=height_))
                        throw std::out_of_range("Surface::at(): arguments out of range");
                return (*this) (x, y);
        }

        Pixel at (int x, int y) const {
                if ((x < 0) | (x>=width_) | (y<0) | (y>=height_))
                        throw std::out_of_range("Surface::at(): arguments out of range");
                return (*this) (x, y);
        }

        int index (int x, int y) const {
                return indexer_(x,y);
        }

        Pixel operator() (int index) const {
                return data_[index];
        }

        Pixel& operator() (int index) {
                return data_[index];
        }

private:
        template <typename T>
        static
        T throw_if_negative (T value, std::string const &msg)
        {
                if (value < T(0)) throw std::out_of_range(msg);
                return value;
        }


private:
        int width_, height_;
        std::vector<Pixel> data_;
        Indexer indexer_;
};

}



#endif // SURFACE_H_20120413

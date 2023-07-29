#pragma once

namespace ztc {

template <typename T>
class PointBase {
   public:
    Point() = default;
    Point(T x = 0, T y = 0) { setXY(x, y); }

    void setX(T x) { x_ = x; }
    void setY(T y) { y_ = y; }

    void setXY(T x, T y)
    {
        x_ = x;
        y_ = y;
    }

    T x() const { return x_; }
    T y() const { return y_; }

   private:
    T x_ = 0;
    T y_ = 0;
};

using Point = PointBase<int>;

}  // namespace ztc
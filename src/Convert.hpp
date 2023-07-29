#pragma once

#include <math.h>

#include <string>
#include <string_view>
#include <variant>

#include "TableAST.hpp"

namespace ztc {

class Parse {
   public:
    static TableAST excel(const std::string_view excel){

    };
};

class Ascii {
   private:
    using HAlign = TableASTCell::HorizontalAlignment;

   public:
    static std::pair<size_t, size_t> space(HAlign h_alignment,
                                           size_t h_interval, size_t sum_space)
    {
        if (sum_space == 0) {
            return std::make_pair(0, 0);
        }

        size_t left = 0, right = 0;
        switch (h_alignment) {
        case HAlign::kLeft:
            left  = h_interval;
            right = sum_space - h_interval;
            return std::make_pair(left, right);
        case HAlign::kRight:
            left  = sum_space - h_interval;
            right = h_interval;
            return std::make_pair(left, right);
        default: break;
        }

        size_t smaller = 0, bigger = 0;
        smaller = (size_t)floor(sum_space / 2.0);  // floor
        bigger  = (size_t)ceil(sum_space / 2.0);   // ceil

        switch (h_alignment) {
        case HAlign::kCenter: return std::make_pair(smaller, bigger);
        case HAlign::kCenterRight: return std::make_pair(bigger, smaller);
        }

        return std::make_pair(0, 0);
    };  // funtion space

};  // class Ascii

class Builder {
   public:
    enum Error { kOk, kUnalignedMatrix };

    static std::string to_ascii(const TableAST &ast)
    {
        std::string ret;

        size_t row_sz = ast.row_size();
        size_t col_sz = ast.column_size();

        std::vector<size_t> col_lim_arr;
        col_lim_arr.reserve(col_sz);

        size_t length = 0, width = 0;

        for (size_t y = 0; y < col_sz; ++y) {
            auto col_lim = ast.column_max_string_size(y);
            col_lim_arr.push_back(col_lim.second);
            length +=
                col_lim.second + ast.interval_horizontal() * 2 + 1 /*边框*/;
        }
        // 结尾边框 "+" 和 换行符
        length += 1 + 1;
        width =
            row_sz + ast.interval_vertical() * 2 + (row_sz + 1) /*边框数量*/;

        printf("pre sum len = %zu (%zu * %zu - 1)\n", length * width - 1,
               length, width);
        ret.reserve(length * width - 1);


        ztc::TableASTCell nullCell;
        for (size_t y = 0; y < row_sz; ++y) {
            // 上分割线
            for (size_t i = 0; i < col_sz; ++i) {
                ret.push_back('+');
                ret.append(col_lim_arr[i] + ast.interval_horizontal() * 2, '-');
            }
            ret.append("+\n");

            // 上间距
            for (size_t j = 0; j < ast.interval_vertical(); ++j) {
                for (size_t i = 0; i < col_sz; ++i) {
                    ret.push_back('|');
                    ret.append(col_lim_arr[i] + ast.interval_horizontal() * 2,
                               ' ');
                }
                ret.append("|\n");
            }

            // 内容
            for (size_t i = 0; i < col_sz; ++i) {
                auto   cell  = ast.get(i, y);
                if (cell == nullptr) {
                    cell = &nullCell;
                }

                size_t space = col_lim_arr[i] + ast.interval_horizontal() * 2 -
                               cell->content_.size();
                size_t left_space = 0, right_space = 0;
                std::tie(left_space, right_space) =
                    Ascii::space(cell->alignment_horizontal_,
                                 ast.interval_horizontal(), space);

                ret.push_back('|');
                ret.append(left_space, ' ');
                ret.append(cell->content_);
                ret.append(right_space, ' ');
            }
            ret.append("|\n");

            // 下间距
            for (size_t j = 0; j < ast.interval_vertical(); ++j) {
                for (size_t i = 0; i < col_sz; ++i) {
                    ret.push_back('|');
                    ret.append(col_lim_arr[i] + ast.interval_horizontal() * 2,
                               ' ');
                }
                ret.append("|\n");
            }
        }

        if (row_sz > 0) {
            // 下分割线
            for (size_t i = 0; i < col_sz; ++i) {
                ret.push_back('+');
                ret.append(col_lim_arr[i] + ast.interval_horizontal() * 2, '-');
            }
            ret.push_back('+');
        }

        printf("final sum len = %zu\n", ret.size());
        return ret;
    }  // funtion to_ascii
};

}  // namespace ztc
#pragma once

#include <algorithm>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "Pos.hpp"

namespace ztc {

class TableASTCell {
   public:
    enum class VerticalAlignment {
        kTop,
        kCenter,
        kBottom,
    };

    enum class HorizontalAlignment {
        kLeft,
        kCenter,
        kCenterRight,
        kRight,
    };

    TableASTCell() = default;

    TableASTCell(const std::string_view strv) : content_(strv) {}

    TableASTCell(std::string &&str) : content_(str) {}

    TableASTCell(const char *str) : TableASTCell(std::string_view(str)) {}

   public:
    using size_type = size_t;

   public:
    ztc::Point leadr_pos_{-1, -1};
    size_type row_combined_num_ = 0;  // 行合并单元格数
    size_type col_combined_num_ = 0;  // 列合并单元格数

    std::string content_;

    VerticalAlignment   alignment_vertical_   = VerticalAlignment::kCenter;
    HorizontalAlignment alignment_horizontal_ = HorizontalAlignment::kCenter;
};

class TableAST {
   public:
    using TableRow = std::vector<TableASTCell>;

    const TableASTCell *get(size_t x_, size_t y_) const
    {
        if (cells_.size() > y_) {
            if (cells_[y_].size() > x_) {
                return &cells_[y_][x_];
            }
        }

        return nullptr;
    }

    std::pair<ztc::Point, size_t> column_max_string_size(size_t x_) const
    {
        ztc::Point pos;
        size_t   max_sz = 0;
        for (size_t y = 0; y < cells_.size(); ++y) {
            if (x_ < cells_[y].size() && max_sz < cells_[y][x_].content_.size())
            {
                pos.setXY(x_, y);
                max_sz = cells_[y][x_].content_.size();
            }
        }

        return std::make_pair(pos, max_sz);
    }

    size_t row_size() const { return cells_.size(); }

    size_t column_size() const { return cells_.empty() ? 0 : cells_[0].size(); }

    int interval_vertical() const { return interval_vertical_; }
    int interval_horizontal() const { return interval_horizontal_; }

    void set_interval_vertical(int val) { interval_vertical_ = val; }
    void set_interval_horizontal(int val) { interval_horizontal_ = val; }

    std::vector<TableRow>& table() { return cells_;
    }

    void make_column_align()
    {
        size_t max_col = 0;
        for (auto &iter : cells_) {
            max_col = std::max(max_col, iter.size());
        }

        for (auto &iter : cells_) {
            iter.reserve(max_col);
            for (int i = 0; i < max_col - iter.size();++i) {
                iter.emplace_back(TableASTCell());
            }
            //iter.insert(iter.end(), max_col - iter.size(), TableASTCell());
        }
    }

   private:
    // [ [cell]->row ]->column
    std::vector<TableRow> cells_{};

    int interval_vertical_   = 0;
    int interval_horizontal_ = 1;
};

}  // namespace ztc
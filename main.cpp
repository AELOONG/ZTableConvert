#include "src/TableAST.hpp"
#include "src/Convert.hpp"

int main()
{
    ztc::TableAST tast;
    tast.set_interval_horizontal(2);
    tast.set_interval_vertical(1);

    tast.table().reserve(2);
    tast.table().push_back(
        {"[0, pos)", "[pos, pos + count)", "[pos + count, size + count)", "xxxxx"});
    tast.table().push_back({"AAAA", "insert_data", "BBBB"});

    tast.table()[1][2].alignment_horizontal_ =
        ztc::TableASTCell::HorizontalAlignment::kCenterRight;
    //tast.make_column_align();

    printf("ascii table = \n%s\n", ztc::Builder::to_ascii(tast).c_str());

    return 0;
}
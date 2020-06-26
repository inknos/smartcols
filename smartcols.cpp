#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <stdio.h>
#include <variant>
#include <vector>

#include <libsmartcols.h>

#include "../fork/libdnf/libdnf/utils/smartcols/Table.hpp"

typedef std::map< std::string, std::variant< std::string > > pkgmap;
typedef std::vector< pkgmap > listpkgmap;


class MapToTable {
private:
    listpkgmap listPkgMap;

    struct libscols_table * tb;
    struct libscols_line  * l1, * l2, * l3;
    enum { COL_KEY, COL_VALUE };
public:
    MapToTable(listpkgmap);
    MapToTable();
    ~MapToTable() { scols_unref_table(this->tb); }
    struct libscols_line * add(std::string, std::string);
    void print();
    void print_table();
};

MapToTable::MapToTable()
{
    setlocale(LC_ALL, "");
    this->tb = scols_new_table();

    scols_table_new_column(this->tb, "KEY",   2, SCOLS_FL_WRAP);
    scols_table_new_column(this->tb, "VALUE", 2, SCOLS_FL_WRAP);
    scols_table_enable_noheadings(this->tb, 1);
    scols_table_set_column_separator(this->tb, " : ");
}

struct libscols_line *
MapToTable::add(std::string key, std::string value)
{
    auto ln = scols_table_new_line(this->tb, NULL);
    scols_line_set_data(ln, COL_KEY, key.c_str());
    scols_line_set_data(ln, COL_VALUE, value.c_str());
    return ln;
}

void
MapToTable::print()
{
    scols_print_table(this->tb);
}

int main() {
    listpkgmap listPkgMap = { // vector
        { // map
            { "name", "bash" },
            { "version", "1.1.1"}
        },
        {
            { "name", "wget" },
            { "version", "2.2.2"}
        }
    };

    for ( auto it = listPkgMap.begin(); it != listPkgMap.end(); it++ ) {
        MapToTable mtt;
        auto pkg = *it;
        mtt.add("name", std::get<std::string>(pkg["name"]));
        mtt.add("version", std::get<std::string>(pkg["version"]));
        mtt.print();
        if ( std::next(it) != listPkgMap.end() ) { std::cout << std::endl; }
    }

}


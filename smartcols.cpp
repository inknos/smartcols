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

void print_table_as_together(listpkgmap listPkgMap) {

    struct libscols_table * tb;
    enum { COL_KEY, COL_VALUE };

    setlocale(LC_ALL, "");

    tb = scols_new_table();

    scols_table_new_column(tb, "KEY",   2, SCOLS_FL_WRAP);
    scols_table_new_column(tb, "VALUE", 2, SCOLS_FL_WRAP);

    scols_table_enable_noheadings(tb, 1);
    scols_table_set_column_separator(tb, " : ");

    for ( auto it = listPkgMap.begin(); it != listPkgMap.end(); it++) {

        auto pkg = *it;
        auto ln = scols_table_new_line(tb, NULL);
        auto lv = scols_table_new_line(tb, NULL);

        scols_line_set_data(ln, COL_KEY, "name");
        scols_line_set_data(ln, COL_VALUE, std::get<std::string>(pkg["name"]).c_str());

        scols_line_set_data(lv, COL_KEY, "version");
        scols_line_set_data(lv, COL_VALUE, std::get<std::string>(pkg["version"]).c_str());


        if ( std::next(it) != listPkgMap.end() ) {
            auto le = scols_table_new_line(tb, NULL);
            scols_line_set_data(le, COL_KEY, "");
            scols_line_set_data(le, COL_VALUE, "");
        }
    }
        scols_print_table(tb);
        scols_unref_table(tb);

}

void print_table_as_separate(listpkgmap listPkgMap) {


    for ( auto it = listPkgMap.begin(); it != listPkgMap.end(); it++) {
        struct libscols_table * tb;
        enum { COL_KEY, COL_VALUE };

        setlocale(LC_ALL, "");

        tb = scols_new_table();

        scols_table_new_column(tb, "KEY",   2, SCOLS_FL_WRAP);
        scols_table_new_column(tb, "VALUE", 2, SCOLS_FL_WRAP);

        scols_table_enable_noheadings(tb, 1);
        scols_table_set_column_separator(tb, " : ");

        auto pkg = *it;
        auto ln = scols_table_new_line(tb, NULL);
        auto lv = scols_table_new_line(tb, NULL);

        scols_line_set_data(ln, COL_KEY, "name");
        scols_line_set_data(ln, COL_VALUE, std::get<std::string>(pkg["name"]).c_str());

        scols_line_set_data(lv, COL_KEY, "version");
        scols_line_set_data(lv, COL_VALUE, std::get<std::string>(pkg["version"]).c_str());

        scols_print_table(tb);
        scols_unref_table(tb);

        if ( std::next(it) != listPkgMap.end() ) { std::cout << std::endl; }
    }

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

    std::cout << "=================" << std::endl;
    for ( auto it = listPkgMap.begin(); it != listPkgMap.end(); it++ ) {
        MapToTable mtt;
        auto pkg = *it;
        mtt.add("name", std::get<std::string>(pkg["name"]));
        mtt.add("version", std::get<std::string>(pkg["version"]));
        mtt.print();
        if ( std::next(it) != listPkgMap.end() ) { std::cout << std::endl; }
    }

    std::cout << "=================" << std::endl;
    print_table_as_separate(listPkgMap);
    std::cout << "=================" << std::endl;
    print_table_as_together(listPkgMap);
    std::cout << "=================" << std::endl;
}


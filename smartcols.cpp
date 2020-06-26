#include <algorithm>
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
    enum { COL_NAME, COL_VERSION, COL_ARCH };
public:
    MapToTable(listpkgmap);
    MapToTable(std::string, std::string, std::string);
    void add(std::string, std::string, std::string);
    void print();
    void print_table();
};

MapToTable::MapToTable(std::string name, std::string version, std::string arch)
{
    setlocale(LC_ALL, "");
    this->tb = scols_new_table();

    scols_table_new_column(this->tb, "NAME",     2, SCOLS_FL_WRAP);
    scols_table_new_column(this->tb, "VERSION",  2, SCOLS_FL_RIGHT);
    scols_table_new_column(this->tb, "ARCH",     2, SCOLS_FL_RIGHT);
    this->add(name, version, arch);
}

void
MapToTable::add(std::string name, std::string version, std::string arch)
{
    pkgmap map;
    map["name"] = name;
    map["version"] = version;
    map["arch"] = arch;
    this->listPkgMap.push_back(map);

    this->l1 = scols_table_new_line(this->tb, l1);
    scols_line_set_data(this->l1, COL_NAME, name.c_str());
    scols_line_set_data(this->l1, COL_VERSION, version.c_str());
    scols_line_set_data(this->l1, COL_ARCH, arch.c_str());

}

void
MapToTable::print()
{
    for ( const auto & el : this->listPkgMap ){
        printf("%s\t: %s\n", "Name", std::get<std::string>(el.at("name")).c_str());
        printf("%s\t: %s\n", "Version", std::get<std::string>(el.at("version")).c_str());
        printf("%s\t: %s\n", "Arch", std::get<std::string>(el.at("arch")).c_str());
        printf("\n");
    }
    scols_print_table(this->tb);
    scols_unref_table(this->tb);
}

int main() {
    listpkgmap listPkgMap;

    MapToTable mtt("bash", "5.0.11", "x86_64");
    mtt.add("wget", "1.20.3", "x86_64");
    mtt.add("dnf", "4.2.23", "noarch");

    mtt.print();
}


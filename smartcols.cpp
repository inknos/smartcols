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
    Table t;

public:
    MapToTable(listpkgmap);
    MapToTable(std::string, std::string, std::string);
    void add(std::string, std::string, std::string);
    void print();
    void print_table();
};

MapToTable::MapToTable(listpkgmap listPkgMap)
    : listPkgMap(listPkgMap)
{}

MapToTable::MapToTable(std::string name, std::string version, std::string arch)
{
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
}

int main() {
    listpkgmap listPkgMap;
    MapToTable mtt("bash", "5.0.11", "x86_64");
    mtt.add("wget", "1.20.3", "x86_64");
    mtt.add("dnf", "4.2.23", "noarch");
    mtt.print();
}


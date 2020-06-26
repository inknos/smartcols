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
typedef std::map< std::string, listpkgmap > maplistpkgmap;

/*
 * Transaction table
 * =====================================
 * Package              Architecture  Version
 * =====================================
 * Installing:
 *  python3-jira        noarch        2.0.0-8.fc33
 * Installing dependencies:
 *  python3-backcall    noarch        0.1.0-10.fc33
 *  python3-defusedxml  noarch        0.7.0~rc1-2.fc33
 *
 *
 *  [ // ordered map
 *      "installing": [ // vector
 *          { //map < string, variant<string> >
 *          "name": "bash",
 *          "version": "1.1.1",
 *          "architecture": "noarch"
 *          },
 *          {...},
 *          {...}
 *      ],
 *      "": [...]
 *
 *  ]
 * */


int main() {
    maplistpkgmap mlpm = { //map
        { "install", { //vector
                { //map
                    { "name", "bash" }, // string, variant
                    { "version", "2.2.2"},
                    { "arch", "noarch"}
                },
                {
                    { "name", "wget" },
                    { "version", "2.2.2"},
                    { "arch", "noarch"}
                }
        }},
        {"remove", {
            {
                { "name", "bash" },
                { "version", "1.1.1"},
                { "arch", "noarch"}
            },
            {
                { "name", "wget" },
                { "version", "1.1.1"},
                { "arch", "noarch"}
            }
        }}
    };

    struct libscols_table * tb;
    struct libscols_line  * ln, * in_ln, * rm_ln;
    enum { COL_NAME, COL_VERSION, COL_ARCH };
    setlocale(LC_ALL, "");

    tb = scols_new_table();
    scols_table_new_column(tb, "Package",      0.4, SCOLS_FL_TREE);
    scols_table_new_column(tb, "Version",      0.3, SCOLS_FL_WRAP);
    scols_table_new_column(tb, "Architecture", 0.3, SCOLS_FL_WRAP);
    scols_table_enable_maxout (tb, 1);

    auto in_list = mlpm["install"];
    auto rm_list = mlpm["remove"];

    /*
     * Installing
     * */
    ln = in_ln = scols_table_new_line(tb, NULL);
    scols_line_set_data(ln, COL_NAME, "Installing");
    scols_line_set_data(ln, COL_VERSION, "");
    scols_line_set_data(ln, COL_ARCH, "");

    for ( auto it = in_list.begin(); it != in_list.end(); it++) {
        auto map = *it;
        ln = scols_table_new_line(tb, in_ln);

        scols_line_set_data(ln, COL_NAME, std::get<std::string>(map["name"]).c_str());
        scols_line_set_data(ln, COL_VERSION, std::get<std::string>(map["version"]).c_str());
        scols_line_set_data(ln, COL_ARCH, std::get<std::string>(map["arch"]).c_str());
    }

    ln = scols_table_new_line(tb, in_ln);


    /*
     * Removing
     * */
    ln = rm_ln = scols_table_new_line(tb, NULL);
    scols_line_set_data(ln, COL_NAME, "Removing");
    scols_line_set_data(ln, COL_VERSION, "");
    scols_line_set_data(ln, COL_ARCH, "");

    for ( auto it = rm_list.begin(); it != rm_list.end(); it++) {
        auto map = *it;
        ln = scols_table_new_line(tb, rm_ln);

        scols_line_set_data(ln, COL_NAME, std::get<std::string>(map["name"]).c_str());
        scols_line_set_data(ln, COL_VERSION, std::get<std::string>(map["version"]).c_str());
        scols_line_set_data(ln, COL_ARCH, std::get<std::string>(map["arch"]).c_str());
    }

    scols_print_table(tb);
    scols_unref_table(tb);
}


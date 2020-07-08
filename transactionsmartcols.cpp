#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <stdio.h>
#include <variant>
#include <vector>

#include <libsmartcols.h>

typedef std::map< std::string, std::variant< std::string > > pkgmap;
typedef std::vector< pkgmap > listpkgmap;
typedef std::map< std::string, listpkgmap > maplistpkgmap;

/*
 * Compile command
 * g++ --std=c++17 -O3 -Wall -Wextra -Werror `pkgconf --cflags smartcols` -l smartcols transactionsmartcols.cpp
 * */

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


struct libscols_line * make_group_line(
        std::string name,
        struct libscols_table * tb,
        struct libscols_line * parent_ln,
        listpkgmap list, bool newline=false) {

        enum { COL_NAME, COL_VERSION, COL_ARCH, COL_REPO, COL_SIZE };

        auto ln = parent_ln;
        scols_line_set_data(ln, COL_NAME, name.c_str());
        scols_line_set_data(ln, COL_VERSION, "");
        scols_line_set_data(ln, COL_ARCH, "");
        scols_line_set_data(ln, COL_REPO, "");
        scols_line_set_data(ln, COL_SIZE, "");

        for ( auto it = list.begin(); it != list.end(); it++) {
            auto map = *it;
            ln = scols_table_new_line(tb, parent_ln);

            scols_line_set_data(ln, COL_NAME, std::get<std::string>(map["name"]).c_str());
            scols_line_set_data(ln, COL_VERSION, std::get<std::string>(map["version"]).c_str());
            scols_line_set_data(ln, COL_ARCH, std::get<std::string>(map["arch"]).c_str());
            scols_line_set_data(ln, COL_REPO, std::get<std::string>(map["repo"]).c_str());
            scols_line_set_data(ln, COL_SIZE, std::get<std::string>(map["size"]).c_str());
        }

        if (newline) {
            ln = scols_table_new_line(tb, parent_ln);
        }
        return ln;
}

void print_table(maplistpkgmap mlpm) {
    struct libscols_table * tb;
    struct libscols_line * in_ln, * rm_ln;
    struct libscols_symbols * sy = scols_new_symbols();
    enum { COL_NAME, COL_VERSION, COL_ARCH };
    setlocale(LC_ALL, "");

    // init table
    tb = scols_new_table();
    scols_table_new_column(tb, "Package",      0.2, SCOLS_FL_TREE);
    scols_table_new_column(tb, "Version",      0.2, SCOLS_FL_WRAP);
    scols_table_new_column(tb, "Arch",         0.2, SCOLS_FL_WRAP);
    scols_table_new_column(tb, "Repository",   0.2, SCOLS_FL_WRAP);
    scols_table_new_column(tb, "Size",         0.2, SCOLS_FL_WRAP);
    scols_table_enable_maxout (tb, 1);

    in_ln = scols_table_new_line(tb, NULL);
    rm_ln = scols_table_new_line(tb, NULL);

    // init tree symbols
    scols_symbols_set_branch(sy, " ");
    scols_symbols_set_right(sy, " ");
    scols_symbols_set_vertical(sy, " ");
    scols_table_set_symbols (tb, sy);

    // split maplist in separate lists
    // now obj are of type listpkgmap
    auto in_list = mlpm["install"];
    auto rm_list = mlpm["remove"];

    make_group_line("Installing", tb, in_ln, in_list, true);
    make_group_line("Removing",   tb, rm_ln, rm_list);

    scols_print_table(tb);
    scols_unref_table(tb);
}

int main() {
    maplistpkgmap mlpm = { //map
        { "install", { //vector
                { //map
                    { "name", "bash" }, // string, variant
                    { "version", "2.2.2"},
                    { "arch", "noarch"},
                    { "repo", "repo1"},
                    { "size", "50MB" }
                },
                {
                    { "name", "wget" },
                    { "version", "2.2.2"},
                    { "arch", "noarch"},
                    { "repo", "repo1"},
                    { "size", "50MB" }

                }
        }},
        {"remove", {
            {
                { "name", "bash" },
                { "version", "1.1.1"},
                { "arch", "noarch"},
                { "repo", "repo1"},
                { "size", "50MB" }

            },
            {
                { "name", "wget" },
                { "version", "1.1.1"},
                { "arch", "noarch"},
                { "repo", "repo1"},
                { "size", "50MB" }
            }
        }}
    };

    print_table(mlpm);
}


#include <map>
#include <string>
#include <variant>
#include <vector>

#include <libsmartcols.h>


typedef std::map< std::string, std::variant< std::string > > pkgmap;
typedef std::vector< pkgmap > listpkgmap;
typedef std::map< std::string, listpkgmap > maplistpkgmap;


void print_repolist(listpkgmap list) {
    struct libscols_table * tb;
    struct libscols_line * ln;
    struct libscols_symbols * sy = scols_new_symbols();
    enum { COL_ID, COL_NAME };
    setlocale(LC_ALL, "");

    // init table
    tb = scols_new_table();
    scols_table_new_column(tb, "Repo ID",   0.2, SCOLS_FL_TREE);
    scols_table_new_column(tb, "Repo Name", 0.2, SCOLS_FL_WRAP);
    scols_table_enable_maxout(tb, 1);


    // init tree symbols
    scols_symbols_set_branch(sy, "");
    scols_symbols_set_right(sy, "");
    scols_symbols_set_vertical(sy, "");
    scols_table_set_symbols (tb, sy);

    for ( auto it = list.begin(); it != list.end(); it++) {
        auto map = *it;
        ln = scols_table_new_line(tb, NULL);

        scols_line_set_data(ln, COL_ID, std::get<std::string>(map["id"]).c_str());
        scols_line_set_data(ln, COL_NAME, std::get<std::string>(map["name"]).c_str());
    }

    scols_print_table(tb);
    scols_unref_table(tb);
}

int main() {
    listpkgmap mlpm = {
        {
            { "id", "fedora" },
            { "name", "Fedora" }
        },
        {
            { "id", "fedora-modular" },
            { "name", "Fedora Modular" }
        },
        {
            { "id", "rcm-tools-fedora-rpms" },
            { "name", "RCM Tools for Fedora 31 (RPMs)" }
        },
        {
            { "id", "rpmfusion-free" },
            { "name", "RPM Fusion for Fedora 31 - Free" }
        }

    };

    print_repolist(mlpm);
}

#include <map>
#include <string>
#include <variant>
#include <vector>

#include <libsmartcols.h>

typedef std::map< std::string, std::variant< std::string > > pkgmap;
typedef std::vector< pkgmap > listpkgmap;


void print_repoinfo(listpkgmap listPkgMap) {

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
        auto l1 = scols_table_new_line(tb, NULL);
        auto l2 = scols_table_new_line(tb, NULL);
        auto l3 = scols_table_new_line(tb, NULL);
        auto l4 = scols_table_new_line(tb, NULL);
        auto l5 = scols_table_new_line(tb, NULL);
        auto l6 = scols_table_new_line(tb, NULL);
        auto l7 = scols_table_new_line(tb, NULL);
        auto l8 = scols_table_new_line(tb, NULL);
        auto l9 = scols_table_new_line(tb, NULL);
        auto l10= scols_table_new_line(tb, NULL);

        scols_line_set_data(l1, COL_KEY, "id");
        scols_line_set_data(l1, COL_VALUE, std::get<std::string>(pkg["id"]).c_str());

        scols_line_set_data(l2, COL_KEY, "name");
        scols_line_set_data(l2, COL_VALUE, std::get<std::string>(pkg["name"]).c_str());

        scols_line_set_data(l3, COL_KEY, "revision");
        scols_line_set_data(l3, COL_VALUE, std::get<std::string>(pkg["revision"]).c_str());

        scols_line_set_data(l4, COL_KEY, "updated");
        scols_line_set_data(l4, COL_VALUE, std::get<std::string>(pkg["updated"]).c_str());

        scols_line_set_data(l5, COL_KEY, "pkgs");
        scols_line_set_data(l5, COL_VALUE, std::get<std::string>(pkg["pkgs"]).c_str());

        scols_line_set_data(l6, COL_KEY, "available-pkgs");
        scols_line_set_data(l6, COL_VALUE, std::get<std::string>(pkg["available-pkgs"]).c_str());

        scols_line_set_data(l7, COL_KEY, "size");
        scols_line_set_data(l7, COL_VALUE, std::get<std::string>(pkg["size"]).c_str());

        scols_line_set_data(l8, COL_KEY, "baseurl");
        scols_line_set_data(l8, COL_VALUE, std::get<std::string>(pkg["baseurl"]).c_str());

        scols_line_set_data(l9, COL_KEY, "expire");
        scols_line_set_data(l9, COL_VALUE, std::get<std::string>(pkg["expire"]).c_str());

        scols_line_set_data(l10, COL_KEY, "filename");
        scols_line_set_data(l10, COL_VALUE, std::get<std::string>(pkg["filename"]).c_str());

        if ( std::next(it) != listPkgMap.end() ) {
            auto le = scols_table_new_line(tb, NULL);
            scols_line_set_data(le, COL_KEY, "");
            scols_line_set_data(le, COL_VALUE, "");
        }
    }
        scols_print_table(tb);
        scols_unref_table(tb);
}

int main() {
    listpkgmap listPkgMap = {
        {
            { "id", "repo-id" },
            { "name", "Repo Name" },
            { "revision", "123456789" },
            { "updated", "Mon 1 Jan 2020" },
            { "pkgs", "11" },
            { "available-pkgs", "11" },
            { "size", "10MB" },
            { "baseurl", "https://repo.it" },
            { "expire", "xxx xxx" },
            { "filename", "/etc/yum.repos.d/repo-id.repo" },
        },
        {
            { "id", "repo-id" },
            { "name", "Repo Name" },
            { "revision", "123456789" },
            { "updated", "Mon 1 Jan 2020" },
            { "pkgs", "11" },
            { "available-pkgs", "11" },
            { "size", "10MB" },
            { "baseurl", "https://repo.it" },
            { "expire", "xxx xxx" },
            { "filename", "/etc/yum.repos.d/repo-id.repo" },
        }
    };

    print_repoinfo(listPkgMap);
}

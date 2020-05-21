import dnf


class Row(object):
    def __init__(self, name, version):
        self.name = name
        self.version = version


    @property
    def name(self):
        return self._name


    @property
    def version(self):
        return self._version


    @name.setter
    def name(self, x):
        self._name = x


    @version.setter
    def version(self, x):
        self._version = x


class Smartcols(object):

    def __init__(self, sep = ":", space = " ",
            space_l = False,
            space_r = True,
            space_b = False,
            space_e = False):

        self.sep = sep
        self.space = space
        self.space_l = space_l
        self.space_r = space_r
        self.space_b = space_b
        self.space_e = space_e

        self._init_sack()
        self._pkglist = []


    def _init_sack(self):
        self._base = dnf.Base()
        self._base.fill_sack()


    @property
    def sep(self):
        return self._sep


    @property
    def space(self):
        return self._space


    @property
    def space_l(self):
        return self._space_l


    @property
    def space_r(self):
        return self._space_r


    @property
    def space_b(self):
        return self._space_b


    @property
    def space_e(self):
        return self._space_e


    @property
    def pkglist(self):
        return self._pkglist


    @sep.setter
    def sep(self, x):
        self._sep = x


    @space.setter
    def space(self, x):
        self._space = x


    @space_l.setter
    def space_l(self, x):
        self._space_l = True if x else False


    @space_r.setter
    def space_r(self, x):
        self._space_r = True if x else False


    @space_b.setter
    def space_b(self, x):
        self._space_b = True if x else False


    @space_e.setter
    def space_e(self, x):
        self._space_e = True if x else False


    def add_name(self, namelist):
        for pkg in  self._base.sack.query().filter(name=namelist):
            self._pkglist.append(Row(pkg.name, pkg.version))


    def _print_line(self, row):
        lspace = self.space if self.space_l else ""
        rspace = self.space if self.space_r else ""
        bspace = self.space if self.space_b else ""
        espace = self.space if self.space_e else ""

        print(bspace,
                row.name,
                lspace,
                self.sep,
                rspace,
                row.version,
                espace,
                sep="")


    def print(self):
        for row in self.pkglist:
            self._print_line(row)


s = Smartcols()
s.sep = ":"
s.space = " "

s.add_name(["bash", "dnf", "wget"])
s.add_name(["yum"])
s.add_name(["yum"])

s.print()


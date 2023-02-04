TEMPLATE = subdirs

SUBDIRS += \
        example \
        lib \
        test \
        themes

example.depends = lib
test.depends = lib


DISTFILES = .astylerc

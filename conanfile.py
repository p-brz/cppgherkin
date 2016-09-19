#!/usr/bin/env python
# -*- coding: utf-8 -*-

from conans import ConanFile
import os

class MyConanFile(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    #Conan dependencies
    requires = (
        "pcre2plus/0.0.4-beta@paulobrizolara/experimental",
        "commonex/1.0.0@paulobrizolara/stable",
        "function_traits/0.0.1-d5bc85c@paulobrizolara/master",
        "tinydir/1.2@paulobrizolara/stable",

        "runcommand-waf-tool/1.0.0@paulobrizolara/master",
        "WafGenerator/0.0.1@paulobrizolara/experimental"
    )
    dev_requires = (
        "catch/1.5.0@TyRoXx/stable",
        "FakeIt/2.0.2-119-g44f1373@paulobrizolara/master"
    )

    generators = "Waf"

    default_options = "FakeIt:header_version=catch"

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin") # From bin to bin
        self.copy("*.dylib*", dst="bin", src="lib") # From lib to bin

        self.copy("run_command.py", src="bin", dst="buildtools")

    def build(self):
        waf = os.path.join(".", 'waf')
        opts = self.get_options()

        self.run('%s configure build %s' % (waf, opts), cwd = self.conanfile_directory)

    def get_options(self):
        opts = []

        if self.settings.build_type == "Debug":
            opts.append("--debug")
        else:
            opts.append("--release")

        return " ".join(opts)

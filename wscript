#!/usr/bin/env python
# -*- coding: utf-8 -*-

from os import path

def load_tools(ctx):
    ctx.load(['compiler_cxx', 'compiler_c'])

def options(ctx):
    #Load options from waf tools (c++ compiler)
    # Execute ./waf --help to see current options
    load_tools(ctx)

    #You can add your own options also

    ctx.add_option('--debug', action='store_true', default=True, dest='debug', help='Do debug build')
    ctx.add_option('--release', action='store_false', dest='debug', help='Do release build')

def configure(ctx):
#    ctx.env['CXX'] = 'clang';
    load_tools(ctx)

    ctx.load('conanbuildinfo_waf', tooldir=".");

    # Allows debug build
    if ctx.options.debug:
        ctx.env['CXXFLAGS'] += ['-g']
        ctx.env['CFLAGS'] += ['-g']

    ctx.env['CXXFLAGS'] += ['-std=c++11']

def build(bld):
    bld.recurse('src')

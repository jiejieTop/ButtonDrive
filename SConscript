#-*- encoding: utf-8 -*-
#---------------------------------------------------------------------------------
# @File:   Sconscript 
# @Author: liu2guang
# @Date:   2018-09-19 18:07:00(v0.1.0) 
# 
# @LICENSE: GPLv3: https://github.com/rtpkgs/buildpkg/blob/master/LICENSE.
#
#---------------------------------------------------------------------------------
import os
from building import * 
Import('RTT_ROOT')
Import('rtconfig')

#---------------------------------------------------------------------------------
# Package configuration
#---------------------------------------------------------------------------------
PKGNAME = "button"
VERSION = "v1.0.0"
DEPENDS = ["PKG_USING_BUTTON"]

#---------------------------------------------------------------------------------
# Compile the configuration 
#---------------------------------------------------------------------------------
SOURCES          = ["button.c"] 

CPPPATH          = [] 
CCFLAGS          = " -std=gnu99" # no pass
ASFLAGS          = ""

LOCAL_CPPPATH    = [""] 
LOCAL_CCFLAGS    = " -std=gnu99" 
LOCAL_ASFLAGS    = ""

CPPDEFINES       = ["CPPDEFINES"]
LOCAL_CPPDEFINES = ["LOCAL_CPPDEFINES"]

LIBS             = ["libmm"] # keil libmm, gcc mm
LIBPATH          = [os.path.join(GetCurrentDir(), "")] 

LINKFLAGS        = "" 

#---------------------------------------------------------------------------------
# Feature clip configuration, optional 
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
# Compiler platform configuration, optional
#---------------------------------------------------------------------------------
if rtconfig.CROSS_TOOL == "gcc":
    pass

if rtconfig.CROSS_TOOL == "iar":
    pass

if rtconfig.CROSS_TOOL == "keil":
    pass

#---------------------------------------------------------------------------------
# Warning: internal related processing, developers do not modify!!!
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
# System variables
#---------------------------------------------------------------------------------
objs = [] 
root = GetCurrentDir()

#---------------------------------------------------------------------------------
# Add relative path support for CPPPATH and LOCAL_CPPPATH
#---------------------------------------------------------------------------------
for index, value in enumerate(CPPPATH): 
    if string.find(value, root) == False: 
        CPPPATH[index] = os.path.join(root, value)

for index, value in enumerate(LOCAL_CPPPATH): 
    if string.find(value, root) == False: 
        LOCAL_CPPPATH[index] = os.path.join(root, value)

if rtconfig.CROSS_TOOL == "gcc": # no test
    for index, value in enumerate(LIBS): 
        if value.startswith("lib") == True: 
            print("Automatic fix the nonstandard lib name, %s -> %s" % (LIBS[index], LIBS[index].lstrip("lib")))
            LIBS[index] = LIBS[index].lstrip("lib")
            
elif rtconfig.CROSS_TOOL == "keil": 
    for index, value in enumerate(LIBS): 
        if value.startswith("lib") == False: 
            print("Automatic fix the nonstandard lib name, %s -> %s" % (LIBS[index], "lib" + LIBS[index]))
            LIBS[index] = "lib" + LIBS[index]

LIBPATH += [root] 

#---------------------------------------------------------------------------------
# Sub target
#---------------------------------------------------------------------------------
list = os.listdir(root)
if GetDepend(DEPENDS):
    for d in list:
        path = os.path.join(root, d)
        if os.path.isfile(os.path.join(path, 'SConscript')):
            objs = objs + SConscript(os.path.join(d, 'SConscript')) 

#---------------------------------------------------------------------------------
# Main target
#---------------------------------------------------------------------------------
objs = DefineGroup(name = PKGNAME, src = SOURCES, depend = DEPENDS, 
                   CPPPATH          = CPPPATH, 
                   CCFLAGS          = CCFLAGS, 
                   ASFLAGS          = ASFLAGS, 
                   LOCAL_CPPPATH    = LOCAL_CPPPATH, 
                   LOCAL_CCFLAGS    = LOCAL_CCFLAGS, 
                   LOCAL_ASFLAGS    = LOCAL_ASFLAGS, 
                   CPPDEFINES       = CPPDEFINES, 
                   LOCAL_CPPDEFINES = LOCAL_CPPDEFINES, 
                   LIBS             = LIBS, 
                   LIBPATH          = LIBPATH,
                   LINKFLAGS        = LINKFLAGS)  

Return("objs")

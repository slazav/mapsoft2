import os
Import ('env')
env_loc=env.Clone()


# add CPPPATH and PKG_CONFIG_PATH to global env
env_loc.Replace (CPPPATH = "#lib")
env_loc.Replace (LIBPATH = "#lib")
env_loc.Replace (LIBS    = "mapsoft")

env_loc.Append (ENV = {'PKG_CONFIG_PATH': os.getcwd()+'/pc'})
if os.getenv('PKG_CONFIG_PATH'):
  env_loc.Append (ENV = {'PKG_CONFIG_PATH':
    [ os.getcwd()+'/pc', os.getenv('PKG_CONFIG_PATH')]})

# swig = env_loc.Clone()
# swig.Append(CPPPATH = [distutils.sysconfig.get_python_inc()])
# swig.Replace(SWIGFLAGS = ['-c++', '-python'])
# swig.Replace(SHLIBPREFIX = "")
# swig.Append(LIBS = Split("geo_io geo 2d fig utils jeeps loaders tiff jpeg png curl"))
# swig.SharedLibrary("_core.so", ["swig.i"])



##################################################
## build mapsoft library

# env_loc.UseLibs('libxml-2.0 libzip libproj libgif libjpeg libpng libtiff-4 libcurl zlib yaml-0.1 shp jansson')
# env_loc.UseLibs('glibmm-2.4 gtkmm-2.4 gthread-2.0')
# env_loc.UseLibs('cairomm-1.0 pixman-1 freetype2 libusb-1.0')

env_loc.UseLibs('jansson')

# all source files
objects = Split ("""
  opt/opt.cpp
""")

env_loc.StaticLibrary('mapsoft', objects)

##################################################
## strange programs inside the core folder
programs=Split("""
""")
#  fig/catfig.cpp

map(env_loc.Program, programs)

##################################################
## simple tests: fail if error code is not zero

simple_tests=Split("""
  err/err.test.cpp
  opt/opt.test.cpp
  2d/point.test.cpp
  2d/rect.test.cpp
  2d/line.test.cpp
  2d/multiline.test.cpp

  ozi/ozi.test.cpp
""")

import os
def builder_test_simple(target, source, env):
    prg = str(source[0].abspath)
    if os.spawnl(os.P_WAIT, prg, prg)==0:
      open(str(target[0]),'w').write("PASSED\n")
    else:
      return 1

# Create a builder for tests
env_loc.Append(BUILDERS = {'TestSimple' : Builder(action = builder_test_simple)})

def build_and_run_simple(src):
  prg = env_loc.Program(src)
  res = str(prg[0]) + ".passed"
  env_loc.TestSimple(res, prg)

map(build_and_run_simple, simple_tests)

##################################################
## script tests: build a program, then run a script

script_tests=Split("""
""")
#  getopt/getopt.test.cpp

def builder_test_script(target, source, env):
    prg = str(source[0].abspath)
    scr = str(source[1].abspath)
    if os.spawnl(os.P_WAIT, scr, scr)==0:
      open(str(target[0]),'w').write("PASSED\n")
    else:
      return 1

# Create a builder for tests
env_loc.Append(BUILDERS = {'TestScript' : Builder(action = builder_test_script)})

def build_and_run_script(src):
  prg = env_loc.Program(src)
  res = str(prg[0]) + ".passed"
  scr = str(prg[0]) + ".script"
  env_loc.TestScript(res, [prg, scr])

map(build_and_run_script, script_tests)

##################################################

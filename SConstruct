######################################
# What do we want to build
subdirs_min = Split("core")
subdirs_max = subdirs_min + Split("")


######################################
## import python libraries
import os
import platform
if platform.python_version()<"2.7":
    import distutils.sysconfig as sysconfig
else:
    import sysconfig


######################################
# Create environment, add some methods

env = Environment ()
Export('env')

# UseLibs -- use pkg-config libraries
def UseLibs(env, libs):
   if isinstance(libs, list):
      libs = " ".join(libs)
   env.ParseConfig('pkg-config --cflags --libs %s' % libs)
env.AddMethod(UseLibs)

# SymLink -- create a symlink
# Arguments target and linkname follow standard order (look at ln(1))
# wd is optional base directory for both
def SymLink(env, target, linkname, wd=None):
   if wd:
      env.Command(wd+'/'+linkname, wd+'/'+target, "ln -s -- %s %s/%s" % (target, wd, linkname))
   else:
      env.Command(linkname, target, "ln -s -- %s %s" % (target, linkname))
env.AddMethod(SymLink)


######################################
## Add default flags

if os.environ.has_key('GCCVER'):
   ver = os.environ['GCCVER']
   env.Replace (CC = ("gcc-%s" % ver))
   env.Replace (CXX = ("g++-%s" % ver))

env.Append (CCFLAGS=['-O2'])
env.Append (CCFLAGS='-std=gnu++11')

env.Append (ENV = {'PKG_CONFIG_PATH': os.getcwd()+'/core/pc'})
if os.getenv('PKG_CONFIG_PATH'):
  env_loc.Append (ENV = {'PKG_CONFIG_PATH':
    [ os.getcwd()+'/core/pc', os.getenv('PKG_CONFIG_PATH')]})

env.Append (CPPPATH = "#core")
env.Append (LIBPATH = "#core")
env.Append (LIBS    = "mapsoft")

######################################
## Parse command-line arguments:

env.PREFIX = ARGUMENTS.get('prefix', '')
env.bindir=env.PREFIX+'/usr/bin'
env.datadir=env.PREFIX+'/usr/share/mapsoft'
env.man1dir=env.PREFIX+'/usr/share/man/man1'
env.figlibdir=env.PREFIX+'/usr/share/xfig/Libraries'
env.libdir=env.PREFIX+ sysconfig.get_config_var('LIBDIR')

env.Alias('install', [env.bindir, env.man1dir,
  env.datadir, env.figlibdir, env.libdir])

if ARGUMENTS.get('debug', 0):
	env.Append (CCFLAGS='-ggdb')
	env.Append (LINKFLAGS='-ggdb')
if ARGUMENTS.get('profile', 0):
	env.Append (CCFLAGS='-pg')
	env.Append (LINKFLAGS='-pg')
if ARGUMENTS.get('gprofile', 0):
	env.Append (LINKFLAGS='-lprofiler')
if ARGUMENTS.get('gheapcheck', 0):
	env.Append (LINKFLAGS='-ltcmalloc')

if ARGUMENTS.get('minimal', 0):
	SConscript (map (lambda(s): s+"/SConscript", subdirs_min))
else:
	SConscript (map (lambda(s): s+"/SConscript", subdirs_max))

Help("""
You can build mapsoft with the following options:
  scons -Q debug=1          // build with -ggdb
  scons -Q profile=1        // build with -pg
  scons -Q gheapcheck=1     // build with -ltcmalloc
  scons -Q minimal=1        // skip misc and tests dirs
  scons -Q prefix=<prefix>  // set installation prefix
""")


# vim: ft=python tw=0

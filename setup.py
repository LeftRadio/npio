
__version__ = "0.1.77"

import os
from setuptools import setup
from setuptools import find_packages
from setuptools import Extension

# ---- Do not chgange
self_path = os.path.dirname(os.path.abspath(__file__))
packages = find_packages()

extensions = []
headers = []
for _root, _dirs, _files in os.walk("source"):
    for f in _files:
        if f.endswith(".c"):
            fpth = os.path.relpath(os.path.join(_root, f))
            extensions.append(fpth.replace('\\', '/'))
        elif f.endswith(".h"):
            fpth = os.path.relpath(os.path.join(_root, f))
            headers.append(fpth.replace('\\', '/'))

data_files = ['README.md', 'sys_info']
data_files.extend(headers)

print("<-> find packages: ", packages)
print("<-> find extensions: ", [os.path.basename(e) for e in extensions])
print("<-> find data files: ", data_files)

# custom package data
setup(
  name             = "npio",
  version          = __version__,
  author           = "Vladislav Kamenev",
  author_email     = "vladislav@inorbit.com",
  description      = "Module to control NanoPi GPIO channels",
  long_description = open('README.md', 'rt').read(),
  long_description_content_type = 'text/markdown',
  license          = "",
  keywords         = "NanoPi GPIO",
  url              = "https://github.com/leftradio/npio",
  classifiers      = [
      'Development Status :: 4 - Beta',
      'Operating System :: POSIX :: Linux',
      'Programming Language :: Python :: 3',
      'Topic :: Software Development',
      'Topic :: Home Automation',
      'Topic :: System :: Hardware'
  ],
  packages         = packages,
  ext_modules      = [ Extension('_npio', extensions) ],
  data_files       = [('/npio' , data_files)])

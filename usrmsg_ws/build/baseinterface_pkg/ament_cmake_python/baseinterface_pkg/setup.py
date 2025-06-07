from setuptools import find_packages
from setuptools import setup

setup(
    name='baseinterface_pkg',
    version='0.0.1',
    packages=find_packages(
        include=('baseinterface_pkg', 'baseinterface_pkg.*')),
)

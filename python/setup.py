#!/usr/bin/env python3
from setuptools import setup, find_packages

setup(name='HP894xx',
      version='1.0.0',
      description='HP894xx Series GPIB Driver',
      url='',
      author='ElectroOptical Innovations, LLC.',
      author_email='simon.hobbs@electrooptical.net',
      license='BSD',
      packages=['HP894xx'],
      #packages=find_packages(),
      install_requires=[
          'prologix-gpib-ethernet',
          'click',
          'pyserial',
          'numpy',
          'timeout_decorator'
      ],
      test_suite='nose.collector',
      tests_require=['nose'],
      scripts=[],
      include_package_data=True,
      zip_safe=True)

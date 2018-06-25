dlib_mmod_detect
================

|Travis| |Appveyor| |License (3-Clause BSD)|

A `dlib  <https://github.com/davisking/dlib>`__ mmod object detection sample with a pre-trained eye detector built with the `Hunter <https://github.com/ruslo/hunter>`__ package manager.  This is a simple test bed for experimenting with dlib's dnn toolkit.

Sample test set output for training a small object detector with 500x2=1000 manually annotated eye crops (without occlusion) from unsplash.com face images.

.. figure::  https://user-images.githubusercontent.com/554720/41860352-7cdb50d8-786c-11e8-9a4d-7a0520533c1f.jpg 
   :alt: unsplash_face_montage

Quick Start
-----------

-  Install compiler:
   http://cgold.readthedocs.io/en/latest/first-step.html
-  Install `CMake <https://github.com/kitware/CMake>`__ (and add to ``PATH``)
-  Install Python (for Polly)
-  Clone `Polly <https://github.com/ruslo/polly>`__ and add ``<polly>/bin`` to ``PATH``

.. code-block:: bash

    export TOOLCHAIN=xcode # for more options: polly.py --help 
    export CONFIG=Release
    polly.py --toolchain ${TOOLCHAIN} --config-all ${CONFIG} --install --verbose --reconfig
    find ${HOME}/path/to/images -name "*.jpg" > images.txt
    _install/${TOOLCHAIN}/bin/dnn_mmod_find_thing_ex -i images.txt -m _install/xcode/share/dlib_mmod_eye_detector.dat -o /tmp/
    
::

...

.. |Travis| image:: https://img.shields.io/travis/elucideye/dlib_mmod_detect/master.svg?style=flat-square&label=Linux%20OSX%20Android%20iOS
   :target: https://travis-ci.org/elucideye/dlib_mmod_detect/builds
.. |Appveyor| image:: https://img.shields.io/appveyor/ci/headupinclouds/dlib-mmod-detect.svg?style=flat-square&label=Windows
.. |License (3-Clause BSD)| image:: https://img.shields.io/badge/license-BSD%203--Clause-brightgreen.svg?style=flat-square
   :target: http://opensource.org/licenses/BSD-3-Clause
   

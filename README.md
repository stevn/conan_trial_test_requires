# Conan Trial for Test Requires Visibility Conflicts

## Introduction

This is an example project showing the usage of libtiff which has a dependency to zlib, together with a unit test which uses libxml2, which also has a dependency to zlib, and the unit test also uses zlib directly itself to unpack a gzipped xml file before parsing it.

The question is how this is handled in Conan package management.

- The zlib transitive dependency on the one hand has the 'visible=True' trait, because it is used in libtiff in the core library.
- The zlib direct/transitive dependency on the other hand also has the 'visible=False' trait:
  - because it is used in libxml2 in the unit test for the core library, and
  - because it is used in the unit test directly.

There are different aspects to this:

- Is the zlib version the same? If not, then this could be a potential package version conflict.
- Are the zlib options the same? If not, then this could be a potential package options conflict.
- Should zlib be propagated to users of our package? (The answer is obviously yes, since it is needed by the core lib via libtiff.)

## Conan checks

- Conan 2.10 introduced 'risk' warnings which find options conflicts.
  - We treat these as errors (see `./conan_config/global.conf`) to avoid undefined behavior.
  - This works well.
- Conan 2.18 extended these checks to the visibility trait, which caused the problem at hand.

## Build

- Run `./build.sh good` which uses 'known good' Conan versions 2.10-2.17 to show that the project can be built and works properly with those versions.
- Run `./build.sh bad` which uses 'broken' Conan versions 2.18 / 2.19 to reproduce the issue regarding the conflict of the visibility trait.

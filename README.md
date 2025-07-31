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

## Workaround

The `bad` mode fails. However there is a workaround:

- Set `ENABLE_VISIBILITY_WORKAROUND=True` in `conanfile.py`.
- This moves the `test_requires` to "normal" `requires`, which means they get `visible=True` requirements trait instead of `visible=False`.
- But this has its own downsides: now we get the error:

```
ERROR: tiff_writer/0.1.0: package_info(): The direct dependency 'libxml2' is not used by any '(cpp_info/components).requires'.
```

So we would have to reference it in our component dependencies, which means it would be linked, which we don't want. It is only used in the unit test.

Another workaround may be to separate out the unit test into a different Conan package, but that is very impractical.

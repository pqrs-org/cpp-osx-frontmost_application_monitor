[![Build Status](https://travis-ci.org/pqrs-org/cpp-osx-frontmost_application_monitor.svg?branch=master)](https://travis-ci.org/pqrs-org/cpp-osx-frontmost_application_monitor)
[![License](https://img.shields.io/badge/license-Boost%20Software%20License-blue.svg)](https://github.com/pqrs-org/cpp-osx-frontmost_application_monitor/blob/master/LICENSE.md)

# cpp-osx-frontmost_application_monitor

A utility of `SCDynamicStoreCopyConsoleUser`.

## Requirements

cpp-osx-frontmost_application_monitor depends the following classes.

- [Nod](https://github.com/fr00b0/nod)
- [pqrs::dispatcher](https://github.com/pqrs-org/cpp-dispatcher)
- [pqrs::weakify](https://github.com/pqrs-org/objc-weakify)

## Install

### Using package manager

You can install `include/pqrs` by using [cget](https://github.com/pfultz2/cget).

```shell
cget install pqrs-org/cget-recipes
cget install pqrs-org/cpp-osx-frontmost_application_monitor
```

And then, add `cget/src/pqrs/osx/frontmost_application_monitor/objc.m` to your project sources.

### Manual install

Copy `include/pqrs` directory into your include directory.

And then, add `src/pqrs/osx/frontmost_application_monitor/objc.m` to your project sources.

[![Build Status](https://github.com/pqrs-org/cpp-osx-frontmost_application_monitor/workflows/CI/badge.svg)](https://github.com/pqrs-org/cpp-osx-frontmost_application_monitor/actions)
[![License](https://img.shields.io/badge/license-Boost%20Software%20License-blue.svg)](https://github.com/pqrs-org/cpp-osx-frontmost_application_monitor/blob/main/LICENSE.md)

# cpp-osx-frontmost_application_monitor

A utility of `NSRunningApplication`.

## Requirements

cpp-osx-frontmost_application_monitor depends the following classes.

- [Nod](https://github.com/fr00b0/nod)
- [pqrs::dispatcher](https://github.com/pqrs-org/cpp-dispatcher)
- [pqrs::hash](https://github.com/pqrs-org/cpp-hash).

## Install

Copy `include/pqrs` and `vendor/vendor/include` directories into your include directory.

And then configure your project as follows:

- Add `src/pqrs/osx/frontmost_application_monitor/PQRSOSXFrontmostApplicationMonitorImpl.swift`
- Add `include/pqrs/osx/frontmost_application_monitor/impl/Bridging-Header.h` as Bridging Header.

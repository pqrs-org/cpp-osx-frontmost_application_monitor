# appendix

## PQRSOSXFrontmostApplicationMonitorImpl.swift

Swift Concurrency version of PQRSOSXFrontmostApplicationMonitorImpl.swift.
It requires `libswift_Concurrency.dylib` on macOS 11, but there is no reliable way to deploy it with C++ project.
So, we use DispatchQueue in `src/pqrs/osx/frontmost_application_monitor/PQRSOSXFrontmostApplicationMonitorImpl.swift`.

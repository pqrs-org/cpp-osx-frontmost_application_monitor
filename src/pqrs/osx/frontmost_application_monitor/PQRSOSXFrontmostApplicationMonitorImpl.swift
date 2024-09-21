// (C) Copyright Takayama Fumihiko 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See https://www.boost.org/LICENSE_1_0.txt)

import AppKit

private actor PQRSOSXFrontmostApplicationMonitor {
  static let shared = PQRSOSXFrontmostApplicationMonitor()

  struct Callback {
    let function: pqrs_osx_frontmost_application_monitor_callback
  }

  var callbacks: [Callback] = []

  init() {
    let sharedWorkspace = NSWorkspace.shared
    let notificationCenter = sharedWorkspace.notificationCenter

    notificationCenter.addObserver(
      forName: NSWorkspace.didActivateApplicationNotification,
      object: sharedWorkspace,
      queue: nil
    ) { note in
      guard let userInfo = note.userInfo else {
        print("Missing notification info on NSWorkspace.didActivateApplicationNotification")
        return
      }

      guard
        let runningApplication = userInfo[NSWorkspace.applicationUserInfoKey]
          as? NSRunningApplication
      else {
        print("Missing runningApplication on NSWorkspace.didActivateApplicationNotification")
        return
      }

      let bundleIdentifier = runningApplication.bundleIdentifier ?? ""
      let path = runningApplication.executableURL?.path ?? ""

      Task.detached {
        await self.runCallback(bundleIdentifier: bundleIdentifier, path: path)
      }
    }
  }

  func register(_ callback: Callback) {
    for c in callbacks {
      let ptr1 = unsafeBitCast(c.function, to: Optional<UnsafeRawPointer>.self)
      let ptr2 = unsafeBitCast(callback.function, to: Optional<UnsafeRawPointer>.self)
      if ptr1 == ptr2 {
        return
      }
    }

    callbacks.append(callback)
  }

  func unregister(_ callback: Callback) {
    callbacks.removeAll(where: {
      let ptr1 = unsafeBitCast($0.function, to: Optional<UnsafeRawPointer>.self)
      let ptr2 = unsafeBitCast(callback.function, to: Optional<UnsafeRawPointer>.self)
      return ptr1 == ptr2
    })
  }

  func runCallback(bundleIdentifier: String, path: String) {
    for c in callbacks {
      bundleIdentifier.utf8CString.withUnsafeBufferPointer { bundleIdentifierPtr in
        path.utf8CString.withUnsafeBufferPointer { pathPtr in
          c.function(
            bundleIdentifierPtr.baseAddress,
            pathPtr.baseAddress
          )
        }
      }
    }
  }

  func runCallbackWithFrontmostApplication() {
    if let runningApplication = NSWorkspace.shared.frontmostApplication {
      let bundleIdentifier = runningApplication.bundleIdentifier ?? ""
      let path = runningApplication.executableURL?.path ?? ""

      runCallback(bundleIdentifier: bundleIdentifier, path: path)
    }
  }
}

@_cdecl("pqrs_osx_frontmost_application_monitor_register")
func register(_ function: pqrs_osx_frontmost_application_monitor_callback) {
  Task.detached {
    await PQRSOSXFrontmostApplicationMonitor.shared.register(
      PQRSOSXFrontmostApplicationMonitor.Callback(
        function: function
      ))

    await PQRSOSXFrontmostApplicationMonitor.shared.runCallbackWithFrontmostApplication()
  }
}

@_cdecl("pqrs_osx_frontmost_application_monitor_unregister")
func unregister(_ function: pqrs_osx_frontmost_application_monitor_callback) {
  Task.detached {
    await PQRSOSXFrontmostApplicationMonitor.shared.unregister(
      PQRSOSXFrontmostApplicationMonitor.Callback(
        function: function
      ))
  }
}

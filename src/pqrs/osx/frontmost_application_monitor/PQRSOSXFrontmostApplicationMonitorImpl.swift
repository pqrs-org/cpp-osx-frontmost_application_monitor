// (C) Copyright Takayama Fumihiko 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See https://www.boost.org/LICENSE_1_0.txt)

import AppKit

private actor PQRSOSXFrontmostApplicationMonitor {
  static let shared = PQRSOSXFrontmostApplicationMonitor()

  var callback: pqrs_osx_frontmost_application_monitor_callback?

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

  func setCallback(_ callback: pqrs_osx_frontmost_application_monitor_callback) {
    self.callback = callback
  }

  func unsetCallback() {
    callback = nil
  }

  func runCallback(bundleIdentifier: String, path: String) {
    bundleIdentifier.utf8CString.withUnsafeBufferPointer { bundleIdentifierPtr in
      path.utf8CString.withUnsafeBufferPointer { pathPtr in
        callback?(
          bundleIdentifierPtr.baseAddress,
          pathPtr.baseAddress
        )
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

@_cdecl("pqrs_osx_frontmost_application_monitor_set_callback")
func PQRSOSXFrontmostApplicationMonitorSetCallback(
  _ callback: pqrs_osx_frontmost_application_monitor_callback
) {
  Task.detached {
    await PQRSOSXFrontmostApplicationMonitor.shared.setCallback(callback)
  }
}

@_cdecl("pqrs_osx_frontmost_application_monitor_unset_callback")
func PQRSOSXFrontmostApplicationMonitorUnsetCallback() {
  Task.detached {
    await PQRSOSXFrontmostApplicationMonitor.shared.unsetCallback()
  }
}

@_cdecl("pqrs_osx_frontmost_application_monitor_trigger")
func PQRSOSXFrontmostApplicationMonitorTrigger() {
  Task.detached {
    await PQRSOSXFrontmostApplicationMonitor.shared.runCallbackWithFrontmostApplication()
  }
}

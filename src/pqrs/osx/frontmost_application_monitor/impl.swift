import AppKit

@MainActor
private class Monitor {
  static let shared = Monitor()

  struct Callback {
    let function: pqrs_osx_frontmost_application_monitor_callback
    let context: UnsafeMutableRawPointer?
  }

  var callbacks: [Callback] = []

  init() {
    let sharedWorkspace = NSWorkspace.shared
    let notificationCenter = sharedWorkspace.notificationCenter

    notificationCenter.addObserver(
      forName: NSWorkspace.didActivateApplicationNotification,
      object: sharedWorkspace,
      queue: OperationQueue.main
    ) { note in
      guard let userInfo = note.userInfo else {
        print("Missing notification info on NSWorkspace.didActivateApplicationNotification")
        return
      }
      let runningApplication = userInfo[NSWorkspace.applicationUserInfoKey] as! NSRunningApplication

      Task { @MainActor in
        self.runCallback(runningApplication)
      }
    }
  }

  func register(_ callback: Callback) {
    for c in callbacks {
      let ptr1 = unsafeBitCast(c.function, to: Optional<UnsafeRawPointer>.self)
      let ptr2 = unsafeBitCast(callback.function, to: Optional<UnsafeRawPointer>.self)
      if ptr1 == ptr2 && c.context == callback.context {
        return
      }
    }

    callbacks.append(callback)
  }

  func unregister(_ callback: Callback) {
    callbacks.removeAll(where: {
      let ptr1 = unsafeBitCast($0.function, to: Optional<UnsafeRawPointer>.self)
      let ptr2 = unsafeBitCast(callback.function, to: Optional<UnsafeRawPointer>.self)
      return ptr1 == ptr2 && $0.context == callback.context
    })
  }

  func runCallback(_ runningApplication: NSRunningApplication) {
    for c in callbacks {
      let bundleIdentifier = runningApplication.bundleIdentifier ?? ""
      let path = runningApplication.executableURL?.absoluteString ?? ""

      bundleIdentifier.utf8CString.withUnsafeBufferPointer { bundleIdentifierPtr in
        path.utf8CString.withUnsafeBufferPointer { pathPtr in
          c.function(
            bundleIdentifierPtr.baseAddress,
            pathPtr.baseAddress,
            c.context
          )
        }
      }
    }
  }

  func runCallbackWithFrontmostApplication() {
    if let runningApplication = NSWorkspace.shared.frontmostApplication {
      runCallback(runningApplication)
    }
  }
}

@_cdecl("pqrs_osx_frontmost_application_monitor_register")
func register(
  _ function: pqrs_osx_frontmost_application_monitor_callback,
  _ context: pqrs_osx_frontmost_application_monitor_callback_context
) {
  Task { @MainActor in
    Monitor.shared.register(
      Monitor.Callback(
        function: function,
        context: context
      ))

    Monitor.shared.runCallbackWithFrontmostApplication()
  }
}

@_cdecl("pqrs_osx_frontmost_application_monitor_unregister")
func unregister(
  _ function: pqrs_osx_frontmost_application_monitor_callback,
  _ context: pqrs_osx_frontmost_application_monitor_callback_context
) {
  Task { @MainActor in
    Monitor.shared.unregister(
      Monitor.Callback(
        function: function,
        context: context
      ))
  }
}

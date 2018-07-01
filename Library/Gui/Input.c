///
/// @file Library/Gui/Input.c
///
/// User input
///

#include <Gui/Client.h>

//// mDevices
///// Input devices
//STATIC ObjectList<CONST Device> mDevices;
//// mEvents
///// Input events
//STATIC ObjectList<CONST Event>  mEvents;
//// mNextEvent
///// Next event index
//STATIC UINTN                    mNextEvent;
//
//// ~Event
///// Input device event destructor
//Event::~Event (
//  VOID
//) {
//}
//
//// Event
///// Input device event constructor
///// @param EventType The input device event type
//Event::Event (
//  Type EventType
//) {
//  mType = EventType;
//}
//
//// Event::GetType
///// Get the input device event type
//Event::Type
//Event::GetType (
//  VOID
//) CONST {
//  return mType;
//}
//
//// ~KeyEvent
///// Input device key press event destructor
//KeyEvent::~KeyEvent (
//  VOID
//) {
//}
//
//// KeyEvent
///// Input device key press event constructor
///// @param KeyPress  The key press data
///// @param EventType The input device event type
//KeyEvent::KeyEvent (
//  CONST EFI_KEY_DATA &KeyPress,
//  Type                EventType
//) : Event(EventType) {
//  mKeyPress = KeyPress;
//}
//
//// Input::KeyEvent
///// Input device key press event constructor
///// @param KeyPress  The key press data
//KeyEvent *
//KeyEvent::Create (
//  CONST EFI_KEY_DATA &KeyPress
//) {
//  return new KeyEvent(KeyPress, Type::Key);
//}
//
//// KeyEvent::GetKeyPress
///// Get the key press data
///// @return The key press data
//CONST EFI_KEY_DATA &
//KeyEvent::GetKeyPress (
//  VOID
//) CONST {
//  return mKeyPress;
//}
//
//// ~PointerEvent
///// Input device pointer event destructor
//PointerEvent::~PointerEvent (
//  VOID
//) {
//}
//
//// PointerEvent
///// Input device pointer event constructor
///// @param X The pointer x axis change
///// @param Y The pointer y axis change
///// @param EventType The input device event type
//PointerEvent::PointerEvent (
//  INTN X,
//  INTN Y,
//  Type EventType
//) : Event(EventType) {
//  mX = X;
//  mY = Y;
//}
//
//// Input::PointerEvent::Create
///// Input device pointer event constructor
///// @param X The pointer x axis change
///// @param Y The pointer y axis change
//PointerEvent *
//PointerEvent::Create (
//  INTN X,
//  INTN Y
//) {
//  return new PointerEvent(X, Y, Type::Pointer);
//}
//
//// Input::PointerEvent::GetX
///// Get the pointer x axis change
///// @return The pointer x axis change
//INTN
//PointerEvent::GetX (
//  VOID
//) CONST {
//  return mX;
//}
//// Input::PointerEvent::GetY
///// Get the pointer y axis change
///// @return The pointer y axis change
//INTN
//PointerEvent::GetY (
//  VOID
//) CONST {
//  return mY;
//}
//
//// ~ScrollEvent
///// Input device scroll event destructor
//ScrollEvent::~ScrollEvent (
//  VOID
//) {
//}
//
//// ScrollEvent
///// Input device scroll event constructor
///// @param HorizontalScroll The horizontal scroll offset
///// @param VerticalScroll   The vertical scroll offset
///// @param EventType        The input device event type
//ScrollEvent::ScrollEvent (
//  INTN            HorizontalScroll,
//  INTN            VerticalScroll,
//  Type            EventType
//) : Event(EventType) {
//  mHorizontalScroll = HorizontalScroll;
//  mVerticalScroll = VerticalScroll;
//}
//
//// Input::ScrollEvent::Create
///// Input device scroll event constructor
///// @param HorizontalScroll The horizontal scroll offset
///// @param VerticalScroll   The vertical scroll offset
//ScrollEvent *
//ScrollEvent::Create (
//  INTN            HorizontalScroll,
//  INTN            VerticalScroll
//) {
//  return new ScrollEvent(HorizontalScroll, VerticalScroll, Type::Scroll);
//}
//
//// ScrollEvent::HorizontalScroll
///// Get the horizontal scroll offset
///// @return The horizontal scroll offset
//INTN
//ScrollEvent::HorizontalScroll (
//  VOID
//) CONST {
//  return mHorizontalScroll;
//}
//// ScrollEvent::VerticalScroll
///// Get the vertical scroll offset
///// @return The vertical scroll offset
//INTN
//ScrollEvent::VerticalScroll (
//  VOID
//) CONST {
//  return mVerticalScroll;
//}
//
//// ~PressEvent
///// Input device touch or pointer button press event destructor
//PressEvent::~PressEvent (
//  VOID
//) {
//}
//
//// PressEvent
///// Input device touch or pointer button press event constructor
///// @param Button    The pressed button index
///// @param EventType The input device event type
//PressEvent::PressEvent (
//  UINTN Button,
//  Type  EventType
//) : Event(EventType) {
//  mButton = Button;
//}
//
//// Input::PressEvent::Create
///// Input device touch or pointer button press event constructor
///// @param Button The pressed button index
//PressEvent *
//PressEvent::Create (
//  UINTN Button
//) {
//  return new PressEvent(Button, Type::Press);
//}
//
//// PressEvent::GetButton
///// Get the pressed button
///// @return The pressed button index
//UINTN
//PressEvent::GetButton (
//  VOID
//) CONST {
//  return mButton;
//}
//
//// ~Device
///// Input device destructor
//Device::~Device (
//  VOID
//) {
//  ClearEvents();
//}
//
//// Device
///// Input device constructor
///// @param Handle The handle for the device
//Device::Device (
//  EFI_HANDLE Handle
//) {
//  mHandle = Handle;
//  mNextEvent = 0;
//  Reset(TRUE);
//}
//
//// Device::Handle
///// Get the handle for the device
///// @return The device handle
//EFI_HANDLE
//Device::Handle (
//  VOID
//) CONST {
//  return mHandle;
//}
//
//// Device::Reset
///// Reset this input device
///// @param HardwareCheck Whether to perform an hardware verification check of this input device
///// @return Whether the input device was reset and reinitialized or not
//BOOLEAN
//Device::Reset (
//  BOOLEAN // HardwareCheck
//) CONST {
//  return TRUE;
//}
//
//// Device::Update
///// Update this input device
//VOID
//Device::Update (
//  VOID
//) {
//}
//
//// Device::HasEvents
///// Check whether this input device has events waiting
///// @param Filter The input device event types to check
//BOOLEAN
//Device::HasEvents (
//  UINTN Filter
//) CONST {
//  if (Filter == 0) {
//    // No filter so return TRUE if there are any pending events
//    return (mEvents.GetCount() > mNextEvent);
//  } else {
//    // Filter events
//    for (UINTN Index = mNextEvent; Index < mEvents.GetCount(); ++Index) {
//      if (mEvents[Index] != NULL) {
//        // Get event type to check filter
//        UINTN Type = mEvents[Index]->GetType();
//        if ((Filter & Type) == Type) {
//          // Matched filter
//          return TRUE;
//        }
//      }
//    }
//  }
//  // No filter match
//  return FALSE;
//}
//// Device::GetEvent
///// Get an event from this input device
///// @param Filter The input device event types to retrieve
///// @return The input device event
//Event *
//Device::GetEvent (
//  UINTN Filter
//) {
//  // Set the event to a dummy event first
//  Event *NextEvent = NULL;
//  // Check to make sure the next event is valid
//  while ((mNextEvent < mEvents.GetCount()) && (mEvents[mNextEvent] == NULL)) {
//    ++mNextEvent;
//  }
//  // Remove any events already dispatched
//  if (mNextEvent != 0) {
//    // Remove the events from the list
//    mEvents.Remove(0, mNextEvent);
//    // Reset the next event to the first
//    mNextEvent = 0;
//  }
//  // Check there is a next event
//  if (mNextEvent < mEvents.GetCount()) {
//    if (Filter == 0) {
//      // No filter so grab next event
//      NextEvent = mEvents[mNextEvent];
//      mEvents[mNextEvent++] = NULL;
//    } else {
//      // Filter events
//      for (UINTN Index = mNextEvent; mNextEvent < mEvents.GetCount(); ++Index) {
//        UINTN Type = mEvents[Index]->GetType();
//        if ((Filter & Type) == Type) {
//          // Filtered event found
//          NextEvent = mEvents[Index];
//          // Check if the event needs swapped with the next event
//          if (Index != mNextEvent) {
//            // Swap the filter match event with the next event to prevent matching this event again
//            mEvents[Index] = mEvents[mNextEvent];
//            mEvents[mNextEvent] = NextEvent;
//          }
//          // Move to the next event
//          mEvents[mNextEvent++] = NULL;
//          break;
//        }
//      }
//    }
//  }
//  // Return the event
//  return NextEvent;
//}
//// Input::Device::ClearEvents
///// Clear all events from this input device
//VOID
//Device::ClearEvents (
//  VOID
//) {
//  mEvents.Clear();
//}
//
//// Device::QueueEvent
///// Place an event in this input device's queue
///// @param Event The event to place in the queue
//VOID
//Device::QueueEvent (
//  Event *Event
//) {
//  // Check event is valid before enqueuing
//  if ((Event != NULL) && (Event->GetType() != 0)) {
//    mEvents.Add(Event);
//  }
//}
//
//// ~KeyboardDevice
///// Keyboard input device destructor
//KeyboardDevice::~KeyboardDevice (
//  VOID
//) {
//}
//
//// KeyboardDevice
///// Keyboard input device constructor
///// @param Handle   The handle for the device
///// @param Keyboard The keyboard protocol interface for this input device
//KeyboardDevice::KeyboardDevice (
//  EFI_HANDLE                      Handle,
//  EFI_SIMPLE_TEXT_INPUT_PROTOCOL *Keyboard
//) : Device(Handle) {
//  mKeyboard = Keyboard;
//}
//
//// Input::KeyboardDevice::Create
///// Keyboard input device constructor
///// @param Handle   The handle for the device
///// @param Keyboard The keyboard protocol interface for this input device
//KeyboardDevice *
//KeyboardDevice::Create (
//  EFI_HANDLE                      Handle,
//  EFI_SIMPLE_TEXT_INPUT_PROTOCOL *Keyboard
//) {
//  return new KeyboardDevice(Handle, Keyboard);
//}
//
//// KeyboardDevice::Reset
///// Reset this input device
///// @param HardwareCheck Whether to perform an hardware verification check of this input device
///// @return Whether the input device was reset and reinitialized or not
//BOOLEAN
//KeyboardDevice::Reset (
//  BOOLEAN HardwareCheck
//) CONST {
//  return ((mKeyboard != NULL) && (mKeyboard->Reset != NULL) && !EFI_ERROR(mKeyboard->Reset(mKeyboard, HardwareCheck)));
//}
//
//// KeyboardDevice::Update
///// Update this input device
//VOID
//KeyboardDevice::Update (
//  VOID
//) {
//  if ((mKeyboard != NULL) && (mKeyboard->ReadKeyStroke != NULL)) {
//    EFI_KEY_DATA Key = { { 0, 0 }, { 0 , 0 } };
//    while (!EFI_ERROR(mKeyboard->ReadKeyStroke(mKeyboard, &(Key.Key)))) {
//      KeyEvent *Event = KeyEvent::Create(Key);
//      if (Event != NULL) {
//        QueueEvent(Event);
//        Event->Release();
//      }
//    }
//  }
//}
//
//// ~KeyboardExDevice
///// Keyboard extended input device destructor
//KeyboardExDevice::~KeyboardExDevice (
//  VOID
//) {
//}
//
//// KeyboardExDevice
///// Keyboard extended input device constructor
///// @param Handle     The handle for the device
///// @param KeyboardEx The keyboard extended protocol interface for this input device
//KeyboardExDevice::KeyboardExDevice (
//  EFI_HANDLE                         Handle,
//  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *KeyboardEx
//) : Device(Handle) {
//  mKeyboardEx = KeyboardEx;
//}
//
//// Input::KeyboardExDevice::Create
///// Keyboard extended input device constructor
///// @param Handle     The handle for the device
///// @param KeyboardEx The keyboard extended protocol interface for this input device
//KeyboardExDevice *
//KeyboardExDevice::Create (
//  EFI_HANDLE                         Handle,
//  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *KeyboardEx
//) {
//  return new KeyboardExDevice(Handle, KeyboardEx);
//}
//
//// KeyboardExDevice::Reset
///// Reset this input device
///// @param HardwareCheck Whether to perform an hardware verification check of this input device
///// @return Whether the input device was reset and reinitialized or not
//BOOLEAN
//KeyboardExDevice::Reset (
//  BOOLEAN HardwareCheck
//) CONST {
//  return ((mKeyboardEx != NULL) && (mKeyboardEx->Reset != NULL) && !EFI_ERROR(mKeyboardEx->Reset(mKeyboardEx, HardwareCheck)));
//}
//
//// KeyboardExDevice::Update
///// Update this input device
//VOID
//KeyboardExDevice::Update (
//  VOID
//) {
//  if ((mKeyboardEx != NULL) && (mKeyboardEx->ReadKeyStrokeEx != NULL)) {
//    EFI_KEY_DATA Key = { { 0, 0 }, { 0, 0 } };
//    while (!EFI_ERROR(mKeyboardEx->ReadKeyStrokeEx(mKeyboardEx, &Key))) {
//      KeyEvent *Event = KeyEvent::Create(Key);
//      if (Event != NULL) {
//        QueueEvent(Event);
//        Event->Release();
//      }
//    }
//  }
//}
//
//// ~PointerDevice
///// Pointer input device destructor
//PointerDevice::~PointerDevice (
//  VOID
//) {
//}
//
//// PointerDevice
///// Pointer input device constructor
///// @param Handle  The handle for the device
///// @param Pointer The pointer protocol interface for the input device
//PointerDevice::PointerDevice (
//  EFI_HANDLE                   Handle,
//  EFI_SIMPLE_POINTER_PROTOCOL *Pointer
//) : Device(Handle) {
//  mPointer = Pointer;
//}
//
//// Input::PointerDevice::Create
///// Pointer input device constructor
///// @param Handle  The handle for the device
///// @param Pointer The pointer protocol interface for the input device
//PointerDevice *
//PointerDevice::Create (
//  EFI_HANDLE                   Handle,
//  EFI_SIMPLE_POINTER_PROTOCOL *Pointer
//) {
//  return new PointerDevice(Handle, Pointer);
//}
//
//// PointerDevice::Reset
///// Reset this input device
///// @param HardwareCheck Whether to perform an hardware verification check of this input device
///// @return Whether the input device was reset and reinitialized or not
//BOOLEAN
//PointerDevice::Reset (
//  BOOLEAN HardwareCheck
//) CONST {
//  return ((mPointer != NULL) && (mPointer->Reset != NULL) && !EFI_ERROR(mPointer->Reset(mPointer, HardwareCheck)));
//}
//
//// PointerDevice::Update
///// Update this input device
//VOID
//PointerDevice::Update (
//  VOID
//) {
//  if ((mPointer != NULL) && (mPointer->GetState != NULL)) {
//    EFI_SIMPLE_POINTER_STATE State = { 0, 0, 0, FALSE, FALSE };
//    if (!EFI_ERROR(mPointer->GetState(mPointer, &State))) {
//      Boot::Console << State.RelativeMovementX << L" " << State.RelativeMovementY << L" " << State.RelativeMovementZ << (State.LeftButton ? L" TRUE" : L" FALSE") << (State.RightButton ? L" TRUE" : L" FALSE") << NewLine;
//      // Check for any pointer movement
//      if ((State.RelativeMovementX != 0) || (State.RelativeMovementY != 0)) {
//        // Create pointer event
//        PointerEvent *Event = PointerEvent::Create(State.RelativeMovementX, State.RelativeMovementY);
//        if (Event != NULL) {
//          QueueEvent(Event);
//          Event->Release();
//        }
//      }
//      // Check for any pointer button press
//      if (State.LeftButton) {
//        PressEvent *Event = PressEvent::Create(PressEvent::PrimaryButton);
//        if (Event != NULL) {
//          QueueEvent(Event);
//          Event->Release();
//        }
//      }
//      if (State.RightButton) {
//        PressEvent *Event = PressEvent::Create(PressEvent::SecondaryButton);
//        if (Event != NULL) {
//          QueueEvent(Event);
//          Event->Release();
//        }
//      }
//      // Check for any pointer wheel scrolling
//      if (State.RelativeMovementZ != 0) {
//        ScrollEvent *Event = ScrollEvent::Create((INTN)State.RelativeMovementZ);
//        if (Event != NULL) {
//          QueueEvent(Event);
//          Event->Release();
//        }
//      }
//    }
//  }
//}
//
//// ~TouchDevice
///// Touch input device destructor
//TouchDevice::~TouchDevice (
//  VOID
//) {
//}
//
//// TouchDevice
///// Touch input device constructor
///// @param Handle The handle for the device
///// @param Touch The touch protocol interface for this input device
//TouchDevice::TouchDevice (
//  EFI_HANDLE                     Handle,
//  EFI_ABSOLUTE_POINTER_PROTOCOL *Touch
//) : Device(Handle) {
//  mTouch = Touch;
//}
//
//// Input::TouchDevice::Create
///// Touch input device constructor
///// @param Handle The handle for the device
///// @param Touch The touch protocol interface for this input device
//TouchDevice *
//TouchDevice::Create (
//  EFI_HANDLE                     Handle,
//  EFI_ABSOLUTE_POINTER_PROTOCOL *Touch
//) {
//  return new TouchDevice(Handle, Touch);
//}
//
//// TouchDevice::Reset
///// Reset this input device
///// @param HardwareCheck Whether to perform an hardware verification check of this input device
///// @return Whether the input device was reset and reinitialized or not
//BOOLEAN
//TouchDevice::Reset (
//  BOOLEAN HardwareCheck
//) CONST {
//  return ((mTouch != NULL) && (mTouch->Reset != NULL) && !EFI_ERROR(mTouch->Reset(mTouch, HardwareCheck)));
//}
//
//// TouchDevice::Update
///// Update this input device
//VOID
//TouchDevice::Update (
//  VOID
//) {
//  if ((mTouch != NULL) && (mTouch->GetState != NULL)) {
//    EFI_ABSOLUTE_POINTER_STATE State = { 0, 0, 0, 0 };
//    if (!EFI_ERROR(mTouch->GetState(mTouch, &State))) {
//      Boot::Console << State.CurrentX << L" " << State.CurrentY << L" " << State.CurrentZ << L" " << State.ActiveButtons << NewLine;
//      // Check if the position has changed or pressure active
//      Position Position(State.CurrentX, State.CurrentY);
//      if ((Position != mPosition) || (State.CurrentZ != 0)) {
//        // Create pointer event
//        PointerEvent *Event = PointerEvent::Create(Position.X - mPosition.X, Position.Y - mPosition.Y);
//        if (Event != NULL) {
//          QueueEvent(Event);
//          Event->Release();
//        }
//        // Set previous position to current
//        mPosition = Position;
//      }
//      // Check if touch is active
//      if ((State.ActiveButtons & EFI_ABSP_TouchActive) != 0) {
//        PressEvent *Event = PressEvent::Create(PressEvent::PrimaryButton);
//        if (Event != NULL) {
//          QueueEvent(Event);
//          Event->Release();
//        }
//      }
//      // Check if secondary is active
//      if ((State.ActiveButtons & EFI_ABSP_AltActive) != 0) {
//        PressEvent *Event = PressEvent::Create(PressEvent::SecondaryButton);
//        if (Event != NULL) {
//          QueueEvent(Event);
//          Event->Release();
//        }
//      }
//    }
//  }
//}
//
//// Devices
///// Get the input devices
///// @return The input devices
//CONST ObjectList<CONST Device> &
//Devices (
//  VOID
//) {
//  return mDevices;
//}
//// Events
///// Get the global input events
///// @return The global input events
//CONST ObjectList<CONST Event> &
//Events (
//  VOID
//) {
//  return mEvents;
//}
//
//// Enumerate
///// Enumerate input devices
//VOID
//Enumerate (
//  VOID
//) {
//  Boot::Console << L"Enumerating input ..." << NewLine;
//  // Clear all previous input devices
//  mDevices.Clear();
//  // Enumerate keyboard devices
//  UINTN             Count = 0;
//  EFI_HANDLE       *Handles = 0;
//  List<EFI_HANDLE>  InputHandles;
//  // Get all the handles with keyboard device protocols
//  if (!EFI_ERROR(EfiLocateHandleBuffer(ByProtocol, &gEfiSimpleTextInputProtocolGuid, NULL, &Count, &Handles)) && (Handles != NULL)) {
//    for (UINTN Index = 0; Index < Count; ++Index) {
//      if (Handles[Index] != NULL) {
//        InputHandles.AddOnce(Handles[Index]);
//      }
//    }
//    Boot::Memory::Free<EFI_HANDLE>(Handles);
//  }
//  // Get all the handles with extended keyboard device protocols
//  Count = 0;
//  Handles = NULL;
//  if (!EFI_ERROR(EfiLocateHandleBuffer(ByProtocol, &gEfiSimpleTextInputExProtocolGuid, NULL, &Count, &Handles)) && (Handles != NULL)) {
//    for (UINTN Index = 0; Index < Count; ++Index) {
//      if (Handles[Index] != NULL) {
//        InputHandles.AddOnce(Handles[Index]);
//      }
//    }
//    Boot::Memory::Free<EFI_HANDLE>(Handles);
//  }
//  // Enumerate keyboard and extended keyboard devices from the handles
//  for (UINTN Index = 0; Index < InputHandles.GetCount(); ++Index) {
//    EFI_SIMPLE_TEXT_INPUT_PROTOCOL    *Keyboard = NULL;
//    EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *KeyboardEx = NULL;
//    EFI_HANDLE                         Handle = InputHandles[Index];
//    CHAR16                            *DevicePathText = EfiDevicePathFullTextFromHandle(Handle);
//    if (DevicePathText != NULL) {
//      // Get the extended keyboard protocol
//      if (!EFI_ERROR(EfiHandleProtocol(Handle, &gEfiSimpleTextInputExProtocolGuid, (VOID **)&KeyboardEx)) && (KeyboardEx != NULL)) {
//        // Add the device if the protocol was found
//        Boot::Console << L"  Found extended keyboard input: " << DevicePathText << NewLine;
//        KeyboardExDevice *Device = KeyboardExDevice::Create(Handle, KeyboardEx);
//        if (Device != NULL) {
//          mDevices.Add(Device);
//          Device->Release();
//        }
//        continue;
//      }
//      if (!EFI_ERROR(EfiHandleProtocol(Handle, &gEfiSimpleTextInputProtocolGuid, (VOID **)&Keyboard)) && (Keyboard != NULL)) {
//        // Add the device if the protocol was found
//        Boot::Console << L"  Found keyboard input: " << DevicePathText << NewLine;
//        KeyboardDevice *Device = KeyboardDevice::Create(Handle, Keyboard);
//        if (Device != NULL) {
//          mDevices.Add(Device);
//          Device->Release();
//        }
//      }
//      Boot::Memory::Free<CHAR16>(DevicePathText);
//    }
//  }
//  // Enumerate pointer devices
//  Count = 0;
//  Handles = NULL;
//  // Get all the handles with pointer device protocols
//  if (!EFI_ERROR(EfiLocateHandleBuffer(ByProtocol, &gEfiSimplePointerProtocolGuid, 0, &Count, &Handles)) && (Handles != NULL)) {
//    // Try to get the pointer device protocol from each handle
//    for (UINTN Index = 0; Index < Count; ++Index) {
//      EFI_SIMPLE_POINTER_PROTOCOL *Pointer = NULL;
//      EFI_HANDLE                   Handle = Handles[Index];
//      CHAR16                      *DevicePathText = EfiDevicePathFullTextFromHandle(Handle);
//      if (DevicePathText != NULL) {
//        // Get the pointer protocol
//        if (!EFI_ERROR(EfiHandleProtocol(Handle, &gEfiSimplePointerProtocolGuid, (VOID **)&Pointer)) && (Pointer != NULL)) {
//          EFI_SIMPLE_POINTER_MODE *Mode = Pointer->Mode;
//          // Determine pointer protocol interface has valid mode
//          if ((Mode != NULL) && (Mode->ResolutionX != 0) && (Mode->ResolutionY != 0) && Mode->LeftButton) {
//            // Add the device if the protocol was found
//            Boot::Console << L"  Found pointer input: " << DevicePathText << NewLine;
//            PointerDevice *Device = PointerDevice::Create(Handle, Pointer);
//            if (Device != NULL) {
//              mDevices.Add(Device);
//              Device->Release();
//            }
//          }
//        }
//        Boot::Memory::Free<CHAR16>(DevicePathText);
//      }
//    }
//    Boot::Memory::Free<EFI_HANDLE>(Handles);
//  }
//  // Enumerate touch devices
//  Count = 0;
//  Handles = NULL;
//  // Get all the handles with touch device protocols
//  if (!EFI_ERROR(EfiLocateHandleBuffer(ByProtocol, &gEfiAbsolutePointerProtocolGuid, 0, &Count, &Handles)) && (Handles != NULL)) {
//    // Try to get the touch device protocol from each handle
//    for (UINTN Index = 0; Index < Count; ++Index) {
//      EFI_ABSOLUTE_POINTER_PROTOCOL *Touch = NULL;
//      EFI_HANDLE                     Handle = Handles[Index];
//      CHAR16                        *DevicePathText = EfiDevicePathFullTextFromHandle(Handle);
//      if (DevicePathText != NULL) {
//        // Get the touch protocol
//        if (!EFI_ERROR(EfiHandleProtocol(Handle, &gEfiAbsolutePointerProtocolGuid, (VOID **)&Touch)) && (Touch != NULL)) {
//          EFI_ABSOLUTE_POINTER_MODE *Mode = Touch->Mode;
//          // Determine touch protocol interface has valid mode
//          if ((Mode != NULL) && ((Mode->AbsoluteMaxX != 0) || (Mode->AbsoluteMinX != 0)) && ((Mode->AbsoluteMaxY != 0) || (Mode->AbsoluteMinY != 0))) {
//            // Add the device if the protocol was found
//            Boot::Console << L"  Found touch input: " << DevicePathText << NewLine;
//            TouchDevice *Device = TouchDevice::Create(Handle, Touch);
//            if (Device != NULL) {
//              mDevices.Add(Device);
//              Device->Release();
//            }
//          }
//        }
//        Boot::Memory::Free<CHAR16>(DevicePathText);
//      }
//    }
//    Boot::Memory::Free<EFI_HANDLE>(Handles);
//  }
//}
//
//// Update
///// Update all input devices
//VOID
//Update (
//  VOID
//) {
//  for (UINTN Index = 0; Index < mDevices.GetCount(); ++Index) {
//    if (mDevices[Index] != NULL) {
//      type_cast<Device>(mDevices[Index])->Update();
//    }
//  }
//}
//
//// HasEvents
///// Check whether any input device has events waiting
///// @param Filter The input device event types to check
//BOOLEAN
//HasEvents (
//  UINTN Filter
//) {
//  // Check if any input device has events
//  for (UINTN Index = 0; Index < mDevices.GetCount(); ++Index) {
//    if ((mDevices[Index] != NULL) && mDevices[Index]->HasEvents(Filter)) {
//      return TRUE;
//    }
//  }
//  if (Filter == 0) {
//    // No filter so return TRUE if there are any pending events
//    return (mEvents.GetCount() > mNextEvent);
//  } else {
//    // Filter events
//    for (UINTN Index = mNextEvent; Index < mEvents.GetCount(); ++Index) {
//      if (mEvents[Index] != NULL) {
//        // Get event type to check filter
//        UINTN Type = mEvents[Index]->GetType();
//        if ((Filter & Type) == Type) {
//          // Matched filter
//          return TRUE;
//        }
//      }
//    }
//  }
//  // No filter match
//  return FALSE;
//}
//// GetEvent
///// Get an event from this input device
///// @param Filter The input device event types to retrieve
///// @return The input device event
//Event *
//GetEvent (
//  UINTN Filter
//) {
//  for (UINTN Index = 0; Index < mDevices.GetCount(); ++Index) {
//    if (mDevices[Index] != NULL) {
//      Event *Event = type_cast<Device>(mDevices[Index])->GetEvent(Filter);
//      if (Event != NULL) {
//        return Event;
//      }
//    }
//  }
//  // Set the event to a dummy event first
//  Event *NextEvent = NULL;
//  // Check to make sure the next event is valid
//  while ((mNextEvent < mEvents.GetCount()) && (mEvents[mNextEvent] == NULL)) {
//    ++mNextEvent;
//  }
//  // Remove any events already dispatched
//  if (mNextEvent != 0) {
//    // Remove the events from the list
//    mEvents.Remove(0, mNextEvent);
//    // Reset the next event to the first
//    mNextEvent = 0;
//  }
//  // Check there is a next event
//  if (mNextEvent < mEvents.GetCount()) {
//    if (Filter == 0) {
//      // No filter so grab next event
//      NextEvent = type_cast<Event>(mEvents[mNextEvent]);
//      mEvents[mNextEvent++] = NULL;
//    } else {
//      // Filter events
//      for (UINTN Index = mNextEvent; mNextEvent < mEvents.GetCount(); ++Index) {
//        UINTN Type = mEvents[Index]->GetType();
//        if ((Filter & Type) == Type) {
//          // Filtered event found
//          NextEvent = type_cast<Event>(mEvents[Index]);
//          // Check if the event needs swapped with the next event
//          if (Index != mNextEvent) {
//            // Swap the filter match event with the next event to prevent matching this event again
//            mEvents[Index] = mEvents[mNextEvent];
//            mEvents[mNextEvent] = NextEvent;
//          }
//          // Move to the next event
//          mEvents[mNextEvent++] = NULL;
//          break;
//        }
//      }
//    }
//  }
//  // Return the event
//  return NextEvent;
//}
//// ClearEvents
///// Clear all events from all input devices
//VOID
//ClearEvents (
//  VOID
//) {
//  for (UINTN Index = 0; Index < mDevices.GetCount(); ++Index) {
//    if (mDevices[Index] != NULL) {
//      type_cast<Device>(mDevices[Index])->ClearEvents();
//    }
//  }
//  mEvents.Clear();
//}
//// QueueEvent
///// Place an event in the global input queue
//VOID
//QueueEvent (
//  Event *Event
//) {
//  // Check event is valid before enqueuing
//  if ((Event != NULL) && (Event->GetType() != 0)) {
//    mEvents.Add(Event);
//  }
//}
//
//// WaitForPress
///// Wait for any input press (key, mouse, or touch)
//VOID
//WaitForPress (
//  VOID
//) {
//  do {
//    Update();
//  } while (!HasEvents(Event::Key | Event::Press));
//}
//// WaitForKeyPress
///// Wait for any key press
//VOID
//WaitForKeyPress (
//  VOID
//) {
//  do {
//    Update();
//  } while (!HasEvents(Event::Key));
//}
//
//};
//
//};
//
//};

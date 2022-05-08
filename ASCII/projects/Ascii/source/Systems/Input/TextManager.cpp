/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Systems/Input/TextManager.h"

TextEvent TextEvent::Write(char character) {
  TextEvent result;

  result.type = TextEventType::Write;
  result.character = character;

  return result;
}

TextEvent TextEvent::Delete(TextAmount amount, TextDirection direction) {
  TextEvent result;

  result.type = TextEventType::Delete;
  result.cursor = { amount, direction };

  return result;
}

TextEvent TextEvent::MoveCursor(TextAmount amount, TextDirection direction) {
  TextEvent result;

  result.type = TextEventType::MoveCursor;
  result.cursor = { amount, direction };

  return result;
}

TextEvent TextEvent::MoveSelection(TextAmount amount, TextDirection direction) {
  TextEvent result;

  result.type = TextEventType::MoveSelection;
  result.cursor = { amount, direction };

  return result;
}

TextEvent TextEvent::Select(TextAmount amount, TextDirection direction) {
  TextEvent result;

  result.type = TextEventType::Select;
  result.cursor = { amount, direction };

  return result;
}

TextEvent TextEvent::Indentation(TextDirection direction) {
  TextEvent result;

  result.type = TextEventType::Indentation;
  result.indentation = direction;

  return result;
}

TextEvent TextEvent::Clipboard(TextClipboardActionType clipboard) {
  TextEvent result;

  result.type = TextEventType::Clipboard;
  result.clipboard = clipboard;

  return result;
}

TextEvent TextEvent::Completion() {
  TextEvent result;

  result.type = TextEventType::Completion;

  return result;
}

TextEvent TextEvent::Cancellation() {
  TextEvent result;

  result.type = TextEventType::Cancellation;

  return result;
}

TextEvent TextEvent::File(TextFileActionType file) {
  TextEvent result;

  result.type = TextEventType::File;
  result.file = file;

  return result;
}

TextEvent TextEvent::History(TextHistoryActionType history) {
  TextEvent result;

  result.type = TextEventType::History;
  result.history = history;

  return result;
}

bool TextEvent::operator ==(TextEvent const & event) const {
  if (event.type != type) {
    return false;
  }

  switch (type) {
    case TextEventType::Write: {
      return event.character == character;
    } break;
    case TextEventType::Delete:
    case TextEventType::MoveCursor:
    case TextEventType::MoveSelection:
    case TextEventType::Select: {
      return event.cursor.amount == cursor.amount && event.cursor.direction == cursor.direction;
    } break;
    case TextEventType::Indentation: {
      return event.indentation == indentation;
    } break;
    case TextEventType::Clipboard: {
      return event.clipboard == clipboard;
    } break;
    case TextEventType::Completion: {
      return true;
    } break;
    case TextEventType::File: {
      return event.file == file;
    } break;
    case TextEventType::History: {
      return event.history == history;
    } break;
    case TextEventType::Custom: {
      return event.custom.value == custom.value && event.custom.data == custom.data;
    } break;
    case TextEventType::Count:
    default: {
      return true;
    } break;
  }
}

TextManager::TextManager(
  std::shared_ptr<IButtonManager>        buttonManager,
  std::shared_ptr<IStateManager> const & stateManager
) {
  SetManagers(buttonManager, stateManager);
}

Delegate<TextEvent> TextManager::AddTextEvent(
  DelegateFunc<TextEvent> const & func
) {
  return m_textDelegator.AddDelegate(func);
}

void TextManager::SetManagers(
  std::shared_ptr<IButtonManager>        buttonManager,
  std::shared_ptr<IStateManager> const & stateManager
) {
  SetButtonManager(buttonManager);
  SetStateManager(stateManager);
}

void TextManager::Update(float dt) {
  if (m_lastButtonPressed != AsciiButton::Count) {
    m_timeToNextRepeat -= dt;

    while (m_timeToNextRepeat <= 0.0f) {
      TriggerButton(m_lastButtonPressed);
      m_timeToNextRepeat += m_repeatDelay;
    }
  }
}

void TextManager::SetButtonManager(
  std::shared_ptr<IButtonManager> buttonManager
) {
  m_buttonFunc = buttonManager->AddGenericButtonEvent([this](AsciiButton button, bool isDown) {
    if (isDown) {
      TriggerButton(button);
      m_lastButtonPressed = button;
      m_timeToNextRepeat = m_repeatInitialDelay;
    }
    else {
      m_lastButtonPressed = AsciiButton::Count;
    }
  });
}

void TextManager::SetStateManager(
  std::shared_ptr<IStateManager> const & stateManager
) {
  m_stateManager = stateManager;
}

void TextManager::TriggerButton(AsciiButton button) {
  bool shift    = false;
  bool control  = false;
  bool capsLock = false;
  bool numLock  = false;

  if (!m_stateManager.expired()) {
    auto stateManager = m_stateManager.lock();

    shift    = stateManager->GetStateValue(AsciiState::Shift);
    control  = stateManager->GetStateValue(AsciiState::Control);
    capsLock = stateManager->GetStateValue(AsciiState::CapsLock);
    numLock  = stateManager->GetStateValue(AsciiState::NumLock);
  }

  bool const letterShift = shift != capsLock;
  bool const numPadShift = shift != numLock;

  TextEvent result;
  result.type = TextEventType::Count;

  if (button >= AsciiButton::ArrowKeysBegin && button < AsciiButton::ArrowKeysEnd) {
    switch (button) {
      case AsciiButton::Left: {
        result = TextEvent::MoveCursor(control ? TextAmount::Word : TextAmount::Character, TextDirection::Previous);
      } break;
      case AsciiButton::Right: {
        result = TextEvent::MoveCursor(control ? TextAmount::Word : TextAmount::Character, TextDirection::Next);
      } break;
      case AsciiButton::Up: {
        result = TextEvent::MoveCursor(TextAmount::Row, TextDirection::Previous);
      } break;
      case AsciiButton::Down: {
        result = TextEvent::MoveCursor(TextAmount::Row, TextDirection::Next);
      } break;
    }

    if (shift && result.type == TextEventType::MoveCursor) {
      result.type = TextEventType::MoveSelection;
    }
  }
  else if (button >= AsciiButton::NumPadBegin && button < AsciiButton::NumPadEnd) {
    switch (button) {
      case AsciiButton::NumPad0: {
        if (numPadShift) {
          result = TextEvent::Write('0');
        }
      } break;
      case AsciiButton::NumPad1: {
        result = numPadShift ? TextEvent::Write('1') : TextEvent::MoveCursor(control ? TextAmount::All : TextAmount::Line, TextDirection::Next);
      } break;
      case AsciiButton::NumPad2: {
        result = numPadShift ? TextEvent::Write('2') : TextEvent::MoveCursor(TextAmount::Row, TextDirection::Next);
      } break;
      case AsciiButton::NumPad3: {
        result = numPadShift ? TextEvent::Write('3') : TextEvent::MoveCursor(control ? TextAmount::All : TextAmount::Page, TextDirection::Next);
      } break;
      case AsciiButton::NumPad4: {
        result = numPadShift ? TextEvent::Write('4') : TextEvent::MoveCursor(control ? TextAmount::Word : TextAmount::Character, TextDirection::Previous);
      } break;
      case AsciiButton::NumPad5: {
        if (numPadShift) {
          result = TextEvent::Write('5');
        }
      } break;
      case AsciiButton::NumPad6: {
        result = numPadShift ? TextEvent::Write('6') : TextEvent::MoveCursor(control ? TextAmount::Word : TextAmount::Character, TextDirection::Next);
      } break;
      case AsciiButton::NumPad7: {
        result = numPadShift ? TextEvent::Write('7') : TextEvent::MoveCursor(control ? TextAmount::All : TextAmount::Line, TextDirection::Previous);
      } break;
      case AsciiButton::NumPad8: {
        result = numPadShift ? TextEvent::Write('8') : TextEvent::MoveCursor(TextAmount::Row, TextDirection::Previous);
      } break;
      case AsciiButton::NumPad9: {
        result = numPadShift ? TextEvent::Write('9') : TextEvent::MoveCursor(control ? TextAmount::All : TextAmount::Page, TextDirection::Previous);
      } break;
      case AsciiButton::NumPadDivide: {
        result = TextEvent::Write('/');
      } break;
      case AsciiButton::NumPadMultiply: {
        result = TextEvent::Write('*');
      } break;
      case AsciiButton::NumPadMinus: {
        result = TextEvent::Write('-');
      } break;
      case AsciiButton::NumPadPlus: {
        result = TextEvent::Write('+');
      } break;
      case AsciiButton::NumPadPeriod: {
        result = numPadShift ? TextEvent::Write('.') : TextEvent::Delete(control ? TextAmount::Word : TextAmount::Character, TextDirection::Next);
      } break;
      case AsciiButton::NumPadEnter: {
        result = shift ? TextEvent::Write('\n') : TextEvent::Completion();
      } break;
    }
  }
  else if (button == AsciiButton::Delete) {
    result = TextEvent::Delete(control ? TextAmount::Word : TextAmount::Character, TextDirection::Next);
  }
  else if (button == AsciiButton::Backspace) {
    result = TextEvent::Delete(control ? TextAmount::Word : TextAmount::Character, TextDirection::Previous);
  }
  else if (button == AsciiButton::Return) {
    result = shift ? TextEvent::Write('\n') : TextEvent::Completion();
  }
  else if (button == AsciiButton::Escape) {
    result = TextEvent::Cancellation();
  }
  else if (button == AsciiButton::Home) {
    result = shift ? TextEvent::MoveSelection(control ? TextAmount::All : TextAmount::Line, TextDirection::Previous) : TextEvent::MoveCursor(control ? TextAmount::All : TextAmount::Line, TextDirection::Previous);
  }
  else if (button == AsciiButton::End) {
    result = shift ? TextEvent::MoveSelection(control ? TextAmount::All : TextAmount::Line, TextDirection::Next) : TextEvent::MoveCursor(control ? TextAmount::All : TextAmount::Line, TextDirection::Next);
  }
  else if (button == AsciiButton::PageUp) {
    result = shift ? TextEvent::MoveSelection(control ? TextAmount::All : TextAmount::Page, TextDirection::Previous) : TextEvent::MoveCursor(control ? TextAmount::All : TextAmount::Page, TextDirection::Previous);
  }
  else if (button == AsciiButton::PageDown) {
    result = shift ? TextEvent::MoveSelection(control ? TextAmount::All : TextAmount::Page, TextDirection::Next) : TextEvent::MoveCursor(control ? TextAmount::All : TextAmount::Page, TextDirection::Next);
  }
  else if (control) {
    if (shift) {
      switch (button) {
        case AsciiButton::S: {
          result = TextEvent::File(TextFileActionType::SaveAs);
        } break;
      }
    }
    else {
      switch (button) {
        case AsciiButton::A: {
          result = TextEvent::Select(TextAmount::All, TextDirection::Current);
        } break;
        case AsciiButton::S: {
          result = TextEvent::File(TextFileActionType::Save);
        } break;
        case AsciiButton::O: {
          result = TextEvent::File(TextFileActionType::Open);
        } break;
        case AsciiButton::N: {
          result = TextEvent::File(TextFileActionType::New);
        } break;
        case AsciiButton::P: {
          result = TextEvent::File(TextFileActionType::Print);
        } break;
        case AsciiButton::C: {
          result = TextEvent::Clipboard(TextClipboardActionType::Copy);
        } break;
        case AsciiButton::X: {
          result = TextEvent::Clipboard(TextClipboardActionType::Cut);
        } break;
        case AsciiButton::V: {
          result = TextEvent::Clipboard(TextClipboardActionType::Paste);
        } break;
        case AsciiButton::Z: {
          result = TextEvent::History(TextHistoryActionType::Undo);
        } break;
        case AsciiButton::Y: {
          result = TextEvent::History(TextHistoryActionType::Redo);
        } break;
      }
    }
  }
  else if (button >= AsciiButton::LettersBegin && button < AsciiButton::LettersEnd) {
    result = TextEvent::Write(int(button) - int(AsciiButton::LettersBegin) + 'a' + (letterShift ? 'A' - 'a' : 0));
  }
  else {
    switch (button) {
      case AsciiButton::Key1: {
        result = TextEvent::Write(shift ? '!' : '1');
      } break;
      case AsciiButton::Key2: {
        result = TextEvent::Write(shift ? '@' : '2');
      } break;
      case AsciiButton::Key3: {
        result = TextEvent::Write(shift ? '#' : '3');
      } break;
      case AsciiButton::Key4: {
        result = TextEvent::Write(shift ? '$' : '4');
      } break;
      case AsciiButton::Key5: {
        result = TextEvent::Write(shift ? '%' : '5');
      } break;
      case AsciiButton::Key6: {
        result = TextEvent::Write(shift ? '^' : '6');
      } break;
      case AsciiButton::Key7: {
        result = TextEvent::Write(shift ? '&' : '7');
      } break;
      case AsciiButton::Key8: {
        result = TextEvent::Write(shift ? '*' : '8');
      } break;
      case AsciiButton::Key9: {
        result = TextEvent::Write(shift ? '(' : '9');
      } break;
      case AsciiButton::Key0: {
        result = TextEvent::Write(shift ? ')' : '0');
      } break;
      case AsciiButton::Space: {
        result = TextEvent::Write(' ');
      } break;
      case AsciiButton::Tab: {
        result = TextEvent::Indentation(shift ? TextDirection::Previous : TextDirection::Next);
      } break;
      case AsciiButton::Grave: {
        result = TextEvent::Write(shift ? '~' : '`');
      } break;
      case AsciiButton::Escape: {
        result = TextEvent::Cancellation();
      } break;
      case AsciiButton::Dash: {
        result = TextEvent::Write(shift ? '_' : '-');
      } break;
      case AsciiButton::Equal: {
        result = TextEvent::Write(shift ? '+' : '=');
      } break;
      case AsciiButton::LeftBracket: {
        result = TextEvent::Write(shift ? '{' : '[');
      } break;
      case AsciiButton::RightBracket: {
        result = TextEvent::Write(shift ? '}' : ']');
      } break;
      case AsciiButton::BackSlash: {
        result = TextEvent::Write(shift ? '|' : '\\');
      } break;
      case AsciiButton::ForwardSlash: {
        result = TextEvent::Write(shift ? '?' : '/');
      } break;
      case AsciiButton::Semicolon: {
        result = TextEvent::Write(shift ? ':' : ';');
      } break;
      case AsciiButton::Apostrophe: {
        result = TextEvent::Write(shift ? '"' : '\'');
      } break;
      case AsciiButton::Comma: {
        result = TextEvent::Write(shift ? '<' : ',');
      } break;
      case AsciiButton::Period: {
        result = TextEvent::Write(shift ? '>' : '.');
      } break;
    }
  }

  if (result.type != TextEventType::Count) {
    m_textDelegator.Trigger(result);
  }
}

/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef ASCII_SYSTEMS_INPUT_TEXTMANAGER_H
#define ASCII_SYSTEMS_INPUT_TEXTMANAGER_H

#include "General/Delagate.h"
#include "Systems/Input/ButtonManager.h"
#include "Systems/Input/StateManager.h"
#include "Window/Window.h"

enum class TextEventType : char
{
  Write,
  Delete,
  MoveCursor,
  MoveSelection,
  Select,
  Indentation,
  Clipboard,
  Completion,
  File,
  History,

  Count
};

enum class TextAmount : char
{
  Character,
  Word,
  Line,
  Page,
  All,

  Count
};

enum class TextDirection : char
{
  Previous,
  Current,
  Next,

  Count
};

enum class TextClipboardActionType : char
{
  Cut,
  Copy,
  Paste,

  Count
};

enum class TextFileActionType : char
{
  New,
  Open,
  Save,
  SaveAs,
  Print,

  Count
};

enum class TextHistoryActionType : char
{
  Undo,
  Redo,

  Count
};

struct TextEvent {
  TextEventType type;
  union {
    char character;
    struct {
      TextAmount    amount;
      TextDirection direction;
    } cursor;
    char                    indentation;
    TextClipboardActionType clipboard;
    TextFileActionType      file;
    TextHistoryActionType   history;
  };
};

class ITextManager {
public:
  virtual ~ITextManager(void) {}

  virtual Delegate<bool> AddTextEvent(
    DelegateFunc<TextEvent> const & func
  ) = 0;
};

class TextManager : public ITextManager {
public:
  TextManager(
    std::shared_ptr<IButtonManager> const & buttonManager,
    std::shared_ptr<IStateManager> const &  stateManager
  );

  virtual ~TextManager(void) override {}

  virtual Delegate<bool> AddTextEvent(
    DelegateFunc<TextEvent> const & func
  ) override;

  void SetManagers(
    std::shared_ptr<IButtonManager> const & buttonManager,
    std::shared_ptr<IStateManager> const &  stateManager
  );

  void SetButtonManager(
    std::shared_ptr<IButtonManager> const & buttonManager
  );

  void SetStateManager(
    std::shared_ptr<IStateManager> const & stateManager
  );

private:
  Delegator<TextEvent>                 m_textDelegator;
  Delegate<AsciiButton, bool>          m_buttonFunc;
  std::weak_ptr<IStateManager> const & m_stateManager;
};

#define DEFINE_MockTextManager()              \
class MockTextManager : public ITextManager { \
public:                                       \
  MOCK_METHOD(                                \
    Delegate<bool>,                           \
    AddTextEvent,                             \
    (DelegateFunc<TextEvent> const &),        \
    (override)                                \
  );                                          \
}

#endif // ASCII_SYSTEMS_INPUT_TEXTMANAGER_H

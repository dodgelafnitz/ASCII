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
  Cancellation,
  File,
  History,
  Custom,

  Count
};

enum class TextAmount : char
{
  Character,
  Word,
  Line,
  Row,
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
  static TextEvent Write(char character);
  static TextEvent Delete(TextAmount amount, TextDirection direction);
  static TextEvent MoveCursor(TextAmount amount, TextDirection direction);
  static TextEvent MoveSelection(TextAmount amount, TextDirection direction);
  static TextEvent Select(TextAmount amount, TextDirection direction);
  static TextEvent Indentation(TextDirection direction);
  static TextEvent Clipboard(TextClipboardActionType clipboard);
  static TextEvent Completion();
  static TextEvent Cancellation();
  static TextEvent File(TextFileActionType file);
  static TextEvent History(TextHistoryActionType history);

  bool operator ==(TextEvent const & event) const;

  TextEventType type;
  union {
    char character;
    struct {
      TextAmount    amount;
      TextDirection direction;
    } cursor;
    TextDirection           indentation;
    TextClipboardActionType clipboard;
    TextFileActionType      file;
    TextHistoryActionType   history;
    struct {
      unsigned char value;
      unsigned char data;
    } custom;
  };
};

class ITextManager {
public:
  virtual ~ITextManager(void) {}

  virtual Delegate<TextEvent> AddTextEvent(
    DelegateFunc<TextEvent> const & func
  ) = 0;
};

class TextManager : public ITextManager {
public:
  TextManager(void) = default;

  TextManager(
    std::shared_ptr<IButtonManager>        buttonManager,
    std::shared_ptr<IStateManager> const & stateManager
  );

  virtual ~TextManager(void) override {}

  virtual Delegate<TextEvent> AddTextEvent(
    DelegateFunc<TextEvent> const & func
  ) override;

  void SetManagers(
    std::shared_ptr<IButtonManager>        buttonManager,
    std::shared_ptr<IStateManager> const & stateManager
  );

  void SetButtonManager(
    std::shared_ptr<IButtonManager> buttonManager
  );

  void SetStateManager(
    std::shared_ptr<IStateManager> const & stateManager
  );

private:
  void TriggerButton(AsciiButton button);

  Delegator<TextEvent>         m_textDelegator;
  Delegate<AsciiButton, bool>  m_buttonFunc;
  std::weak_ptr<IStateManager> m_stateManager;
};

#define DEFINE_MockTextManager()              \
class MockTextManager : public ITextManager { \
public:                                       \
  MOCK_METHOD(                                \
    Delegate<TextEvent>,                      \
    AddTextEvent,                             \
    (DelegateFunc<TextEvent> const &),        \
    (override)                                \
  );                                          \
}

#endif // ASCII_SYSTEMS_INPUT_TEXTMANAGER_H

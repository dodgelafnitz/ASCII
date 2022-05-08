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

  virtual void Update(float dt) = 0;

  virtual float GetRepeatDelay(void) const = 0;
  virtual void SetRepeatDelay(float delay) = 0;
  virtual float GetInitialRepeatDelay(void) const = 0;
  virtual void SetInitialRepeatDelay(float delay) = 0;

  virtual Delegate<TextEvent> AddTextEvent(
    DelegateFunc<TextEvent> const & func
  ) = 0;
};

static float const s_defaultTextInitialRepeatDelay = 0.5f;
static float const s_defaultTextRepeatDelay        = 0.1f;

class TextManager : public ITextManager {
public:
  TextManager(void) = default;

  TextManager(
    std::shared_ptr<IButtonManager>        buttonManager,
    std::shared_ptr<IStateManager> const & stateManager
  );

  TextManager(
    std::shared_ptr<IButtonManager>        buttonManager,
    std::shared_ptr<IStateManager> const & stateManager,
    float                                  initialRepeatDelay,
    float                                  repeatDelay
  );

  virtual ~TextManager(void) override {}

  virtual Delegate<TextEvent> AddTextEvent(
    DelegateFunc<TextEvent> const & func
  ) override;

  virtual void Update(float dt) override;

  virtual float GetRepeatDelay(void) const override;
  virtual void SetRepeatDelay(float delay) override;
  virtual float GetInitialRepeatDelay(void) const override;
  virtual void SetInitialRepeatDelay(float delay) override;

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
  AsciiButton                  m_lastButtonPressed;
  float                        m_timeToNextRepeat   = 0.0f;
  float                        m_initialRepeatDelay = s_defaultTextInitialRepeatDelay;
  float                        m_repeatDelay        = s_defaultTextRepeatDelay;
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
  MOCK_METHOD(                                \
    void,                                     \
    Update,                                   \
    (float),                                  \
    (override)                                \
  );                                          \
  MOCK_METHOD(                                \
    float,                                    \
    GetRepeatDelay,                           \
    (),                                       \
    (const, override)                         \
  );                                          \
  MOCK_METHOD(                                \
    void,                                     \
    SetRepeatDelay,                           \
    (float),                                  \
    (override)                                \
  );                                          \
  MOCK_METHOD(                                \
    float,                                    \
    GetInitialRepeatDelay,                    \
    (),                                       \
    (const, override)                         \
  );                                          \
  MOCK_METHOD(                                \
    void,                                     \
    SetInitialRepeatDelay,                    \
    (float),                                  \
    (override)                                \
  );                                          \
}

#endif // ASCII_SYSTEMS_INPUT_TEXTMANAGER_H

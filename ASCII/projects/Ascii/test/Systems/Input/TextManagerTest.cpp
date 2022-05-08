/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Systems/Input/TextManager.h"
#include "gtest/gtest.h"

#include "gmock/gmock.h"

#include "Containers/DynamicArray.h"

DEFINE_MockTextManager();

namespace {
  struct TextEventExpectation {
    AsciiButton                                      button;
    DynamicArray<AsciiState, int(AsciiState::Count)> onStates;
    DynamicArray<AsciiState, int(AsciiState::Count)> offStates;
    TextEvent                                        expectation;
  };

  static const TextEventExpectation s_baseTextEventExpectations[] = {
    { AsciiButton::A, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('a') },
    { AsciiButton::B, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('b') },
    { AsciiButton::C, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('c') },
    { AsciiButton::D, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('d') },
    { AsciiButton::E, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('e') },
    { AsciiButton::F, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('f') },
    { AsciiButton::G, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('g') },
    { AsciiButton::H, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('h') },
    { AsciiButton::I, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('i') },
    { AsciiButton::J, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('j') },
    { AsciiButton::K, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('k') },
    { AsciiButton::L, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('l') },
    { AsciiButton::M, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('m') },
    { AsciiButton::N, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('n') },
    { AsciiButton::O, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('o') },
    { AsciiButton::P, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('p') },
    { AsciiButton::Q, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('q') },
    { AsciiButton::R, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('r') },
    { AsciiButton::S, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('s') },
    { AsciiButton::T, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('t') },
    { AsciiButton::U, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('u') },
    { AsciiButton::V, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('v') },
    { AsciiButton::W, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('w') },
    { AsciiButton::X, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('x') },
    { AsciiButton::Y, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('y') },
    { AsciiButton::Z, {}, { AsciiState::Shift, AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('z') },

    { AsciiButton::A, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('A') },
    { AsciiButton::B, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('B') },
    { AsciiButton::C, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('C') },
    { AsciiButton::D, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('D') },
    { AsciiButton::E, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('E') },
    { AsciiButton::F, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('F') },
    { AsciiButton::G, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('G') },
    { AsciiButton::H, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('H') },
    { AsciiButton::I, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('I') },
    { AsciiButton::J, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('J') },
    { AsciiButton::K, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('K') },
    { AsciiButton::L, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('L') },
    { AsciiButton::M, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('M') },
    { AsciiButton::N, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('N') },
    { AsciiButton::O, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('O') },
    { AsciiButton::P, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('P') },
    { AsciiButton::Q, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('Q') },
    { AsciiButton::R, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('R') },
    { AsciiButton::S, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('S') },
    { AsciiButton::T, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('T') },
    { AsciiButton::U, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('U') },
    { AsciiButton::V, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('V') },
    { AsciiButton::W, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('W') },
    { AsciiButton::X, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('X') },
    { AsciiButton::Y, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('Y') },
    { AsciiButton::Z, { AsciiState::Shift }, { AsciiState::CapsLock, AsciiState::Control }, TextEvent::Write('Z') },

    { AsciiButton::A, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('A') },
    { AsciiButton::B, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('B') },
    { AsciiButton::C, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('C') },
    { AsciiButton::D, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('D') },
    { AsciiButton::E, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('E') },
    { AsciiButton::F, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('F') },
    { AsciiButton::G, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('G') },
    { AsciiButton::H, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('H') },
    { AsciiButton::I, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('I') },
    { AsciiButton::J, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('J') },
    { AsciiButton::K, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('K') },
    { AsciiButton::L, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('L') },
    { AsciiButton::M, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('M') },
    { AsciiButton::N, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('N') },
    { AsciiButton::O, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('O') },
    { AsciiButton::P, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('P') },
    { AsciiButton::Q, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('Q') },
    { AsciiButton::R, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('R') },
    { AsciiButton::S, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('S') },
    { AsciiButton::T, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('T') },
    { AsciiButton::U, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('U') },
    { AsciiButton::V, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('V') },
    { AsciiButton::W, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('W') },
    { AsciiButton::X, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('X') },
    { AsciiButton::Y, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('Y') },
    { AsciiButton::Z, { AsciiState::CapsLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('Z') },

    { AsciiButton::A, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('a') },
    { AsciiButton::B, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('b') },
    { AsciiButton::C, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('c') },
    { AsciiButton::D, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('d') },
    { AsciiButton::E, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('e') },
    { AsciiButton::F, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('f') },
    { AsciiButton::G, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('g') },
    { AsciiButton::H, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('h') },
    { AsciiButton::I, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('i') },
    { AsciiButton::J, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('j') },
    { AsciiButton::K, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('k') },
    { AsciiButton::L, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('l') },
    { AsciiButton::M, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('m') },
    { AsciiButton::N, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('n') },
    { AsciiButton::O, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('o') },
    { AsciiButton::P, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('p') },
    { AsciiButton::Q, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('q') },
    { AsciiButton::R, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('r') },
    { AsciiButton::S, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('s') },
    { AsciiButton::T, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('t') },
    { AsciiButton::U, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('u') },
    { AsciiButton::V, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('v') },
    { AsciiButton::W, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('w') },
    { AsciiButton::X, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('x') },
    { AsciiButton::Y, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('y') },
    { AsciiButton::Z, { AsciiState::Shift, AsciiState::CapsLock }, { AsciiState::Control }, TextEvent::Write('z') },

    { AsciiButton::Key0,         {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('0') },
    { AsciiButton::Key1,         {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('1') },
    { AsciiButton::Key2,         {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('2') },
    { AsciiButton::Key3,         {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('3') },
    { AsciiButton::Key4,         {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('4') },
    { AsciiButton::Key5,         {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('5') },
    { AsciiButton::Key6,         {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('6') },
    { AsciiButton::Key7,         {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('7') },
    { AsciiButton::Key8,         {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('8') },
    { AsciiButton::Key9,         {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('9') },
    { AsciiButton::Grave,        {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('`') },
    { AsciiButton::Dash,         {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('-') },
    { AsciiButton::Equal,        {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('=') },
    { AsciiButton::LeftBracket,  {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('[') },
    { AsciiButton::RightBracket, {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write(']') },
    { AsciiButton::BackSlash,    {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('\\') },
    { AsciiButton::Semicolon,    {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write(';') },
    { AsciiButton::Apostrophe,   {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('\'') },
    { AsciiButton::Comma,        {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write(',') },
    { AsciiButton::Period,       {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('.') },
    { AsciiButton::ForwardSlash, {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('/') },

    { AsciiButton::Key0,         { AsciiState::Shift }, { AsciiState::Control }, TextEvent::Write(')') },
    { AsciiButton::Key1,         { AsciiState::Shift }, { AsciiState::Control }, TextEvent::Write('!') },
    { AsciiButton::Key2,         { AsciiState::Shift }, { AsciiState::Control }, TextEvent::Write('@') },
    { AsciiButton::Key3,         { AsciiState::Shift }, { AsciiState::Control }, TextEvent::Write('#') },
    { AsciiButton::Key4,         { AsciiState::Shift }, { AsciiState::Control }, TextEvent::Write('$') },
    { AsciiButton::Key5,         { AsciiState::Shift }, { AsciiState::Control }, TextEvent::Write('%') },
    { AsciiButton::Key6,         { AsciiState::Shift }, { AsciiState::Control }, TextEvent::Write('^') },
    { AsciiButton::Key7,         { AsciiState::Shift }, { AsciiState::Control }, TextEvent::Write('&') },
    { AsciiButton::Key8,         { AsciiState::Shift }, { AsciiState::Control }, TextEvent::Write('*') },
    { AsciiButton::Key9,         { AsciiState::Shift }, { AsciiState::Control }, TextEvent::Write('(') },
    { AsciiButton::Grave,        { AsciiState::Shift }, { AsciiState::Control }, TextEvent::Write('~') },
    { AsciiButton::Dash,         { AsciiState::Shift }, { AsciiState::Control }, TextEvent::Write('_') },
    { AsciiButton::Equal,        { AsciiState::Shift }, { AsciiState::Control }, TextEvent::Write('+') },
    { AsciiButton::LeftBracket,  { AsciiState::Shift }, { AsciiState::Control }, TextEvent::Write('{') },
    { AsciiButton::RightBracket, { AsciiState::Shift }, { AsciiState::Control }, TextEvent::Write('}') },
    { AsciiButton::BackSlash,    { AsciiState::Shift }, { AsciiState::Control }, TextEvent::Write('|') },
    { AsciiButton::Semicolon,    { AsciiState::Shift }, { AsciiState::Control }, TextEvent::Write(':') },
    { AsciiButton::Apostrophe,   { AsciiState::Shift }, { AsciiState::Control }, TextEvent::Write('"') },
    { AsciiButton::Comma,        { AsciiState::Shift }, { AsciiState::Control }, TextEvent::Write('<') },
    { AsciiButton::Period,       { AsciiState::Shift }, { AsciiState::Control }, TextEvent::Write('>') },
    { AsciiButton::ForwardSlash, { AsciiState::Shift }, { AsciiState::Control }, TextEvent::Write('?') },

    { AsciiButton::Space, {}, { AsciiState::Control }, TextEvent::Write(' ') },

    { AsciiButton::Return,      {}, { AsciiState::Shift }, TextEvent::Completion() },
    { AsciiButton::NumPadEnter, {}, { AsciiState::Shift }, TextEvent::Completion() },

    { AsciiButton::Return,      { AsciiState::Shift }, {}, TextEvent::Write('\n') },
    { AsciiButton::NumPadEnter, { AsciiState::Shift }, {}, TextEvent::Write('\n') },

    { AsciiButton::Escape, {}, {}, TextEvent::Cancellation() },

    { AsciiButton::NumPadDivide, {}, { AsciiState::Control }, TextEvent::Write('/') },
    { AsciiButton::NumPadMultiply, {}, { AsciiState::Control }, TextEvent::Write('*') },
    { AsciiButton::NumPadMinus, {}, { AsciiState::Control }, TextEvent::Write('-') },
    { AsciiButton::NumPadPlus, {}, { AsciiState::Control }, TextEvent::Write('+') },

    { AsciiButton::NumPad1,      {}, { AsciiState::NumLock, AsciiState::Shift, AsciiState::Control }, TextEvent::MoveCursor(TextAmount::Line, TextDirection::Next) },
    { AsciiButton::NumPad2,      {}, { AsciiState::NumLock, AsciiState::Shift, AsciiState::Control }, TextEvent::MoveCursor(TextAmount::Row, TextDirection::Next) },
    { AsciiButton::NumPad3,      {}, { AsciiState::NumLock, AsciiState::Shift, AsciiState::Control }, TextEvent::MoveCursor(TextAmount::Page, TextDirection::Next) },
    { AsciiButton::NumPad4,      {}, { AsciiState::NumLock, AsciiState::Shift, AsciiState::Control }, TextEvent::MoveCursor(TextAmount::Character, TextDirection::Previous) },
    { AsciiButton::NumPad6,      {}, { AsciiState::NumLock, AsciiState::Shift, AsciiState::Control }, TextEvent::MoveCursor(TextAmount::Character, TextDirection::Next) },
    { AsciiButton::NumPad7,      {}, { AsciiState::NumLock, AsciiState::Shift, AsciiState::Control }, TextEvent::MoveCursor(TextAmount::Line, TextDirection::Previous) },
    { AsciiButton::NumPad8,      {}, { AsciiState::NumLock, AsciiState::Shift, AsciiState::Control }, TextEvent::MoveCursor(TextAmount::Row, TextDirection::Previous) },
    { AsciiButton::NumPad9,      {}, { AsciiState::NumLock, AsciiState::Shift, AsciiState::Control }, TextEvent::MoveCursor(TextAmount::Page, TextDirection::Previous) },
    { AsciiButton::NumPadPeriod, {}, { AsciiState::NumLock, AsciiState::Shift, AsciiState::Control }, TextEvent::Delete(TextAmount::Character, TextDirection::Next) },

    { AsciiButton::NumPad0,      { AsciiState::NumLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('0') },
    { AsciiButton::NumPad1,      { AsciiState::NumLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('1') },
    { AsciiButton::NumPad2,      { AsciiState::NumLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('2') },
    { AsciiButton::NumPad3,      { AsciiState::NumLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('3') },
    { AsciiButton::NumPad4,      { AsciiState::NumLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('4') },
    { AsciiButton::NumPad5,      { AsciiState::NumLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('5') },
    { AsciiButton::NumPad6,      { AsciiState::NumLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('6') },
    { AsciiButton::NumPad7,      { AsciiState::NumLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('7') },
    { AsciiButton::NumPad8,      { AsciiState::NumLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('8') },
    { AsciiButton::NumPad9,      { AsciiState::NumLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('9') },
    { AsciiButton::NumPadPeriod, { AsciiState::NumLock }, { AsciiState::Shift, AsciiState::Control }, TextEvent::Write('.') },

    { AsciiButton::NumPad0,      { AsciiState::Shift }, { AsciiState::NumLock, AsciiState::Control }, TextEvent::Write('0') },
    { AsciiButton::NumPad1,      { AsciiState::Shift }, { AsciiState::NumLock, AsciiState::Control }, TextEvent::Write('1') },
    { AsciiButton::NumPad2,      { AsciiState::Shift }, { AsciiState::NumLock, AsciiState::Control }, TextEvent::Write('2') },
    { AsciiButton::NumPad3,      { AsciiState::Shift }, { AsciiState::NumLock, AsciiState::Control }, TextEvent::Write('3') },
    { AsciiButton::NumPad4,      { AsciiState::Shift }, { AsciiState::NumLock, AsciiState::Control }, TextEvent::Write('4') },
    { AsciiButton::NumPad5,      { AsciiState::Shift }, { AsciiState::NumLock, AsciiState::Control }, TextEvent::Write('5') },
    { AsciiButton::NumPad6,      { AsciiState::Shift }, { AsciiState::NumLock, AsciiState::Control }, TextEvent::Write('6') },
    { AsciiButton::NumPad7,      { AsciiState::Shift }, { AsciiState::NumLock, AsciiState::Control }, TextEvent::Write('7') },
    { AsciiButton::NumPad8,      { AsciiState::Shift }, { AsciiState::NumLock, AsciiState::Control }, TextEvent::Write('8') },
    { AsciiButton::NumPad9,      { AsciiState::Shift }, { AsciiState::NumLock, AsciiState::Control }, TextEvent::Write('9') },
    { AsciiButton::NumPadPeriod, { AsciiState::Shift }, { AsciiState::NumLock, AsciiState::Control }, TextEvent::Write('.') },

    { AsciiButton::NumPad1,      { AsciiState::NumLock, AsciiState::Shift }, { AsciiState::Control }, TextEvent::MoveCursor(TextAmount::Line, TextDirection::Next) },
    { AsciiButton::NumPad2,      { AsciiState::NumLock, AsciiState::Shift }, { AsciiState::Control }, TextEvent::MoveCursor(TextAmount::Row, TextDirection::Next) },
    { AsciiButton::NumPad3,      { AsciiState::NumLock, AsciiState::Shift }, { AsciiState::Control }, TextEvent::MoveCursor(TextAmount::Page, TextDirection::Next) },
    { AsciiButton::NumPad4,      { AsciiState::NumLock, AsciiState::Shift }, { AsciiState::Control }, TextEvent::MoveCursor(TextAmount::Character, TextDirection::Previous) },
    { AsciiButton::NumPad6,      { AsciiState::NumLock, AsciiState::Shift }, { AsciiState::Control }, TextEvent::MoveCursor(TextAmount::Character, TextDirection::Next) },
    { AsciiButton::NumPad7,      { AsciiState::NumLock, AsciiState::Shift }, { AsciiState::Control }, TextEvent::MoveCursor(TextAmount::Line, TextDirection::Previous) },
    { AsciiButton::NumPad8,      { AsciiState::NumLock, AsciiState::Shift }, { AsciiState::Control }, TextEvent::MoveCursor(TextAmount::Row, TextDirection::Previous) },
    { AsciiButton::NumPad9,      { AsciiState::NumLock, AsciiState::Shift }, { AsciiState::Control }, TextEvent::MoveCursor(TextAmount::Page, TextDirection::Previous) },
    { AsciiButton::NumPadPeriod, { AsciiState::NumLock, AsciiState::Shift }, { AsciiState::Control }, TextEvent::Delete(TextAmount::Character, TextDirection::Next) },

    { AsciiButton::NumPad1,      { AsciiState::Control }, { AsciiState::NumLock, AsciiState::Shift }, TextEvent::MoveCursor(TextAmount::All, TextDirection::Next) },
    { AsciiButton::NumPad2,      { AsciiState::Control }, { AsciiState::NumLock, AsciiState::Shift }, TextEvent::MoveCursor(TextAmount::Row, TextDirection::Next) },
    { AsciiButton::NumPad3,      { AsciiState::Control }, { AsciiState::NumLock, AsciiState::Shift }, TextEvent::MoveCursor(TextAmount::All, TextDirection::Next) },
    { AsciiButton::NumPad4,      { AsciiState::Control }, { AsciiState::NumLock, AsciiState::Shift }, TextEvent::MoveCursor(TextAmount::Word, TextDirection::Previous) },
    { AsciiButton::NumPad6,      { AsciiState::Control }, { AsciiState::NumLock, AsciiState::Shift }, TextEvent::MoveCursor(TextAmount::Word, TextDirection::Next) },
    { AsciiButton::NumPad7,      { AsciiState::Control }, { AsciiState::NumLock, AsciiState::Shift }, TextEvent::MoveCursor(TextAmount::All, TextDirection::Previous) },
    { AsciiButton::NumPad8,      { AsciiState::Control }, { AsciiState::NumLock, AsciiState::Shift }, TextEvent::MoveCursor(TextAmount::Row, TextDirection::Previous) },
    { AsciiButton::NumPad9,      { AsciiState::Control }, { AsciiState::NumLock, AsciiState::Shift }, TextEvent::MoveCursor(TextAmount::All, TextDirection::Previous) },
    { AsciiButton::NumPadPeriod, { AsciiState::Control }, { AsciiState::NumLock, AsciiState::Shift }, TextEvent::Delete(TextAmount::Word, TextDirection::Next) },

    { AsciiButton::NumPad1,      { AsciiState::NumLock, AsciiState::Shift, AsciiState::Control }, {}, TextEvent::MoveCursor(TextAmount::All, TextDirection::Next) },
    { AsciiButton::NumPad2,      { AsciiState::NumLock, AsciiState::Shift, AsciiState::Control }, {}, TextEvent::MoveCursor(TextAmount::Row, TextDirection::Next) },
    { AsciiButton::NumPad3,      { AsciiState::NumLock, AsciiState::Shift, AsciiState::Control }, {}, TextEvent::MoveCursor(TextAmount::All, TextDirection::Next) },
    { AsciiButton::NumPad4,      { AsciiState::NumLock, AsciiState::Shift, AsciiState::Control }, {}, TextEvent::MoveCursor(TextAmount::Word, TextDirection::Previous) },
    { AsciiButton::NumPad6,      { AsciiState::NumLock, AsciiState::Shift, AsciiState::Control }, {}, TextEvent::MoveCursor(TextAmount::Word, TextDirection::Next) },
    { AsciiButton::NumPad7,      { AsciiState::NumLock, AsciiState::Shift, AsciiState::Control }, {}, TextEvent::MoveCursor(TextAmount::All, TextDirection::Previous) },
    { AsciiButton::NumPad8,      { AsciiState::NumLock, AsciiState::Shift, AsciiState::Control }, {}, TextEvent::MoveCursor(TextAmount::Row, TextDirection::Previous) },
    { AsciiButton::NumPad9,      { AsciiState::NumLock, AsciiState::Shift, AsciiState::Control }, {}, TextEvent::MoveCursor(TextAmount::All, TextDirection::Previous) },
    { AsciiButton::NumPadPeriod, { AsciiState::NumLock, AsciiState::Shift, AsciiState::Control }, {}, TextEvent::Delete(TextAmount::Word, TextDirection::Next) },

    { AsciiButton::Backspace, {}, { AsciiState::Control }, TextEvent::Delete(TextAmount::Character, TextDirection::Previous) },
    { AsciiButton::Delete,    {}, { AsciiState::Control }, TextEvent::Delete(TextAmount::Character, TextDirection::Next) },

    { AsciiButton::Backspace, { AsciiState::Control }, {}, TextEvent::Delete(TextAmount::Word, TextDirection::Previous) },
    { AsciiButton::Delete,    { AsciiState::Control }, {}, TextEvent::Delete(TextAmount::Word, TextDirection::Next) },

    { AsciiButton::Left,     {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::MoveCursor(TextAmount::Character, TextDirection::Previous) },
    { AsciiButton::Right,    {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::MoveCursor(TextAmount::Character, TextDirection::Next) },
    { AsciiButton::Home,     {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::MoveCursor(TextAmount::Line, TextDirection::Previous) },
    { AsciiButton::End,      {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::MoveCursor(TextAmount::Line, TextDirection::Next) },
    { AsciiButton::PageUp,   {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::MoveCursor(TextAmount::Page, TextDirection::Previous) },
    { AsciiButton::PageDown, {}, { AsciiState::Shift, AsciiState::Control }, TextEvent::MoveCursor(TextAmount::Page, TextDirection::Next) },

    { AsciiButton::Left,     { AsciiState::Shift }, { AsciiState::Control }, TextEvent::MoveSelection(TextAmount::Character, TextDirection::Previous) },
    { AsciiButton::Right,    { AsciiState::Shift }, { AsciiState::Control }, TextEvent::MoveSelection(TextAmount::Character, TextDirection::Next) },
    { AsciiButton::Home,     { AsciiState::Shift }, { AsciiState::Control }, TextEvent::MoveSelection(TextAmount::Line, TextDirection::Previous) },
    { AsciiButton::End,      { AsciiState::Shift }, { AsciiState::Control }, TextEvent::MoveSelection(TextAmount::Line, TextDirection::Next) },
    { AsciiButton::PageUp,   { AsciiState::Shift }, { AsciiState::Control }, TextEvent::MoveSelection(TextAmount::Page, TextDirection::Previous) },
    { AsciiButton::PageDown, { AsciiState::Shift }, { AsciiState::Control }, TextEvent::MoveSelection(TextAmount::Page, TextDirection::Next) },

    { AsciiButton::Left,     { AsciiState::Control }, { AsciiState::Shift }, TextEvent::MoveCursor(TextAmount::Word, TextDirection::Previous) },
    { AsciiButton::Right,    { AsciiState::Control }, { AsciiState::Shift }, TextEvent::MoveCursor(TextAmount::Word, TextDirection::Next) },
    { AsciiButton::Home,     { AsciiState::Control }, { AsciiState::Shift }, TextEvent::MoveCursor(TextAmount::All, TextDirection::Previous) },
    { AsciiButton::End,      { AsciiState::Control }, { AsciiState::Shift }, TextEvent::MoveCursor(TextAmount::All, TextDirection::Next) },
    { AsciiButton::PageUp,   { AsciiState::Control }, { AsciiState::Shift }, TextEvent::MoveCursor(TextAmount::All, TextDirection::Previous) },
    { AsciiButton::PageDown, { AsciiState::Control }, { AsciiState::Shift }, TextEvent::MoveCursor(TextAmount::All, TextDirection::Next) },

    { AsciiButton::Left,     { AsciiState::Shift, AsciiState::Control }, {}, TextEvent::MoveSelection(TextAmount::Word, TextDirection::Previous) },
    { AsciiButton::Right,    { AsciiState::Shift, AsciiState::Control }, {}, TextEvent::MoveSelection(TextAmount::Word, TextDirection::Next) },
    { AsciiButton::Home,     { AsciiState::Shift, AsciiState::Control }, {}, TextEvent::MoveSelection(TextAmount::All, TextDirection::Previous) },
    { AsciiButton::End,      { AsciiState::Shift, AsciiState::Control }, {}, TextEvent::MoveSelection(TextAmount::All, TextDirection::Next) },
    { AsciiButton::PageUp,   { AsciiState::Shift, AsciiState::Control }, {}, TextEvent::MoveSelection(TextAmount::All, TextDirection::Previous) },
    { AsciiButton::PageDown, { AsciiState::Shift, AsciiState::Control }, {}, TextEvent::MoveSelection(TextAmount::All, TextDirection::Next) },

    { AsciiButton::Up,    {}, { AsciiState::Shift }, TextEvent::MoveCursor(TextAmount::Row, TextDirection::Previous) },
    { AsciiButton::Down,  {}, { AsciiState::Shift }, TextEvent::MoveCursor(TextAmount::Row, TextDirection::Next) },
    { AsciiButton::Up,    { AsciiState::Shift }, {}, TextEvent::MoveSelection(TextAmount::Row, TextDirection::Previous) },
    { AsciiButton::Down,  { AsciiState::Shift }, {}, TextEvent::MoveSelection(TextAmount::Row, TextDirection::Next) },

    { AsciiButton::A, { AsciiState::Control }, { AsciiState::Shift }, TextEvent::Select(TextAmount::All, TextDirection::Current) },

    { AsciiButton::C, { AsciiState::Control }, { AsciiState::Shift }, TextEvent::Clipboard(TextClipboardActionType::Copy) },
    { AsciiButton::X, { AsciiState::Control }, { AsciiState::Shift }, TextEvent::Clipboard(TextClipboardActionType::Cut) },
    { AsciiButton::V, { AsciiState::Control }, { AsciiState::Shift }, TextEvent::Clipboard(TextClipboardActionType::Paste) },

    { AsciiButton::Z, { AsciiState::Control }, { AsciiState::Shift }, TextEvent::History(TextHistoryActionType::Undo) },
    { AsciiButton::Y, { AsciiState::Control }, { AsciiState::Shift }, TextEvent::History(TextHistoryActionType::Redo) },

    { AsciiButton::N, { AsciiState::Control }, { AsciiState::Shift }, TextEvent::File(TextFileActionType::New) },
    { AsciiButton::O, { AsciiState::Control }, { AsciiState::Shift }, TextEvent::File(TextFileActionType::Open) },
    { AsciiButton::P, { AsciiState::Control }, { AsciiState::Shift }, TextEvent::File(TextFileActionType::Print) },

    { AsciiButton::S, { AsciiState::Control }, { AsciiState::Shift }, TextEvent::File(TextFileActionType::Save) },
    { AsciiButton::S, { AsciiState::Control, AsciiState::Shift}, {}, TextEvent::File(TextFileActionType::SaveAs) },
  };
}

TEST(TextManagerTest, DefaultConstructed_ButtonManagerChanged_ButtonManagerConnected) {
  TextManager textManager;

  auto buttonManager = std::make_shared<ButtonManager>();
  textManager.SetButtonManager(buttonManager);

  bool triggered = false;
  auto textDelegate = textManager.AddTextEvent([&triggered](TextEvent const &) {
    triggered = true;
  });

  EXPECT_FALSE(triggered);
  buttonManager->SetButtonState(AsciiButton::A, true);
  buttonManager->SetButtonState(AsciiButton::A, false);
  EXPECT_TRUE(triggered);
}

TEST(TextManagerTest, DefaultConstructed_StateManagerChanged_StateManagerConnected) {
  TextManager textManager;

  auto buttonManager = std::make_shared<ButtonManager>();
  textManager.SetButtonManager(buttonManager);

  auto stateManager = std::make_shared<StateManager>();
  textManager.SetStateManager(stateManager);

  bool isWriteEvent = false;
  bool isA          = false;
  bool isCaps       = false;
  auto textDelegate = textManager.AddTextEvent([&isWriteEvent, &isA, &isCaps](TextEvent const & event) {
    if (event.type != TextEventType::Write) {
      isWriteEvent = false;
      return;
    }
    isWriteEvent = true;

    if (event.character != 'A' && event.character != 'a') {
      isA = false;
      return;
    }
    isA = true;

    isCaps = event.character == 'A';
  });

  buttonManager->SetButtonState(AsciiButton::A, true);
  buttonManager->SetButtonState(AsciiButton::A, false);
  EXPECT_TRUE(isWriteEvent);
  EXPECT_TRUE(isA);
  EXPECT_FALSE(isCaps);

  stateManager->SetStateValue(AsciiState::Shift, true);

  buttonManager->SetButtonState(AsciiButton::A, true);
  buttonManager->SetButtonState(AsciiButton::A, false);
  EXPECT_TRUE(isWriteEvent);
  EXPECT_TRUE(isA);
  EXPECT_TRUE(isCaps);
}

TEST(TextManagerTest, AnyTextManager_ButtonManagerChanged_OldButtonManagerNotConnected) {
  auto buttonManager = std::make_shared<ButtonManager>();
  auto stateManager  = std::make_shared<StateManager>();

  TextManager textManager(buttonManager, stateManager);

  auto newButtonManager = std::make_shared<ButtonManager>();

  bool triggered = false;
  auto textDelegate = textManager.AddTextEvent([&triggered](TextEvent const &) {
    triggered = true;
  });

  textManager.SetButtonManager(newButtonManager);

  buttonManager->SetButtonState(AsciiButton::A, true);
  buttonManager->SetButtonState(AsciiButton::A, false);

  EXPECT_FALSE(triggered);

  newButtonManager->SetButtonState(AsciiButton::A, true);
  newButtonManager->SetButtonState(AsciiButton::A, false);

  EXPECT_TRUE(triggered);
}

TEST(TextManagerTest, AnyTextManager_StateManagerChanged_OldButtonManagerNotConnected) {
  auto buttonManager = std::make_shared<ButtonManager>();
  auto stateManager = std::make_shared<StateManager>();

  TextManager textManager(buttonManager, stateManager);

  auto newStateManager = std::make_shared<StateManager>();

  bool isWriteEvent = false;
  bool isA          = false;
  bool isCaps       = false;
  auto textDelegate = textManager.AddTextEvent([&isWriteEvent, &isA, &isCaps](TextEvent const & event) {
    if (event.type != TextEventType::Write) {
      isWriteEvent = false;
      return;
    }
    isWriteEvent = true;

    if (event.character != 'A' && event.character != 'a') {
      isA = false;
      return;
    }
    isA = true;

    isCaps = event.character == 'A';
  });

  buttonManager->SetButtonState(AsciiButton::A, true);
  buttonManager->SetButtonState(AsciiButton::A, false);
  EXPECT_TRUE(isWriteEvent);
  EXPECT_TRUE(isA);
  EXPECT_FALSE(isCaps);

  stateManager->SetStateValue(AsciiState::Shift, true);

  textManager.SetStateManager(newStateManager);

  buttonManager->SetButtonState(AsciiButton::A, true);
  buttonManager->SetButtonState(AsciiButton::A, false);
  EXPECT_TRUE(isWriteEvent);
  EXPECT_TRUE(isA);
  EXPECT_FALSE(isCaps);
}

TEST(TextManagerTest, TextManagerMadeWithRepeatTime_CheckRepeatTime_TimeIsCorrect) {
  auto const buttonManager = std::make_shared<ButtonManager>();
  auto const stateManager = std::make_shared<StateManager>();

  TextManager textManager(buttonManager, stateManager, 10.0f, 0.5f);

  EXPECT_EQ(textManager.GetRepeatDelay(), 0.5f);
}

TEST(TextManagerTest, TextManagerMadeWithRepeatTime_CheckInitialRepeatTime_TimeIsCorrect) {
  auto const buttonManager = std::make_shared<ButtonManager>();
  auto const stateManager = std::make_shared<StateManager>();

  TextManager textManager(buttonManager, stateManager, 10.0f, 0.5f);

  EXPECT_EQ(textManager.GetInitialRepeatDelay(), 10.0f);
}

TEST(TextManagerTest, AnyTextManager_ButtonHeld_TextRepeatedCorrectly) {
  auto buttonManager = std::make_shared<ButtonManager>();
  auto stateManager = std::make_shared<StateManager>();

  TextManager textManager(buttonManager, stateManager, 10.0f, 0.5f);

  int triggerCount = 0;

  auto textDelegate = textManager.AddTextEvent([&triggerCount](TextEvent const &) {
    ++triggerCount;
  });

  buttonManager->SetButtonState(AsciiButton::A, true);

  EXPECT_EQ(triggerCount, 1);

  textManager.Update(12.0f);

  EXPECT_EQ(triggerCount, 6);

  buttonManager->SetButtonState(AsciiButton::A, false);

  textManager.Update(12.0f);

  EXPECT_EQ(triggerCount, 6);
}

TEST(TextManagerTest, AnyTextManager_RepeatTimeSet_RepeatTimeIsCorrect) {
  TextManager textManager;

  textManager.SetRepeatDelay(5.0f);

  EXPECT_EQ(textManager.GetRepeatDelay(), 5.0f);
}

TEST(TextManagerTest, AnyTextManager_InitialRepeatTimeSet_InitialRepeatTimeIsCorrect) {
  TextManager textManager;

  textManager.SetRepeatDelay(5.0f);

  EXPECT_EQ(textManager.GetRepeatDelay(), 5.0f);
}

class TextManagerEventCorrectness : public testing::TestWithParam<TextEventExpectation> {};

// These tests fast enough that separating these into different tests for the "arbitrary" states
// causes slowdowns due to test setup. Technically this breaks the idea of unit testing since we
// are batching test runs here, but it's incredibly slow to test while doing otherwise.
TEST_P(TextManagerEventCorrectness, AnyTextManager_ButtonsAndStatesPressed_CorrectTextEventsTrigger) {
  TextEventExpectation const & expectation = GetParam();
  
  auto buttonManager = std::make_shared<ButtonManager>();
  auto stateManager = std::make_shared<StateManager>();

  TextManager textManager(buttonManager, stateManager);

  bool lockedStates[int(AsciiState::Count)] = { 0 };

  for (AsciiState state : expectation.onStates) {
    stateManager->SetStateValue(state, true);
    lockedStates[int(state)] = true;
  }

  for (AsciiState state : expectation.offStates) {
    lockedStates[int(state)] = true;
  }

  DynamicArray<AsciiState, int(AsciiState::Count)> arbitraryStates;

  for (int i = 0; i < int(AsciiState::Count); ++i) {
    if (!lockedStates[i]) {
      arbitraryStates.Emplace(AsciiState(i));
    }
  }

  using ArbitraryStateBitField = int;
  static_assert(sizeof(ArbitraryStateBitField) * 8 > int(AsciiState::Count), "AsciiState might not fit in bitfield.");

  ArbitraryStateBitField const bitFieldEnd = 0x1 << arbitraryStates.Count();

  TextEvent lastTextEvent;
  bool      textSent;

  auto const textDelegate = textManager.AddTextEvent([&](TextEvent const & event) {
    lastTextEvent = event;
    textSent = true;
  });

  for (ArbitraryStateBitField currentStates = 0; currentStates != bitFieldEnd; ++currentStates) {
    for (int i = 0; i < arbitraryStates.Count(); ++i) {
      stateManager->SetStateValue(arbitraryStates[i], currentStates & (0x1 << i));
    }
    textSent = false;

    buttonManager->SetButtonState(expectation.button, true);
    buttonManager->SetButtonState(expectation.button, false);

    EXPECT_TRUE(textSent);
    EXPECT_EQ(lastTextEvent, expectation.expectation);
  }
}

INSTANTIATE_TEST_SUITE_P(Core, TextManagerEventCorrectness, testing::ValuesIn(s_baseTextEventExpectations));

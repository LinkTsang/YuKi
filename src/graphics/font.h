#pragma once
#include "core/object.h"
#include "core/string.hpp"

namespace yuki {
/// <summary>
/// Alignment of paragraph text along the reading direction axis relative to
/// the leading and trailing edge of the layout box.
/// </summary>
enum class TextAlignment {
  /// <summary>
  /// The leading edge of the paragraph text is aligned to the layout box's
  /// leading edge.
  /// </summary>
  Leading = 0,

  /// <summary>
  /// The trailing edge of the paragraph text is aligned to the layout box's
  /// trailing edge.
  /// </summary>
  Trailing = 1,

  /// <summary>
  /// The center of the paragraph text is aligned to the center of the layout
  /// box.
  /// </summary>
  Center = 2,

  /// <summary>
  /// Align text to the leading side, and also justify text to fill the lines.
  /// </summary>
  Justified = 3
};

/// <summary>
/// Alignment of paragraph text along the flow direction axis relative to the
/// flow's beginning and ending edge of the layout box.
/// </summary>
enum class ParagraphAlignment {
  /// <summary>
  /// The first line of paragraph is aligned to the flow's beginning edge of the
  /// layout box.
  /// </summary>
  Near = 0,

  /// <summary>
  /// The last line of paragraph is aligned to the flow's ending edge of the
  /// layout box.
  /// </summary>
  Far = 1,

  /// <summary>
  /// The center of the paragraph is aligned to the center of the flow of the
  /// layout box.
  /// </summary>
  Center = 2
};

/// <summary>
/// Word wrapping in multiline paragraph.
/// </summary>
enum class WordWrapping {
  /// <summary>
  /// Words are broken across lines to avoid text overflowing the layout box.
  /// </summary>
  Wrap = 0,

  /// <summary>
  /// Words are kept within the same line even when it overflows the layout box.
  /// This option is often used with scrolling to reveal overflow text.
  /// </summary>
  NoWrap = 1,

  /// <summary>
  /// Words are broken across lines to avoid text overflowing the layout box.
  /// Emergency wrapping occurs if the word is larger than the maximum width.
  /// </summary>
  EmergencyBreak = 2,

  /// <summary>
  /// Only wrap whole words, never breaking words (emergency wrapping) when the
  /// layout width is too small for even a single word.
  /// </summary>
  WholeWord = 3,

  /// <summary>
  /// Wrap between any valid characters clusters.
  /// </summary>
  Character = 4,
};

/// <summary>
/// The font weight enumeration describes common values for degree of blackness
/// or thickness of strokes of characters in a font. Font weight values less
/// than 1 or greater than 999 are considered to be invalid, and they are
/// rejected by font API functions.
/// </summary>
enum class FontWeight {
  /// <summary>
  /// Predefined font weight : Thin (100).
  /// </summary>
  Thin = 100,

  /// <summary>
  /// Predefined font weight : Extra-light (200).
  /// </summary>
  ExtraLight = 200,

  /// <summary>
  /// Predefined font weight : Ultra-light (200).
  /// </summary>
  UltraLight = 200,

  /// <summary>
  /// Predefined font weight : Light (300).
  /// </summary>
  Light = 300,

  /// <summary>
  /// Predefined font weight : Semi-light (350).
  /// </summary>
  SemiLight = 350,

  /// <summary>
  /// Predefined font weight : Normal (400).
  /// </summary>
  Normal = 400,

  /// <summary>
  /// Predefined font weight : Regular (400).
  /// </summary>
  Regular = 400,

  /// <summary>
  /// Predefined font weight : Medium (500).
  /// </summary>
  Medium = 500,

  /// <summary>
  /// Predefined font weight : Demi-bold (600).
  /// </summary>
  DemiBold = 600,

  /// <summary>
  /// Predefined font weight : Semi-bold (600).
  /// </summary>
  SemiBold = 600,

  /// <summary>
  /// Predefined font weight : Bold (700).
  /// </summary>
  Bold = 700,

  /// <summary>
  /// Predefined font weight : Extra-bold (800).
  /// </summary>
  ExtraBold = 800,

  /// <summary>
  /// Predefined font weight : Ultra-bold (800).
  /// </summary>
  UltraBold = 800,

  /// <summary>
  /// Predefined font weight : Black (900).
  /// </summary>
  Black = 900,

  /// <summary>
  /// Predefined font weight : Heavy (900).
  /// </summary>
  Heavy = 900,

  /// <summary>
  /// Predefined font weight : Extra-black (950).
  /// </summary>
  ExtraBlack = 950,

  /// <summary>
  /// Predefined font weight : Ultra-black (950).
  /// </summary>
  UltraBlack = 950
};

class TextFormat : public Object {
public:
  TextFormat() = default;
  TextFormat(const TextFormat&) = default;
  TextFormat(TextFormat&&) = default;
  TextFormat& operator=(const TextFormat&) = default;
  TextFormat& operator=(TextFormat&&) = default;
  virtual ~TextFormat() = default;
  virtual String getFontFamilyName() const = 0;
  virtual float getSize() const = 0;
  virtual int getWeight() const = 0;
  virtual void setTextAlignment(TextAlignment textAlignment) = 0;
  virtual TextAlignment getTextAlignment() const = 0;
  virtual void setParagraphAlignment(ParagraphAlignment paragraphAlignment) = 0;
  virtual ParagraphAlignment getParagraphAlignment() const = 0;
  virtual void setWordWrapping(WordWrapping wordWrapping) = 0;
  virtual WordWrapping getWordWrapping() const = 0;
};
} // namespace yuki

#pragma once

#include <memory>
#include "core/string.hpp"
#include "graphics/bitmap.h"
#include "graphics/brush.h"
#include "graphics/color.h"
#include "graphics/font.h"
#include "graphics/geometry.h"

namespace yuki {
class StrokeStyle : Object {
public:
  StrokeStyle() = default;
  StrokeStyle(const StrokeStyle&) = default;
  StrokeStyle(StrokeStyle&&) = default;
  StrokeStyle& operator=(const StrokeStyle&) = default;
  StrokeStyle& operator=(StrokeStyle&&) = default;
  virtual ~StrokeStyle() = 0;
};

class Context2D : Object {
public:
  Context2D(const Context2D&) = delete;
  Context2D(Context2D&&) = delete;
  Context2D& operator=(const Context2D&) = delete;
  Context2D& operator=(Context2D&&) = delete;
  virtual ~Context2D() = default;

  virtual void resetSize(SizeF size) = 0;

  virtual void begin() = 0;
  virtual bool flush() = 0;
  virtual bool end() = 0;

  virtual void setTransform(const Transform2D& transform) = 0;
  virtual void resetTransform() = 0;
  virtual Transform2D getTransform() const = 0;

  virtual void clear(Color color) = 0;
  virtual void clear(const ColorF& color) = 0;

  virtual void drawCircle(const CircleF& circle, const Brush* brush,
                          float strokeWidth = 1,
                          StrokeStyle* strokeStyle = nullptr) = 0;
  virtual void drawEllipse(const EllipseF& ellipse, const Brush* brush,
                           float strokeWidth = 1,
                           StrokeStyle* strokeStyle = nullptr) = 0;
  virtual void drawLine(const LineF& line, const Brush* brush,
                        float strokeWidth = 1,
                        StrokeStyle* strokeStyle = nullptr) = 0;
  virtual void drawRect(const RectF& rect, const Brush* brush,
                        float strokeWidth = 1,
                        StrokeStyle* strokeStyle = nullptr) = 0;
  virtual void drawRoundedRect(const RoundedRectF& rect, const Brush* brush,
                               float strokeWidth = 1,
                               StrokeStyle* strokeStyle = nullptr) = 0;

  virtual void fillCircle(const CircleF& circle, const Brush* brush) = 0;
  virtual void fillEllipse(const EllipseF& ellipse, const Brush* brush) = 0;
  virtual void fillRect(const RectF& rect, const Brush* brush) = 0;
  virtual void fillRoundedRect(const RoundedRectF& rect,
                               const Brush* brush) = 0;

  virtual void drawBitmap(
    const Bitmap* bitmap, const RectF* destionationRectangle = nullptr,
    float opacity = 1.0f,
    BitmapInterpolationMode mode = BitmapInterpolationMode::Linear,
    const RectF* sourceRectangle = nullptr) = 0;

  virtual void drawText(const String& text, const TextFormat* font,
                        const RectF& rect, const Brush* brush) = 0;

  virtual void pushClip(const RectF& rect) = 0;
  virtual void popClip() = 0;

  virtual void setDpi(float dpiX, float dpiY) = 0;
  virtual void getDpi(float* dpiX, float* dpiY) = 0;

  virtual std::unique_ptr<Brush> createSolidBrush(const ColorF& color) = 0;
  virtual std::unique_ptr<TextFormat> createTextFormat(
    const String& name, float size,
    FontWeight weight = FontWeight::Normal) = 0;
  virtual std::unique_ptr<Bitmap> loadBitmap(const String& filename) = 0;

protected:
  Context2D() = default;
};
} // namespace yuki

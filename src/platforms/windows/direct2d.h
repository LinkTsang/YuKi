#pragma once

#include <Wincodec.h>
#include <d2d1_1.h>
#include <d3d11_1.h>
#include <dwrite_1.h>
#include <windows.h>
#include <wrl.h>
#include <boost/type_traits.hpp>
#include <boost/compute/detail/lru_cache.hpp>
#include <memory>
#include <unordered_map>
#include "graphics/painter.h"

namespace yuki {
namespace platforms {
namespace windows {
using namespace yuki::graphic;
class D2DContext2D;

/**
 * \brief DirectX Resource Manager class
 */
class DirectXRes {
 public:
  DirectXRes() = delete;

  static void init();
  static void releaseAll();
  static void handleDeviceLost();

  static Microsoft::WRL::ComPtr<IDXGIFactory2> getDXGIFactory();
  static Microsoft::WRL::ComPtr<ID3D11Device> getD3D11Device();
  static Microsoft::WRL::ComPtr<ID2D1Factory1> getD2DFactory();
  static Microsoft::WRL::ComPtr<ID2D1Device> getD2DDevice();

  static Microsoft::WRL::ComPtr<IDWriteFactory1> getDWriteFactory();

  static Microsoft::WRL::ComPtr<IWICImagingFactory> getImagingFactory();

  static std::unique_ptr<D2DContext2D> createContextFromHWnd(HWND hWnd);

 private:
  static Microsoft::WRL::ComPtr<ID3D11Device> d3device_;
  static Microsoft::WRL::ComPtr<IDXGIDevice> dxdevice_;
  static Microsoft::WRL::ComPtr<IDXGIAdapter> adapter_;
  static Microsoft::WRL::ComPtr<IDXGIFactory2> factory_;

  static Microsoft::WRL::ComPtr<ID2D1Factory1> d2dFactory_;
  static Microsoft::WRL::ComPtr<ID2D1Device> d2dDevice_;

  static Microsoft::WRL::ComPtr<IWICImagingFactory> imagingFactory_;

  static Microsoft::WRL::ComPtr<IDWriteFactory1> dWriteFactory_;
  static void createDeviceResources();

  static HRESULT createD3D11Device(
      D3D_DRIVER_TYPE type, Microsoft::WRL::ComPtr<ID3D11Device>& device);
};

/**
 * \brief Direct2D Brush Allocation class
 */
class D2DBrushAllocation {
 public:
  D2DBrushAllocation();
  Microsoft::WRL::ComPtr<ID2D1Brush> getD2DBrush(ID2D1DeviceContext* d2dContext,
                                                 const Brush* brush);
  void reset();

 private:
  boost::compute::detail::lru_cache<ColorF, Microsoft::WRL::ComPtr<ID2D1Brush>>
      soildColorBrushcache_;
};

/**
 * \brief Direct2D Context class
 */
class D2DContext2D : public Context2D {
 public:
  explicit D2DContext2D(HWND hWnd);

  void resetSize(SizeF size) override;

  void begin() override;
  bool flush() override;
  bool end() override;

  void setTransform(const Transform2D& transform) override;
  void resetTransform() override;
  Transform2D getTransform() const override;

  void clear(Color color) override;
  void clear(const ColorF& color) override;

  void drawCircle(const CircleF& circle, const Brush* brush, float strokeWidth,
                  StrokeStyle* strokeStyle) override;
  void drawEllipse(const EllipseF& ellipse, const Brush* brush,
                   float strokeWidth, StrokeStyle* strokeStyle) override;
  void drawLine(const LineF& line, const Brush* brush, float strokeWidth,
                StrokeStyle* strokeStyle) override;
  void drawRect(const RectF& rect, const Brush* brush, float strokeWidth,
                StrokeStyle* strokeStyle) override;
  void drawRoundedRect(const RoundedRectF& rect, const Brush* brush,
                       float strokeWidth, StrokeStyle* strokeStyle) override;
  void fillCircle(const CircleF& circle, const Brush* brush) override;
  void fillEllipse(const EllipseF& ellipse, const Brush* brush) override;
  void fillRect(const RectF& rect, const Brush* brush) override;
  void fillRoundedRect(const RoundedRectF& rect, const Brush* brush) override;

  void pushClip(const RectF& rect) override;
  void popClip() override;

  void setDpi(float dpiX, float dpiY) override;
  void getDpi(float* dpiX, float* dpiY) override;

  void drawBitmap(const Bitmap* bitmap, const RectF* destionationRectangle,
                  float opacity, BitmapInterpolationMode mode,
                  const RectF* sourceRectangle) override;

  void drawText(const String& text, const TextFormat* font, const RectF& rect,
                const Brush* brush) override;

  std::unique_ptr<TextFormat> createTextFormat(const String& name, float size,
                                               FontWeight weight) override;

  std::unique_ptr<Bitmap> loadBitmap(const String& filename) override;

 private:
  Microsoft::WRL::ComPtr<ID2D1DeviceContext> context_;
  Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain_;
  Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap_;
  std::unique_ptr<D2DBrushAllocation> brushAllocation_;

  void createDeviceContextFromHWnd(HWND hWnd);
  void createDeviceSwapChainBitmap();
  void handleDeviceLost();

  void beginDraw();
  bool endDraw();

  friend class NativeWindowManager;
};
}  // namespace windows
}  // namespace platforms
}  // namespace yuki

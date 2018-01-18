#include "direct2d.h"

#include <algorithm>
#include <cassert>
#include <exception>
#undef max
#undef min

#pragma comment(lib, "d2d1")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "dxguid.lib")

namespace yuki {
using Microsoft::WRL::ComPtr;
/*******************************************************************************
 * Direct2D Device Dependent Resource
 ******************************************************************************/
class D2DDeviceDependentRes {
 public:
  virtual void recreate(){};
  virtual ~D2DDeviceDependentRes() = default;
};
/*******************************************************************************
 * class D2DBrush
 ******************************************************************************/
class D2DBrush : public D2DDeviceDependentRes {
 public:
  virtual ComPtr<ID2D1Brush> getD2DBrush() const = 0;
  virtual ~D2DBrush() = default;
};
/*******************************************************************************
 * class D2DSoildColorBrush
 ******************************************************************************/
class D2DSolidColorBrush : public SolidColorBrush, public D2DBrush {
 public:
  explicit D2DSolidColorBrush(const ComPtr<ID2D1SolidColorBrush>& brush)
      : brush_(brush) {}
  explicit D2DSolidColorBrush(ComPtr<ID2D1SolidColorBrush>&& brush)
      : brush_(std::move(brush)) {}
  virtual ~D2DSolidColorBrush() = default;
  ComPtr<ID2D1Brush> getD2DBrush() const override { return brush_; }

 private:
  ComPtr<ID2D1SolidColorBrush> brush_;
};
class D2DBitmap : public Bitmap, public D2DDeviceDependentRes {
 public:
  explicit D2DBitmap(const ComPtr<ID2D1Bitmap>& bitmap) : bitmap_(bitmap) {}
  explicit D2DBitmap(ComPtr<ID2D1Bitmap>&& bitmap)
      : bitmap_(std::move(bitmap)) {}
  virtual ComPtr<ID2D1Bitmap> getD2DBitmap() const { return bitmap_; }
  virtual ~D2DBitmap() = default;

 private:
  ComPtr<ID2D1Bitmap> bitmap_;
};
/*******************************************************************************
 * DirectX Type Helper Functions
 ******************************************************************************/
static float ConvertDipsToPixels(float dips, float dpi) {
  static const auto DIPS_PER_INCH = 96.0f;
  return floorf(dips * dpi / DIPS_PER_INCH + 0.5f);
}

static constexpr D2D1_POINT_2F ToD2DPointF(const PointF& point) noexcept {
  return D2D1_POINT_2F{point.x(), point.y()};
}

static constexpr D2D1_RECT_F ToD2DRectF(const RectF& rect) noexcept {
  return D2D1_RECT_F{rect.left(), rect.top(), rect.right(), rect.bottom()};
}

static constexpr D2D1_RECT_F ToD2DRectF(const RectF* rect) noexcept {
  if (rect == nullptr) return D2D1_RECT_F{};
  return D2D1_RECT_F{rect->left(), rect->top(), rect->right(), rect->bottom()};
}

static constexpr D2D1_ROUNDED_RECT ToD2DRoundedRectF(
    const RoundedRectF& rect) noexcept {
  return D2D1_ROUNDED_RECT{rect.left(),   rect.top(),     rect.right(),
                           rect.bottom(), rect.radiusX(), rect.radiusY()};
}

static constexpr D2D1_ELLIPSE ToD2DEllipse(const EllipseF& ellipse) {
  return D2D1_ELLIPSE{ellipse.x(), ellipse.y(), ellipse.radiusX(),
                      ellipse.radiusY()};
}

static constexpr D2D1_ELLIPSE ToD2DEllipse(const CircleF& circle) {
  return D2D1_ELLIPSE{circle.x(), circle.y(), circle.radius(), circle.radius()};
}

static constexpr D2D1_COLOR_F ToD2DColorF(const ColorF& color) noexcept {
  return D2D1_COLOR_F{color.red(), color.green(), color.blue(), color.alpha()};
}

static ComPtr<ID2D1Brush> ToD2DBrush(const Brush* brush) noexcept {
  const auto b = dynamic_cast<const D2DBrush*>(brush);
  return b->getD2DBrush();
}

static ComPtr<ID2D1StrokeStyle> ToD2DStrokeStyle(
    const StrokeStyle* strokeStyle) {
  return ComPtr<ID2D1StrokeStyle>();
}

static ComPtr<ID2D1Bitmap1> ToD2DBitmap(const Bitmap* bitmap) {
  return ComPtr<ID2D1Bitmap1>();
}
/*******************************************************************************
 * class ComException
 ******************************************************************************/
struct ComException : std::exception {
  HRESULT const hr;
  explicit ComException(HRESULT const value) : hr(value) {}
};

inline void ThrowIfFailed(HRESULT const hr) {
  if (S_OK != hr) throw ComException(hr);
}
/*******************************************************************************
 * class DirectXRes
 ******************************************************************************/

ComPtr<ID3D11Device> DirectXRes::d3device_;
ComPtr<IDXGIDevice> DirectXRes::dxdevice_;
ComPtr<IDXGIAdapter> DirectXRes::adapter_;
ComPtr<IDXGIFactory2> DirectXRes::factory_;

ComPtr<ID2D1Factory1> DirectXRes::d2dFactory_;
ComPtr<ID2D1Device> DirectXRes::d2dDevice_;

ComPtr<IWICImagingFactory> DirectXRes::imagingFactory_;

void DirectXRes::init() { createDeviceResources(); }

void DirectXRes::releaseAll() {
  d3device_.Reset();
  dxdevice_.Reset();
  adapter_.Reset();
  factory_.Reset();
  d2dFactory_.Reset();
  d2dDevice_.Reset();
}

void DirectXRes::handleDeviceLost() { throw; }

inline ComPtr<IDXGIFactory2> DirectXRes::getDXGIFactory() {
  assert(factory_);
  return factory_;
}

inline ComPtr<ID3D11Device> DirectXRes::getD3D11Device() {
  assert(d3device_);
  return d3device_;
}

inline ComPtr<ID2D1Factory1> DirectXRes::getD2DFactory() {
  assert(d2dFactory_);
  return d2dFactory_;
}

inline ComPtr<ID2D1Device> DirectXRes::getD2DDevice() {
  assert(d2dDevice_);
  return d2dDevice_;
}

ComPtr<IWICImagingFactory> DirectXRes::getImagingFactory() {
  if (imagingFactory_ == nullptr) {
    ThrowIfFailed(CoCreateInstance(CLSID_WICImagingFactory, nullptr,
                                   CLSCTX_INPROC_SERVER,
                                   IID_PPV_ARGS(&imagingFactory_)));
  }
  return imagingFactory_;
}

std::unique_ptr<D2DContext2D> DirectXRes::createContextFromHWnd(HWND hWnd) {
  return std::make_unique<D2DContext2D>(hWnd);
}

void DirectXRes::createDeviceResources() {
  auto hr = createD3D11Device(D3D_DRIVER_TYPE_HARDWARE, d3device_);
  if (DXGI_ERROR_UNSUPPORTED == hr) {
    hr = createD3D11Device(D3D_DRIVER_TYPE_WARP, d3device_);
  }
  ThrowIfFailed(hr);

  ThrowIfFailed(d3device_.As(&dxdevice_));
  ThrowIfFailed(dxdevice_->GetAdapter(adapter_.GetAddressOf()));
  ThrowIfFailed(adapter_->GetParent(IID_PPV_ARGS(&factory_)));

  D2D1_FACTORY_OPTIONS options = {};
#ifdef _DEBUG
  options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif
  ThrowIfFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options,
                                  d2dFactory_.GetAddressOf()));
  ThrowIfFailed(
      d2dFactory_->CreateDevice(dxdevice_.Get(), d2dDevice_.GetAddressOf()));
}

inline HRESULT DirectXRes::createD3D11Device(D3D_DRIVER_TYPE const type,
                                             ComPtr<ID3D11Device>& device) {
  UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
  flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
  return D3D11CreateDevice(nullptr, type, nullptr, flags, nullptr, 0,
                           D3D11_SDK_VERSION, device.GetAddressOf(), nullptr,
                           nullptr);
}
/*******************************************************************************
 * class D2DContext
 ******************************************************************************/
void D2DContext2D::resetSize(SizeF size) {
  using namespace Microsoft::WRL;
  const int DOUBLE_BUFFER_COUNT = 2;
  context_->SetTarget(nullptr);
  bitmap_.Reset();

  UINT width = std::max(lround(size.width()), 8L);
  UINT height = std::max(lround(size.height()), 8L);
  auto hr = swapChain_->ResizeBuffers(DOUBLE_BUFFER_COUNT, width, height,
                                      DXGI_FORMAT_B8G8R8A8_UNORM, 0);

  if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
    handleDeviceLost();
    return;
  }

  ThrowIfFailed(hr);
  createDeviceSwapChainBitmap();
}

D2DContext2D::D2DContext2D(HWND hWnd) {
  createDeviceContextFromHWnd(hWnd);
  createDeviceSwapChainBitmap();
}

D2DContext2D::~D2DContext2D() = default;

inline void D2DContext2D::begin() {}

inline bool D2DContext2D::flush() {
  ThrowIfFailed(context_->Flush());
  return true;
}
inline bool D2DContext2D::end() { return true; }

void D2DContext2D::setTransform(const Transform2D& transform) {}
void D2DContext2D::resetTransform() {}
Transform2D D2DContext2D::getTransform() const {
  return Transform2D::identity();
}
void D2DContext2D::clear(Color color) {}
void D2DContext2D::clear(const ColorF& color) {
  context_->Clear(ToD2DColorF(color));
}

void D2DContext2D::drawCircle(const CircleF& circle, const Brush* brush,
                              float strokeWidth, StrokeStyle* strokeStyle) {
  context_->DrawEllipse(ToD2DEllipse(circle), ToD2DBrush(brush).Get(),
                        strokeWidth, ToD2DStrokeStyle(strokeStyle).Get());
}

void D2DContext2D::drawEllipse(const EllipseF& ellipse, const Brush* brush,
                               float strokeWidth, StrokeStyle* strokeStyle) {
  context_->DrawEllipse(ToD2DEllipse(ellipse), ToD2DBrush(brush).Get(),
                        strokeWidth, ToD2DStrokeStyle(strokeStyle).Get());
}

void D2DContext2D::drawLine(const LineF& line, const Brush* brush,
                            float strokeWidth, StrokeStyle* strokeStyle) {
  context_->DrawLine(ToD2DPointF(line.p1()), ToD2DPointF(line.p2()),
                     ToD2DBrush(brush).Get(), strokeWidth,
                     ToD2DStrokeStyle(strokeStyle).Get());
}

void D2DContext2D::drawRect(const RectF& rect, const Brush* brush,
                            float strokeWidth, StrokeStyle* strokeStyle) {
  context_->DrawRectangle(ToD2DRectF(rect), ToD2DBrush(brush).Get(),
                          strokeWidth, ToD2DStrokeStyle(strokeStyle).Get());
}

void D2DContext2D::drawRoundedRect(const RoundedRectF& rect, const Brush* brush,
                                   float strokeWidth,
                                   StrokeStyle* strokeStyle) {
  context_->DrawRoundedRectangle(ToD2DRoundedRectF(rect),
                                 ToD2DBrush(brush).Get(), strokeWidth,
                                 ToD2DStrokeStyle(strokeStyle).Get());
}

void D2DContext2D::fillCircle(const CircleF& circle, const Brush* brush) {
  context_->FillEllipse(ToD2DEllipse(circle), ToD2DBrush(brush).Get());
}

void D2DContext2D::fillEllipse(const EllipseF& ellipse, const Brush* brush) {
  context_->FillEllipse(ToD2DEllipse(ellipse), ToD2DBrush(brush).Get());
}

void D2DContext2D::fillRect(const RectF& rect, const Brush* brush) {
  context_->FillRectangle(ToD2DRectF(rect), ToD2DBrush(brush).Get());
}

void D2DContext2D::fillRoundedRect(const RoundedRectF& rect,
                                   const Brush* brush) {
  context_->FillRoundedRectangle(ToD2DRoundedRectF(rect),
                                 ToD2DBrush(brush).Get());
}

void D2DContext2D::pushClip(const RectF& rect) {
  context_->PushAxisAlignedClip(ToD2DRectF(rect),
                                D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
}

void D2DContext2D::popClip() { context_->PopAxisAlignedClip(); }

void D2DContext2D::setDpi(float dpiX, float dpiY) {
  context_->SetDpi(dpiX, dpiY);
}

void D2DContext2D::getDpi(float* dpiX, float* dpiY) {
  context_->GetDpi(dpiX, dpiY);
}

void D2DContext2D::drawBitmap(const Bitmap* bitmap,
                              const RectF* destionationRectangle, float opacity,
                              BitmapInterpolationMode mode,
                              const RectF* sourceRectangle) {
  if (destionationRectangle == nullptr) {
    context_->DrawBitmap(ToD2DBitmap(bitmap).Get());
  }
  D2D1_BITMAP_INTERPOLATION_MODE interpolationMode;
  switch (mode) {
    case BitmapInterpolationMode::Linear:
      interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
      break;
    case BitmapInterpolationMode::NearestNeighbor:
      interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
      break;
    default:
      interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
      break;
  }
  if (sourceRectangle == nullptr) {
    context_->DrawBitmap(ToD2DBitmap(bitmap).Get(),
                         ToD2DRectF(*destionationRectangle), opacity,
                         interpolationMode);
  } else {
    context_->DrawBitmap(ToD2DBitmap(bitmap).Get(),
                         ToD2DRectF(*destionationRectangle), opacity,
                         interpolationMode, ToD2DRectF(sourceRectangle));
  }
}

void D2DContext2D::drawText(const PointF& position, const Font& font,
                            const String& text) {}

std::unique_ptr<Brush> D2DContext2D::createSolidBrush(const ColorF& color) {
  ComPtr<ID2D1SolidColorBrush> brush;
  ThrowIfFailed(context_->CreateSolidColorBrush(ToD2DColorF(color), &brush));
  return std::make_unique<D2DSolidColorBrush>(std::move(brush));
}

std::unique_ptr<Bitmap> D2DContext2D::loadBitmap(const String& filename) {
  auto factory = DirectXRes::getImagingFactory();

  ComPtr<IWICBitmapDecoder> decoder;
  ThrowIfFailed(factory->CreateDecoderFromFilename(
      filename.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad,
      &decoder));

  ComPtr<IWICBitmapFrameDecode> source;
  ThrowIfFailed(decoder->GetFrame(0, &source));

  ComPtr<IWICFormatConverter> converter;
  factory->CreateFormatConverter(&converter);
  ThrowIfFailed(converter->Initialize(
      source.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone,
      nullptr, 0.f, WICBitmapPaletteTypeMedianCut));

  ComPtr<ID2D1Bitmap> bitmap;
  ThrowIfFailed(context_->CreateBitmapFromWicBitmap(source.Get(), &bitmap));

  return std::make_unique<D2DBitmap>(std::move(bitmap));
}

void D2DContext2D::createDeviceContextFromHWnd(HWND hWnd) {
  DXGI_SWAP_CHAIN_DESC1 props_dscd = {};
  props_dscd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  props_dscd.SampleDesc.Count = 1;
  props_dscd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  props_dscd.BufferCount = 2;
  ThrowIfFailed(DirectXRes::getDXGIFactory()->CreateSwapChainForHwnd(
      DirectXRes::getD3D11Device().Get(), hWnd, &props_dscd, nullptr, nullptr,
      swapChain_.GetAddressOf()));

  ThrowIfFailed(DirectXRes::getD2DDevice()->CreateDeviceContext(
      D2D1_DEVICE_CONTEXT_OPTIONS_NONE, context_.GetAddressOf()));
}

void D2DContext2D::createDeviceSwapChainBitmap() {
  float dpiX, dpiY;
  DirectXRes::getD2DFactory()->GetDesktopDpi(&dpiX, &dpiY);
  context_->SetDpi(dpiX, dpiY);

  ComPtr<IDXGISurface> surface;
  ThrowIfFailed(swapChain_->GetBuffer(0, IID_PPV_ARGS(&surface)));

  const auto properties = D2D1::BitmapProperties1(
      D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
      D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
      dpiX, dpiY);

  ThrowIfFailed(context_->CreateBitmapFromDxgiSurface(surface.Get(), properties,
                                                      bitmap_.GetAddressOf()));
  context_->SetTarget(bitmap_.Get());
  context_->SetUnitMode(D2D1_UNIT_MODE_PIXELS);
}

void D2DContext2D::handleDeviceLost() {
  HWND hWnd;
  ThrowIfFailed(swapChain_->GetHwnd(&hWnd));
  DirectXRes::handleDeviceLost();
  context_->SetTarget(nullptr);
  bitmap_.Reset();
  context_.Reset();
  swapChain_.Reset();
  createDeviceContextFromHWnd(hWnd);
  createDeviceSwapChainBitmap();
}

void D2DContext2D::beginDraw() { context_->BeginDraw(); }

bool D2DContext2D::endDraw() {
  ThrowIfFailed(context_->EndDraw());
  const auto hr = swapChain_->Present(1, 0);
  if (S_OK != hr && DXGI_STATUS_OCCLUDED != hr) {
    ThrowIfFailed(hr);
  }
  return true;
}
}  // namespace yuki
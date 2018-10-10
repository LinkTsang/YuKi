#include "direct2d.h"

#include <algorithm>
#include <cassert>
#include <exception>
#include <string_view>
#include <utility>

#undef max
#undef min

#pragma comment(lib, "d2d1")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "dxguid.lib")

namespace yuki {
namespace platforms {
namespace windows {
using Microsoft::WRL::ComPtr;

template <typename R, typename S>
static constexpr R ConvertTo(S s) {
  return static_cast<R>(s);
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

inline void WarnIfFailed(HRESULT const hr) {
  if (S_OK != hr) throw ComException(hr);
}

/*******************************************************************************
 * Direct2D Device Dependent Resource
 ******************************************************************************/
class D2DDeviceDependentRes {
 public:
  D2DDeviceDependentRes() = default;
  D2DDeviceDependentRes(const D2DDeviceDependentRes&) = default;
  D2DDeviceDependentRes(D2DDeviceDependentRes&&) = default;
  D2DDeviceDependentRes& operator=(const D2DDeviceDependentRes&) = default;
  D2DDeviceDependentRes& operator=(D2DDeviceDependentRes&&) = default;
  virtual ~D2DDeviceDependentRes() = default;

  virtual void recreate() {}
};

/*******************************************************************************
 * class D2DBrush
 ******************************************************************************/
class D2DBrush : public D2DDeviceDependentRes {
 public:
  D2DBrush() = default;
  virtual ComPtr<ID2D1Brush> getD2DBrush() const = 0;
};

/*******************************************************************************
 * class D2DSoildColorBrush
 ******************************************************************************/
class D2DSolidColorBrush : public D2DBrush {
 public:
  explicit D2DSolidColorBrush(ComPtr<ID2D1SolidColorBrush> brush)
      : brush_(std::move(brush)) {}

  ComPtr<ID2D1Brush> getD2DBrush() const override { return brush_; }

 private:
  ComPtr<ID2D1SolidColorBrush> brush_;
};

class D2DBitmap : public Bitmap, public D2DDeviceDependentRes {
 public:
  D2DBitmap() = default;
  explicit D2DBitmap(ComPtr<ID2D1Bitmap> bitmap) : bitmap_(std::move(bitmap)) {}
  D2DBitmap(const D2DBitmap&) = default;
  D2DBitmap(D2DBitmap&&) = default;
  D2DBitmap& operator=(const D2DBitmap&) = default;
  D2DBitmap& operator=(D2DBitmap&&) = default;
  virtual ~D2DBitmap() = default;

  virtual ComPtr<ID2D1Bitmap> getD2DBitmap() const { return bitmap_; }

 private:
  ComPtr<ID2D1Bitmap> bitmap_;
};

/*******************************************************************************
 * DirectWrite TextFormat Wrapper
 ******************************************************************************/
template <>
constexpr DWRITE_TEXT_ALIGNMENT ConvertTo(TextAlignment textAlignment) {
  DWRITE_TEXT_ALIGNMENT result = DWRITE_TEXT_ALIGNMENT_LEADING;
  switch (textAlignment) {
    case TextAlignment::Leading:
      result = DWRITE_TEXT_ALIGNMENT_LEADING;
      break;
    case TextAlignment::Trailing:
      result = DWRITE_TEXT_ALIGNMENT_TRAILING;
      break;
    case TextAlignment::Center:
      result = DWRITE_TEXT_ALIGNMENT_CENTER;
      break;
    case TextAlignment::Justified:
      result = DWRITE_TEXT_ALIGNMENT_JUSTIFIED;
      break;
  }
  return result;
}

template <>
constexpr TextAlignment ConvertTo(DWRITE_TEXT_ALIGNMENT textAlignment) {
  TextAlignment result = TextAlignment::Leading;
  switch (textAlignment) {
    case DWRITE_TEXT_ALIGNMENT_LEADING:
      result = TextAlignment::Leading;
      break;
    case DWRITE_TEXT_ALIGNMENT_TRAILING:
      result = TextAlignment::Trailing;
      break;
    case DWRITE_TEXT_ALIGNMENT_CENTER:
      result = TextAlignment::Center;
      break;
    case DWRITE_TEXT_ALIGNMENT_JUSTIFIED:
      result = TextAlignment::Justified;
      break;
  }
  return result;
}

template <>
constexpr DWRITE_PARAGRAPH_ALIGNMENT ConvertTo(
    ParagraphAlignment paragraphAlignment) {
  DWRITE_PARAGRAPH_ALIGNMENT result = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
  switch (paragraphAlignment) {
    case ParagraphAlignment::Near:
      result = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
      break;
    case ParagraphAlignment::Far:
      result = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
      break;
    case ParagraphAlignment::Center:
      result = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
      break;
  }
  return result;
}

template <>
constexpr ParagraphAlignment ConvertTo(
    DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment) {
  ParagraphAlignment result = ParagraphAlignment::Near;
  switch (paragraphAlignment) {
    case DWRITE_PARAGRAPH_ALIGNMENT_NEAR:
      result = ParagraphAlignment::Near;
      break;
    case DWRITE_PARAGRAPH_ALIGNMENT_FAR:
      result = ParagraphAlignment::Far;
      break;
    case DWRITE_PARAGRAPH_ALIGNMENT_CENTER:
      result = ParagraphAlignment::Center;
      break;
  }
  return result;
}

template <>
constexpr DWRITE_WORD_WRAPPING ConvertTo(WordWrapping paragraphAlignment) {
  DWRITE_WORD_WRAPPING result = DWRITE_WORD_WRAPPING_WRAP;
  switch (paragraphAlignment) {
    case WordWrapping::Wrap:
      result = DWRITE_WORD_WRAPPING_WRAP;
      break;
    case WordWrapping::NoWrap:
      result = DWRITE_WORD_WRAPPING_NO_WRAP;
      break;
    case WordWrapping::EmergencyBreak:
      result = DWRITE_WORD_WRAPPING_EMERGENCY_BREAK;
      break;
    case WordWrapping::WholeWord:
      result = DWRITE_WORD_WRAPPING_WHOLE_WORD;
      break;
    case WordWrapping::Character:
      result = DWRITE_WORD_WRAPPING_CHARACTER;
      break;
  }
  return result;
}

template <>
constexpr WordWrapping ConvertTo(DWRITE_WORD_WRAPPING paragraphAlignment) {
  WordWrapping result = WordWrapping::Wrap;
  switch (paragraphAlignment) {
    case DWRITE_WORD_WRAPPING_WRAP:
      result = WordWrapping::Wrap;
      break;
    case DWRITE_WORD_WRAPPING_NO_WRAP:
      result = WordWrapping::NoWrap;
      break;
    case DWRITE_WORD_WRAPPING_EMERGENCY_BREAK:
      result = WordWrapping::EmergencyBreak;
      break;
    case DWRITE_WORD_WRAPPING_WHOLE_WORD:
      result = WordWrapping::WholeWord;
      break;
    case DWRITE_WORD_WRAPPING_CHARACTER:
      result = WordWrapping::Character;
      break;
  }
  return result;
}

template <>
constexpr FontWeight ConvertTo(DWRITE_FONT_WEIGHT fontWeight) {
  return static_cast<FontWeight>(fontWeight);
}

template <>
constexpr DWRITE_FONT_WEIGHT ConvertTo(FontWeight fontWeight) {
  return static_cast<DWRITE_FONT_WEIGHT>(fontWeight);
}

class DWriteTextFormat : public TextFormat {
 public:
  explicit DWriteTextFormat(ComPtr<IDWriteTextFormat> textFormat)
      : textFormat_(std::move(textFormat)) {}

  ComPtr<IDWriteTextFormat> getTextFormat() const { return textFormat_; }

  String getFontFamilyName() const override {
    auto length = textFormat_->GetFontFamilyNameLength();
    auto buffer = std::make_unique<WCHAR[]>(length);
    WarnIfFailed(textFormat_->GetFontFamilyName(buffer.get(), length));
    std::wstring_view wstringView(buffer.get(), length);
    return String(wstringView.begin(), wstringView.end());
  }

  float getSize() const override { return textFormat_->GetFontSize(); }

  void setTextAlignment(TextAlignment textAlignment) override {
    WarnIfFailed(textFormat_->SetTextAlignment(
        ConvertTo<DWRITE_TEXT_ALIGNMENT>(textAlignment)));
  }

  TextAlignment getTextAlignment() const override {
    return ConvertTo<TextAlignment>(textFormat_->GetTextAlignment());
  }

  void setParagraphAlignment(ParagraphAlignment paragraphAlignment) override {
    WarnIfFailed(textFormat_->SetParagraphAlignment(
        ConvertTo<DWRITE_PARAGRAPH_ALIGNMENT>(paragraphAlignment)));
  }

  ParagraphAlignment getParagraphAlignment() const override {
    return ConvertTo<ParagraphAlignment>(textFormat_->GetParagraphAlignment());
  }

  void setWordWrapping(WordWrapping wordWrapping) override {
    WarnIfFailed(textFormat_->SetWordWrapping(
        ConvertTo<DWRITE_WORD_WRAPPING>(wordWrapping)));
  }

  WordWrapping getWordWrapping() const override {
    return ConvertTo<WordWrapping>(textFormat_->GetWordWrapping());
  }

  int getWeight() const override { return textFormat_->GetFontWeight(); }

 private:
  ComPtr<IDWriteTextFormat> textFormat_;
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

static ComPtr<ID2D1StrokeStyle> ToD2DStrokeStyle(
    const StrokeStyle* strokeStyle) {
  return ComPtr<ID2D1StrokeStyle>();
}

static ComPtr<ID2D1Bitmap1> ToD2DBitmap(const Bitmap* bitmap) {
  return ComPtr<ID2D1Bitmap1>();
}

static ComPtr<IDWriteTextFormat> ToWriteTextFormat(const TextFormat* font) {
  return dynamic_cast<const DWriteTextFormat*>(font)->getTextFormat();
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

ComPtr<IDWriteFactory1> DirectXRes::dWriteFactory_;

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

inline ComPtr<IDWriteFactory1> DirectXRes::getDWriteFactory() {
  if (dWriteFactory_ == nullptr) {
    DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED, __uuidof(dWriteFactory_),
        reinterpret_cast<IUnknown**>(dWriteFactory_.GetAddressOf()));
  }
  return dWriteFactory_;
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
 * class D2DBrushAllocation
 ******************************************************************************/

ComPtr<ID2D1Brush> D2DBrushAllocation::getD2DBrush(
    ID2D1DeviceContext* d2dContext, const Brush* brush) {
  if (brush == nullptr) {
    return nullptr;
  }
  switch (brush->style()) {
    case BrushStyle::SolidColor: {
      const auto solidColorBrush = static_cast<const SolidColorBrush*>(brush);
      ComPtr<ID2D1SolidColorBrush> d2dBrush;
      ThrowIfFailed(d2dContext->CreateSolidColorBrush(
          ToD2DColorF(solidColorBrush->getColor()), &d2dBrush));
      return d2dBrush;
    }
    default:
      return nullptr;
  }
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
  auto d2dBrush = brushAllocation_->getD2DBrush(context_.Get(), brush);
  context_->DrawEllipse(ToD2DEllipse(circle), d2dBrush.Get(), strokeWidth,
                        ToD2DStrokeStyle(strokeStyle).Get());
}

void D2DContext2D::drawEllipse(const EllipseF& ellipse, const Brush* brush,
                               float strokeWidth, StrokeStyle* strokeStyle) {
  auto d2dBrush = brushAllocation_->getD2DBrush(context_.Get(), brush);
  context_->DrawEllipse(ToD2DEllipse(ellipse), d2dBrush.Get(), strokeWidth,
                        ToD2DStrokeStyle(strokeStyle).Get());
}

void D2DContext2D::drawLine(const LineF& line, const Brush* brush,
                            float strokeWidth, StrokeStyle* strokeStyle) {
  auto d2dBrush = brushAllocation_->getD2DBrush(context_.Get(), brush);
  context_->DrawLine(ToD2DPointF(line.p1()), ToD2DPointF(line.p2()),
                     d2dBrush.Get(), strokeWidth,
                     ToD2DStrokeStyle(strokeStyle).Get());
}

void D2DContext2D::drawRect(const RectF& rect, const Brush* brush,
                            float strokeWidth, StrokeStyle* strokeStyle) {
  auto d2dBrush = brushAllocation_->getD2DBrush(context_.Get(), brush);
  context_->DrawRectangle(ToD2DRectF(rect), d2dBrush.Get(), strokeWidth,
                          ToD2DStrokeStyle(strokeStyle).Get());
}

void D2DContext2D::drawRoundedRect(const RoundedRectF& rect, const Brush* brush,
                                   float strokeWidth,
                                   StrokeStyle* strokeStyle) {
  auto d2dBrush = brushAllocation_->getD2DBrush(context_.Get(), brush);
  context_->DrawRoundedRectangle(ToD2DRoundedRectF(rect), d2dBrush.Get(),
                                 strokeWidth,
                                 ToD2DStrokeStyle(strokeStyle).Get());
}

void D2DContext2D::fillCircle(const CircleF& circle, const Brush* brush) {
  auto d2dBrush = brushAllocation_->getD2DBrush(context_.Get(), brush);
  context_->FillEllipse(ToD2DEllipse(circle), d2dBrush.Get());
}

void D2DContext2D::fillEllipse(const EllipseF& ellipse, const Brush* brush) {
  auto d2dBrush = brushAllocation_->getD2DBrush(context_.Get(), brush);
  context_->FillEllipse(ToD2DEllipse(ellipse), d2dBrush.Get());
}

void D2DContext2D::fillRect(const RectF& rect, const Brush* brush) {
  auto d2dBrush = brushAllocation_->getD2DBrush(context_.Get(), brush);
  context_->FillRectangle(ToD2DRectF(rect), d2dBrush.Get());
}

void D2DContext2D::fillRoundedRect(const RoundedRectF& rect,
                                   const Brush* brush) {
  auto d2dBrush = brushAllocation_->getD2DBrush(context_.Get(), brush);
  context_->FillRoundedRectangle(ToD2DRoundedRectF(rect), d2dBrush.Get());
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

void D2DContext2D::drawText(const String& text, const TextFormat* font,
                            const RectF& rect, const Brush* brush) {
  auto d2dBrush = brushAllocation_->getD2DBrush(context_.Get(), brush);
  context_->DrawTextW(text.c_str(), text.size(), ToWriteTextFormat(font).Get(),
                      ToD2DRectF(rect), d2dBrush.Get());
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
      DirectXRes ::getD3D11Device().Get(), hWnd, &props_dscd, nullptr, nullptr,
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

std::unique_ptr<TextFormat> D2DContext2D::createTextFormat(const String& name,
                                                           float size,
                                                           FontWeight weight) {
  ComPtr<IDWriteTextFormat> textFormat;
  auto factory = DirectXRes::getDWriteFactory();
  ThrowIfFailed(factory->CreateTextFormat(
      name.c_str(), nullptr, ConvertTo<DWRITE_FONT_WEIGHT>(weight),
      DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, size, TEXT(""),
      &textFormat));
  return std::make_unique<DWriteTextFormat>(std::move(textFormat));
}
}  // namespace windows
}  // namespace platforms
}  // namespace yuki

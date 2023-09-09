#pragma once
#include <type_traits>

struct NFColor
{
};

struct PinInfo
{
};

template <class...>
struct NotImplemented : std::bool_constant<false>
{
};

//

class NFPainter;
class NFFont;
class NFSize;
class NFGradient;
class NFPixmap;

enum class CaptionPosition
{
    CaptionTop,
    CaptionBottom,
};

enum class CaptionAlignment
{
    Left,
    Right,
    Center
};

struct CaptionMetrics
{
    // 	NFColor captionFontColor;
    // 	NFSize captionFontSize;
    // 	NFGradient captionBackgroundColor;
    // 	CaptionPosition captionPos;
    // 	CaptionAlignment captionAlig;
};

struct NodeDrawSettings
{
    // 	NFGradient backgroundColor;
    // 	NFColor borderColor;
    // 	double borderWidth;
    // 	double borderRadius;
};

struct PinMargins
{
    int top;
    int bottom;
    int left;
    int right;
};

template <typename IMPL>
class AbstractNodeStyle
{
public:
    NFColor pinColor(PinInfo info)
    {
        return impl().pinColor(info);
    }

protected:
    NFColor getPinColor(PinInfo info)
    {
        static_assert(
            NotImplemented<AbstractNodeStyle>::value,
            "method 'NFColor pinColor(PinInfo info)' not implemented. Override in derived");
    }

    PinMargins getPinMargins(PinInfo info)
    {
        static_assert(
            NotImplemented<AbstractNodeStyle>::value,
            "method 'PinMargins getPinMargins(PinInfo info)' not implemented. Override in derived");
    }

    CaptionMetrics getCaptionMetrics()
    {
        static_assert(
            NotImplemented<AbstractNodeStyle>::value,
            "method 'CaptionSettings captionSettings()' not implemented. Override in derived");
    }

    NFGradient getBackgroundColor(/*NodeType*/)
    {
        static_assert(NotImplemented<AbstractNodeStyle>::value,
                      "method 'NFGradient backgroundColor()' not implemented. Override in derived");
    }

    double getBackgroundOpacity()
    {
        static_assert(NotImplemented<AbstractNodeStyle>::value,
                      "method 'NFGradient backgroundColor()' not implemented. Override in derived");
    }

    NFGradient getBorderColor()
    {
        static_assert(NotImplemented<AbstractNodeStyle>::value,
                      "method 'NFGradient backgroundColor()' not implemented. Override in derived");
    }

    int getBorderThickness()
    {
        static_assert(NotImplemented<AbstractNodeStyle>::value,
                      "method 'NFGradient backgroundColor()' not implemented. Override in derived");
    }

    int getBorderRadius()
    {
        static_assert(NotImplemented<AbstractNodeStyle>::value,
                      "method 'NFGradient backgroundColor()' not implemented. Override in derived");
    }

    void pinDrawDelegate(PinInfo, NFPainter* painter)
    {
        static_assert(NotImplemented<AbstractNodeStyle>::value,
                      "method 'void pinShape(PinInfo info, NFPainter* painter)' not implemented. "
                      "Override in derived");
    }

    void nodeBodyDrawDelegate(PinInfo, NFPainter* painter)
    {
        static_assert(NotImplemented<AbstractNodeStyle>::value,
                      "method 'void pinShape(PinInfo info, NFPainter* painter)' not implemented. "
                      "Override in derived");
    }

private:
    IMPL& impl()
    {
        return *static_cast<IMPL*>(this);
    }
    const IMPL& impl() const
    {
        return *static_cast<IMPL const*>(this);
    }
};

template <typename IMPL = void>
class Style : public std::conditional_t<std::is_same_v<IMPL, void>,
                                        AbstractNodeStyle<Style<>>,
                                        AbstractNodeStyle<IMPL>>
{
    friend AbstractNodeStyle<Style>;

protected:
    NFColor pinColor(PinInfo info)
    {
        pprint("Style");
        return {};
    }

    // 	NFColor getPinColor(PinInfo info)
    // 	{
    // 		pprint("Ja");
    // 		return {};
    // 	}
    // 	friend class AbstractNodeStyle<Styleee>;
};

class SpecStyle : public Style<SpecStyle>
{
    friend AbstractNodeStyle<SpecStyle>;

protected:
    NFColor pinColor(PinInfo info)
    {
        pprint("SpecStyle");
        return {};
    }
};
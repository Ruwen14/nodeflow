namespace nf
{
struct TypeInfo
{
    constexpr TypeInfo() noexcept = default;

    template <typename T>
    constexpr TypeInfo(const T& val) noexcept
        : fundamentalInfo{ fundamentalFlag<T>() }
        , typeID{ entt::type_hash<T>::value() }
    {
    }

    template <typename T>
    constexpr TypeInfo(std::in_place_type_t<T>) noexcept
        : fundamentalInfo{ fundamentalFlag<T>() }
        , typeID{ entt::type_hash<T>::value() }
    {
    }

    template <typename T>
    static constexpr TypeInfo get() noexcept
    {
        return TypeInfo{ std::in_place_type<T> };
    }

    template <typename T>
    constexpr bool equals() const noexcept
    {
        return TypeInfo::get<T>().typeID == typeID;
    }

    constexpr bool isArithmetic() const noexcept
    {
        return !(fundamentalInfo == FundamentalFlag::NonFundamental
                 || fundamentalInfo == FundamentalFlag::Void
                 || fundamentalInfo == FundamentalFlag::Nullptr_t);
    }

    constexpr bool isValidType() const noexcept
    {
        return !(fundamentalInfo == FundamentalFlag::Void
                 || fundamentalInfo == FundamentalFlag::Nullptr_t);
    }

    constexpr bool isVoid() const noexcept
    {
        return fundamentalInfo == FundamentalFlag::Void;
    }

    constexpr bool isConst() const noexcept
    {
        return false;
    }

    constexpr bool isReference() const noexcept
    {
        return false;
    }

    constexpr bool isPointer() const noexcept
    {
        return false;
    }

    constexpr bool isContainer() const noexcept
    {
        return false;
    }

    constexpr bool isStreamable() const noexcept
    {
        return false;
    }

public:
    const FundamentalFlag fundamentalInfo = FundamentalFlag::NonFundamental;
    const std::uint64_t typeID = 0;
};
} // namespace nf
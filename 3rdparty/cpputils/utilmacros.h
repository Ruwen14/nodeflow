#pragma once

/**
 * A utility to build get and set methods on top of a property.
 *
 * @param type The type of the property.
 * @param method The human-readable name for the method.
 * @param name The machine-readable name of the property.
 */
#define GETTERSETTER(type, method, name) \
    /** Retrieves the name value for the object. @return The name value of the object. */ \
    inline type Get##method() const { return name; }  \
    /** Sets the name value for the object. @param value The value of which to set name to. */ \
    inline void Set##method(type value) { name = value; }

#define ADD_ENUM_OPERATORS(enumtype) \
inline enumtype operator~(enumtype rhs) noexcept {return static_cast<enumtype>(~static_cast<std::underlying_type_t<enumtype>>(rhs));} \
inline enumtype operator|(enumtype lhs, enumtype rhs) noexcept { return static_cast<enumtype>(static_cast<std::underlying_type_t<enumtype>>(lhs) | static_cast<std::underlying_type_t<enumtype>>(rhs));} \
inline enumtype operator&(enumtype lhs, enumtype rhs) noexcept { return static_cast<enumtype>(static_cast<std::underlying_type_t<enumtype>>(lhs) & static_cast<std::underlying_type_t<enumtype>>(rhs));} \
inline enumtype operator^(enumtype lhs, enumtype rhs) noexcept { return static_cast<enumtype>(static_cast<std::underlying_type_t<enumtype>>(lhs) ^ static_cast<std::underlying_type_t<enumtype>>(rhs));} \
inline enumtype& operator|=(enumtype& lhs, enumtype rhs) noexcept { return lhs = (lhs | rhs);} \
inline enumtype& operator&=(enumtype& lhs, enumtype rhs) noexcept { return lhs = (lhs & rhs);} \
inline enumtype& operator^=(enumtype& lhs, enumtype rhs) noexcept { return lhs = (lhs ^ rhs);} \

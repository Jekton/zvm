
#pragma once

#include <string>
#include <vector>

#include "util/File.h"
#include "util/debug.h"
#include "util/types.h"

namespace zvm::loader {

struct CPInfo {
    static constexpr int kTagUtf8 = 1;
    static constexpr int kTagClass = 7;
    static constexpr int kTagString = 8;
    static constexpr int kTagFieldRef = 9;
    static constexpr int kTagMethodRef = 10;
    static constexpr int kTagNameAndType = 12;

    u1 tag;

    static const char* nameOfTag(int tag);
};


class ConstantPool {
public:
    struct Utf8Info {
        u1 tag;
        u2 length;
        u1 bytes[0];
    };

    struct ClassInfo {
        u1 tag;
        u2 nameIndex;
    };

    struct StringInfo {
        u1 tag;
        u2 stringIndex;
    };

    struct FieldRef {
        u1 tag;
        u2 classIndex;
        u2 nameAndTypeIndex;
    };

    struct MethodRef {
        u1 tag;
        u2 classIndex;
        u2 nameAndTypeIndex;
    };

    struct NameAndTypeInfo {
        u1 tag;
        u2 nameIndex;
        u2 descriptorIndex;
    };

    using StringPair = std::pair<const std::string*, const std::string*>;

    ConstantPool() = default;
    ConstantPool(const ConstantPool&) = delete;
    ConstantPool& operator=(const ConstantPool&) = delete;
    ConstantPool(ConstantPool&&) = delete;
    ConstantPool& operator=(ConstantPool&&) = delete;

    bool parseCPInfo(util::File& in);
    auto count() const { return mInfos.size(); }

    const CPInfo* get(size_t index) const {
        return mInfos[index];
    }
    const CPInfo* operator[](size_t index) const {
        return get(index);
    }

    const std::string* getUtf8(size_t index) const;
    const std::string* getClass(size_t index) const;
    // TODO: use a String class index
    const std::string* getString(size_t index) const;
    const StringPair* getNameAndType(size_t index) const;

private:
    static CPInfo* parseOne(util::File& in);

    std::vector<CPInfo*> mInfos;
    mutable std::vector<const void*> mParsedInfos;
};


inline auto asUtf8Info(const CPInfo* info) {
    check(info->tag == CPInfo::kTagUtf8);
    return reinterpret_cast<const ConstantPool::Utf8Info*>(info);
}

inline auto asClassInfo(const CPInfo* info) {
    check(info->tag == CPInfo::kTagClass);
    return reinterpret_cast<const ConstantPool::ClassInfo*>(info);
}

inline auto asStringInfo(const CPInfo* info) {
    check(info->tag == CPInfo::kTagString);
    return reinterpret_cast<const ConstantPool::StringInfo*>(info);
}

inline auto asFieldRef(const CPInfo* info) {
    check(info->tag == CPInfo::kTagFieldRef);
    return reinterpret_cast<const ConstantPool::FieldRef*>(info);
}

inline auto asMethodRef(const CPInfo* info) {
    check(info->tag == CPInfo::kTagMethodRef);
    return reinterpret_cast<const ConstantPool::MethodRef*>(info);
}

inline auto asNameAndType(const CPInfo* info) {
    check(info->tag == CPInfo::kTagNameAndType);
    return reinterpret_cast<const ConstantPool::NameAndTypeInfo*>(info);
}



}
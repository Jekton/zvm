
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

    // TODO: use a String class index
    const std::string* getString(size_t index) const;
    const std::string* getUtf8(size_t index) const;

private:
    static CPInfo* parseOne(util::File& in);

    std::vector<CPInfo*> mInfos;
    mutable std::vector<void*> mParsedInfos;
};


inline auto asUtf8Info(CPInfo* info) {
    check(info->tag == CPInfo::kTagUtf8);
    return reinterpret_cast<ConstantPool::Utf8Info*>(info);
}

inline auto asStringInfo(CPInfo* info) {
    check(info->tag == CPInfo::kTagString);
    return reinterpret_cast<ConstantPool::StringInfo*>(info);
}


}

#include "loader/ConstantPool.h"

#include <utility>
#include <map>

#include "util/debug.h"

namespace zvm::loader {

using CPInfoParser = CPInfo* (*)(util::File&, u1 tag);

static CPInfo* utf8Parser(util::File& in, u1 tag);
static CPInfo* classParser(util::File& in, u1 tag);
static CPInfo* stringParser(util::File& in, u1 tag);
static CPInfo* fieldRefParser(util::File& in, u1 tag);
static CPInfo* methodRefParser(util::File& in, u1 tag);
static CPInfo* nameAndTypeParser(util::File& in, u1 tag);

static std::map<u1, CPInfoParser> sParsers = {
    {CPInfo::kTagUtf8, utf8Parser},
    {CPInfo::kTagClass, classParser},
    {CPInfo::kTagString, stringParser},
    {CPInfo::kTagFieldRef, fieldRefParser},
    {CPInfo::kTagMethodRef, methodRefParser},
    {CPInfo::kTagNameAndType, nameAndTypeParser},
};


const char* CPInfo::nameOfTag(int tag) {
    switch (tag) {
        case kTagUtf8: return "Utf8";
        case kTagClass: return "Class";
        case kTagString: return "String";
        case kTagFieldRef: return "Fieldref";
        case kTagMethodRef: return "Methodref";
        case kTagNameAndType: return "NameAndType";
        default: return "[unknown]";
    }
}


bool ConstantPool::parseCPInfo(util::File& in) {
    u2 constantPoolCount;
    ensure(in.readBigEndian(&constantPoolCount), "fail to read constant_pool_count");
    mInfos.push_back(nullptr);
    for (u2 i = 1; i < constantPoolCount; ++i) {
        auto info = ConstantPool::parseOne(in);
        if (!info) {
            return false;
        }
        mInfos.push_back(info);
    }
    mParsedInfos.resize(mInfos.size());
    return true;
}


CPInfo* ConstantPool::parseOne(util::File& in) {
    u1 tag;
    if (!in.read(&tag)) {
        fprintf(stderr, "ConstantPool: fail to read tag\n");
        return nullptr;
    }
    auto parser = sParsers[tag];
    if (parser == nullptr) {
        fprintf(stderr, "ConstantPool: Unknown tag %u\n", tag);
        return nullptr;
    }
    auto info = parser(in, tag);
    if (!info) {
        fprintf(stderr, "ConstantPool: Fail to parse tag %u\n", tag);
    }
    return info;
}


static CPInfo* utf8Parser(util::File& in, u1 tag) {
    u2 length;
    if (!in.readBigEndian(&length)) {
        return nullptr;
    }
    auto offset = offsetof(ConstantPool::Utf8Info, bytes);
    auto ptr = new u1[length + offset];
    if (!in.read(ptr + offset, length)) {
        return nullptr;
    }
    auto info = reinterpret_cast<ConstantPool::Utf8Info*>(ptr);
    info->tag = tag;
    info->length = length;
    return reinterpret_cast<CPInfo*>(info);
}

static CPInfo* classParser(util::File& in, u1 tag) {
    auto info = new ConstantPool::ClassInfo{tag, 0};
    if (!in.readBigEndian(&info->nameIndex)) {
        return nullptr;
    }
    return reinterpret_cast<CPInfo*>(info);
}

static CPInfo* stringParser(util::File& in, u1 tag) {
    auto info = new ConstantPool::StringInfo{tag};
    if (!in.readBigEndian(&info->stringIndex)) {
        return nullptr;
    }
    return reinterpret_cast<CPInfo*>(info);
}

static CPInfo* fieldRefParser(util::File& in, u1 tag) {
    auto info = new ConstantPool::FieldRef{tag};
    if (!in.readBigEndian(&info->classIndex)) {
        return nullptr;
    }
    if (!in.readBigEndian(&info->nameAndTypeIndex)) {
        return nullptr;
    }
    return reinterpret_cast<CPInfo*>(info);
}

static CPInfo* methodRefParser(util::File& in, u1 tag) {
    auto info = new ConstantPool::MethodRef{tag};
    if (!in.readBigEndian(&info->classIndex)) {
        return nullptr;
    }
    if (!in.readBigEndian(&info->nameAndTypeIndex)) {
        return nullptr;
    }
    return reinterpret_cast<CPInfo*>(info);
}

static CPInfo* nameAndTypeParser(util::File& in, u1 tag) {
    auto info = new ConstantPool::NameAndTypeInfo{tag};
    if (!in.readBigEndian(&info->nameIndex)) {
        return nullptr;
    }
    if (!in.readBigEndian(&info->descriptorIndex)) {
        return nullptr;
    }
    return reinterpret_cast<CPInfo*>(info);
}

const std::string* ConstantPool::getString(size_t index) const {
    auto ptr = mParsedInfos[index];
    if (ptr) return static_cast<const std::string*>(ptr);
    auto info = asStringInfo(mInfos[index]);
    return getUtf8(info->stringIndex);
}

const std::string* ConstantPool::getUtf8(size_t index) const {
    auto ptr = mParsedInfos[index];
    if (ptr) return static_cast<const std::string*>(ptr);
    auto info = asUtf8Info(mInfos[index]);
    auto chars = reinterpret_cast<const char*>(info->bytes);
    auto str = new std::string(chars, info->length);
    mParsedInfos[index] = str;
    return str;
}

const std::string* ConstantPool::getClass(size_t index) const {
    auto ptr = mParsedInfos[index];
    if (ptr) return static_cast<const std::string*>(ptr);
    auto info = asClassInfo(mInfos[index]);
    auto str = getUtf8(info->nameIndex);
    mParsedInfos[index] = str;
    return str;
}

const ConstantPool::StringPair* ConstantPool::getNameAndType(size_t index) const {
    auto ptr = mParsedInfos[index];
    if (ptr) return static_cast<const StringPair*>(ptr);
    auto info = asNameAndType(mInfos[index]);
    auto name = getUtf8(info->nameIndex);
    auto descriptor = getUtf8(info->descriptorIndex);
    auto p = new StringPair(name, descriptor);
    mParsedInfos[index] = p;
    return p;
}

}
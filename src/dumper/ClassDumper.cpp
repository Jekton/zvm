
#include "dumper/ClassDumper.h"

namespace zvm::dumper {

void ClassDumper::dump(FILE* out) const {
    fprintf(out, "Magic: %8x\n", mClassFile->magic());
    fprintf(out, "minor version: %u\n", mClassFile->minorVersion());
    fprintf(out, "major version: %u\n", mClassFile->majorVersion());
    dumpAccessFlags(out);
    {
        char buf[256];
        snprintf(buf, sizeof(buf), "this_class: #%d", mClassFile->thisClassIndex());
        fprintf(out, "%-41s // %s\n", buf, mClassFile->thisClass()->c_str());
        snprintf(buf, sizeof(buf), "super_class: #%d", mClassFile->superClassIndex());
        fprintf(out, "%-41s // %s\n", buf, mClassFile->superClass()->c_str());
    }
    dumpConstantPool(out);
}

void ClassDumper::dumpConstantPool(FILE* out) const {
    auto& constantPool = mClassFile->constantPool();
    auto count = constantPool.count();
    char buf[1024];
    for (int i = 1; i < count; ++i) {
        snprintf(buf, sizeof(buf), "#%d", i);
        auto rawInfo = constantPool.get(i);
        auto tag = rawInfo->tag;
        using loader::CPInfo;
        fprintf(out, "%5s = %-19s", buf, CPInfo::nameOfTag(tag));
        switch (tag) {
            case CPInfo::kTagUtf8: {
                auto str = constantPool.getUtf8(i);
                fprintf(out, "%s\n", str->c_str());
                break;
            }
            case CPInfo::kTagClass: {
                fprintf(out, "#%-14u// %s\n",
                        loader::asClassInfo(rawInfo)->nameIndex,
                        constantPool.getClass(i)->c_str());
                break;
            }
            case CPInfo::kTagString: {
                fprintf(out, "#%-14u// %s\n",
                        loader::asStringInfo(rawInfo)->stringIndex,
                        constantPool.getString(i)->c_str());
                break;
            }
            case CPInfo::kTagFieldRef: {
                auto info = loader::asFieldRef(rawInfo);
                snprintf(buf, sizeof(buf), "#%u.#%u", info->classIndex, info->nameAndTypeIndex);
                auto clazz = constantPool.getClass(info->classIndex);
                auto nameAndType = constantPool.getNameAndType(info->nameAndTypeIndex);
                fprintf(out, "%-15s// %s.%s:%s\n",
                        buf, clazz->c_str(),
                        nameAndType->first->c_str(),
                        nameAndType->second->c_str());
                break;
            }
            case CPInfo::kTagMethodRef: {
                auto info = loader::asMethodRef(rawInfo);
                snprintf(buf, sizeof(buf), "#%u.#%u", info->classIndex, info->nameAndTypeIndex);
                auto clazz = constantPool.getClass(info->classIndex);
                auto nameAndType = constantPool.getNameAndType(info->nameAndTypeIndex);
                fprintf(out, "%-15s// %s.%s:%s\n",
                        buf, clazz->c_str(),
                        nameAndType->first->c_str(),
                        nameAndType->second->c_str());
                break;
            }
            case CPInfo::kTagNameAndType: {
                auto info = loader::asNameAndType(rawInfo);
                snprintf(buf, sizeof(buf), "#%u:#%u", info->nameIndex, info->descriptorIndex);
                auto nameAndType = constantPool.getNameAndType(i);
                fprintf(out, "%-15s// %s:%s\n",
                        buf,
                        nameAndType->first->c_str(),
                        nameAndType->second->c_str());
                break;
            }
            default: {
                fprintf(out, "[unknown tag]\n");
                break;
            }
        }
    }
}

static void dumpAFlags(
    FILE* out, u2 flags,
    const int flagInt,
    const char* flagStr,
    bool& isFirst
) {
    if (flags & flagInt) {
        if (isFirst) {
            isFirst = false;
            fprintf(out, " %s", flagStr);
        } else {
            fprintf(out, ", %s", flagStr);
        }
    }
}

void ClassDumper::dumpAccessFlags(FILE* out) const {
    auto flags = mClassFile->accessFlags();
    fprintf(out, "flags: (0x%04x)", flags);
    bool isFirst = true;
    using namespace loader;
    dumpAFlags(out, flags, ClassFile::kAccPublic, "ACC_PUBLIC", isFirst);
    dumpAFlags(out, flags, ClassFile::kAccFinal, "ACC_FINAL", isFirst);
    dumpAFlags(out, flags, ClassFile::kAccSuper, "ACC_SUPER", isFirst);
    dumpAFlags(out, flags, ClassFile::kAccInterface, "ACC_INTERFACE", isFirst);
    dumpAFlags(out, flags, ClassFile::kAccAbstract, "ACC_ABSTRACT", isFirst);
    dumpAFlags(out, flags, ClassFile::kAccSynthetic, "ACC_SYNTHETIC", isFirst);
    dumpAFlags(out, flags, ClassFile::kAccAnnotation, "ACC_ANNOTATION", isFirst);
    dumpAFlags(out, flags, ClassFile::kAccEnum, "ACC_ENUM", isFirst);
    fprintf(out, "\n");
}


}
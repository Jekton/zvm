
#include "dumper/ClassDumper.h"

namespace zvm::dumper {

void ClassDumper::dump(FILE* out) const {
    fprintf(out, "Magic: %8x\n", mClassFile->magic());
    fprintf(out, "minor version: %u\n", mClassFile->minorVersion());
    fprintf(out, "major version: %u\n", mClassFile->majorVersion());
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

}
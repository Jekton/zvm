
#include "ClassFile.h"
#include "util/debug.h"

namespace zvm::loader {

const ClassFile::Code* ClassFile::Method::getCode() const {
    auto it = attributes.find(kAttributeCode);
    if (it == attributes.end()) {
        return nullptr;
    }
    return reinterpret_cast<const Code*>(it->second);
}


}

#include <arpa/inet.h>

#include "loader/ClassFileParser.h"
#include "loader/ClassVerifier.h"

namespace zvm::loader {

static bool verifyClass(ClassFile* classFile);


std::unique_ptr<ClassFile> ClassFileParser::parse(util::File file) {
    auto classFile = std::make_unique<ClassFile>();
    u4 magic;
    fread(&magic, 4, 1, file.get());
    classFile->mMagic = ntohl(magic);

    u2 version;
    fread(&version, 2, 1, file.get());
    classFile->mMinorVersion = ntohs(version);
    fread(&version, 2, 1, file.get());
    classFile->mMajorVersion = ntohs(version);

    if (!verifyClass(classFile.get())) {
        classFile.reset();
    }
    return classFile;
}


static bool verifyClass(ClassFile* classFile) {
    ClassVerifier verifier(classFile);
    return verifier.verify();
}

}


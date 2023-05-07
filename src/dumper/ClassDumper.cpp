
#include "dumper/ClassDumper.h"

namespace zvm::dumper {

void ClassDumper::dump(FILE* out) const {
    fprintf(out, "Magic: %8x\n", mClassFile->magic());
}

}
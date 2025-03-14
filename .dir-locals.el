;;; Directory Local Variables            -*- no-byte-compile: t -*-
;;; For more information see (info "(emacs) Directory Variables")

((nil . ((compile-command .
                          (eval
                           (format "cmake -B build -S . -DCMAKE_C_COMIPLER=%S -DCMAKE_CXX_COMIPLER=%S -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DFETCHCONTENT_FULLY_DISCONNECTED=OFF -G Ninja && cmake --build build"
                                   (executable-find "clang")
                                   (executable-find "clang++")))))))

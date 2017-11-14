#/bin/bash
func="TIoptimizer"
file="TIoptimizer"
#buildfolder="~/clang-llvm/build"
#toolsfolder="~/clang-llvm/llvm/tools/clang/tools/extra/"$func""

#step 1: modify the cmakelists file in the parent folder.
step1(){
  echo "add_subdirectory("$func")" >> ../CMakeLists.txt
}

#step 2: add a cmakelists file in the current folder with the following content
step2(){
  echo "\
    set(LLVM_LINK_COMPONENTS support)

  add_clang_executable("$func"
  "$file".cpp
  )

  target_link_libraries("$func"
  clangTooling
  clangBasic
  clangASTMatchers
  )" > CMakeLists.txt
}

#step3: build the tool
step3(){
  cd ~/clang-llvm/build 
  #cd $(echo $buildfolder | tr -d '\r')
  ninja "$func"
}


#step4: test the tool
step4(){
  cd ~/clang-llvm/llvm/tools/clang/tools/extra/"$func"
  "$buildfolder"/bin/"$func" test.c --  
}

#MAIN
step1
step2
step3
#step4

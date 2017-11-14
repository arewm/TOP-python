/*
   Yufei Ding
   TIoptimizer Prototype
   */
//------------------------------------------------------------------------------
#include <sstream>
#include <string>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;

static llvm::cl::OptionCategory ToolingSampleCategory("Tooling Sample");
bool iterativeP; //whether the problem is iterative;
bool Q_e_T;      //whether Q and T are the same data set;
std::vector<bool> QTupdateStatus(2,false); //which data sets get updated if it is a iterative problem; QTupdateStatus[0] -> Q, QTupdateStatus[1] -> T.
std::vector<std::string> argumentList(2);
int CategoryStep1;
std::string DistanceType;
int bigCat;
//Taxonomy of landmark definitions in each category of distance-related problems.
int BigCatetory(bool iterativeP, bool Q_e_T, int QTupdateStatus){
  int Catetory;
  if(iterativeP == false) Catetory = 1;
  else{
    if (Q_e_T == true) Catetory = 5;
    else{
      if(QTupdateStatus == 1) Catetory = 2; //only T gets updated;
      else if(QTupdateStatus == 2) Catetory = 3; //only Q gets updated;
      else Catetory = 4; //both gets updated
    }
  }
  return Catetory;
}


std::string Expr2str(CallExpr *d, Rewriter &TheRewriter) {
  SourceManager &sm = TheRewriter.getSourceMgr();
  // (T, U) => "T,,"
  std::string text = Lexer::getSourceText(CharSourceRange::getTokenRange(d->getSourceRange()), sm, LangOptions(), 0);
  if (text.at(text.size()-1) == ',')
    return Lexer::getSourceText(CharSourceRange::getCharRange(d->getSourceRange()), sm, LangOptions(), 0);
  return text;
}

// By implementing RecursiveASTVisitor, we can specify which AST nodes
// we're interested in by overriding relevant methods.
class MyASTVisitor_pass1 : public RecursiveASTVisitor<MyASTVisitor_pass1> {
  public:
    MyASTVisitor_pass1(Rewriter &R, ASTContext *Context) : TheRewriter(R), TheContext(Context) {}

    //Pass1: 
    //find whether the problem is iterative: bool iterativeP;  
    //find whether Q and T are the same data set: bool Q_e_T;
    //find distanceTypp
    bool VisitCallExpr(CallExpr *f) {
      // Function name
      FunctionDecl *functioncall =  f->getDirectCallee(); 

      if(functioncall){
        DeclarationName DeclName = functioncall->getNameInfo().getName();
        std::string FuncName = DeclName.getAsString();

        //check D: "distance definition". 
        if (boost::starts_with(FuncName, "TOP_defDistance")){
          
          clang::LangOptions LangOpts;
          LangOpts.CPlusPlus = true;
          clang::PrintingPolicy Policy(LangOpts);
          std::string TypeS;
          llvm::raw_string_ostream s(TypeS);
          f->getArg(0)->printPretty(s, 0, Policy);
          //std::cout << "Yufei arg: " << s.str() << "\n";
          DistanceType = s.str();
          std::cout << "Pass1, DistanceType: " << DistanceType << "\n";
        }


        //find R "relation".
        if (boost::starts_with(FuncName, "TOP_findClosestTargets")){

          //print function args   
          clang::LangOptions LangOpts;
          LangOpts.CPlusPlus = true;
          clang::PrintingPolicy Policy(LangOpts);

          for(int i = 0, j = f->getNumArgs(); i < j; i++){
            //std::cout << "argType: " << f->getArg(i)->getType().getAsString() << "\n";
            std::string TypeS;
            llvm::raw_string_ostream s(TypeS);
            f->getArg(i)->printPretty(s, 0, Policy);
            std::cout << "Pass1, arg: " << s.str() << "\n";
          }

          //check whether Q and T are the same data set:
           Q_e_T = false;
          //for(int i = 0, j = f->getNumArgs(); i < j; i++)
          for(int i = 1, j = 2; i <= j; i++){
            std::string TypeS;
            llvm::raw_string_ostream s(TypeS);
            f->getArg(i)->printPretty(s, 0, Policy);
            argumentList[i-1] = s.str();
            std::cout << "Pass1, arg: " << s.str() << "\n";
            //std::cout << "argType: " << f->getArg(i)->getType().getAsString() << "\n";
          }

          if(argumentList[0] == argumentList[1]){
            Q_e_T = true;
          }
          
          //step2: check whether top is inside a while or for loop
          iterativeP = false;
          auto it = TheContext->getParents(*f).begin();
          auto itsend = TheContext->getParents(*f).end();
          while(it != itsend){
            const Stmt *aStmt = it->get<clang::Stmt>();
            if(aStmt){
              if(isa<WhileStmt>(aStmt)){
                std::stringstream SInsert;
                SInsert << "// While loop with function call " << FuncName << "\n";
                TheRewriter.InsertTextAfterToken(aStmt->getLocEnd().getLocWithOffset(1), SInsert.str());
                iterativeP = true;
                std::cout << "Pass1, iterativeP " << iterativeP << std::endl;
                break;
              }
              else{
                it = TheContext->getParents(*aStmt).begin();
                itsend = TheContext->getParents(*aStmt).end();
              }
            }
            else{
              break;
            }
          }
        }

      }
      return true;
    }

  private:
    Rewriter &TheRewriter;
    ASTContext *TheContext;
};


// 
class MyASTVisitor_pass2 : public RecursiveASTVisitor<MyASTVisitor_pass2> {
  public:
    MyASTVisitor_pass2(Rewriter &R, ASTContext *Context) : TheRewriter(R), TheContext(Context) {}

    //find which data sets get updated if it is a iterative problem: 
    //We need to make it in the second pass, as we need info about Q and T in TOP_find function (argumentList[]):
    bool VisitCallExpr(CallExpr *f) {
      // Function name
      FunctionDecl *functioncall =  f->getDirectCallee(); 

      if(functioncall){
        DeclarationName DeclName = functioncall->getNameInfo().getName();
        std::string FuncName = DeclName.getAsString();

        //find R "relation".
        if (boost::starts_with(FuncName, "TOP_update")){
          //step3: check which gets updated Q or T;
          //print function args   
          clang::LangOptions LangOpts;
          LangOpts.CPlusPlus = true;
          clang::PrintingPolicy Policy(LangOpts);
          //for(int i = 0, j = f->getNumArgs(); i < j; i++)
          for(int i = 0, j = 1; i < j; i++){
            std::string TypeS;
            llvm::raw_string_ostream s(TypeS);
            f->getArg(i)->printPretty(s, 0, Policy);
            if(argumentList[0] == s.str()){
              QTupdateStatus[0] = true;              

            } 
            else if(argumentList[1] == s.str()){
              QTupdateStatus[1] = true;
            } 

            //std::cout << "arg: " << s.str() << "  QTupdateStatus[0] " <<  QTupdateStatus[0] << "  QTupdateStatus[1] " <<  QTupdateStatus[1] << "\n";
            //std::cout << "argType: " << f->getArg(i)->getType().getAsString() << "\n";
          }
        }

      }
      return true;
    }

  private:
    Rewriter &TheRewriter;
    ASTContext *TheContext;
};


//after determining the bigCat,
//1. insert functions to further determine the fineCat and number of landmarks to create
//2. add functions to create landmarks and connect them with S or T for later optimizations.
//3. replace high-level functions with low-level optimized function.
class MyASTVisitor_pass3 : public RecursiveASTVisitor<MyASTVisitor_pass3> {
  public:
    MyASTVisitor_pass3(Rewriter &R, ASTContext *Context) : TheRewriter(R), TheContext(Context) {}
    
    /*
    bool VisitFunctionDecl(FunctionDecl *f) {
      // Only function definitions (with bodies), not declarations.
      if (f->hasBody()) {
        Stmt *FuncBody = f->getBody();

        // Type name as string
        QualType QT = f->getReturnType();
        std::string TypeStr = QT.getAsString();

        // Function name
        DeclarationName DeclName = f->getNameInfo().getName();
        std::string FuncName = DeclName.getAsString();

        // Add comment before
        
        if(FuncName == "main"){

          std::stringstream SInsert;

          //add distance function
          if (DistanceType == "\"Euclidean\""){
            SInsert<< "\n double distance(const std::vector<double> &a, const std::vector<double> &b){ \n double total  = 0; \n for(unsigned int i = 0; i < a.size(); i++){ \n double d = a[i] - b[i]; \n total += d * d; \n }\n return sqrt(total);\n}\n\n";
            TheRewriter.InsertText(f->getLocStart(), SInsert.str());
          }

          if(bigCat == 4){
            std::cout << "Yufei bigCat 4" << std::endl;
            SInsert<< "\n double defCate(int a){ \n return a/10; \n }\n\n";
            TheRewriter.InsertText(f->getLocStart(), SInsert.str());
          }

          // And after
          SourceLocation ST = f->getSourceRange().getBegin();
          std::stringstream SSAfter;
          SSAfter << "\n// End function " << FuncName;
          ST = FuncBody->getLocEnd().getLocWithOffset(1);
          TheRewriter.InsertText(ST, SSAfter.str(), true, true);
        }
        

      }

      return true;
    }
    */

    bool VisitCallExpr(CallExpr *f) {
      // Function name
      FunctionDecl *functioncall =  f->getDirectCallee(); 
      if(functioncall){
        DeclarationName DeclName = functioncall->getNameInfo().getName();
        std::string FuncName = DeclName.getAsString();

        //check D: "distance definition". @Yufei It should be a declaration or function call?
        if (boost::starts_with(FuncName, "TOP_defDistance")){
          std::stringstream SInsert;
          //SInsert << "// Distance Defintion: \n";
          //TheRewriter.InsertText(f->getLocStart(), SInsert.str());
          //TheRewriter.ReplaceText(f->getLocStart(), FuncName.length(), "TOP_disChanged");
          
          //check whether it uses one of our default distance definition. In our current settings, we only have "Euclidean".
          //we check the first args of function "TOP_defDistance"
          clang::LangOptions LangOpts;
          LangOpts.CPlusPlus = true;
          clang::PrintingPolicy Policy(LangOpts);
          std::string TypeS;
          llvm::raw_string_ostream s(TypeS);
          f->getArg(0)->printPretty(s, 0, Policy);
          //std::cout << "Yufei arg: " << s.str() << "\n";
          
          if (s.str() == "\"Euclidean\""){
            //TheRewriter.RemoveText(f->getSourceRange()); //doesn't remove anything 
            std::string temp = Expr2str(f, TheRewriter); 
            //TheRewriter.RemoveText(f->getLocStart(), temp.size() + 1);
            TheRewriter.ReplaceText(f->getLocStart(), temp.size() + 1, "//function call removed");
          }
        }


        if (boost::starts_with(FuncName, "TOP_findClosestTargets")){

          //print function args   
          clang::LangOptions LangOpts;
          LangOpts.CPlusPlus = true;
          clang::PrintingPolicy Policy(LangOpts);

          int length_augument = f->getNumArgs();
          std::vector<std::string> TOP_argumentList(length_augument);
          for(int i = 0, j = length_augument; i < j; i++){
            //std::cout << "argType: " << f->getArg(i)->getType().getAsString() << "\n";
            std::string TypeS;
            llvm::raw_string_ostream s(TypeS);
            f->getArg(i)->printPretty(s, 0, Policy);
            TOP_argumentList[i] = s.str();
          }
          
          std::string length_TOP_find = Expr2str(f, TheRewriter); 
          std::stringstream SReplace;
          SReplace << "TOP_findClosestTargets_bigCat_2_distanceType_Euclidean(" << TOP_argumentList[0] << ", " << "catType, num_cclusters, iteration_count, " << TOP_argumentList[1] << ", " << TOP_argumentList[2] << ", C_CLUSTERS_changemax);\n";
          TheRewriter.ReplaceText(f->getLocStart(), length_TOP_find.size() + 1, SReplace.str());


          //step2: if TOP is inside a while (for loop), find the "while" loop.
          if(iterativeP){
            auto it = TheContext->getParents(*f).begin();
            auto itsend = TheContext->getParents(*f).end();
            while(it != itsend){
              const Stmt *aStmt = it->get<clang::Stmt>();
              if(aStmt){
                if(isa<WhileStmt>(aStmt)){
                  std::stringstream SInsert;
                  //SInsert<< "\n double distance(const std::vector<double> &a, const std::vector<double> &b){ \n double total  = 0; \n for(unsigned int i = 0; i < a.size(); i++){ \n double d = a[i] - b[i]; \n total += d * d; \n }\n return sqrt(total);\n}\n\n";
                  SInsert << "\n\tint Budget = 1000;\n" //obtain such value using system call.
                   "\tint bigCat = " << bigCat << ";\n" 
                   "\tbool distributedPlatform = false;\n "
                   "\tfineCat selectCat_Land = detailedCategory(Budget, bigCat, " << TOP_argumentList[1] << ", " << TOP_argumentList[2] << ", distributedPlatform);\n "
                   "\tint catType = selectCat_Land.catType;\n "
                   "\tint num_cclusters = selectCat_Land.num_landmarks;\n "
                   "\tstd::vector<double> C_CLUSTERS_changemax;\n "
                   "\tC_CLUSTERS_changemax.resize(num_cclusters);\n " 
                   "\tTOP_buildLandmark_bigCat_2_distanceType_Euclidean(" << TOP_argumentList[0] << ", " << "catType, num_cclusters, " << TOP_argumentList[1] << ", " << TOP_argumentList[2] << ", C_CLUSTERS_changemax);\n "
                   "\tint iteration_count = 0; \n\n";

                  TheRewriter.InsertText(aStmt->getLocStart(), SInsert.str());
                  

                  //SInsert << "// While loop with function call " << FuncName << "\n";
                  //TheRewriter.InsertTextAfterToken(aStmt->getLocEnd().getLocWithOffset(1), SInsert.str());
                  break;
                }
                else{
                  it = TheContext->getParents(*aStmt).begin();
                  itsend = TheContext->getParents(*aStmt).end();
                }
              }
              else{
                break;
              }
            }
          }
        }
        /*
        //check R "relation":
        if (boost::starts_with(FuncName, "TOP_find")){

          std::string temp = Expr2str(f, TheRewriter);
          std::cout << "Yufei print " << temp << std::endl; 

          std::stringstream SInsert;
          SInsert << "// Begin function call " << FuncName << "\n";
          //SourceLocation ST = f->getLocStart();
          //TheRewriter.InsertText(ST, SInsert.str(), true, true);
          //TheRewriter.InsertTextAfterToken(f->getLocEnd().getLocWithOffset(1), SInsert.str());

          //change function name:
          TheRewriter.ReplaceText(f->getLocStart(), FuncName.length(), "TOP_changed");
        }
        */
      }

      return true;
    }

  private:
    Rewriter &TheRewriter;
    ASTContext *TheContext;
};



// By implementing RecursiveASTVisitor, we can specify which AST nodes
// we're interested in by overriding relevant methods.
class MyASTVisitor_pass4 : public RecursiveASTVisitor<MyASTVisitor_pass4> {
  public:
    MyASTVisitor_pass4(Rewriter &R, ASTContext *Context) : TheRewriter(R), TheContext(Context) {}

    bool VisitFunctionDecl(FunctionDecl *f) {
      // Only function definitions (with bodies), not declarations.
      if (f->hasBody()) {
        Stmt *FuncBody = f->getBody();

        // Type name as string
        QualType QT = f->getReturnType();
        std::string TypeStr = QT.getAsString();

        // Function name
        DeclarationName DeclName = f->getNameInfo().getName();
        std::string FuncName = DeclName.getAsString();

       // Add comment before
        if(FuncName == "main"){
          std::stringstream SSBefore;
          SSBefore << "// Begin function " << FuncName << " returning " << TypeStr
            << "\n";
          SourceLocation ST = f->getSourceRange().getBegin();
          TheRewriter.InsertText(ST, SSBefore.str(), true, true);

          // And after
          std::stringstream SSAfter;
          SSAfter << "\n// End function " << FuncName;
          ST = FuncBody->getLocEnd().getLocWithOffset(1);
          TheRewriter.InsertText(ST, SSAfter.str(), true, true);
        }
      }
      return true;
    }

    // find whether the problem is iterative: bool iterativeP;  find whether Q and T are the same data set: bool Q_e_T; find which data sets get updated if it is a iterative problem:
    bool VisitCallExpr(CallExpr *f) {
      // Function name
      FunctionDecl *functioncall =  f->getDirectCallee(); 

      if(functioncall){
        DeclarationName DeclName = functioncall->getNameInfo().getName();
        std::string FuncName = DeclName.getAsString();

        //find R "relation".
        if (boost::starts_with(FuncName, "TOP_update")){
          //step3: check which gets updated Q or T;
          //print function args   
          clang::LangOptions LangOpts;
          LangOpts.CPlusPlus = true;
          clang::PrintingPolicy Policy(LangOpts);
          //for(int i = 0, j = f->getNumArgs(); i < j; i++)
          for(int i = 0, j = 1; i < j; i++){
            std::string TypeS;
            llvm::raw_string_ostream s(TypeS);
            f->getArg(i)->printPretty(s, 0, Policy);
            if(argumentList[0] == s.str()){
              QTupdateStatus[0] = true; 
            } 
            else if(argumentList[1] == s.str()){
              QTupdateStatus[1] = true;
            } 

            //std::cout << "arg: " << s.str() << "  QTupdateStatus[0] " <<  QTupdateStatus[0] << "  QTupdateStatus[1] " <<  QTupdateStatus[1] << "\n";
            //std::cout << "argType: " << f->getArg(i)->getType().getAsString() << "\n";
          }
        }
      }
      return true;
    }

  private:
    Rewriter &TheRewriter;
    ASTContext *TheContext;
};

// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser.
class MyASTConsumer_pass1 : public ASTConsumer {
  public:
    MyASTConsumer_pass1(Rewriter &R, ASTContext *Context) : Visitor(R, Context) {}

    // Override the method that gets called for each parsed top-level
    // declaration.
    bool HandleTopLevelDecl(DeclGroupRef DR) override {
      for (DeclGroupRef::iterator b = DR.begin(), e = DR.end(); b != e; ++b) {
        // Traverse the declaration using our AST visitor.
        Visitor.TraverseDecl(*b);
        //(*b)->dump();
      }
      return true;
    }

  private:
    MyASTVisitor_pass1 Visitor;
};

// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser.
class MyASTConsumer_pass2 : public ASTConsumer {
  public:
    MyASTConsumer_pass2(Rewriter &R, ASTContext *Context) : Visitor(R, Context) {}

    // Override the method that gets called for each parsed top-level
    // declaration.
    bool HandleTopLevelDecl(DeclGroupRef DR) override {
      for (DeclGroupRef::iterator b = DR.begin(), e = DR.end(); b != e; ++b) {
        // Traverse the declaration using our AST visitor.
        Visitor.TraverseDecl(*b);
        //(*b)->dump();
      }
      return true;
    }

  private:
    MyASTVisitor_pass2 Visitor;
};

// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser.
class MyASTConsumer_pass3 : public ASTConsumer {
  public:
    MyASTConsumer_pass3(Rewriter &R, ASTContext *Context) : Visitor(R, Context) {}

    // Override the method that gets called for each parsed top-level
    // declaration.
    bool HandleTopLevelDecl(DeclGroupRef DR) override {
      for (DeclGroupRef::iterator b = DR.begin(), e = DR.end(); b != e; ++b) {
        // Traverse the declaration using our AST visitor.
        Visitor.TraverseDecl(*b);
        //(*b)->dump();
      }
      return true;
    }

  private:
    MyASTVisitor_pass3 Visitor;
};

// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser.
class MyASTConsumer_pass4 : public ASTConsumer {
  public:
    MyASTConsumer_pass4(Rewriter &R, ASTContext *Context) : Visitor(R, Context) {}
    bool HandleTopLevelDecl(DeclGroupRef DR) override {
      for (DeclGroupRef::iterator b = DR.begin(), e = DR.end(); b != e; ++b) {
        // Traverse the declaration using our AST visitor.
        Visitor.TraverseDecl(*b);
        //(*b)->dump();
      }
      return true;
    }
  private:
    MyASTVisitor_pass4 Visitor;
};

// For each source file provided to the tool, a new FrontendAction is created.
class MyFrontendAction_pass1 : public ASTFrontendAction {
  public:
    MyFrontendAction_pass1() {}

    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
        StringRef file) override {
      //llvm::errs() << "** Creating AST consumer for: " << file << "\n";
      TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
      return llvm::make_unique<MyASTConsumer_pass1>(TheRewriter, &CI.getASTContext());
    }

  private:
    Rewriter TheRewriter;
};


// For each source file provided to the tool, a new FrontendAction is created.
class MyFrontendAction_pass2 : public ASTFrontendAction {
  public:
    MyFrontendAction_pass2() {}


    //void EndSourceFileAction() override {
    //SourceManager &SM = TheRewriter.getSourceMgr();
    //llvm::errs() << "** EndSourceFileAction for: " << SM.getFileEntryForID(SM.getMainFileID())->getName() << "\n";
    //}

    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
        StringRef file) override {
      //llvm::errs() << "** Creating AST consumer for: " << file << "\n";
      TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
      return llvm::make_unique<MyASTConsumer_pass2>(TheRewriter, &CI.getASTContext());
    }

  private:
    Rewriter TheRewriter;
};



// For each source file provided to the tool, a new FrontendAction is created.
class MyFrontendAction_pass3 : public ASTFrontendAction {
  public:
    MyFrontendAction_pass3() {}
    void EndSourceFileAction() override {
      SourceManager &SM = TheRewriter.getSourceMgr();
      //llvm::errs() << "** EndSourceFileAction for: " << SM.getFileEntryForID(SM.getMainFileID())->getName() << "\n";

      // Now emit the rewritten buffer.
      //  TheRewriter.getEditBuffer(SM.getMainFileID()).write(llvm::outs()); --> this will output to screen as what you got.
      std::error_code error_code;
      std::stringstream outputFile;
      outputFile << "TIoptimizer_generated.cpp";
      //llvm::raw_fd_ostream outFile("output.cpp", error_code, llvm::sys::fs::F_None);
      llvm::raw_fd_ostream outFile(outputFile.str(), error_code, llvm::sys::fs::F_None);
      TheRewriter.getEditBuffer(SM.getMainFileID()).write(outFile); // --> this will write the result to outFile
      outFile.close();
    }

    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
        StringRef file) override {
      //llvm::errs() << "** Creating AST consumer for: " << file << "\n";
      TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
      return llvm::make_unique<MyASTConsumer_pass3>(TheRewriter, &CI.getASTContext());
    }

  private:
    Rewriter TheRewriter;
};

// For each source file provided to the tool, a new FrontendAction is created.
class MyFrontendAction_pass4 : public ASTFrontendAction {
  public:
    MyFrontendAction_pass4() {}
    //void EndSourceFileAction() override {
    //  SourceManager &SM = TheRewriter.getSourceMgr();
    //llvm::errs() << "** EndSourceFileAction for: " << SM.getFileEntryForID(SM.getMainFileID())->getName() << "\n";
    // }

    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
        StringRef file) override {
      //llvm::errs() << "** Creating AST consumer for: " << file << "\n";
      TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
      return llvm::make_unique<MyASTConsumer_pass4>(TheRewriter, &CI.getASTContext());
    }

  private:
    Rewriter TheRewriter;
};


int main(int argc, const char **argv) {
  CommonOptionsParser op(argc, argv, ToolingSampleCategory);
  ClangTool Tool(op.getCompilations(), op.getSourcePathList());

  // ClangTool::run accepts a FrontendActionFactory, which is then used to
  // create new objects implementing the FrontendAction interface. Here we use
  // the helper newFrontendActionFactory to create a default factory that will
  // return a new MyFrontendAction object every time.

  std::cout << "Before: iterativeP " << iterativeP << " Q_e_T " << Q_e_T << " QTupdateStatus " << QTupdateStatus[0] << " " << QTupdateStatus[1] << " DistanceType " << DistanceType << std::endl; 

  //first run to find whether the problem is iterative: bool iterativeP;  find whether Q and T are the same data set: bool Q_e_T; find which data sets get updated if it is a iterative problem:
  int run1 = Tool.run(newFrontendActionFactory<MyFrontendAction_pass1>().get());

  std::cout << "run1 " << run1 << std::endl; 

  std::cout << "After run1: iterativeP " << iterativeP << " Q_e_T " << Q_e_T << " QTupdateStatus " << QTupdateStatus[0] << " " << QTupdateStatus[1] << " DistanceType " <<  DistanceType << std::endl; 

  //then we could decide which big category of landmark and the number of landmarks to use. 

  int run2 = Tool.run(newFrontendActionFactory<MyFrontendAction_pass2>().get());
  std::cout << "After run2: iterativeP " << iterativeP << " Q_e_T " << Q_e_T << " QTupdateStatus " << QTupdateStatus[0] << " " << QTupdateStatus[1] << std::endl; 
  std::cout << "run2 " << run2 << std::endl; 

  int updateStatus;
  if(QTupdateStatus[0] == true){
    if(QTupdateStatus[1] == true){
      updateStatus = 3;  //both Q and T are updated
    }
    else{
      updateStatus = 2; //only Q gets updated
    }
  }
  else if(QTupdateStatus[1] == true){
    updateStatus = 1; //only T gets updated
  }  
  
  std::cout << "updateStatus " << updateStatus << std::endl;

  bigCat = BigCatetory(iterativeP, Q_e_T, updateStatus);
  std::cout << "bigCat " << bigCat << std::endl;

  int run3 = Tool.run(newFrontendActionFactory<MyFrontendAction_pass3>().get());
  std::cout << "run3 " << run3 << std::endl;

  return run2;
}

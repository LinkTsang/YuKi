#include <rpml/rpml_loader.h>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>

#include <antlr4-runtime.h>
#include "XMLLexer.h"
#include "XMLParser.h"
#include "XMLParserBaseVisitor.h"

using namespace rpml;

class TypeInfo;

class MetaInfoContext {
 public:
  void registerModule() {}

  void registerTypeInfo() {}

  std::shared_ptr<TypeInfo> getTypeInfo(const std::string& typeName) const {
    if (auto itr = typeInfos_.find(typeName); itr != typeInfos_.end()) {
      return itr->second;
    }
    return {};
  }

 private:
  std::map<std::string, std::shared_ptr<TypeInfo>> typeInfos_;
};

class PropertyInfo {
 public:
  const char* name() const { return name_; }
  const TypeInfo* typeInfo() const { return typeInfo_.get(); }

 private:
  const char* name_;
  std::shared_ptr<TypeInfo> typeInfo_;
};

class MethodInfo {
 public:
  const char* name() const { return name_; }

  template <typename R = void, typename... Args>
  R invoke(Args... args) {
    return R();
  }

 private:
  const char* name_;
};

class TypeInfo {
 public:
  const std::string name() const { return name_; }
  const std::vector<PropertyInfo>& properties() const { return properties_; }
  const std::vector<MethodInfo>& methods() const { return methods_; }

 private:
  std::vector<PropertyInfo> properties_;
  std::vector<MethodInfo> methods_;
  std::string name_;
  friend class TypeInfoBuilder;
};

class TypeInfoBuilder {
 public:
  TypeInfoBuilder() = default;
  void setName(const std::string& name) { typeInfo_.name_ = name; }
  void addProperty(const PropertyInfo& propertyInfo) {
    typeInfo_.properties_.push_back(propertyInfo);
  }
  void addMethod(const MethodInfo& methodInfo) {
    typeInfo_.methods_.push_back(methodInfo);
  }
  TypeInfo build() { return typeInfo_; }

 private:
  TypeInfo typeInfo_;
};

class ModuleInfo {
 public:
  const std::string name() const { return name_; }

 private:
  std::string name_;
};

using namespace antlr4;
using namespace antlrxmlparser;

class Visitor : public XMLParserBaseVisitor {
 public:
  virtual antlrcpp::Any visitDocument(
      XMLParser::DocumentContext* ctx) override {
    std::cout << ctx->getText() << std::endl;
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitProlog(XMLParser::PrologContext* ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitContent(XMLParser::ContentContext* ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitElement(XMLParser::ElementContext* ctx) override {
    for (auto attribute : ctx->attribute()) {
      auto name = attribute->Name()->getText();
      auto value = attribute->STRING()->getText();
      std::cout << name << ": " << value << "\n";
    }
    std::cout << ctx->getText() << std::endl;
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReference(
      XMLParser::ReferenceContext* ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAttribute(
      XMLParser::AttributeContext* ctx) override {
    return visitChildren(ctx);
  }
  virtual antlrcpp::Any visitChardata(
      XMLParser::ChardataContext* ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMisc(XMLParser::MiscContext* ctx) override {
    return visitChildren(ctx);
  }

 private:
};

class ErrorListener : public antlr4::BaseErrorListener {
  virtual void syntaxError(Recognizer* recognizer, Token* offendingSymbol,
                           size_t line, size_t charPositionInLine,
                           const std::string& msg,
                           std::exception_ptr e) override {
    auto stack =
        static_cast<antlr4::Parser*>(recognizer)->getRuleInvocationStack();
    std::cout << "Parsing failed!:\n";
    std::cout << "  Rule Stack:\n";
    for (auto info : stack) {
      std::cout << " " << info << "\n";
    }
    std::cout << "  line: " << line << ":" << charPositionInLine << " at "
              << offendingSymbol->toString() << ": " << msg << "\n";
  }
};

class Object {
 public:
  virtual const TypeInfo* typeInfo() const {}

 private:
  static TypeInfo typeInfo_;
};

class Foo : public Object {
 public:
  Foo() {}
  void setValue(int value) { value_ = value; }
  int value() const { return value_; }

  int menthod(int value) { return value_ &= value; }

 private:
  int value_;
};

#include <new>

template <typename T = void>
class TReflection {
 public:
  static T* createInstance(void* p) { return new (p)(); }
  static Object* createInstance() { return nullptr; }
  static MethodInfo getMethod() { return {}; }

 private:
};

class Reflection {
 public:
  static Object* createInstance(const char* className) { return nullptr; }
};

void testReflection() {
  MethodInfo methodA;
  methodA.invoke<int>(42);

  Object* objectB = Reflection::createInstance("Foo");
  auto method = TReflection<Foo>::getMethod();
  method.invoke(objectB, 1, 2, 3);
}

int main() {
  ANTLRInputStream input(R"(
    <View>
      <TextBox Content="Hello XML</>~"/>
    </View>
)");

  antlrxmlparser::XMLLexer lexer(&input);
  CommonTokenStream tokens(&lexer);

  antlrxmlparser::XMLParser parser(&tokens);
  auto errorListener = std::make_unique<ErrorListener>();
  parser.removeErrorListeners();
  parser.addErrorListener(errorListener.get());
  try {
    tree::ParseTree* tree = parser.document();
    auto visitor = std::make_unique<Visitor>();
    tree->accept(visitor.get());
    std::cout << "Parse Tree: " << tree->toStringTree(&parser) << std::endl;
  } catch (antlr4::LexerNoViableAltException& ex) {
    std::cout << "error: " << ex.toString() << "\n";
  }
  return 0;
}

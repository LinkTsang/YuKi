#include <rpml/rpml_loader.h>
#include <map>
#include <memory>
#include <unordered_map>
#include <iostream>

#include <antlr4-runtime.h>
#include "XMLLexer.h"
#include "XMLParser.h"

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
  const std::string& name() const { return name_; }
  const TypeInfo* typeInfo() const { return typeInfo_.get(); }

 private:
  std::string name_;
  std::shared_ptr<TypeInfo> typeInfo_;
};

class MethodInfo {
 public:
  const std::string name() const { return name_; }
  void invoke() {}

 private:
  std::string name_;
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

int main() {
  using namespace antlr4;
  ANTLRInputStream input(R"(
    <View>
      <TextBox Content="Hello XML~"/>
    </View>
)");

  antlrxmlparser::XMLLexer lexer(&input);
  CommonTokenStream tokens(&lexer);

  antlrxmlparser::XMLParser parser(&tokens);
  tree::ParseTree* tree = parser.document();

  std::cout << "Parse Tree: " << tree->toStringTree(&parser) << std::endl;
}

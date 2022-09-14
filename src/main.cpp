#include "core/NFAbstractNodeModel.hpp"

#include "3rdparty/entt/single_include/entt/entt.hpp"

#include "3rdparty/cpputils/prettyprint.h"

#include "core/TypeTricks.hpp"


using namespace cpputils;


// #ifdef _WIN32
// #pragma comment(lib, "liblua54.a")
// #endif



#include "core/FreeFunctionNode.hpp"






#include "core/CodeGen.hpp"

#include "core/Reflection.hpp"
// 



void doStuff(int a)
{

}

struct A
{
	int Sda(int a)
	{
		return 1;
	}
};

void ja()
{

}

void doRefStuff(int& a)
{

}






struct ada
{
	int** jad(int& e)
	{

	}

	static int jaja()
	{}
};




namespace nf
{
	struct Serializer
	{
		uint64_t id;
		std::function<void(std::ostringstream&, void*)> func;
	};

	template<typename T>
	Serializer MakeSerializer()
	{
		auto lambda = [](std::ostringstream& stream, void* data) {
			stream << *static_cast<T*>(data);
		};

		return { entt::type_hash<T>::value(), lambda };
	}
}

#include <ostream>
namespace nf
{
	template<typename T>
	class OOutputPort : public ValueWrapper<T>
	{
		friend class OutputPinHandle;
	public:
		static constexpr bool valid = true;
	public:
		OOutputPort() = default;
		OOutputPort(const T& defaultVal)
			: ValueWrapper<T>(defaultVal)
		{}

		void serialize(std::ostringstream& stream)
		{
			stream << this->value;
		}


	private:
		int portIndex = -1;
	};

}



namespace nf
{
	class NodeDataModel;

	enum class PortDirection { Input, Output };

	struct PortConnectionContext
	{
		NodeDataModel* linkedNode = nullptr;
		int portIndex = -1;
	};

	struct NodeOutputSocket
	{
		// opaque pointer on the value of the output-port
		OutputPinHandle valueView;
		PortConnectionContext connectionContext;
	};

	class OutputBus
	{

	private:
		std::vector<NodeOutputSocket> sockets;
	};

	class NodeDataModel
	{
	public:
		NodeDataModel() = default;
		virtual ~NodeDataModel() = default;

		template<typename T>
		void setNodeOutput(const OutputPort<T>& port, const T& val);

		template<typename T>
		void setNodeOutput(int portIndex, const T& val);

		template<typename T>
		T* getNodeInput(const InputPort<T>& port);

	private:
	};
}

#include <format>


// 	static constexpr std::array<std::string_view, sizeof...(Ts)> value = { {entt::type_name<Ts>::value()...} };

// void assign_values(std::array<int, N>& arr, Values... vals) { 
// 	arr = { vals... }; 
// }



template<auto Func>
struct NfFunction
{};





#define NF_FUNCTION(Func,...) \
template<> struct NfFunction<Func> \
{ \
	static constexpr auto name = #Func; \
	static consteval auto genFuncSpecifierStorage(auto... args) \
	{  \
		return std::array<std::string_view, sizeof...(args)>{args...}; \
	} \
	static constexpr auto FuncSpecifierArgs = genFuncSpecifierStorage(__VA_ARGS__);  \
}; \




// 
// void dostuff(); 
// NF_FUNCTION(dostuff, DisplayName("HEY"));

#include <boost/preprocessor.hpp>

#define NF_TUPLE(a, b) (a, b)



#define MACRO(r, data, elem) \
BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_TUPLE_ELEM(2, 0, elem), 0), node.setDisplayName(BOOST_PP_TUPLE_ELEM(2,1, elem))) \


#define SEQ ((0, "Hey"))((1, "Ja"))






// NF_FUNCTION(doStuff, DisplayName("doStuff"), ArgumentNames("A", "B"), COMPILABLE)
namespace nf
{
	template<typename T>
	struct Variable
	{
		OutputPort<T> var;
	};

	template<typename T>
	struct VariableSetter
	{

		VariableSetter(Variable<T>& var)
			: view(var.var)
		{}

		void set(const T& val)
		{
			auto ref = view.valueRef<T>();
			*ref = val;
		}

		nf::OutputPinHandle view;
	};




	// template<bool B>
	// struct NfFunctionSpecifierEval
	// {
	// 	static constexpr bool CompilableSpecifier()
	// 	{
	// 		return B;
	// 	}
	// };




}



struct DisplayName_Spec {};

struct Compilable_Spec {};

struct ArgumentNames_Spec {};


template<class ... Args>
struct NfFunctionParamaterEvaluator
{
};

template <>
struct NfFunctionParamaterEvaluator<>
{
	static constexpr auto DisplayNameIndex = -1;
	static constexpr auto ArgumentNamesIndex = -1;
	static constexpr auto CompilableFlagIndex = -1;
};

template <>
struct NfFunctionParamaterEvaluator<DisplayName_Spec, ArgumentNames_Spec, Compilable_Spec >
{
	static constexpr auto DisplayNameIndex = 0;
	static constexpr auto ArgumentNamesIndex = 1;
	static constexpr auto CompilableFlagIndex = 2;
};

template <>
struct NfFunctionParamaterEvaluator<Compilable_Spec, ArgumentNames_Spec, DisplayName_Spec>
{
	static constexpr auto DisplayNameIndex = 2;
	static constexpr auto ArgumentNamesIndex = 1;
	static constexpr auto CompilableFlagIndex = 0;
};


template <>
struct NfFunctionParamaterEvaluator<ArgumentNames_Spec, DisplayName_Spec>
{
	static constexpr auto DisplayNameIndex = 1;
	static constexpr auto ArgumentNamesIndex = 0;
	static constexpr auto CompilableFlagIndex = -1;
};

template <>
struct NfFunctionParamaterEvaluator<DisplayName_Spec, ArgumentNames_Spec>
{
	static constexpr auto DisplayNameIndex = 0;
	static constexpr auto ArgumentNamesIndex = 1;
	static constexpr auto CompilableFlagIndex = -1;
};



template <>
struct NfFunctionParamaterEvaluator<DisplayName_Spec, Compilable_Spec>
{
	static constexpr auto DisplayNameIndex = 0;
	static constexpr auto ArgumentNamesIndex = -1;
	static constexpr auto CompilableFlagIndex = 1;
};

template <>
struct NfFunctionParamaterEvaluator<ArgumentNames_Spec, Compilable_Spec>
{
	static constexpr auto DisplayNameIndex = -1;
	static constexpr auto ArgumentNamesIndex = 0;
	static constexpr auto CompilableFlagIndex = 1;
};

template <>
struct NfFunctionParamaterEvaluator<ArgumentNames_Spec>
{
	static constexpr auto DisplayNameIndex = -1;
	static constexpr auto ArgumentNamesIndex = 0;
	static constexpr auto CompilableFlagIndex = -1;
};

template <>
struct NfFunctionParamaterEvaluator<Compilable_Spec>
{
	static constexpr auto DisplayNameIndex = -1;
	static constexpr auto ArgumentNamesIndex = -1;
	static constexpr auto CompilableFlagIndex = 0;
};

template <>
struct NfFunctionParamaterEvaluator<DisplayName_Spec>
{
	static constexpr auto DisplayNameIndex = 0;
	static constexpr auto ArgumentNamesIndex = -1;
	static constexpr auto CompilableFlagIndex = -1;
};






#define DisplayName(name) (name, DisplayName_Spec)

#define ArgumentNames(...) (__VA_ARGS__, ArgumentNames_Spec)

#define Compilable(boolean) (boolean, Compilable_Spec)


#define NF_FUNCTION(func, arg1) 
struct _Node
{

};

struct FreeFunctionParams
{
	std::string functionName = "";
	std::vector<std::string> paramNames{};
};

struct ClassNodeBuilder
{
	template<auto func>
	ClassNodeBuilder& method(const std::string& methodName = "", std::initializer_list<std::string> paramNames = {})
	{
		return *this;
	}

	template<auto func>
	ClassNodeBuilder& ctor()
	{
		return *this;
	}

	template<auto func>
	ClassNodeBuilder& member()
	{
		return *this;
	}


	std::string className = "";
};

enum class VarInteractionLevel
{
	OnlySet,
	OnlyGet,
	SetAndGet
};

struct NodeFlowDataRegistrar
{


	void RegisterNodeModel();

	template<class Class>
	void RegisterClass(const std::string& className = "")
	{

	}

	template<auto func>
	void RegisterFreeFunction(const FreeFunctionParams& params)
	{

	}

	template<auto func>
	void RegisterFreeFunction(const std::string& funcName = "", const std::vector<std::string>& paramNames = {})
	{

	}

	template<typename T>
	void MakeTypeSpawnable(const std::string& name, VarInteractionLevel l = VarInteractionLevel::SetAndGet)
	{

	}


private:
	std::vector<ClassNodeBuilder> classes;

};


template<int A>
void e()
{
}

using Serializer = std::function<void(std::stringstream& archive, void* data)>;



template<typename T>
Serializer GenerateSerializer()
{
	static_assert(nf::has_ostream_operator_v<T>, "No << operator found.");
	auto lambda = [](std::stringstream& archive, void* data) {
		archive << *static_cast<T*>(data);
	};

	return lambda;
}




struct CPPCodeInstruction
{
	enum InstructionType
	{
		VariableInitialization,
		VariableSet,
		VariableGet,
		FreeFunctionCall,
		MemberFunctionCall
	};

	template<auto func, bool e = false>
	static CPPCodeInstruction GenCallInstruction_FreeFunction()
	{
		using codeContext = nf::FunctionCodeContext<func>;
		using retType = codeContext::ReturnType;

		CPPCodeInstruction ci;
		ci.instructionType = CPPCodeInstruction::FreeFunctionCall;

		std::stringstream stream;
		if constexpr (!std::is_same_v<retType, void>)
		{
			stream << codeContext::ReturnTypeName() << " $r1 = ";
		}

		stream << codeContext::FunctionName() << "(";

		if constexpr (constexpr int argCount = codeContext::ArgumentCount() != 0)
		{
			ci.functionArgumentCount = argCount;
			auto argNames = codeContext::ArgumentTypeNames();

			for (size_t i = 0; i < argNames.size(); i++)
			{
				stream << "$a" << i + 1;

				if (i != argNames.size() - 1)
				{
					stream << ", ";

				}
			}
		}

		stream << ");";


		ci.instruction = stream.str();
		return ci;
	}

	InstructionType instructionType;
	int functionArgumentCount = -1;
	std::string instruction;
};

float heyja(int a)
{
	return 1;
}


template<typename T>
T erarar()
{
	T t;
	return t;
}

template<auto T>
void era()
{
	std::string_view pretty_function{ ENTT_PRETTY_FUNCTION };
	pprint(pretty_function);
}

namespace eri
{
	namespace b
	{
		int* func1(const std::string* b, float e)
		{
			return nullptr;
		}
	}

	float cece()
	{
		return 3.0;
	}

	std::string* func3(std::string* b)
	{
		return b;
	}
}

template<typename T, bool enable = true>
struct BType
{
	using Type = T;
};

template<typename T>
struct AC
{
	using Type = BType<T>::Type;
};

AC<int>::Type Eer()
{
	AC<int>::Type c{};
	return c;
}


struct BaseBase
{
public:
	virtual int ret()
	{
		return 0;
	};
	virtual ~BaseBase() = default;
};

struct Derived_1 : public BaseBase
{
public:
	int ret() override
	{
		return 123;
	}
	virtual ~Derived_1() = default;
};

struct Derived_2 : public Derived_1
{
public:
	int ret() override
	{
		return 2123123;
	}
	virtual ~Derived_2() = default;
};


// std::intptr_t

struct NFFunctorConfig
{
	std::string displayName = "";
	bool compilable = false;
};

struct ClassMethodNodeBuilder
{
	std::string className;
	std::string displayName;
};

#define NF_CLASS(Class,...) 
#define NF_METHOD(...)
#define NF_FIELD(...)
#define NF_FREEFUNC(...)

#define NF


class WhateverTest
{

public:
	void dostuff() {

	}
	int a = 3;

private:
	NF_CLASS(
		WhateverTest,
		NF_METHOD(dostuff),
		NF_FIELD(a)
	)

	
};

struct NFColor
{
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
};


struct GlobalNodeStyle
{
	template<typename T>
	void AddTypeColor(const NFColor& color)
	{
		typeColors[entt::type_hash<T>::value()] = color;
	}

	std::map<uint64_t, NFColor> typeColors;
};

void testRValue(int&& rval)
{

}

class EEE
{
public:
	int a = 3;
};

std::ostream& operator<< (std::ostream& out, const EEE& e)
{
	out << e.a;
	return out;
}


#define REPEAT_0(WHAT)
#define REPEAT_1(WHAT) WHAT REPEAT_0(WHAT)
#define REPEAT_2(WHAT) WHAT REPEAT_1(WHAT)
#define REPEAT_3(WHAT) WHAT REPEAT_2(WHAT)

#define NOP_N(N) REPEAT_##N(asm("nop");)



constexpr int f(int a, int b, int c)
{
	return a + b + c;
}


template<typename T>
struct wrapper
{
public:
	T val{};
};

template<typename T>
constexpr T wrapper_get(const wrapper<T>& w)
{
	return w.val;
}


namespace nf
{
	// Implementation taken from std::apply MSVC
	// Compare to https://en.cppreference.com/w/cpp/utility/apply - Implementation

	template <class Func, class Tuple, size_t... seq>
	constexpr decltype(auto) ApplyPinsOnCallable_Impl(Func&& callable, Tuple&& tupl, std::index_sequence<seq...>)
	{
		return std::invoke(std::forward<Func>(callable),
			wrapper_get(std::get<seq>(std::forward<Tuple>(tupl)))...);
	}

	template <class Func, class Tuple>
	constexpr decltype(auto) ApplyPinsOnCallable(Func&& callable, Tuple&& tpl)
	{
		return ApplyPinsOnCallable_Impl(std::forward<Func>(callable), std::forward<Tuple>(tpl),
			std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
	}

}


#include "utility/timer.h"

int main()
{
	
	



	return 0;
	GlobalNodeStyle style;
	style.AddTypeColor<int>({ 123,123,123 });
	pprint((int)style.typeColors[entt::type_hash<int>::value()].b);


	// 	std::cout << ad<nf::InputPort<int>>();
	// 	std::cout << entt::type_name<nf::nf::InputPort<int>>



	// 	extern int _SAL_L_Source_(__source_code_content, (typ), _SA_annotes1(SAL_source_code_content, typ)) __iSAL_Source_Code_Content##typ;
	// 	auto e = NAMEOF(GenerateSerializer<int>);

	// 	std::cout << e;

	// 	pprint(nf::FunctionCodeContext<CPPCodeInstruction::GenCallInstruction_FreeFunction<eri::cece>>::FunctionName());
	return 1;

	nf::ExecutionPinOut pin;
	pin.MakeBranchable();
	pin.BranchFirst();

	pin.ForceStopExecution();

	using C = AC<int>;
	pprint(entt::type_name<AC<int>::Type>::value());

	CPPCodeInstruction instr3 = CPPCodeInstruction::GenCallInstruction_FreeFunction<Eer>();
	CPPCodeInstruction instr2 = CPPCodeInstruction::GenCallInstruction_FreeFunction<eri::cece>();
	CPPCodeInstruction instr1 = CPPCodeInstruction::GenCallInstruction_FreeFunction<eri::func3>();

	pprint(instr1.instruction);
	pprint(instr2.instruction);
	pprint(instr3.instruction);

	std::string ee = "";

	class std::basic_string<char, struct std::char_traits<char>, class std::allocator<char> >* returN1 = eri::func3(&ee);
	float returnFloat = eri::cece();
	int* returIns = eri::b::func1(returN1, returnFloat);


	// 	int* $r1 = eri::b::func1($a1, $a2);
	// 	int* $r1 = eri::func1($a1, $a2);


	// 	int* $r1 = func1($a1, $a2);


	// 	pprint(nf::FunctionCodeContext< pprint<std::string>>::FunctionName());

	// 	auto serializer = GenerateSerializer<std::vector<int>>();

	// 	serializer(e, &ada);







	// 	NodeFlowDataRegistrar registrar;

	// 	registrar.RegisterFreeFunction<ad>("ad", {"a"});
	// 	registrar.MakeTypeSpawnable<int>("int");
	// 	registrar.MakeTypeSpawnable<NodeFlowDataRegistrar>("NodeFlowDataRegistrar");
	// 	registrar.MakeTypeSpawnable<float>("float");













	// 	using e = BOOST_PP_TUPLE_ELEM(2, 1, DisplayName("hey"))

	// 	auto e = "HEy";

	// 	using arrElemType = std::remove_all_extents<decltype(e)>::type;

	// 	NF_FUNCTOR(main)



	// 	std::common_type_t<decltype(e)>;
	// 	using b = std::common_type_t<decltype(false)>;



	// 	NfFunctionParamaterEvaluator<decltype("Hey"), decltype("Hey2")>::T

	// 	DisplayName()
	// 	constexpr std::tuple<int, std::string_view> e {// 0, "hey"};

	// 	DisplayName("hey");

	// 	NF_FUNCTION("DisplayName = integerPointer", "ReturnName = Pointer")




	// 
	// 
	// 	
	// 	pprint(c->get());



	


	// 	pprint(*static_cast<decltype(a)*>(strptr));
	// 	std::vector<nf::NodeOutputSocket> e;







		// 	using ReturnType_t = typename nf::FuncSignature<decltype(std::function{ Func }) > ::ReturnType_t;
		// 	nf::FuncSignature< std::function<decltype(&ada::jad)>>::ReturnType_t
		// 	using e = Whatever<&ada::jad>::ArgumentTypes_t;
		// 		static constexpr std::array<std::string_view, sizeof...(Ts)> value = { {entt::type_name<Ts>::value()...} }

		// 	pprint(nf::FunctionReflection<doStuff>::ArgumentTypeNames());

		// 	using e = std::tuple_element_t<1, std::tuple<int,float>>;
		// 	using type = std::conditional_t<false, bool, char>;



		// 	;

		// 	std::function<decltype(doStuff)> b;

		// 	// td::tuple_size_v<decltype(b)::InputPortsList>


		// 	addClass<LUA_API_Node>("Node", 
		// 		nf::method<&LUA_API_Node::displayName>("Hey", "a", "b"),
		// 		nf::ctor<
		// 		)



		// 	ClassNodeContext<LUA_API_Node> c();
		// 	pprint(c.name_);
		// 
		// 	sol::state lua;
		// 	lua.open_libraries(sol ::lib::base);
		// 	lua.new_usertype<LUA_API_Node>("Node", "setDisplayName", &LUA_API_Node::setDisplayName);
		// 	auto result = lua.script_file(R"(C:\Users\ruwen\Desktop\Learning_CPP\nodeflow\src\lua\scripts\example1.lua)");


		// 	auto node = lua.get<LUA_API_Node>("node");




		// 	pprint(entt::type_name<LUA_API_Node::displayName()>.value());

		// 	uint16_t ar[9] = { 64700, 42456, 7048, 2347, 40176, 36864, 62528, 48497, 43064, };





		// 	adada<&Foo::get>();

		// 	pprint(HeaderDependencyContext<HASH_STRING("pprint_type<int>")>::headerfile);

		// 	pprint(entt::type_name<FunctionExpressionExtractor<doStuffad>>::value());
		// 		pprint(FunctionCodeContext<entt::hashed_string::value("doStuffad")>::signature_t);
				// 	pprint(Hey::CodeLocationContext::headerFile);
				// 	std::ifstream in(Hey::CodeLocationContext::headerFile); // the in flag is optional
				// 	std::stringstream buffer;
				// 	buffer << in.rdbuf();

				// 	pprint(buffer.str())
}

